#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager()
{
}

bool DatabaseManager::openDatabase(const QString& path)
{
    if (QSqlDatabase::contains("healthtracker_connection")) {
        database = QSqlDatabase::database("healthtracker_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE", "healthtracker_connection");
    }

    database.setDatabaseName(path);

    if (!database.open()) {
        qDebug() << "Error al abrir la base de datos:" << database.lastError().text();
        return false;
    }

    QSqlQuery query(database);
    query.exec("PRAGMA foreign_keys = ON;");

    qDebug() << "Base de datos creada o abierta correctamente.";
    return true;
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(database);

    const QString usersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            email TEXT NOT NULL UNIQUE,
            password_hash TEXT NOT NULL,
            created_at TEXT DEFAULT CURRENT_TIMESTAMP
        );
    )";

    if (!query.exec(usersTable)) {
        qDebug() << "Error creando tabla users:" << query.lastError().text();
        return false;
    }

    const QString userProfilesTable = R"(
        CREATE TABLE IF NOT EXISTS user_profiles (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL UNIQUE,
            age INTEGER,
            height_cm REAL,
            sex TEXT,
            activity_level TEXT,
            goal TEXT,
            FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE
        );
    )";

    if (!query.exec(userProfilesTable)) {
        qDebug() << "Error creando tabla user_profiles:" << query.lastError().text();
        return false;
    }

    const QString weightLogsTable = R"(
        CREATE TABLE IF NOT EXISTS weight_logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            weight_kg REAL NOT NULL,
            measured_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE
        );
    )";

    if (!query.exec(weightLogsTable)) {
        qDebug() << "Error creando tabla weight_logs:" << query.lastError().text();
        return false;
    }

    const QString bloodPressureLogsTable = R"(
        CREATE TABLE IF NOT EXISTS blood_pressure_logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            systolic INTEGER NOT NULL,
            diastolic INTEGER NOT NULL,
            measured_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE
        );
    )";

    if (!query.exec(bloodPressureLogsTable)) {
        qDebug() << "Error creando tabla blood_pressure_logs:" << query.lastError().text();
        return false;
    }

    const QString glucoseLogsTable = R"(
        CREATE TABLE IF NOT EXISTS glucose_logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            glucose_mg_dl REAL NOT NULL,
            measured_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE
        );
    )";

    if (!query.exec(glucoseLogsTable)) {
        qDebug() << "Error creando tabla glucose_logs:" << query.lastError().text();
        return false;
    }

    qDebug() << "Tablas creadas correctamente.";
    return true;
}

bool DatabaseManager::insertTestData()
{
    QSqlQuery query(database);

    query.prepare(R"(
        INSERT OR IGNORE INTO users (id, name, email, password_hash)
        VALUES (1, :name, :email, :password_hash);
    )");

    query.bindValue(":name", "Cesar Carranza");
    query.bindValue(":email", "cesar@email.com");
    query.bindValue(":password_hash", "1234_hash");

    if (!query.exec()) {
        qDebug() << "Error insertando usuario:" << query.lastError().text();
        return false;
    }

    QSqlQuery weightQuery(database);
    weightQuery.prepare(R"(
        INSERT INTO weight_logs (user_id, weight_kg)
        VALUES (1, 70.5);
    )");

    if (!weightQuery.exec()) {
        qDebug() << "Error insertando peso:" << weightQuery.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::showTestData()
{
    return true;
}

void DatabaseManager::closeDatabase()
{
    const QString connectionName = database.connectionName();
    database.close();
    database = QSqlDatabase();
    if (!connectionName.isEmpty()) {
        QSqlDatabase::removeDatabase(connectionName);
    }
}

QSqlDatabase DatabaseManager::getDatabase() const
{
    return database;
}
