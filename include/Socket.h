#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <stdexcept>
#include <unistd.h>

class Socket {
    private:
        int sockfd;
        explicit Socket(int fd); // Private constructor for internal use

    public:
        // 1. Lifecycle
        Socket(); 
        ~Socket(); 

        // 2. Memory Safety (Rule of 5)
        Socket(const Socket&) = delete;            // No Copying
        Socket& operator=(const Socket&) = delete; // No Copy Assignment
        
        Socket(Socket&& other) noexcept;            // Yes Moving
        Socket& operator=(Socket&& other) noexcept; // Yes Move Assignment

        void bind(int port); // Bind method

        void listen(int backlog = 5); // Listen method

        void connect(const std::string& host, int port); // Connect method

        Socket accept(); // Accept method

        int getFD() const {
                return sockfd;
            }


        // Sends a string to the connected socket
        // Returns number of bytes sent
        ssize_t send(const std::string& message);

        // Receives raw bytes into a buffer
        // Returns number of bytes received (0 means disconnected)
        ssize_t recv(char* buffer, size_t size);
};
