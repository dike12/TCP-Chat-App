
## Tech Stack & Architecture

| Component | Technology | Description |
| :--- | :--- | :--- |
| **Language** | C++17 | Core logic and memory management. |
| **Networking** | Berkeley Sockets | Raw socket programming (`<sys/socket.h>`). |
| **Server Model** | Single-Threaded Multiplexing | Uses `fd_set` and `select()` to manage connection states. |
| **Database** | SQLite3 | Embedded SQL engine for message persistence. |
| **GUI Framework** | Qt5 (Widgets) | Frontend interface for the client. |
| **Build System** | CMake | Cross-platform build configuration. |

### Architecture Overview
1.  **The Server:** Listens on a specified port. When data arrives, it broadcasts the message to all other connected file descriptors (clients) and commits the log to `chat_history.db`.
2.  **The Client (GUI):** Spawns a background `std::thread` to listen for incoming packets. When a message arrives, it emits a Qt Signal to update the main UI thread safely.

## Installation & Build

### Prerequisites (Linux/Fedora)
Ensure you have a C++ compiler, CMake, and the necessary development libraries.

```bash
# Install dependencies on Fedora
sudo dnf install gcc-c++ cmake sqlite-devel qt5-qtbase-devel

# 1. Clone the repository
git clone [https://github.com/dike12/TCP-Chat-App.gitt](https://github.com/dike12/TCP-Chat-App.git)
cd TCP-Chat-App

# 2. Create build directory
mkdir build && cd build

# 3. Compile
cmake ..
make 
```


## Usage
### 1. Start the Server
Run the server first in a dedicated terminal window. You must specify the port number.
```bash
./chat_server 8080
# Output: Server listening on port 8080...
```

### 2. Start the Client (GUI)

```bash
./gui_client
```

**Automatic Connection**: The client automatically attempts to connect to 127.0.0.1 on port 8080.

**Messaging**: Type a message in the input field and press Enter or click Send.


### You Don't neet this, but if you prefer the CLI client

```bash
./chat_client 127.0.0.1 8080
```


### Example
![TCP Chat App Demo](TCP-Chat-App/media/app screenshot.png)