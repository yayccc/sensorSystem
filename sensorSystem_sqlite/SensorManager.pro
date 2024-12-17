QT += core gui sql
CONFIG += c++11

# 添加Qt Charts模块
QT += charts

# 添加SQLite库
# LIBS += -lsqlite3

SOURCES += main.cpp \
    mainwindow.cpp \
    sensor.cpp \
    sensor.cpp

HEADERS += mainwindow.h \
    sensor.h \
    sensor.h

FORMS += mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
