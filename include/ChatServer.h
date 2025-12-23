#pragma once
#include "Socket.h"
#include <map>
#include <sys/select.h>
#include <string>
#include <vector>


class ChatServer {
    public:
        ChatServer(int port);  //constructor, just need port number to start listening
        
        ~ChatServer(); //destructor

        void run(); //main server loop to accept and handle clients - runs forever(well...)

    private:
        Socket serverSocket; // master socker - listens for incoming connections
        int port; //port number to listen on

        std::map<int, Socket> clients; //map of client sockets keyed by their file descriptor

        fd_set masterSet; //master file descriptor set (list of everyone we are watching)
        fd_set readSet;   //temp file descriptor set for select()

        int max_fd; //maximum file descriptor number

        void handleNewConnection(); //accept new client connections
        void handleClientMessage(int client_fd); //handle messages from clients
        void broadcastMessage(int sender_fd, const std::string& message); //send message to all clients except sender
};