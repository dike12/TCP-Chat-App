#include <Socket.h>

Socket::Socket(int fd) : sockfd(fd) {
    if (sockfd < 0) {
        throw std::runtime_error("Invalid file descriptor");
    }
}

Socket::Socket() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        throw std::runtime_error("Failed to create socket");
    }
}


Socket::~Socket() {
    if (sockfd >= 0) {
        close(sockfd);
    }
}

Socket::Socket(Socket&& other) noexcept : sockfd(other.sockfd) {
    other.sockfd = -1; // Invalidate the moved-from socket
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        if (sockfd >= 0) {
            close(sockfd);
        }
        sockfd = other.sockfd;
        other.sockfd = -1; // Invalidate the moved-from socket
    }
    return *this;
}