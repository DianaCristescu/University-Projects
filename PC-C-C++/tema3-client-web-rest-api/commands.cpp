#include "commands.h"

/*takes a message, sends it to the server;
 returns the response*/
string send_request(string message) {
    int sockfd = open_connection("34.241.4.235", 8080, PF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    // cout << "\nCLIENT\n\n" << message;

    message = receive_from_server(sockfd);
    close_connection(sockfd);
    // cout << "\n\nSERVER\n\n" << message;
    // cout << "\n\n";

    return message;
}

/*interprets the server's response to the register command;
 returns the result - done / error*/
string process_register(string message) {
    message = send_request(message);
    const char *ptr = strstr(message.c_str(), "error");
    if (ptr == NULL) {
        return "*Done.\n";
    } else {
        return "Error: The username is taken.\n";
    }
}

/*gets username and password from the user and constructs the message for the server;
 returns the result - done / error*/
string command_register() {
    string username, password;
    cout << "username=";
    getline(cin, username);
    cout << "password=";
    getline(cin, password);

    const char *ptr1 = strstr(username.c_str(), " ");
    const char *ptr2 = strstr(password.c_str(), " ");
    if (ptr1 != NULL || ptr2 != NULL) {
        return "Error: Space isn't a valid character.\n";
    }

    json payload = {
        {"username", username},
        {"password", password},
    };

    return process_register(compute_post_request("34.241.4.235", "/api/v1/tema/auth/register",
                             "application/json", payload.dump(), NULL, 0, ""));
}

/*interprets the server's response to the login command;
 returns the result - done / error - and login cookie*/
command_output_plus process_login(string message) {
    command_output_plus output;

    message = send_request(message);

    const char *ptr = strstr(message.c_str(), "error");
    if (ptr == NULL) {
        int begin = message.find("Cookie: ") + string("Cookie: ").length();
        int end = message.find("; Path");
        output.aditional_data = message.substr(begin, end - begin);

        output.result = "*Done.\n";
    } else {
        output.result = "Error: Account doesn't exist.\n";
    }

    return output;
}

/*gets username and password from the user and constructs the message for the server;
 returns result - done / error - and login cookie*/
command_output_plus command_login() {
    string username, password;
    cout << "username=";
    getline(cin, username);
    cout << "password=";
    getline(cin, password);

    json payload = {
        {"username", username},
        {"password", password},
    };

    return process_login(compute_post_request("34.241.4.235", "/api/v1/tema/auth/login",
                         "application/json", payload.dump(), NULL, 0, ""));
}

/*constructs the message for the server for the enter_library command and interprets the response;
 returns the result - done / error - and the JWT token*/
command_output_plus command_enter_library(string loginCookie) {
    command_output_plus output;

    if (loginCookie != "") {
        string cookies[1] = {loginCookie};

        string message = send_request(compute_get_request("34.241.4.235",
                 "/api/v1/tema/library/access", "", cookies, 1, ""));

        const char *ptr = strstr(message.c_str(), "error");
        if (ptr == NULL) {
            int begin = message.find("token\":\"") + string("token\":\"").length();
            int end = message.find("\"}");

            output.aditional_data = "Authorization: Bearer ";
            output.aditional_data += message.substr(begin, end - begin);
            output.result = "*Done.\n";
        } else {
            output.aditional_data = "";
            output.result = "Error: Acces denied. You are logged out.\n";
        }
    } else {
        output.aditional_data = "";
        output.result = "Error: Acces denied. You are logged out.\n";
    }

    return output;
}

/*constructs the message for the server for the get_books command and interprets the response;
 returns the response - a list of books (in string form) / error*/
string command_get_books(string jwt) {
    if (jwt != "") {
        string message = send_request(compute_get_request("34.241.4.235",
                     "/api/v1/tema/library/books", "", NULL, 0, jwt));

        const char *ptr = strstr(message.c_str(), "error");
        if (ptr == NULL) {
            if (message.find("[{") == string::npos) {
                return "*The library is empty.\n";
            } else {
                int begin = message.find("[{");
                int end = message.find("}]") + 3;

                message = message.substr(begin, end - begin);
                json json_array = json::parse(message);

                message = "*Done.\n";
                message += "\n*-----*\n";
                message += "Books:\n";
                for (int i = 0; i < int(json_array.size()); i++) {
                    message += "#" + to_string(json_array[i]["id"]) + " ";
                    message += string(json_array[i]["title"]) + "\n";
                }
                message += "*-----*\n";
                return message;
            }
        } else {
            return "Error: Bad JWT token.\n";
        }
    } else {
        return "Error: Please enter the library first.\n";
    }
}

/*interprets the server's response to the get_book command;
 returns the result - a list of the book's characteristics (in string form) / error*/
string process_get_book(string message) {
    message = send_request(message);

    const char *ptr = strstr(message.c_str(), "404");
    if (ptr == NULL) {
        int begin = message.find("[{");
        int end = message.find("}]") + 3;
        message = message.substr(begin, end - begin);

        json json_array = json::parse(message);

        message = "*Done.\n\n";
        message += "*--\"" + string(json_array[0]["title"]) + "\"--*\n";
        message +=  "Author: " + string(json_array[0]["author"]) + "\n";
        message +=  "Publisher: " + string(json_array[0]["publisher"]) + "\n";
        message +=  "Genre: " + string(json_array[0]["genre"]) + "\n";
        message +=  "Nr. of pages: " + to_string(json_array[0]["page_count"]) + "\n";
        return message;
    } else {
        return "Error: Bad id.\n";
    }
}

/*gets id and constructs the message for the server for the get_book command;
 returns the result - a list of the book's characteristics (in string form) / error*/
string command_get_book(string jwt) {
    string id;
    cout << "id=";
    getline(cin, id);

    if (jwt != "") {
        string message = "/api/v1/tema/library/books/" + string(id);
        return process_get_book(compute_get_request("34.241.4.235", message, "", NULL, 0, jwt));
    } else {
        return "Error: Please enter the library first.\n";
    }
}

/*interprets the server's response to the add_book command;
 returns the result - done / error*/
string process_add_book(string message) {
    message = send_request(message);

    const char *ptr = strstr(message.c_str(), "error");
    if (ptr == NULL) {
        return "*Done.\n";
    } else {
        return "Error: TO DO.\n";
    }
}

/*gets title, author, genre, publisher and page count and
 constructs the message for the server for the get_book command;
 returns the result - done / error*/
string command_add_book(string jwt) {
    string title, author, genre, publisher, page_count;
    cout << "title=";
    getline(cin, title);
    cout << "author=";
    getline(cin, author);
    cout << "genre=";
    getline(cin, genre);
    cout << "publisher=";
    getline(cin, publisher);
    cout << "page_count=";
    getline(cin, page_count);

    if (!isNumber(page_count)) {
        return "Error: Page count isn't a number.\n";
    }

    json payload = {
        {"title", title},
        {"author", author},
        {"genre", genre},
        {"publisher", publisher},
        {"page_count", page_count},
    };

    if (jwt != "") {
        return process_add_book(compute_post_request("34.241.4.235", "/api/v1/tema/library/books",
                                             "application/json", payload.dump(), NULL, 0, jwt));
    } else {
        return "Error: Please enter the library first.\n";
    }
}

/*interprets the server's response to the delete_book command;
 returns the result - done / error*/
string process_delete_book(string message) {
    message = send_request(message);

    const char *ptr = strstr(message.c_str(), "error");
    if (ptr == NULL) {
        return "*Done.\n";
    } else {
        return "Error: Bad id.\n";
    }
}

/*gets id and constructs the message for the server for the delete_book command;
 returns the response - done / error*/
string command_delete_book(string jwt) {
    string id;
    cout << "id=";
    getline(cin, id);

    if (jwt != "") {
        string message = "/api/v1/tema/library/books/" + string(id);
        return process_delete_book(compute_delete_request("34.241.4.235", message, "", NULL, 0, jwt));
    } else {
        return "Error: Bad id.\n";
    }
}

/*interprets the server's response to the logout command;
 returns the result - done / error*/
string process_logout(string message) {
    message = send_request(message);

    const char *ptr = strstr(message.c_str(), "error");
    if (ptr == NULL) {
        return "*Done.\n";
    } else {
        return "Error: Bad Cookie.\n";
    }
}

/*constructs the message for the server for the logout command and interprets the response;
 returns the response - done / error*/
string command_logout(string loginCookie) {
    if (loginCookie.length()) {
        string cookies[1] = {loginCookie};
        return process_logout(compute_get_request("34.241.4.235", "/api/v1/tema/auth/logout", "", cookies, 1, ""));
    } else {
        return "Error: You are logged out.\n";
    }
}