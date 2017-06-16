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
#include <QtCore/QList>

class QObject;
class AbstractExtensionFactory;

class ExtensionManagerPrivate
{
public:
    ExtensionManagerPrivate();
    ~ExtensionManagerPrivate();
    ExtensionManagerPrivate(const ExtensionManagerPrivate&) = delete;
    ExtensionManagerPrivate& operator=(const ExtensionManagerPrivate&) = delete;

    void registerExtensions(AbstractExtensionFactory* factory, const QString& iid = QString());
    void unregisterExtensions(AbstractExtensionFactory* factory, const QString& iid = QString());

    QObject* extension(QObject* object, const QString& iid) const;

private:
    using FactoryList = QList<AbstractExtensionFactory*>;
    using FactoryMap = QHash<QString, FactoryList>;
    FactoryMap extensions_;
    FactoryList globalExtension_;
};
