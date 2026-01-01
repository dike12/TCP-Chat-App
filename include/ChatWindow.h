#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>


class ChatWindow : public QWidget {
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
    QTextEdit *chatDisplay;
    QLineEdit *inputField;
    QPushButton *sendButton;


    void setupUI();
};