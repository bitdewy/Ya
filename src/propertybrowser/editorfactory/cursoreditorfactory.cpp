/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "cursoreditorfactory.h"
#include "abstracteditorfactory.h"
#include "editorfactory/enumeditorfactory.h"
#include "propertymanager/cursorpropertymanager.h"
#include "private/cursoreditorfactory_p.h"
#include <QtGui/QCursor>
#include <QtWidgets/QWidget>

CursorEditorFactory::CursorEditorFactory(QObject* parent)
    : AbstractEditorFactory<CursorPropertyManager>(parent), impl_(new CursorEditorFactoryPrivate())
{
    impl_->cursorEditorFactoryPtr_ = this;

    impl_->enumEditorFactory_ = new EnumEditorFactory(this);
    impl_->enumPropertyManager_ = new EnumPropertyManager(this);
    connect(impl_->enumPropertyManager_, &EnumPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotEnumChanged(property, value); });
    impl_->enumEditorFactory_->addPropertyManager(impl_->enumPropertyManager_);
}

CursorEditorFactory::~CursorEditorFactory()
{
}

void CursorEditorFactory::connectPropertyManager(CursorPropertyManager* manager)
{
    connect(manager, &CursorPropertyManager::valueChanged,
        this, &CursorEditorFactory::slotPropertyChanged);
}

QWidget* CursorEditorFactory::createEditor(CursorPropertyManager* manager, Property* property,
    QWidget* parent)
{
    Property* enumProp = nullptr;
    if (impl_->propertyToEnum_.contains(property))
    {
        enumProp = impl_->propertyToEnum_[property];
    }
    else
    {
        enumProp = impl_->enumPropertyManager_->addProperty(property->propertyName());
        impl_->enumPropertyManager_->setEnumNames(enumProp, cursorDatabase()->cursorShapeNames());
        impl_->enumPropertyManager_->setEnumIcons(enumProp, cursorDatabase()->cursorShapeIcons());
        impl_->enumPropertyManager_->setValue(enumProp, cursorDatabase()->cursorToValue(manager->value(property)));
        impl_->propertyToEnum_[property] = enumProp;
        impl_->enumToProperty_[enumProp] = property;
    }
    AbstractEditorFactoryBase* af = impl_->enumEditorFactory_;
    auto editor = af->createEditor(enumProp, parent);
    impl_->enumToEditors_[enumProp].append(editor);
    impl_->editorToEnum_[editor] = enumProp;
    connect(editor, &QWidget::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void CursorEditorFactory::disconnectPropertyManager(CursorPropertyManager* manager)
{
    disconnect(manager, &CursorPropertyManager::valueChanged,
        this, &CursorEditorFactory::slotPropertyChanged);
}

void CursorEditorFactory::slotPropertyChanged(Property* property, const QCursor& cursor)
{
    impl_->slotPropertyChanged(property, cursor);
}
