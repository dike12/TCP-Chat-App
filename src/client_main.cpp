#include <iostream>
#include "Socket.h"
#include <thread>
#include <atomic>
#include <string>

// a flag to tell the threads when  to stop
std::atomic<bool> running(true);

//this is the ear thread, it runs in parallel to listen for incoming messages
void receive_messages(Socket* socket) {
   char buffer[4096];

   while(running){
        try{
            //this blocks until data arrives, but it doesnt block the main thread
            ssize_t bytes = socket->recv(buffer, 4096);

            if(bytes == 0){
                std::cout << "\nServer disconnected.\n";
                running = false;
                break;
            }

            //print the incoming message
            std::string msg(buffer, bytes);
            // \r clears the current line so the prompt ">" doesn't get messed up
            std::cout << "\r" << msg << "\n> " << std::flush;

        }
        catch(...){
            running = false;
            break;
        }
    
   }
}

//this is the mouth(main) thread, it runs in parallel to send messages
int main(int argc, char* argv[]) {
    if(argc < 3){
        std::cout << "Usage: " << argv[0] << " <IP> <PORT>" << std::endl;
        return 1;
    }

    std::string ip = argv[1];
    int port = std::stoi(argv[2]);

    try{
        //create and connect the socket
        Socket socket;
        socket.connect(ip, port);
        std::cout << "Connected to server! Type '/exit' to quit." << std::endl;

        //launch the ear thread
        std::thread receiverThread(receive_messages, &socket);

        //main loop for sending messages
        std::string line;
        while(running){
            std::cout << "> " << std::flush;

            // This blocks waiting for you to type...
            if (!std::getline(std::cin, line)) break; 
            
            if (line == "/exit") {
                running = false;
                break; // Break the loop to close the program
            }

            socket.send(line);
        }
        
        //clean up - we close the socket and wait for the ear thread to finish
        if (receiverThread.joinable()) {
            receiverThread.detach(); // Allow the thread to die on its own
        }
    }catch(const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}