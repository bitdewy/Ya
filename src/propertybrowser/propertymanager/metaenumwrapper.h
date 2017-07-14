/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtWidgets/QSizePolicy>

class MetaEnumWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSizePolicy::Policy policy READ policy)
public:
    QSizePolicy::Policy policy() const;
private:
    MetaEnumWrapper(QObject* parent);
};
