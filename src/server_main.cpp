#include <iostream>
#include "ChatServer.h"
#include <cstdlib>


int main(int argc, char* argv[]) {
    int port = 8080; //default port

    if(argc > 1) {
        port = std::atoi(argv[1]); //get port from command line argument
    }

    try {
        std::cout << "Starting Chat Server on port " << port << "..." << std::endl;
        
        // server manager 
        ChatServer server(port);

        //  starts the logic loop
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}