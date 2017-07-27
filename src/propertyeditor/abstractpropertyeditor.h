/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtWidgets/QWidget>

class QString;
class QVariant;

class AbstractPropertyEditor: public QWidget
{
    Q_OBJECT
public:
    explicit AbstractPropertyEditor(QWidget* parent, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~AbstractPropertyEditor();

    virtual bool isReadOnly() const = 0;
    virtual QObject* object() const = 0;

    virtual QString currentPropertyName() const = 0;

Q_SIGNALS:
    void propertyChanged(const QString& name, const QVariant& value);

public Q_SLOTS:
    virtual void setObject(QObject* object) = 0;
    virtual void setPropertyValue(const QString& name, const QVariant& value, bool changed = true) = 0;
    virtual void setReadOnly(bool readOnly) = 0;
};
