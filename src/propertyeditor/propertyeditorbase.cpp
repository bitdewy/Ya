/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "propertyeditorbase.h"
#include "abstractformeditor.h"
#include "extension/extensionmanager.h"
#include "dynamicpropertysheet.h"
#include "propertysheet.h"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QAction>

using StringPropertyParameters = PropertyEditorBase::StringPropertyParameters;
using PropertyNameTypeMap = QHash<QString, StringPropertyParameters>;

static const PropertyNameTypeMap& stringPropertyTypes()
{
    static PropertyNameTypeMap propertyNameTypeMap;
    if (propertyNameTypeMap.empty())
    {
        const StringPropertyParameters richtext(ValidationRichText, true);

        propertyNameTypeMap.insert(QStringLiteral("accessibleDescription"), richtext);
        propertyNameTypeMap.insert(QStringLiteral("accessibleName"), richtext);

        const StringPropertyParameters objectName(ValidationObjectName, false);
        propertyNameTypeMap.insert(QStringLiteral("buddy"), objectName);
        propertyNameTypeMap.insert(QStringLiteral("currentItemName"), objectName);
        propertyNameTypeMap.insert(QStringLiteral("currentPageName"), objectName);
        propertyNameTypeMap.insert(QStringLiteral("currentTabName"), objectName);
        propertyNameTypeMap.insert(QStringLiteral("layoutName"), objectName);
        propertyNameTypeMap.insert(QStringLiteral("spacerName"), objectName);

        propertyNameTypeMap.insert(QStringLiteral("styleSheet"), StringPropertyParameters(ValidationStyleSheet, false));

        const StringPropertyParameters multiline(ValidationMultiLine, true);
        propertyNameTypeMap.insert(QStringLiteral("description"), multiline);
        propertyNameTypeMap.insert(QStringLiteral("iconText"), multiline);

        propertyNameTypeMap.insert(QStringLiteral("toolTip"), richtext);
        propertyNameTypeMap.insert(QStringLiteral("whatsThis"), richtext);
        propertyNameTypeMap.insert(QStringLiteral("windowIconText"), richtext);
        propertyNameTypeMap.insert(QStringLiteral("html"), richtext);

        propertyNameTypeMap.insert(QStringLiteral("pageId"), StringPropertyParameters(ValidationSingleLine, false));

        propertyNameTypeMap.insert(QStringLiteral("plainText"), StringPropertyParameters(ValidationMultiLine, true));
    }
    return propertyNameTypeMap;
}

PropertyEditorBase::PropertyEditorBase(QWidget* parent, Qt::WindowFlags flags) :
    AbstractPropertyEditor(parent, flags),
    propertyChangedForwardingBlocked_(false)
{
    connect(this, &AbstractPropertyEditor::propertyChanged,
        this, &PropertyEditorBase::slotPropertyChanged);
}

static inline bool isDynamicProperty(AbstractFormEditor* core, QObject* object, const QString& propertyName)
{
    if (const auto dynamicSheet = extension<DynamicPropertySheetExtension*>(core->extensionManager(), object))
    {
        if (dynamicSheet->dynamicPropertiesAllowed())
        {
            if (auto propertySheet = extension<PropertySheetExtension*>(core->extensionManager(), object))
            {
                const auto index = propertySheet->indexOf(propertyName);
                return index >= 0 && dynamicSheet->isDynamicProperty(index);
            }
        }
    }
    return false;
}

PropertyEditorBase::StringPropertyParameters PropertyEditorBase::textPropertyValidationMode(
    AbstractFormEditor* core, const QObject* object, const QString& propertyName, bool isMainContainer)
{
    if (propertyName == QStringLiteral("objectName"))
    {
        const auto vm =  isMainContainer ? ValidationObjectNameScope : ValidationObjectName;
        return StringPropertyParameters(vm, false);
    }

    if (isDynamicProperty(core, const_cast<QObject*>(object), propertyName))
    {
        return StringPropertyParameters(ValidationMultiLine, true);
    }

    const auto hit = stringPropertyTypes().constFind(propertyName);
    if (hit != stringPropertyTypes().constEnd())
    {
       return hit.value();
    }

    if (propertyName == QStringLiteral("text"))
    {
        if (qobject_cast<const QAction*>(object) || qobject_cast<const QLineEdit*>(object))
        {
            return StringPropertyParameters(ValidationSingleLine, true);
        }
        if (qobject_cast<const QAbstractButton*>(object))
        {
            return StringPropertyParameters(ValidationMultiLine, true);
        }
        return StringPropertyParameters(ValidationRichText, true);
    }

    if (propertyName.endsWith(QStringLiteral("Name")))
    {
        return StringPropertyParameters(ValidationSingleLine, true);
    }

    if (propertyName.endsWith(QStringLiteral("ToolTip")))
    {
        return StringPropertyParameters(ValidationRichText, true);
    }

    return StringPropertyParameters(ValidationSingleLine, true);
}

void PropertyEditorBase::emitPropertyValueChanged(const QString& name, const QVariant& value, bool enableSubPropertyHandling)
{
    propertyChangedForwardingBlocked_ = true;
    emit propertyValueChanged(name, value, enableSubPropertyHandling);
    emit propertyChanged(name, value);
    propertyChangedForwardingBlocked_ = false;
}

void PropertyEditorBase::slotPropertyChanged(const QString& name, const QVariant& value)
{
    if (!propertyChangedForwardingBlocked_)
    {
        emit propertyValueChanged(name, value, true);
    }
}
