#include "ChatWindow.h"
#include <QVBoxLayout>
#include <iostream>
#include <QMessageBox>


ChatWindow::ChatWindow(QWidget *parent) : QWidget(parent), running(true) {
    setupUI();

    //connect interna;l signal to UI update slot
    connect(this, &ChatWindow::msgReceived, this, &ChatWindow::onMessageReceived, Qt::QueuedConnection);

    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::onSendClicked);
    connect(inputField, &QLineEdit::returnPressed, this, &ChatWindow::onSendClicked);
    
    connectToServer();
}

ChatWindow::~ChatWindow() {
    // Qt will automatically delete child widgets

    running = false;
    if(receiverThread.joinable()) {
        receiverThread.detach();
    }
}

void ChatWindow::connectToServer() {
    try
    {
        chatDisplay->append("Connecting to server...");
        socket.connect("127.0.0.1", 8080);
        chatDisplay->append("Connected!");

        receiverThread = std::thread(&ChatWindow::receiveLoop, this);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        QMessageBox::critical(this, "Connection Error", e.what());
    }
    
}


void ChatWindow::setupUI() {
    setWindowTitle("TCP Chat Client");
    resize(500,400);

    // create Widgets
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true); //user should not edit chat display (history)

    inputField = new QLineEdit(this);
    inputField->setPlaceholderText("Type your message here...");

    sendButton = new QPushButton("Send", this);

    // botton row (input field + send button)
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(inputField);
    bottomLayout->addWidget(sendButton);

    // main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(chatDisplay);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
}

void ChatWindow::receiveLoop() {
    char buffer[4096];
    while(running){
        try
        {
            ssize_t bytes = socket.recv(buffer, 4096);

            if(bytes <= 0){
                emit msgReceived("System: Disconnected from server.");
                running = false;
                break;
            }

            //emit the signal to the main thread
            std::string msg(buffer, bytes);
            emit msgReceived(QString::fromStdString(msg));
        }
        catch(...)
        {
           running = false;
           break;
        }
    }
}

void ChatWindow::onMessageReceived(QString message) {
    chatDisplay->append(message);
}

void ChatWindow::onSendClicked(){
    QString text = inputField->text();
    if(text.isEmpty()) return;

    try{
        //convert Qt String to std::string and send
        socket.send(text.toStdString());

        //add own message to chat display
        chatDisplay->append("Me: " + text);

        //clear input field
        inputField->clear();
    } catch(const std::exception& e){
        chatDisplay->append("Error sending: " + QString(e.what()));
    }
}