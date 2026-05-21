#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>

class DatabaseManager
{
public:
    DatabaseManager();

    bool openDatabase(const QString& path = "health_tracker.db");
    bool createTables();
    bool insertTestData();
    bool showTestData();
    void closeDatabase();

    QSqlDatabase getDatabase() const;

private:
    QSqlDatabase database;
};

#endif
