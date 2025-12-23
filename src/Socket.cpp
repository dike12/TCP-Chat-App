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



void Socket::bind(int port){
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    if(::bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        throw std::runtime_error("Bind failed");
    }
}

void Socket::listen(int backlog) {
    if (::listen(sockfd, backlog) < 0) {
        throw std::runtime_error("Listen failed");
    }
}



Socket Socket::accept() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = ::accept(sockfd, (struct sockaddr*)&client_addr, &client_len);

    if(client_fd < 0){
        throw std::runtime_error("Accept failed");
    }

    return Socket(client_fd);
}