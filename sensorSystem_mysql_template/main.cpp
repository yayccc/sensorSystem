#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");//ʹ�ñ������ݿ� 
    db.setDatabaseName("sensor_data");//���ӵ����ݿ����֣���Ҫ��mysql����ǰ���� 
    db.setUserName("root");//mysql�û���Ĭ����root 
    db.setPassword("your MySql password");//�ĳ����mysql���� 

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Error", "Failed to connect to database: " + db.lastError().text());
        return -1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
