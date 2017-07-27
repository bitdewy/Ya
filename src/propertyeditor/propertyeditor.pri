INCLUDEPATH += $$PWD

include(../propertybrowser/propertybrowser.pri)

RESOURCES += $$PWD/propertyeditor.qrc

HEADERS += \
    $$PWD/abstractpropertyeditor.h

SOURCES += \
    $$PWD/abstractpropertyeditor.cpp
