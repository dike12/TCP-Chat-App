#pragma once

#include <sqlite3.h> 
#include <string>
#include <vector>
#include <iostream>

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    // Connects to the DB file and creates tables if missing
    bool open(const std::string& dbPath);

    // Saves a message to the "messages" table
    void saveMessage(int senderId, const std::string& message);

    // Returns all past messages
    std::vector<std::string> getHistory();

private:
    sqlite3* db; // Points to the open database connection
};