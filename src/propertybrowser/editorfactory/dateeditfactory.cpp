/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "dateeditfactory.h"
#include "private/dateeditfactory_p.h"
#include "propertymanager/datepropertymanager.h"
#include "private/propertybrowserutils_p.h"

DateEditFactory::DateEditFactory(QObject *parent) :
    AbstractEditorFactory<DatePropertyManager>(parent),
    impl_(new DateEditFactoryPrivate())
{
    impl_->dateEditFactoryPtr_ = this;

}

DateEditFactory::~DateEditFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void DateEditFactory::connectPropertyManager(DatePropertyManager* manager)
{
    connect(manager, &DatePropertyManager::valueChanged,
        this, &DateEditFactory::slotPropertyChanged);
    connect(manager, &DatePropertyManager::rangeChanged,
        this, &DateEditFactory::slotRangeChanged);
}

QWidget* DateEditFactory::createEditor(DatePropertyManager* manager, Property* property,
    QWidget* parent)
{
    auto editor = impl_->createEditor(property, parent);
    editor->setDisplayFormat(PropertyBrowserUtils::dateFormat());
    editor->setCalendarPopup(true);
    editor->setDateRange(manager->minimum(property), manager->maximum(property));
    editor->setDate(manager->value(property));

    connect(editor, &QDateEdit::dateChanged,
        this, [this](const QDate& date) { impl_->slotSetValue(date); });
    connect(editor, &QDateEdit::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void DateEditFactory::disconnectPropertyManager(DatePropertyManager* manager)
{
    disconnect(manager, &DatePropertyManager::valueChanged,
        this, &DateEditFactory::slotPropertyChanged);
    disconnect(manager, &DatePropertyManager::rangeChanged,
        this, &DateEditFactory::slotRangeChanged);
}

void DateEditFactory::slotPropertyChanged(Property* property, const QDate& value)
{
    impl_->slotPropertyChanged(property, value);
}

void DateEditFactory::slotRangeChanged(Property* property, const QDate& min, const QDate& max)
{
    impl_->slotRangeChanged(property, min, max);
}
