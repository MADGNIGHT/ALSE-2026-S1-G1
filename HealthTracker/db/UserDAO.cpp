#include "UserDAO.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>

UserDAO::UserDAO(QSqlDatabase database)
{
    this->database = database;
}

QString UserDAO::hashPassword(const QString& password)
{
    const QByteArray passwordData = password.toUtf8();
    const QByteArray hash = QCryptographicHash::hash(passwordData, QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

bool UserDAO::emailExists(const QString& email)
{
    QSqlQuery query(database);
    query.prepare(R"(
        SELECT id
        FROM users
        WHERE email = :email;
    )");
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Error verificando correo:" << query.lastError().text();
        return false;
    }

    return query.next();
}

bool UserDAO::createUser(const QString& name,
                         const QString& email,
                         const QString& password)
{
    if (emailExists(email)) {
        qDebug() << "El correo ya esta registrado.";
        return false;
    }

    QSqlQuery query(database);
    query.prepare(R"(
        INSERT INTO users (name, email, password_hash)
        VALUES (:name, :email, :password_hash);
    )");

    query.bindValue(":name", name.trimmed().isEmpty() ? email.trimmed() : name.trimmed());
    query.bindValue(":email", email.trimmed());
    query.bindValue(":password_hash", hashPassword(password));

    if (!query.exec()) {
        qDebug() << "Error creando usuario:" << query.lastError().text();
        return false;
    }

    qDebug() << "Usuario creado correctamente.";
    return true;
}

int UserDAO::validateLogin(const QString& email,
                           const QString& password)
{
    QSqlQuery query(database);
    query.prepare(R"(
        SELECT id
        FROM users
        WHERE email = :email
          AND (password_hash = :password_hash OR password_hash = :plain_password)
        LIMIT 1;
    )");

    query.bindValue(":email", email.trimmed());
    query.bindValue(":password_hash", hashPassword(password));
    query.bindValue(":plain_password", password.trimmed());

    if (!query.exec()) {
        qDebug() << "Error validando login:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        return query.value("id").toInt();
    }

    return -1;
}

bool UserDAO::saveUserProfile(int userId,
                              int age,
                              double heightCm,
                              const QString& sex,
                              const QString& activityLevel,
                              const QString& goal)
{
    QSqlQuery query(database);
    query.prepare(R"(
        UPDATE user_profiles
        SET age = :age,
            height_cm = :height_cm,
            sex = :sex,
            activity_level = :activity_level,
            goal = :goal
        WHERE user_id = :user_id;
    )");

    query.bindValue(":user_id", userId);
    query.bindValue(":age", age);
    query.bindValue(":height_cm", heightCm);
    query.bindValue(":sex", sex);
    query.bindValue(":activity_level", activityLevel);
    query.bindValue(":goal", goal);

    if (!query.exec()) {
        qDebug() << "Error actualizando perfil:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() > 0) {
        qDebug() << "Perfil actualizado correctamente.";
        return true;
    }

    query.finish();

    query.prepare(R"(
        INSERT INTO user_profiles (user_id, age, height_cm, sex, activity_level, goal)
        VALUES (:user_id, :age, :height_cm, :sex, :activity_level, :goal);
    )");

    query.bindValue(":user_id", userId);
    query.bindValue(":age", age);
    query.bindValue(":height_cm", heightCm);
    query.bindValue(":sex", sex);
    query.bindValue(":activity_level", activityLevel);
    query.bindValue(":goal", goal);

    if (!query.exec()) {
        qDebug() << "Error guardando perfil:" << query.lastError().text();
        return false;
    }

    qDebug() << "Perfil guardado correctamente.";
    return true;
}

bool UserDAO::loadUserProfile(int userId,
                              int& age,
                              double& heightCm,
                              QString& sex,
                              QString& activityLevel,
                              QString& goal)
{
    QSqlQuery query(database);
    query.prepare(R"(
        SELECT age, height_cm, sex, activity_level, goal
        FROM user_profiles
        WHERE user_id = :user_id
        LIMIT 1;
    )");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "Error cargando perfil:" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        return false;
    }

    age = query.value("age").toInt();
    heightCm = query.value("height_cm").toDouble();
    sex = query.value("sex").toString();
    activityLevel = query.value("activity_level").toString();
    goal = query.value("goal").toString();
    return true;
}

bool UserDAO::getUserName(int userId, QString& name)
{
    QSqlQuery query(database);
    query.prepare(R"(
        SELECT name
        FROM users
        WHERE id = :user_id
        LIMIT 1;
    )");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "Error leyendo usuario:" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        return false;
    }

    name = query.value("name").toString();
    return true;
}
