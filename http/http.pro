QT += network widgets httpserver

HEADERS += httpwindow.h \
    idnamebody.h
SOURCES += httpwindow.cpp \
           idnamebody.cpp \
           main.cpp
FORMS += authenticationdialog.ui

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/http
INSTALLS += target
