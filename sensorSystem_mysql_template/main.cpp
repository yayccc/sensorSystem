#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("sensor_data");
    db.setUserName("root");
    db.setPassword("560320");

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Error", "Failed to connect to database: " + db.lastError().text());
        return -1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
