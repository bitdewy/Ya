/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QHash>
#include <QtCore/QMap>
#include <QtCore/QPair>

class QObject;
class ExtensionFactory;
class ExtensionFactoryPrivate
{
public:
    ExtensionFactoryPrivate();
    ~ExtensionFactoryPrivate();
    ExtensionFactoryPrivate(const ExtensionFactoryPrivate&) = delete;
    ExtensionFactoryPrivate& operator=(const ExtensionFactoryPrivate&) = delete;

    QObject* extension(ExtensionFactory* factory, QObject* object, const QString& iid);

private:
    void objectDestroyed(QObject* object);

private:
    using IdObjectKey = QPair<QString,QObject*>;
    using ExtensionMap = QMap< IdObjectKey, QObject*>;
    using ExtendedSet = QHash<QObject*, bool>;
    ExtensionMap extensions_;
    ExtendedSet extended_;
};
