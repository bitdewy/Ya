/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "datetimeeditfactory.h"
#include "private/datetimeeditfactory_p.h"
#include "private/editorfactory_p.h"
#include "private/propertybrowserutils_p.h"
#include <QtWidgets/QDateTimeEdit>

DateTimeEditFactory::DateTimeEditFactory(QObject *parent) :
    AbstractEditorFactory<DateTimePropertyManager>(parent),
    impl_(new DateTimeEditFactoryPrivate())
{
    impl_->dateTimeEditPropertyPtr_ = this;
}

DateTimeEditFactory::~DateTimeEditFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void DateTimeEditFactory::connectPropertyManager(DateTimePropertyManager* manager)
{
    connect(manager, &DateTimePropertyManager::valueChanged,
                this, &DateTimeEditFactory::slotPropertyChanged);
}

QWidget* DateTimeEditFactory::createEditor(DateTimePropertyManager* manager,
    Property* property, QWidget* parent)
{
    auto editor =  impl_->createEditor(property, parent);
    editor->setDisplayFormat(PropertyBrowserUtils::dateTimeFormat());
    editor->setDateTime(manager->value(property));

    connect(editor, &QDateTimeEdit::dateTimeChanged,
        this, [this](const QDateTime& datetime) { impl_->slotSetValue(datetime); });
    connect(editor, &QDateTimeEdit::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void DateTimeEditFactory::disconnectPropertyManager(DateTimePropertyManager* manager)
{
    disconnect(manager, &DateTimePropertyManager::valueChanged,
        this, &DateTimeEditFactory::slotPropertyChanged);
}

void DateTimeEditFactory::slotPropertyChanged(Property* property, const QDateTime& value)
{
    impl_->slotPropertyChanged(property, value);
}

