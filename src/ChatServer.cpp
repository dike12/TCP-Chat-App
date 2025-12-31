#include <ChatServer.h>
#include <Socket.h>
#include <iostream>

ChatServer::ChatServer(int port) : port(port) {
    // Initialize the database
    if(!db.open("chat_history.db")){
        std::cerr << "Warning: Could not open database!" << std::endl;
    }

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
    //this creates a new Socket object for the incoming connection for this client
    Socket new_client = serverSocket.accept();

    //get the FD
    int client_fd = new_client.getFD();

    //add to master set
    FD_SET(client_fd, &masterSet);

    //if FD is higher than max_fd, update max_fd
    if(client_fd > max_fd) {
        max_fd = client_fd;
    }

    //move the socket into the clients map
    clients.emplace(client_fd, std::move(new_client));

    std::cout << "New connection from Client " << client_fd << std::endl;

    //Send a welcome message
    clients[client_fd].send("Welcome to the Chat Server!\n");
}

void ChatServer::handleClientMessage(int client_fd) {
    char buffer[4096]; //4KB buffer - a container for the incoming message
    std::fill(buffer, buffer + sizeof(buffer), 0); //clear the buffer

    try
    {
        //read data
        ssize_t bytes_received = clients[client_fd].recv(buffer, sizeof(buffer));

        //check for disconnection
        if(bytes_received == 0) {
            std::cout << "Client " << client_fd << " disconnected." << std::endl;
            
            //remove from master set so select() stops watching it
            FD_CLR(client_fd, &masterSet); 

            clients.erase(client_fd); //remove from clients map
            return;
        }

        // process the message
        std::string message(buffer, bytes_received);
        std::cout << "Received message from Client " << client_fd << ": " << message;

        // save the message to the database
        db.saveMessage(client_fd, message);

        // broadcast the message to other clients
        broadcastMessage(client_fd, message);
    }
    catch(const std::exception& e)
    {
        //if recv fails (e.g connection reset), treat it as a disconnection
        std::cerr << "Error with client " << client_fd << ": " << e.what() << std::endl;
        FD_CLR(client_fd, &masterSet);
        clients.erase(client_fd);
    }

    
}

    void ChatServer::broadcastMessage(int sender_fd, const std::string& message) {
        // Construct final string
        std::string final_msg = "Client " + std::to_string(sender_fd) + ": " + message;

        // loop through all clients - pair.firsrt is the fd, pair.second is the Socket
        for(auto& pair : clients){
            int target_fd = pair.first;
            Socket& target_socket = pair.second;

            //dont send to the sender
            if(target_fd != sender_fd) {
                try
                {
                    target_socket.send(final_msg);
                }
                catch(const std::exception& e)
                {
                    std::cerr << "Failed to send to client " << target_fd << std::endl;
                }
                
            }
        }
}