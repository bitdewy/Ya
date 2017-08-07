/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "enums.h"
#include "abstractpropertyeditor.h"

class AbstractFormEditor;
class PropertyEditorBase : public AbstractPropertyEditor
{
    Q_OBJECT
public:
    explicit PropertyEditorBase(QWidget* parent = nullptr, Qt::WindowFlags flags = 0);

    using StringPropertyParameters = QPair<TextPropertyValidationMode, bool>;

    static StringPropertyParameters textPropertyValidationMode(AbstractFormEditor* core,
        const QObject* object, const QString& propertyName, bool isMainContainer);
Q_SIGNALS:
    void propertyValueChanged(const QString& name, const QVariant& value, bool enableSubPropertyHandling);
    void resetProperty(const QString& name);
    void addDynamicProperty(const QString& name, const QVariant& value);
    void removeDynamicProperty(const QString& name);
    void editorOpened();
    void editorClosed();

public Q_SLOTS:

    virtual void updatePropertySheet() = 0;
    virtual void reloadResourceProperties() = 0;

private Q_SLOTS:
    void slotPropertyChanged(const QString& name, const QVariant& value);

protected:
    void emitPropertyValueChanged(const QString& name, const QVariant& value, bool enableSubPropertyHandling);

private:
    bool propertyChangedForwardingBlocked_;
};
