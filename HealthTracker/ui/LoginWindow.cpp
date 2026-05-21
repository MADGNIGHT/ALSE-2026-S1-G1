#include "LoginWindow.h"
#include "../db/UserDAO.h"

#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void LoginWindow::setupUI()
{
    lblTitle    = new QLabel("HealthTracker");
    lblName     = new QLabel("Name (for registration):");
    lblEmail    = new QLabel("Email:");
    lblPassword = new QLabel("Password:");
    lblMessage  = new QLabel("");

    inputName = new QLineEdit();
    inputName->setPlaceholderText("Your name");

    inputEmail = new QLineEdit();
    inputEmail->setPlaceholderText("example@email.com");

    inputPassword = new QLineEdit();
    inputPassword->setPlaceholderText("Your password");
    inputPassword->setEchoMode(QLineEdit::Password);

    btnLogin    = new QPushButton("Log In");
    btnRegister = new QPushButton("Register");

    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: #1F4E79; margin-bottom: 8px;"
    );

    lblName->setStyleSheet("color: #333333;");
    lblEmail->setStyleSheet("color: #333333;");
    lblPassword->setStyleSheet("color: #333333;");

    lblMessage->setAlignment(Qt::AlignCenter);
    lblMessage->setWordWrap(true);

    const QString inputStyle =
        "QLineEdit { border: 1px solid #CCCCCC; border-radius: 4px;"
        "padding: 6px 10px; font-size: 13px; background-color: white; color: black; }"
        "QLineEdit:focus { border-color: #2E75B6; }";

    inputName->setStyleSheet(inputStyle);
    inputEmail->setStyleSheet(inputStyle);
    inputPassword->setStyleSheet(inputStyle);

    btnLogin->setStyleSheet(
        "QPushButton { background-color: #2E75B6; color: white;"
        "padding: 9px; border-radius: 4px; font-size: 14px; }"
        "QPushButton:hover { background-color: #1F4E79; }"
    );

    btnRegister->setStyleSheet(
        "QPushButton { background-color: #EEEEEE; color: #333333;"
        "padding: 9px; border-radius: 4px; font-size: 14px; }"
        "QPushButton:hover { background-color: #CCCCCC; }"
    );

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(8);

    layout->addWidget(lblTitle);
    layout->addSpacing(10);
    layout->addWidget(lblName);
    layout->addWidget(inputName);
    layout->addWidget(lblEmail);
    layout->addWidget(inputEmail);
    layout->addWidget(lblPassword);
    layout->addWidget(inputPassword);
    layout->addSpacing(12);
    layout->addWidget(btnLogin);
    layout->addWidget(btnRegister);
    layout->addSpacing(6);
    layout->addWidget(lblMessage);

    setLayout(layout);
    setWindowTitle("HealthTracker — Login");
    setFixedSize(360, 460);
    setStyleSheet("background-color: #FFFFFF;");

    connect(btnLogin,      &QPushButton::clicked,     this, &LoginWindow::onLoginClicked);
    connect(btnRegister,   &QPushButton::clicked,     this, &LoginWindow::onRegisterClicked);
    connect(inputPassword, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked()
{
    const QString email    = inputEmail->text().trimmed();
    const QString password = inputPassword->text().trimmed();

    if (email.isEmpty() || password.isEmpty()) {
        lblMessage->setStyleSheet("color: #CC0000;");
        lblMessage->setText("Please fill in email and password.");
        return;
    }

    UserDAO userDao(QSqlDatabase::database("healthtracker_connection"));
    const int userId = userDao.validateLogin(email, password);

    if (userId >= 0) {
        emit loginSuccess(userId);
        return;
    }

    lblMessage->setStyleSheet("color: #CC0000;");
    lblMessage->setText("Incorrect email or password.");
    inputPassword->clear();
}

void LoginWindow::onRegisterClicked()
{
    const QString name     = inputName->text().trimmed();
    const QString email    = inputEmail->text().trimmed();
    const QString password = inputPassword->text().trimmed();

    if (name.isEmpty() || email.isEmpty() || password.isEmpty()) {
        lblMessage->setStyleSheet("color: #CC0000;");
        lblMessage->setText("Please fill in name, email and password.");
        return;
    }

    UserDAO userDao(QSqlDatabase::database("healthtracker_connection"));

    if (userDao.createUser(name, email, password)) {
        lblMessage->setStyleSheet("color: #1A7A1A;");
        lblMessage->setText("Account created. You can now log in.");
        inputPassword->clear();
        return;
    }

    lblMessage->setStyleSheet("color: #CC0000;");
    lblMessage->setText("Could not create account. Email may already exist.");
}
