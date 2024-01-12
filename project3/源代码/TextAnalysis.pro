QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QT += charts

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    analysis.cpp \
    chartwidget.cpp \
    cppanalysis.cpp \
    cppanalysiswidget.cpp \
    cpphighlighter.cpp \
    finddialog.cpp \
    main.cpp \
    mainwindow.cpp \
    txtanalysis.cpp \
    txtanalysiswidget.cpp \
    word.cpp

HEADERS += \
    analysis.h \
    chartwidget.h \
    cppanalysis.h \
    cppanalysiswidget.h \
    cpphighlighter.h \
    finddialog.h \
    mainwindow.h \
    txtanalysis.h \
    txtanalysiswidget.h \
    word.h

FORMS += \
    chartwidget.ui \
    cppanalysiswidget.ui \
    finddialog.ui \
    mainwindow.ui \
    txtanalysiswidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc


