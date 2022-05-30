#include "helpers.h"

typedef struct {
    string result;
    string aditional_data;
} command_output_plus;

/*takes a message, sends it to the server;
 returns the response*/
string send_request(string message);

/*interprets the server's response to the register command;
 returns the result - done / error*/
string process_register(string message);

/*gets username and password from the user and constructs the message for the server;
 returns the result - done / error*/
string command_register();

/*interprets the server's response to the login command;
 returns the result - done / error - and login cookie*/
command_output_plus process_login(string message);

/*gets username and password from the user and constructs the message for the server;
 returns result - done / error - and login cookie*/
command_output_plus command_login();

/*constructs the message for the server for the enter_library command and interprets the response;
 returns the result - done / error - and the JWT token*/
command_output_plus command_enter_library(string loginCookie);

/*constructs the message for the server for the get_books command and interprets the response;
 returns the response - a list of books (in string form) / error*/
string command_get_books(string jwt);

/*interprets the server's response to the get_book command;
 returns the result - a list of the book's characteristics (in string form) / error*/
string process_get_book(string message);

/*gets id and constructs the message for the server for the get_book command;
 returns the result - a list of the book's characteristics (in string form) / error*/
string command_get_book(string jwt);

/*interprets the server's response to the add_book command;
 returns the result - done / error*/
string process_add_book(string message);

/*gets title, author, genre, publisher and page count and
 constructs the message for the server for the get_book command;
 returns the result - done / error*/
string command_add_book(string jwt);

/*interprets the server's response to the delete_book command;
 returns the result - done / error*/
string process_delete_book(string message);

/*gets id and constructs the message for the server for the delete_book command;
 returns the response - done / error*/
string command_delete_book(string jwt);

/*interprets the server's response to the logout command;
 returns the result - done / error*/
string process_logout(string message);

/*constructs the message for the server for the logout command and interprets the response;
 returns the response - done / error*/
string command_logout(string loginCookie);