QT -= gui
QT += httpserver
QT += sql
QT += network
QT += concurrent

CONFIG += c++17 console
CONFIG -= app_bundle


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/DatabaseConnector.cpp \
    src/DichotomySolver.cpp \
    src/AbstractSolver.cpp \
    src/HttpServerHandler.cpp \
    src/main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/AbstractSolver.h \
    include/DatabaseConnector.h \
    include/DichotomySolver.h \
    include/HttpServerHandler.h \

DISTFILES += \
    scripts/dichotomy.py

RESOURCES += \
    certificates/certificates.qrc \
    webpages/webpages.qrc
