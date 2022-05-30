#ifndef _HELPERS_
#define _HELPERS_

#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <ctype.h>

#include <string>
#include <iostream>

using namespace std;

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#define BUFLEN 4096
#define LINELEN 1000

// shows the current error
void error(const string msg);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(string host_ip, int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, string message);

// receives and returns the message from a server
string receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
string basic_extract_json_response(string str);

/*---*/

// computes and returns a GET request string (query_params
// can be set to "" and cookies can be set to NULL if not needed)
string compute_get_request(string host, string url, string query_params,
							string *cookies, int cookies_count, string token);

// computes and returns a POST request string (cookies can be NULL if not needed)
string compute_post_request(string host, string url, string content_type, string body_data,
                             string *cookies, int cookies_count, string token);

// computes and returns a DELETE request string (query_params
// can be set to "" and cookies can be set to NULL if not needed)
string compute_delete_request(string host, string url, string query_params, 
                                string *cookies, int cookies_count, string token);

/*---*/

typedef struct {
    char *data;
    size_t size;
} buffer;

// initializes a buffer
buffer buffer_init(void);

// destroys a buffer
void buffer_destroy(buffer *buffer);

// adds data of size data_size to a buffer
void buffer_add(buffer *buffer, const char *data, size_t data_size);

// checks if a buffer is empty
int buffer_is_empty(buffer *buffer);

// finds data of size data_size in a buffer and returns its position
int buffer_find(buffer *buffer, const char *data, size_t data_size);

// finds data of size data_size in a buffer in a
// case-insensitive fashion and returns its position
int buffer_find_insensitive(buffer *buffer, const char *data, size_t data_size);

bool isNumber(const string& str);

#endif