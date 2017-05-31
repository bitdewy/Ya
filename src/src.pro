QT += core gui widgets

TARGET = ya
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

macx:INCLUDEPATH += "../thirdparty/opencv3.2.0/macOS/include"
macx:LIBS += -L"$$PWD/../thirdparty/opencv3.2.0/macOS/lib" -lopencv_core

win32 {
    INCLUDEPATH += "../thirdparty/opencv3.2.0/windows/include"
    LIBS += -L"$$PWD/../thirdparty/opencv3.2.0/windows/$$QMAKE_HOST.arch/vc14/lib"
    opencv.path = $$PWD/../thirdparty/opencv3.2.0/windows/$$QMAKE_HOST.arch/vc14/bin
    CONFIG(debug, debug|release) {
        LIBS += -lopencv_core320d
        QMAKE_POST_LINK += copy /y "$$shell_path($$opencv.path/opencv_core320d.dll)" "$$shell_path($$OUT_PWD/debug)"
    } else {
        LIBS += -lopencv_core320
        QMAKE_POST_LINK += copy /y "$$shell_path($$opencv.path/opencv_core320.dll)" "$$shell_path($$OUT_PWD/release)"
    }
}

SOURCES += main.cpp \
    mainwindow.cpp

HEADERS  += mainwindow.h
