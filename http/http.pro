QT += network widgets httpserver

HEADERS += httpwindow.h \
    idnamebody.h \
    #tcpserver.h \
    tcpserverdlg.h
SOURCES += httpwindow.cpp \
           idnamebody.cpp \
           main.cpp \
           #tcpserver.cpp \
           tcpserverdlg.cpp
FORMS += authenticationdialog.ui \
    #tcpserver.ui \
    tcpserverdlg.ui

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/http
INSTALLS += target
