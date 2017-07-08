/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "timeeditfactory.h"
#include "private/timeeditfactory_p.h"
#include "private/propertybrowserutils_p.h"

TimeEditFactory::TimeEditFactory(QObject* parent) :
    AbstractEditorFactory<TimePropertyManager>(parent),
    impl_(new TimeEditFactoryPrivate())
{
    impl_->timeEditFactoryPtr_ = this;

}

TimeEditFactory::~TimeEditFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void TimeEditFactory::connectPropertyManager(TimePropertyManager* manager)
{
    connect(manager, &TimePropertyManager::valueChanged,
        this, &TimeEditFactory::slotPropertyChanged);
}

QWidget* TimeEditFactory::createEditor(TimePropertyManager* manager, Property* property,
    QWidget* parent)
{
    auto editor = impl_->createEditor(property, parent);
    editor->setDisplayFormat(PropertyBrowserUtils::timeFormat());
    editor->setTime(manager->value(property));

    connect(editor, &QTimeEdit::timeChanged,
        this, [this](const QTime& time) { impl_->slotSetValue(time); });
    connect(editor, &QTimeEdit::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void TimeEditFactory::disconnectPropertyManager(TimePropertyManager* manager)
{
    disconnect(manager, &TimePropertyManager::valueChanged,
        this, &TimeEditFactory::slotPropertyChanged);
}

void TimeEditFactory::slotPropertyChanged(Property* property, const QTime& value)
{
    impl_->slotPropertyChanged(property, value);
}
