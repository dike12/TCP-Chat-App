#include <ChatServer.h>
#include <Socket.h>
#include <iostream>

ChatServer::ChatServer(int port) : port(port) {
    //FD_ZERO initializes the file descriptor sets to be empty
    FD_ZERO(&masterSet);
    FD_ZERO(&readSet);


    // We bind to the port and start listening
    serverSocket.bind(port);
    serverSocket.listen();


    //FD_SET turn on the bit for the server socket in this socker ID
    //serverSocket.getFD() gets the underlying file descriptor integer
    FD_SET(serverSocket.getFD(), &masterSet);

    max_fd = serverSocket.getFD(); //initially the max fd is the server socket

    std::cout << "Server started on port " << port << std::endl;
}

ChatServer::~ChatServer() {
    // Destructor - sockets will be closed automatically by their destructors
}

void ChatServer::run() {
    std::cout << "Server is running and waiting for connections..." << std::endl;

    while (true) {
        //Select() is destructive. It erases sockets that didnt have activity
        //so we copy the masterSet to the readSet each time
        readSet = masterSet;


        //select pasues the program until there is activity on one of the sockets
        if(select(max_fd + 1, &readSet, nullptr, nullptr, nullptr) < 0) {
            throw std::runtime_error("Select call failed");
        }

        //we loop through every possible ID to check
        for(int i = 0; i <= max_fd; i++){
            if(FD_ISSET(i, &readSet)) { //if this socket had activity
                if(i == serverSocket.getFD()) {
                    //Activity on the master server socket means a new connection
                    handleNewConnection();
                } else {
                    //Activity on a client socket means a message from that client
                    handleClientMessage(i);
                }
            }
        }
    }
}

void ChatServer::handleNewConnection() {
    std::cout << "Someone is trying to connect (logic coming soon)..." << std::endl;
}

void ChatServer::handleClientMessage(int client_fd) {
    std::cout << "Client " << client_fd << " sent a message (Logic coming soon)" << std::endl;
}

void ChatServer::broadcastMessage(int sender_fd, const std::string& message) {
    // Logic coming soon
}