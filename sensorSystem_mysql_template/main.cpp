#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");//使用本地数据库 
    db.setDatabaseName("sensor_data");//连接的数据库名字，需要在mysql里提前创建 
    db.setUserName("root");//mysql用户名默认是root 
    db.setPassword("your MySql password");//改成你的mysql密码 

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Error", "Failed to connect to database: " + db.lastError().text());
        return -1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
