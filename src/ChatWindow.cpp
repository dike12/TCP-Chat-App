#include "ChatWindow.h"
#include <QVBoxLayout>

ChatWindow::ChatWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
}

ChatWindow::~ChatWindow() {
    // Qt will automatically delete child widgets
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