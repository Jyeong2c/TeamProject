QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    imagemanagementform.cpp \
    imageview.cpp \
    main.cpp \
    mainwindow.cpp \
    memoalbum.cpp \
    patientcareform.cpp

HEADERS += \
    imagemanagementform.h \
    imageview.h \
    mainwindow.h \
    memoalbum.h \
    patientcareform.h

FORMS += \
    imagemanagementform.ui \
    mainwindow.ui \
    patientcareform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    picture.qrc
