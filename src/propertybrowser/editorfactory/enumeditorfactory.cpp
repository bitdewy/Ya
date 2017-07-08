/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "enumeditorfactory.h"
#include "private/enumeditorfactory_p.h"
#include "propertymanager/enumpropertymanager.h"
#include <QtWidgets/QAbstractItemView>

EnumEditorFactory::EnumEditorFactory(QObject* parent) :
    AbstractEditorFactory<EnumPropertyManager>(parent),
    impl_(new EnumEditorFactoryPrivate())
{
    impl_->enumEditorFactoryPtr_ = this;

}

EnumEditorFactory::~EnumEditorFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void EnumEditorFactory::connectPropertyManager(EnumPropertyManager* manager)
{
    connect(manager, &EnumPropertyManager::valueChanged,
        this, &EnumEditorFactory::slotPropertyChanged);
    connect(manager, &EnumPropertyManager::enumNamesChanged,
        this, &EnumEditorFactory::slotEnumNamesChanged);
}

QWidget* EnumEditorFactory::createEditor(EnumPropertyManager* manager, Property* property,
    QWidget *parent)
{
    auto editor = impl_->createEditor(property, parent);
    editor->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    editor->view()->setTextElideMode(Qt::ElideRight);
    auto enumNames = manager->enumNames(property);
    editor->addItems(enumNames);
    auto enumIcons = manager->enumIcons(property);
    const auto enumNamesCount = enumNames.count();
    for (auto i = 0; i < enumNamesCount; i++)
    {
        editor->setItemIcon(i, enumIcons.value(i));
    }
    editor->setCurrentIndex(manager->value(property));

    connect(editor, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [this](int value) { impl_->slotSetValue(value); });
    connect(editor, &QComboBox::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void EnumEditorFactory::disconnectPropertyManager(EnumPropertyManager* manager)
{
    disconnect(manager, &EnumPropertyManager::valueChanged,
        this, &EnumEditorFactory::slotPropertyChanged);
    disconnect(manager, &EnumPropertyManager::enumNamesChanged,
        this, &EnumEditorFactory::slotEnumNamesChanged);
}

void EnumEditorFactory::slotPropertyChanged(Property* property, int value)
{
    impl_->slotPropertyChanged(property, value);
}

void EnumEditorFactory::slotEnumNamesChanged(Property* property, const QStringList& enumNames)
{
    impl_->slotEnumNamesChanged(property, enumNames);
}
