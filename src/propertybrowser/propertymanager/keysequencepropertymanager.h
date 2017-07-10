/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "abstractpropertymanager.h"

class KeySequencePropertyManagerPrivate;
class KeySequencePropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit KeySequencePropertyManager(QObject* parent = nullptr);
    ~KeySequencePropertyManager();

    QKeySequence value(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QKeySequence& val);
Q_SIGNALS:
    void valueChanged(Property* property, const QKeySequence& val);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<KeySequencePropertyManagerPrivate> impl_;
    friend class KeySequencePropertyManagerPrivate;
};
