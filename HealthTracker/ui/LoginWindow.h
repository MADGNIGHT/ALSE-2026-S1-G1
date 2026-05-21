#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccess(int userId);

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    QLabel      *lblTitle;
    QLabel      *lblName;
    QLabel      *lblEmail;
    QLabel      *lblPassword;
    QLabel      *lblMessage;

    QLineEdit   *inputName;
    QLineEdit   *inputEmail;
    QLineEdit   *inputPassword;

    QPushButton *btnLogin;
    QPushButton *btnRegister;

    void setupUI();
};

#endif // LOGINWINDOW_H
