#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

#include "Socket.h"
#include <atomic>
#include <thread>


class ChatWindow : public QWidget {
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

signals:
    void msgReceived(QString msg);

private slots:
    void onSendClicked();
    void onMessageReceived(QString msg);

private:
    QTextEdit *chatDisplay;
    QLineEdit *inputField;
    QPushButton *sendButton;
    Socket socket;

    std::thread receiverThread;
    std::atomic<bool> running;

    void setupUI();
    void connectToServer();

    void receiveLoop();
};