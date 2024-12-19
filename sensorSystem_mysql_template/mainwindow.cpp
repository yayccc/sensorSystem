// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sensor.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QDateTime>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QtCharts/QScatterSeries>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupDatabase();
    setupModel();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupDatabase()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("sensor_data");//���ӵ����ݿ����� 
    db.setUserName("root");//mysql�û��� 
    db.setPassword("your mysql password");//mysql���� 

    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Failed to connect to database: " + db.lastError().text());
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS sensors (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255), type VARCHAR(50), timestamp DATETIME, value DOUBLE)");
}

void MainWindow::setupModel()
{
    model = new QSqlTableModel(this, db);
    model->setTable("sensors");
    model->select();

    ui->tableView->setModel(model);
}


void MainWindow::on_addButton_clicked()
{
    QString name = ui->nameEdit->text();
    QString type = ui->typeEdit->text();
    QString timestamp = ui->timestampEdit->text();
    double value = ui->valueEdit->text().toDouble();

    QDateTime dateTime = QDateTime::fromString(timestamp, "yyyy-MM-dd HH:mm:ss");
    if (!dateTime.isValid()) {
        QMessageBox::critical(this, "Error", "Invalid timestamp format. Please use 'yyyy-MM-dd HH:mm:ss'.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO sensors (name, type, timestamp, value) VALUES (?, ?, ?, ?)");
    query.addBindValue(name);
    query.addBindValue(type);
    query.addBindValue(timestamp);
    query.addBindValue(value);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to add sensor: " + query.lastError().text());
    } else {
        model->select();
    }
}

void MainWindow::on_deleteButton_clicked()
{
    int row = ui->tableView->currentIndex().row();
    model->removeRow(row);
    model->submitAll();
    model->select();
}

void MainWindow::on_updateButton_clicked()
{
    int row = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(row, 0);
    int id = model->data(index).toInt();

    QString name = ui->nameEdit->text();
    QString type = ui->typeEdit->text();
    QString timestamp = ui->timestampEdit->text();
    double value = ui->valueEdit->text().toDouble();

    QDateTime dateTime = QDateTime::fromString(timestamp, "yyyy-MM-dd HH:mm:ss");
    if (!dateTime.isValid()) {
        QMessageBox::critical(this, "Error", "Invalid timestamp format. Please use 'yyyy-MM-dd HH:mm:ss'.");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE sensors SET name = ?, type = ?, timestamp = ?, value = ? WHERE id = ?");
    query.addBindValue(name);
    query.addBindValue(type);
    query.addBindValue(timestamp);
    query.addBindValue(value);
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update sensor: " + query.lastError().text());
    } else {
        model->select();
    }
}


void MainWindow::on_viewButton_clicked()
{
    QString sensorType = ui->typeComboBox->currentText().toLower();
    QLineSeries *series = new QLineSeries();

    QSqlQuery query;
    query.prepare("SELECT timestamp, value FROM sensors WHERE type = ? ORDER BY timestamp ASC");
    query.addBindValue(sensorType);
    query.exec();

    while (query.next()) {
        QString timestampStr = query.value(0).toString();
        double value = query.value(1).toDouble();
        QDateTime timestamp = QDateTime::fromString(timestampStr, "yyyy-MM-ddTHH:mm:ss.zzz");
        if (timestamp.isValid()) {
            series->append(timestamp.toMSecsSinceEpoch(), value);
        }
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(sensorType.left(1).toUpper() + sensorType.mid(1) + " Sensor Data");

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("yyyy-MM-dd HH:mm");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Value");

    // 根据传感器类型设置Y轴范围
    if (sensorType == "湿度") {
        axisY->setRange(0, 100);
    } else if (sensorType == "温度") {
        axisY->setRange(-20, 40);
    }

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow *chartWindow = new QMainWindow();
    chartWindow->setCentralWidget(chartView);
    chartWindow->resize(800, 600);
    chartWindow->show();
}


void MainWindow::on_importButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open file");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if (fields.size() == 4) {
            QString name = fields[0];
            QString type = fields[1];
            QString timestamp = fields[2];
            double value = fields[3].toDouble();

            QDateTime dateTime = QDateTime::fromString(timestamp, "yyyy-MM-dd HH:mm:ss");
            if (!dateTime.isValid()) {
                QMessageBox::critical(this, "Error", "Invalid timestamp format in file. Please use 'yyyy-MM-dd HH:mm:ss'.");
                return;
            }

            QSqlQuery query;
            query.prepare("INSERT INTO sensors (name, type, timestamp, value) VALUES (?, ?, ?, ?)");
            query.addBindValue(name);
            query.addBindValue(type);
            query.addBindValue(timestamp);
            query.addBindValue(value);

            if (!query.exec()) {
                QMessageBox::critical(this, "Error", "Failed to add sensor: " + query.lastError().text());
                return;
            }
        }
    }

    file.close();
    model->select(); // 刷新表格视图
    QMessageBox::information(this, "Success", "Data imported successfully");
}


void MainWindow::on_filterButton_clicked()
{
    QString sensorType = ui->typeComboBox->currentText().toLower();
    QLineSeries *series = new QLineSeries();

    QDateTime startTime = ui->startTimeEdit->dateTime();
    QDateTime endTime = ui->endTimeEdit->dateTime();

    QSqlQuery query;
    query.prepare("SELECT timestamp, value FROM sensors WHERE type = ? AND timestamp BETWEEN ? AND ? ORDER BY timestamp ASC");
    query.addBindValue(sensorType);
    query.addBindValue(startTime.toString("yyyy-MM-ddTHH:mm:ss.zzz"));
    query.addBindValue(endTime.toString("yyyy-MM-ddTHH:mm:ss.zzz"));
    query.exec();

    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::min();
    QDateTime minTimestamp, maxTimestamp;

    while (query.next()) {
        QString timestampStr = query.value(0).toString();
        double value = query.value(1).toDouble();
        QDateTime timestamp = QDateTime::fromString(timestampStr, "yyyy-MM-ddTHH:mm:ss.zzz");
        if (timestamp.isValid()) {
            series->append(timestamp.toMSecsSinceEpoch(), value);
            if (value < minValue) {
                minValue = value;
                minTimestamp = timestamp;
            }
            if (value > maxValue) {
                maxValue = value;
                maxTimestamp = timestamp;
            }
        } else {
            qDebug() << "Invalid timestamp:" << timestampStr;
        }
    }

    if (series->count() == 0) {
        QMessageBox::information(this, "No Data", "No data available for the selected time range and sensor type.");
        return;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(sensorType.left(1).toUpper() + sensorType.mid(1) + " Sensor Data");

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("yyyy-MM-dd HH:mm");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Value");

    // 根据传感器类型设置Y轴范围
    if (sensorType == "湿度") {
        axisY->setRange(0, 100);
    } else if (sensorType == "温度") {
        axisY->setRange(-20, 40);
    }

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);



    // 添加最大值和最小值的标记
    QScatterSeries *minSeries = new QScatterSeries();
    minSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    minSeries->setMarkerSize(10.0);
    minSeries->append(minTimestamp.toMSecsSinceEpoch(), minValue);
    minSeries->setName(QString("Min: %1").arg(minValue));
    chart->addSeries(minSeries);
    minSeries->attachAxis(axisX);
    minSeries->attachAxis(axisY);

    QScatterSeries *maxSeries = new QScatterSeries();
    maxSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    maxSeries->setMarkerSize(10.0);
    maxSeries->append(maxTimestamp.toMSecsSinceEpoch(), maxValue);
    maxSeries->setName(QString("Max: %1").arg(maxValue));
    chart->addSeries(maxSeries);
    maxSeries->attachAxis(axisX);
    maxSeries->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow *chartWindow = new QMainWindow();
    chartWindow->setCentralWidget(chartView);
    chartWindow->resize(800, 600);
    chartWindow->show();
}

void MainWindow::on_applyFilterButton_clicked()
{
    QString filterType = ui->filterTypeComboBox->currentText();

    if (filterType == "全部") {
        model->setFilter(""); // 显示所有传感器
    } else {
        QString type;
        if (filterType == "温度") {
            type = "温度";
        } else if (filterType == "湿度") {
            type = "湿度";
        }
        model->setFilter(QString("type = '%1'").arg(type)); // 根据选择的类型过滤传感器
    }

    model->select(); // 刷新表格视图
}


void MainWindow::on_clearDataButton_clicked()
{
    QSqlQuery query;
    if (!query.exec("DELETE FROM sensors")) {
        QMessageBox::critical(this, "Error", "Failed to clear data: " + query.lastError().text());
    } else {
        model->select(); // 刷新表格视图
        QMessageBox::information(this, "Success", "Data cleared successfully");
    }
}
