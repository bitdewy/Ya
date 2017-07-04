/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "abstracteditorfactory.h"
#include "propertymanager/stringpropertymanager.h"

class LineEditFactoryPrivate;
class LineEditFactory : public AbstractEditorFactory<StringPropertyManager>
{
    Q_OBJECT
public:
    explicit LineEditFactory(QObject* parent = nullptr);
    ~LineEditFactory();
protected:
    void connectPropertyManager(StringPropertyManager* manager);
    QWidget* createEditor(StringPropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(StringPropertyManager* manager);
private:
    void slotPropertyChanged(Property* property, const QString& value);
    void slotRegExpChanged(Property* property, const QRegExp& regExp);
    QScopedPointer<LineEditFactoryPrivate> impl_;
    friend class LineEditFactoryPrivate;
};
