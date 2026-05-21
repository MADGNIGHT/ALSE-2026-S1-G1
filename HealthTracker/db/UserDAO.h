#ifndef USERDAO_H
#define USERDAO_H

#include <QString>
#include <QSqlDatabase>

class UserDAO
{
public:
    UserDAO(QSqlDatabase database);

    bool createUser(const QString& name,
                    const QString& email,
                    const QString& password);

    int validateLogin(const QString& email,
                      const QString& password);

    bool emailExists(const QString& email);

    bool saveUserProfile(int userId,
                         int age,
                         double heightCm,
                         const QString& sex,
                         const QString& activityLevel,
                         const QString& goal);

    bool loadUserProfile(int userId,
                         int& age,
                         double& heightCm,
                         QString& sex,
                         QString& activityLevel,
                         QString& goal);

    bool getUserName(int userId, QString& name);

private:
    QSqlDatabase database;

    QString hashPassword(const QString& password);
};

#endif
