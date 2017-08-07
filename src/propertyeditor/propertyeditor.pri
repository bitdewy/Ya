INCLUDEPATH += $$PWD \
    $$PWD/../

include(../propertybrowser/propertybrowser.pri)

RESOURCES += $$PWD/propertyeditor.qrc

HEADERS += \
    $$PWD/abstractpropertyeditor.h \
    $$PWD/propertyeditorbase.h \
    $$PWD/enums.h \
    $$PWD/abstractformeditor.h \
    $$PWD/dynamicpropertysheet.h \
    $$PWD/propertysheet.h

SOURCES += \
    $$PWD/abstractpropertyeditor.cpp \
    $$PWD/propertyeditorbase.cpp \
    $$PWD/abstractformeditor.cpp
