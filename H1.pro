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
        CPP_Data/myarticleobj.cpp \
        CPP_Data/myrequestobj.cpp \
        CPP_Data/myuserobj.cpp \
        CPP_Interaction/loginpagehandler.cpp \
        CPP_Interaction/regulatorpagehandler.cpp \
        CPP_Interaction/senderpagehandler.cpp \
        CPP_Interaction/translatorpagehandler.cpp \
        CPP_Interaction/userinfopagehandler.cpp \
        CPP_Model/articleslist.cpp \
        CPP_Model/messagelist.cpp \
        CPP_Model/requestuserlist.cpp \
        CPP_Storage/globalcomponents.cpp \
        CPP_Storage/storageunit.cpp \
        main.cpp

RESOURCES += qml.qrc

RC_ICONS = Resources/pen.ico

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    CPP_Data/myarticleobj.h \
    CPP_Data/myrequestobj.h \
    CPP_Data/myuserobj.h \
    CPP_Interaction/loginpagehandler.h \
    CPP_Interaction/regulatorpagehandler.h \
    CPP_Interaction/senderpagehandler.h \
    CPP_Interaction/translatorpagehandler.h \
    CPP_Interaction/userinfopagehandler.h \
    CPP_Model/articleslist.h \
    CPP_Model/messagelist.h \
    CPP_Model/requestuserlist.h \
    CPP_Storage/globalcomponents.h \
    CPP_Storage/storageunit.h

DISTFILES +=
