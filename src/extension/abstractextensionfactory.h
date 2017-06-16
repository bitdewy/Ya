/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QObject>

class QString;
class AbstractExtensionFactory
{
public:
    virtual ~AbstractExtensionFactory() {}

    virtual QObject* extension(QObject* object, const QString& iid) const = 0;
};

Q_DECLARE_INTERFACE(AbstractExtensionFactory, "bitdewy.Ya.AbstractExtensionFactory")
