#include "helpers.h"

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

void error(string msg)
{
    perror(msg.c_str());
    exit(0);
}

int open_connection(string host_ip, int portno, int ip_type, int socket_type, int flag)
{
    struct sockaddr_in serv_addr;
    int sockfd = socket(ip_type, socket_type, flag);
    if (sockfd < 0)
        error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip.c_str(), &serv_addr.sin_addr);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    return sockfd;
}

void close_connection(int sockfd)
{
    close(sockfd);
}

void send_to_server(int sockfd, string message)
{
    int bytes, sent = 0;
    int total = message.length();
    char * message_char = (char *)malloc(total);
    strcpy(message_char, message.c_str());

    do
    {
        bytes = write(sockfd, message_char + sent, total - sent);
        if (bytes < 0) {
            error("ERROR writing message to socket");
        }

        if (bytes == 0) {
            break;
        }

        sent += bytes;
    } while (sent < total);
}

string receive_from_server(int sockfd)
{
    char response[BUFLEN];
    buffer buffer = buffer_init();
    int header_end = 0;
    int content_length = 0;

    do {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0){
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
        
        header_end = buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

        if (header_end >= 0) {
            header_end += HEADER_TERMINATOR_SIZE;
            
            int content_length_start = buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);
            
            if (content_length_start < 0) {
                continue;           
            }

            content_length_start += CONTENT_LENGTH_SIZE;
            content_length = strtol(buffer.data + content_length_start, NULL, 10);
            break;
        }
    } while (1);
    size_t total = content_length + (size_t) header_end;
    
    while (buffer.size < total) {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0) {
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
    }
    buffer_add(&buffer, "", 1);
    return string(buffer.data);
}

string basic_extract_json_response(string str)
{
    return strstr(str.c_str(), "{\"");
}

/*---*/

buffer buffer_init(void)
{
    buffer buffer;

    buffer.data = NULL;
    buffer.size = 0;

    return buffer;
}

void buffer_destroy(buffer *buffer)
{
    if (buffer->data != NULL) {
        free(buffer->data);
        buffer->data = NULL;
    }

    buffer->size = 0;
}

int buffer_is_empty(buffer *buffer)
{
    return buffer->data == NULL;
}

void buffer_add(buffer *buffer, const char *data, size_t data_size)
{
    if (buffer->data != NULL) {
        buffer->data = (char *)realloc(buffer->data, (buffer->size + data_size) * sizeof(char));
    } else {
        buffer->data = (char *)calloc(data_size, sizeof(char));
    }

    memcpy(buffer->data + buffer->size, data, data_size);

    buffer->size += data_size;
}

int buffer_find(buffer *buffer, const char *data, size_t data_size)
{
    if (data_size > buffer->size)
        return -1;

    size_t last_pos = buffer->size - data_size + 1;

    for (size_t i = 0; i < last_pos; ++i) {
        size_t j;

        for (j = 0; j < data_size; ++j) {
            if (buffer->data[i + j] != data[j]) {
                break;
            }
        }

        if (j == data_size)
            return i;
    }

    return -1;
}

int buffer_find_insensitive(buffer *buffer, const char *data, size_t data_size)
{
    if (data_size > buffer->size)
        return -1;

    size_t last_pos = buffer->size - data_size + 1;

    for (size_t i = 0; i < last_pos; ++i) {
        size_t j;

        for (j = 0; j < data_size; ++j) {
            if (tolower(buffer->data[i + j]) != tolower(data[j])) {
                break;
            }
        }

        if (j == data_size)
            return i;
    }

    return -1;
}

/*---*/

string compute_get_request(string host, string url, string query_params,
                            string *cookies, int cookies_count, string token)
{
    string message;
    string line;

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (!query_params.length()) {
        line = "GET " + url + "?" + query_params + " HTTP/1.1";
    } else {
        line = "GET " + url + " HTTP/1.1";
    }

    message += line + "\r\n";

    // Step 2: add the host
    message += "Host: " + host + "\r\n";

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookies != NULL) {
        message += "Cookie: " + cookies[0];
        for (int i = 1; i < cookies_count; i++) {
            message += "; " + cookies[i];
        }
        message += "\r\n";
    }

    if (token.length()) message += token + "\r\n";

    // Step 4: add final new line
    message += "\r\n";
    return message;
}

string compute_post_request(string host, string url, string content_type, string body_data,
                             string *cookies, int cookies_count, string token)
{
    string message;
    string line;
    string body_data_buffer;

    // Step 1: write the method name, URL and protocol type
    line = "POST " + url + " HTTP/1.1";
    message += line + "\r\n";
    
    // Step 2: add the host
    message += "Host: " + host + "\r\n";

    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    message += "Content-Type: " + content_type + "\r\n";
    message += "Content-Length: " + to_string(body_data.length()) + "\r\n";

    // Step 4 (optional): add cookies
    if (cookies != NULL) {
        message += "Cookie: " + cookies[0];
        for (int i = 1; i < cookies_count; i++) {
            message += "; " + cookies[i];
        }
        message += "\r\n";
    }

    if (token.length()) message += token + "\r\n";

    // Step 5: add new line at end of header
    message += "\r\n";

    // Step 6: add the actual payload data
    message += body_data + "\r\n";

    return message;
}

string compute_delete_request(string host, string url, string query_params, 
                                string *cookies, int cookies_count, string token) {
    string message;
    string line;

    if (!query_params.length()) {
        line = "DELETE " + url + "?" + query_params + " HTTP/1.1";
    } else {
        line = "DELETE " + url + " HTTP/1.1";
    }

    message += line + "\r\n";

    // Step 2: add the host
    message += "Host: " + host + "\r\n";

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookies != NULL) {
        message += "Cookie: " + cookies[0];
        for (int i = 1; i < cookies_count; i++) {
            message += "; " + cookies[i];
        }
        message += "\r\n";
    }

    if (token.length()) message += token + "\r\n";

    // Step 4: add final new line
    message += "\r\n";
    return message;
}

bool isNumber(const string& str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}