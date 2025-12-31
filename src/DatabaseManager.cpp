#include "DatabaseManager.h"

DatabaseManager::DatabaseManager() : db(nullptr) {}

DatabaseManager::~DatabaseManager() {
    if(db){
        sqlite3_close(db);
        std::cout << "Database connection closed." << std::endl;
    }
}

bool DatabaseManager::open(const std::string& dbPath){
    //open the file
    int exit = sqlite3_open(dbPath.c_str(), &db);
    if(exit != SQLITE_OK){
        std::cerr << "Error opening DB: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    //create the table if it doesn't exist
    std::string sql = "CREATE TABLE IF NOT EXISTS messages ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "sender_id INTEGER, "
                        "message TEXT, "
                        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";

    char* errorMessage;
    exit = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errorMessage);

    if(exit != SQLITE_OK){
        std::cerr << "Error creating table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }

    std::cout << "Database ready." << std::endl;
    return true;
}

void DatabaseManager::saveMessage(int senderId, const std::string& message){
    std::string sql = "INSERT INTO messages (sender_id, message) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    // prepare the statement
    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        std::cerr << "Error preparing insert statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    //bind the values
    sqlite3_bind_int(stmt, 1, senderId);
    sqlite3_bind_text(stmt, 2, message.c_str(), -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE){
        std::cerr << "Failed to insert message: " << sqlite3_errmsg(db) << std::endl;
    }

    //cleanup
    sqlite3_finalize(stmt);
}

