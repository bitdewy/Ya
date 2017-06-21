/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "abstractextensionfactory.h"

class ExtensionManager;
class ExtensionFactoryPrivate;
class ExtensionFactory : public QObject, public AbstractExtensionFactory
{
    Q_OBJECT
    Q_INTERFACES(AbstractExtensionFactory)
    friend class ExtensionFactoryPrivate;
public:
    explicit ExtensionFactory(ExtensionManager* parent = nullptr);
    ~ExtensionFactory();

    QObject* extension(QObject* object, const QString& iid) const override;
    ExtensionManager* extensionManager() const;

protected:
    virtual QObject* createExtension(QObject* object, const QString& iid, QObject* parent) const;

private:
    QScopedPointer<ExtensionFactoryPrivate> impl_;
};
