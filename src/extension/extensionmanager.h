/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <extension/abstractextensionmanager.h>

class ExtensionManagerPrivate;
class ExtensionManager : public QObject, public AbstractExtensionManager
{
    Q_OBJECT
    Q_INTERFACES(AbstractExtensionManager)
public:
    explicit ExtensionManager(QObject* parent = nullptr);
    ~ExtensionManager();

    void registerExtensions(AbstractExtensionFactory* factory, const QString& iid = QString()) override;
    void unregisterExtensions(AbstractExtensionFactory* factory, const QString& iid = QString()) override;

    QObject* extension(QObject* object, const QString& iid) const override;

private:
    QScopedPointer<ExtensionManagerPrivate> impl_;
};
