/*

*/
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <map>

static const std::map<std::string, std::string> my_map = {
    {"30117194Cupcake", "1"},
    {"30111234Program", "2"},
    {"30118890Howdy", "3"}};

int main()
{
    /*
    If correct user_pass -> allocate user secret information
    When someone puts in the wrong password more than 3 times check is_valid and print out previous user secret information
    */
    std::string user_pass;
    std::cout << "Welcome to Student Information Login.\n";
    int attempts = 0;

    while(true) {
        bool * is_valid = new bool(false);
        while(attempts < 3) {
            std::cout << "Please input your UCID + Secret Key: ";
            std::cin >> user_pass;
            std::cout << user_pass << std::endl;

            if(my_map.find(user_pass) != my_map.end()) {
                *is_valid = true;
                std::cout << my_map.at(user_pass) << std::endl;
                break;
            }
            else {
                std::cout << "Your UCID + Secret Key does not match our records. Try again." << std::endl;
                attempts++;
            }
        }

        if(attempts >= 3){
            std::cout << "You have attempted an incorrect UCID + Secret Key too many times. Try again later" << std::endl;
            attempts = 0;
            delete is_valid;
            bool * attempts_greater = new bool(true);
            std::cout << *is_valid << std::endl;
            std::cout << *attempts_greater << std::endl;
        }

        if(*is_valid) {
            std::cout << "Secret" << std::endl;
            //show information
        }
    }

    // prompt user to log out (misuse free)
}