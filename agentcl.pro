QT = core gui widgets network uiplugin

CONFIG += c++20 cmdline
# QMAKE_CXXFLAGS += -fPIC # учесть особенности спп*, gcc -fPIC -shared -o libagent.so main.c ram.c cpu.c loop.c UniPipes.c cpphelp.c -Wl,-e,main такое  работает
# QMAKE_LFLAGS += -fPIC
# QMAKE_LFLAGS += -Wl,-e,main
# QMAKE_LFLAGS += -shared

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
        client.cpp \
        clloop.cpp \
        main.cpp \
        mainwindow.cpp \
        mythread.cpp
         # unity.cpp #один из вариантов компила shared runnable # возможно бессмысленно, т.к можно и несколько файлов в shared runnable

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    clientQ.h \
    clloop.h \
    mainwindow.h \
    mythread.h

FORMS += \
    mainwindow.ui
unix:!macx: LIBS += -lcap
