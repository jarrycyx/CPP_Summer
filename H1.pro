QT += quick
CONFIG += c++11

QT += sql

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QTPLUGIN += qsvg
QT += svg

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cpp-Components/articleslist.cpp \
        cpp-Components/globalcomponents.cpp \
        cpp-Components/messagelist.cpp \
        cpp-Components/requestuserlist.cpp \
        cppobjs/myarticleobj.cpp \
        cppobjs/myrequestobj.cpp \
        cppobjs/myuserobj.cpp \
        cppobjs/storageunit.cpp \
        loginpagehandler.cpp \
        main.cpp \
        regulatorpagehandler.cpp \
        senderpagehandler.cpp \
        translatorpagehandler.cpp \
        userinfopagehandler.cpp

RESOURCES += qml.qrc

RC_ICONS = pen.ico

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cpp-Components/articleslist.h \
    cpp-Components/globalcomponents.h \
    cpp-Components/messagelist.h \
    cpp-Components/requestuserlist.h \
    cppobjs/myarticleobj.h \
    cppobjs/myrequestobj.h \
    cppobjs/myuserobj.h \
    cppobjs/storageunit.h \
    loginpagehandler.h \
    regulatorpagehandler.h \
    senderpagehandler.h \
    translatorpagehandler.h \
    userinfopagehandler.h

INCLUDEPATH += $$PWD/cppobjs
DEPENDPATH += $$PWD/cppobjs

DISTFILES +=
