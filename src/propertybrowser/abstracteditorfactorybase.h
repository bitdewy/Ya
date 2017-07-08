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

class QWidget;
class Property;
class AbstractPropertyManager;
class AbstractEditorFactoryBase : public QObject
{
    Q_OBJECT
public:
    virtual QWidget* createEditor(Property* property, QWidget* parent) = 0;

protected:
    explicit AbstractEditorFactoryBase(QObject* parent = nullptr);

    virtual void breakConnection(AbstractPropertyManager* manager) = 0;

protected Q_SLOTS:
    virtual void managerDestroyed(QObject* manager) = 0;
    friend class AbstractPropertyBrowser;
};
