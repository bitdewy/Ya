/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "checkboxfactory.h"
#include "private/checkboxfactory_p.h"

CheckBoxFactory::CheckBoxFactory(QObject* parent) :
    AbstractEditorFactory<BoolPropertyManager>(parent),
    impl_(new CheckBoxFactoryPrivate())
{
    impl_->checkBoxFactoryPtr_ = this;
}

CheckBoxFactory::~CheckBoxFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void CheckBoxFactory::connectPropertyManager(BoolPropertyManager* manager)
{
    connect(manager, &BoolPropertyManager::valueChanged,
        this, &CheckBoxFactory::slotPropertyChanged);
}

QWidget* CheckBoxFactory::createEditor(BoolPropertyManager* manager, Property* property,
    QWidget* parent)
{
    auto editor = impl_->createEditor(property, parent);
    editor->setChecked(manager->value(property));

    connect(editor, &BoolEditWidget::toggled,
        this, [this](bool value) { impl_->slotSetValue(value); });
    connect(editor, &BoolEditWidget::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void CheckBoxFactory::disconnectPropertyManager(BoolPropertyManager* manager)
{
    disconnect(manager, &BoolPropertyManager::valueChanged,
        this, &CheckBoxFactory::slotPropertyChanged);
}

void CheckBoxFactory::slotPropertyChanged(Property* property, bool value)
{
    impl_->slotPropertyChanged(property, value);
}
