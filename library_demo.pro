QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    borrowitem.cpp \
    changepassword.cpp \
    dboperator.cpp \
    ensure.cpp \
    loading.cpp \
    main.cpp \
    mainwindow.cpp \
    returnitem.cpp \
    student.cpp \
    stulogin.cpp \
    teacher.cpp \
    teacherlogin.cpp

HEADERS += \
    borrowitem.h \
    changepassword.h \
    dboperator.h \
    ensure.h \
    loading.h \
    mainwindow.h \
    returnitem.h \
    student.h \
    stulogin.h \
    teacher.h \
    teacherlogin.h

FORMS += \
    borrowitem.ui \
    changepassword.ui \
    ensure.ui \
    loading.ui \
    mainwindow.ui \
    student.ui \
    stulogin.ui \
    teacher.ui \
    teacherlogin.ui \
    test.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../libManaSys/libManaSys.mdb \
    libManaSys.mdb

RC_ICONS = logo.ico

RESOURCES += \
    resource.qrc
