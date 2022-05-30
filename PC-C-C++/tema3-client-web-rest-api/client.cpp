#include "commands.h"

/*Tema 3 Client Web. Comunicaţie cu REST API.
  commands: register      -> username, password                           => done / error
            login         -> username, password                           => done / error
            enter_library -> no input                                     => done / error
            get_books     -> no input                                     => list of books
            get_book      -> id                                           => book details
            add_book      -> title, author, publisher, genre, page count  => done / error
            delete_book   -> id                                           => done / error
            logout        -> no input                                     => done / error
            exit          -> no input                                     => quits program
  This program comunicates with the rest api, keeping the login cookie and the JWT token.
  It waits until it the user writes one of the above commands and acts accordingly.*/

int main() {
    command_output_plus output;
    string buffer;
    string loginCookie = "";
    string jwt = "";

    while (1) {

        getline(cin, buffer);

        if (buffer == "register") {
            
            cout << command_register();

        } else if (buffer == "login") {

            output = command_login();
            loginCookie = output.aditional_data;
            cout << output.result;

        } else if (buffer == "enter_library") {

            output = command_enter_library(loginCookie);
            jwt = output.aditional_data;
            cout << output.result;

        } else if (buffer == "get_books") {

            cout << command_get_books(jwt);

        } else if (buffer == "get_book") {

            cout << command_get_book(jwt);

        } else if (buffer == "add_book") {

            cout << command_add_book(jwt);

        } else if (buffer == "delete_book") {

            cout << command_delete_book(jwt);

        } else if (buffer == "logout") {

            buffer = command_logout(loginCookie);
            if (buffer == "*Done.\n") {
                loginCookie = "";
                jwt = "";
            }
            cout << buffer;

        } else if (buffer == "exit") {

            return 0;

        }

        cout << "\n";

    }
}