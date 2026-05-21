#include <QApplication>
#include <QDebug>

#include "db/DatabaseManager.h"
#include "ui/LoginWindow.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DatabaseManager db;

    if (!db.openDatabase("health_tracker.db")) {
        qDebug() << "Could not open database.";
        return 1;
    }

    if (!db.createTables()) {
        qDebug() << "Could not create tables.";
        db.closeDatabase();
        return 1;
    }

    LoginWindow loginWindow;

    QObject::connect(&loginWindow, &LoginWindow::loginSuccess, [&loginWindow](int userId) {
        auto *tracker = new MainWindow(userId);
        tracker->setAttribute(Qt::WA_DeleteOnClose);

        QObject::connect(tracker, &QObject::destroyed, &loginWindow, [&loginWindow]() {
            loginWindow.show();
            loginWindow.raise();
            loginWindow.activateWindow();
        });

        loginWindow.hide();
        tracker->show();
    });

    loginWindow.show();

    const int result = app.exec();
    db.closeDatabase();
    return result;
}
