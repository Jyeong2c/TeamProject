QT += network widgets httpserver network

HEADERS += httpwindow.h \
    #filetranferdlg.h \
    filesenderdlg.h \
    idnamebody.h \
    #tcpserver.h \
    tcpserverdlg.h
SOURCES += httpwindow.cpp \
           #filetranferdlg.cpp \
           filesenderdlg.cpp \
           idnamebody.cpp \
           main.cpp \
           #tcpserver.cpp \
           tcpserverdlg.cpp
FORMS += authenticationdialog.ui \
    #tcpserver.ui \
    #filetranferdlg.ui \
    filesenderdlg.ui \
    tcpserverdlg.ui

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/http
INSTALLS += target
