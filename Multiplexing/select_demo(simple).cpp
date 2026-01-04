#include <iostream>
#include <cstdio>
#include <sys/select.h>
#include <unistd.h>

int main() {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds); // Monitor stdin (file descriptor 0)

    struct timeval timeout;
    timeout.tv_sec = 5;  // 5 seconds timeout
    timeout.tv_usec = 0;

    std::cout << "Waiting for input (5 seconds timeout)...\n";

    int activity = select(1, &readfds, NULL, NULL, &timeout);

    if (activity < 0) {
        std::cerr << "Select error\n";
        return 1;
    } else if (activity == 0) {
        std::cout << "Timeout occurred! No input received.\n";
    } else {
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            std::string input;
            std::getline(std::cin, input);
            std::cout << "You entered: " << input << "\n";
        }
    }

    return 0;
}