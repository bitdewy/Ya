/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "varianteditorfactory.h"
#include "editorfactory/private/varianteditorfactory_p.h"
#include "editorfactory/spinboxfactory.h"
#include "editorfactory/fonteditorfactory.h"
#include "editorfactory/checkboxfactory.h"
#include "editorfactory/enumeditorfactory.h"
#include "editorfactory/chareditorfactory.h"
#include "editorfactory/doublespinboxfactory.h"
#include "editorfactory/dateeditfactory.h"
#include "editorfactory/lineeditfactory.h"
#include "editorfactory/timeeditfactory.h"
#include "editorfactory/datetimeeditfactory.h"
#include "editorfactory/keysequenceeditorfactory.h"
#include "editorfactory/coloreditorfactory.h"
#include "editorfactory/cursoreditorfactory.h"
#include "propertymanager/variantpropertymanager.h"
#include "propertymanager/private/variantpropertymanager_p.h"
#include "propertymanager/localepropertymanager.h"
#include "propertymanager/pointpropertymanager.h"
#include "propertymanager/sizepolicypropertymanager.h"
#include "propertymanager/flagpropertymanager.h"
#include "propertymanager/sizepropertymanager.h"
#include "propertymanager/rectpropertymanager.h"
#include "propertymanager/pointfpropertymanager.h"
#include "propertymanager/sizefpropertymanager.h"
#include "propertymanager/rectfpropertymanager.h"
#include <QtCore/QVariant>

VariantEditorFactory::VariantEditorFactory(QObject* parent) :
    AbstractEditorFactory<VariantPropertyManager>(parent),
    impl_(new VariantEditorFactoryPrivate())
{
    impl_->variantEditorFactoryPtr_ = this;

    impl_->spinBoxFactory_ = new SpinBoxFactory(this);
    impl_->factoryToType_[impl_->spinBoxFactory_] = QVariant::Int;
    impl_->typeToFactory_[QVariant::Int] = impl_->spinBoxFactory_;

    impl_->doubleSpinBoxFactory_ = new DoubleSpinBoxFactory(this);
    impl_->factoryToType_[impl_->doubleSpinBoxFactory_] = QVariant::Double;
    impl_->typeToFactory_[QVariant::Double] = impl_->doubleSpinBoxFactory_;

    impl_->checkBoxFactory_ = new CheckBoxFactory(this);
    impl_->factoryToType_[impl_->checkBoxFactory_] = QVariant::Bool;
    impl_->typeToFactory_[QVariant::Bool] = impl_->checkBoxFactory_;

    impl_->lineEditFactory_ = new LineEditFactory(this);
    impl_->factoryToType_[impl_->lineEditFactory_] = QVariant::String;
    impl_->typeToFactory_[QVariant::String] = impl_->lineEditFactory_;

    impl_->dateEditFactory_ = new DateEditFactory(this);
    impl_->factoryToType_[impl_->dateEditFactory_] = QVariant::Date;
    impl_->typeToFactory_[QVariant::Date] = impl_->dateEditFactory_;

    impl_->timeEditFactory_ = new TimeEditFactory(this);
    impl_->factoryToType_[impl_->timeEditFactory_] = QVariant::Time;
    impl_->typeToFactory_[QVariant::Time] = impl_->timeEditFactory_;

    impl_->dateTimeEditFactory_ = new DateTimeEditFactory(this);
    impl_->factoryToType_[impl_->dateTimeEditFactory_] = QVariant::DateTime;
    impl_->typeToFactory_[QVariant::DateTime] = impl_->dateTimeEditFactory_;

    impl_->keySequenceEditorFactory_ = new KeySequenceEditorFactory(this);
    impl_->factoryToType_[impl_->keySequenceEditorFactory_] = QVariant::KeySequence;
    impl_->typeToFactory_[QVariant::KeySequence] = impl_->keySequenceEditorFactory_;

    impl_->charEditorFactory_ = new CharEditorFactory(this);
    impl_->factoryToType_[impl_->charEditorFactory_] = QVariant::Char;
    impl_->typeToFactory_[QVariant::Char] = impl_->charEditorFactory_;

    impl_->cursorEditorFactory_ = new CursorEditorFactory(this);
    impl_->factoryToType_[impl_->cursorEditorFactory_] = QVariant::Cursor;
    impl_->typeToFactory_[QVariant::Cursor] = impl_->cursorEditorFactory_;

    impl_->colorEditorFactory_ = new ColorEditorFactory(this);
    impl_->factoryToType_[impl_->colorEditorFactory_] = QVariant::Color;
    impl_->typeToFactory_[QVariant::Color] = impl_->colorEditorFactory_;

    impl_->fontEditorFactory_ = new FontEditorFactory(this);
    impl_->factoryToType_[impl_->fontEditorFactory_] = QVariant::Font;
    impl_->typeToFactory_[QVariant::Font] = impl_->fontEditorFactory_;

    impl_->comboBoxFactory_ = new EnumEditorFactory(this);
    const int enumId = VariantPropertyManager::enumTypeId();
    impl_->factoryToType_[impl_->comboBoxFactory_] = enumId;
    impl_->typeToFactory_[enumId] = impl_->comboBoxFactory_;
}

VariantEditorFactory::~VariantEditorFactory()
{
}

void VariantEditorFactory::connectPropertyManager(VariantPropertyManager* manager)
{
    const auto intPropertyManagers = manager->findChildren<IntPropertyManager*>();
    for (auto&& manager : intPropertyManagers)
    {
        impl_->spinBoxFactory_->addPropertyManager(manager);
    }
    const auto doublePropertyManagers = manager->findChildren<DoublePropertyManager*>();
    for (auto&& manager : doublePropertyManagers)
    {
        impl_->doubleSpinBoxFactory_->addPropertyManager(manager);
    }
    const auto boolPropertyManagers = manager->findChildren<BoolPropertyManager*>();
    for (auto&& manager : boolPropertyManagers)
    {
        impl_->checkBoxFactory_->addPropertyManager(manager);
    }
    const auto stringPropertyManagers = manager->findChildren<StringPropertyManager*>();
    for (auto&& manager : stringPropertyManagers)
    {
        impl_->lineEditFactory_->addPropertyManager(manager);
    }
    const auto datePropertyManagers = manager->findChildren<DatePropertyManager*>();
    for (auto&& manager : datePropertyManagers)
    {
        impl_->dateEditFactory_->addPropertyManager(manager);
    }
    const auto timePropertyManagers = manager->findChildren<TimePropertyManager*>();
    for (auto&& manager : timePropertyManagers)
    {
        impl_->timeEditFactory_->addPropertyManager(manager);
    }
    const auto dateTimePropertyManagers = manager->findChildren<DateTimePropertyManager*>();
    for (auto&& manager : dateTimePropertyManagers)
    {
        impl_->dateTimeEditFactory_->addPropertyManager(manager);
    }
    const auto keySequencePropertyManagers = manager->findChildren<KeySequencePropertyManager*>();
    for (auto&& manager : keySequencePropertyManagers)
    {
        impl_->keySequenceEditorFactory_->addPropertyManager(manager);
    }
    const auto charPropertyManagers = manager->findChildren<CharPropertyManager*>();
    for (auto&& manager : charPropertyManagers)
    {
        impl_->charEditorFactory_->addPropertyManager(manager);
    }
    const auto localePropertyManagers = manager->findChildren<LocalePropertyManager*>();
    for (auto&& manager : localePropertyManagers)
    {
        impl_->comboBoxFactory_->addPropertyManager(manager->subEnumPropertyManager());
    }
    const auto pointPropertyManagers = manager->findChildren<PointPropertyManager*>();
    for (auto&& manager : pointPropertyManagers)
    {
        impl_->spinBoxFactory_->addPropertyManager(manager->subIntPropertyManager());
    }
    const auto pointFPropertyManagers = manager->findChildren<PointFPropertyManager*>();
    for (auto&& manager : pointFPropertyManagers)
    {
        impl_->doubleSpinBoxFactory_->addPropertyManager(manager->subDoublePropertyManager());
    }
    const auto sizePropertyManagers = manager->findChildren<SizePropertyManager *>();
    for (auto&& manager : sizePropertyManagers)
    {
        impl_->spinBoxFactory_->addPropertyManager(manager->subIntPropertyManager());
    }
    const auto sizeFPropertyManagers = manager->findChildren<SizeFPropertyManager *>();
    for (auto&& manager : sizeFPropertyManagers)
    {
        impl_->doubleSpinBoxFactory_->addPropertyManager(manager->subDoublePropertyManager());
    }
    const auto rectPropertyManagers = manager->findChildren<RectPropertyManager *>();
    for (auto&& manager : rectPropertyManagers)
    {
        impl_->spinBoxFactory_->addPropertyManager(manager->subIntPropertyManager());
    }
    const auto rectFPropertyManagers = manager->findChildren<RectFPropertyManager *>();
    for (auto&& manager : rectFPropertyManagers)
    {
        impl_->doubleSpinBoxFactory_->addPropertyManager(manager->subDoublePropertyManager());
    }
    const auto colorPropertyManagers = manager->findChildren<ColorPropertyManager *>();
    for (auto&& manager : colorPropertyManagers)
    {
        impl_->colorEditorFactory_->addPropertyManager(manager);
        impl_->spinBoxFactory_->addPropertyManager(manager->subIntPropertyManager());
    }

    const auto enumPropertyManagers = manager->findChildren<EnumPropertyManager *>();
    for (auto&& manager : enumPropertyManagers)
    {
        impl_->comboBoxFactory_->addPropertyManager(manager);
    }
    const auto sizePolicyPropertyManagers = manager->findChildren<SizePolicyPropertyManager *>();
    for (auto&& manager : sizePolicyPropertyManagers)
    {
        impl_->spinBoxFactory_->addPropertyManager(manager->subIntPropertyManager());
        impl_->comboBoxFactory_->addPropertyManager(manager->subEnumPropertyManager());
    }

    const auto fontPropertyManagers = manager->findChildren<FontPropertyManager *>();
    for (auto&& manager : fontPropertyManagers)
    {
        impl_->fontEditorFactory_->addPropertyManager(manager);
        impl_->spinBoxFactory_->addPropertyManager(manager->subIntPropertyManager());
        impl_->comboBoxFactory_->addPropertyManager(manager->subEnumPropertyManager());
        impl_->checkBoxFactory_->addPropertyManager(manager->subBoolPropertyManager());
    }

    const auto cursorPropertyManagers = manager->findChildren<CursorPropertyManager *>();
    for (auto&& manager : cursorPropertyManagers)
    {
        impl_->cursorEditorFactory_->addPropertyManager(manager);
    }
    const auto flagPropertyManagers = manager->findChildren<FlagPropertyManager *>();
    for (auto&& manager : flagPropertyManagers)
    {
        impl_->checkBoxFactory_->addPropertyManager(manager->subBoolPropertyManager());
    }
}

QWidget* VariantEditorFactory::createEditor(VariantPropertyManager* manager,
    Property* property, QWidget *parent)
{
    const auto propType = manager->propertyType(property);
    auto factory = impl_->typeToFactory_.value(propType, 0);
    return factory ? factory->createEditor(wrappedProperty(property), parent) : nullptr;
}

void VariantEditorFactory::disconnectPropertyManager(VariantPropertyManager* manager)
{
    const auto intPropertyManagers = manager->findChildren<IntPropertyManager*>();
    for (auto&& manager : intPropertyManagers)
    {
        impl_->spinBoxFactory_->removePropertyManager(manager);
    }
    const auto doublePropertyManagers = manager->findChildren<DoublePropertyManager*>();
    for (auto&& manager : doublePropertyManagers)
    {
        impl_->doubleSpinBoxFactory_->removePropertyManager(manager);
    }
    const auto boolPropertyManagers = manager->findChildren<BoolPropertyManager*>();
    for (auto&& manager : boolPropertyManagers)
    {
        impl_->checkBoxFactory_->removePropertyManager(manager);
    }
    const auto stringPropertyManagers = manager->findChildren<StringPropertyManager*>();
    for (auto&& manager : stringPropertyManagers)
    {
        impl_->lineEditFactory_->removePropertyManager(manager);
    }
    const auto datePropertyManagers = manager->findChildren<DatePropertyManager*>();
    for (auto&& manager : datePropertyManagers)
    {
        impl_->dateEditFactory_->removePropertyManager(manager);
    }
    const auto timePropertyManagers = manager->findChildren<TimePropertyManager*>();
    for (auto&& manager : timePropertyManagers)
    {
        impl_->timeEditFactory_->removePropertyManager(manager);
    }
    const auto dateTimePropertyManagers = manager->findChildren<DateTimePropertyManager*>();
    for (auto&& manager : dateTimePropertyManagers)
    {
        impl_->dateTimeEditFactory_->removePropertyManager(manager);
    }
    const auto keySequencePropertyManagers = manager->findChildren<KeySequencePropertyManager*>();
    for (auto&& manager : keySequencePropertyManagers)
    {
        impl_->keySequenceEditorFactory_->removePropertyManager(manager);
    }
    const auto charPropertyManagers = manager->findChildren<CharPropertyManager*>();
    for (auto&& manager : charPropertyManagers)
    {
        impl_->charEditorFactory_->removePropertyManager(manager);
    }
    const auto localePropertyManagers = manager->findChildren<LocalePropertyManager*>();
    for (auto&& manager : localePropertyManagers)
    {
        impl_->comboBoxFactory_->removePropertyManager(manager->subEnumPropertyManager());
    }
    const auto pointPropertyManagers = manager->findChildren<PointPropertyManager*>();
    for (auto&& manager : pointPropertyManagers)
    {
        impl_->spinBoxFactory_->removePropertyManager(manager->subIntPropertyManager());
    }
    const auto pointFPropertyManagers = manager->findChildren<PointFPropertyManager*>();
    for (auto&& manager : pointFPropertyManagers)
    {
        impl_->doubleSpinBoxFactory_->removePropertyManager(manager->subDoublePropertyManager());
    }
    const auto sizePropertyManagers = manager->findChildren<SizePropertyManager*>();
    for (auto&& manager : sizePropertyManagers)
    {
        impl_->spinBoxFactory_->removePropertyManager(manager->subIntPropertyManager());
    }
    const auto sizeFPropertyManagers = manager->findChildren<SizeFPropertyManager*>();
    for (auto&& manager : sizeFPropertyManagers)
    {
        impl_->doubleSpinBoxFactory_->removePropertyManager(manager->subDoublePropertyManager());
    }
    const auto rectPropertyManagers = manager->findChildren<RectPropertyManager*>();
    for (auto&& manager : rectPropertyManagers)
    {
        impl_->spinBoxFactory_->removePropertyManager(manager->subIntPropertyManager());
    }
    const auto rectFPropertyManagers = manager->findChildren<RectFPropertyManager*>();
    for (auto&& manager : rectFPropertyManagers)
    {
        impl_->doubleSpinBoxFactory_->removePropertyManager(manager->subDoublePropertyManager());
    }
    const auto colorPropertyManagers = manager->findChildren<ColorPropertyManager*>();
    for (auto&& manager : colorPropertyManagers)
    {
        impl_->colorEditorFactory_->removePropertyManager(manager);
        impl_->spinBoxFactory_->removePropertyManager(manager->subIntPropertyManager());
    }
    const auto enumPropertyManagers = manager->findChildren<EnumPropertyManager*>();
    for (auto&& manager : enumPropertyManagers)
    {
        impl_->comboBoxFactory_->removePropertyManager(manager);
    }
    const auto sizePolicyPropertyManagers = manager->findChildren<SizePolicyPropertyManager*>();
    for (auto&& manager : sizePolicyPropertyManagers)
    {
        impl_->spinBoxFactory_->removePropertyManager(manager->subIntPropertyManager());
        impl_->comboBoxFactory_->removePropertyManager(manager->subEnumPropertyManager());
    }
    const auto fontPropertyManagers = manager->findChildren<FontPropertyManager*>();
    for (auto&& manager : fontPropertyManagers)
    {
        impl_->fontEditorFactory_->removePropertyManager(manager);
        impl_->spinBoxFactory_->removePropertyManager(manager->subIntPropertyManager());
        impl_->comboBoxFactory_->removePropertyManager(manager->subEnumPropertyManager());
        impl_->checkBoxFactory_->removePropertyManager(manager->subBoolPropertyManager());
    }

    const auto cursorPropertyManagers = manager->findChildren<CursorPropertyManager*>();
    for (auto&& manager : cursorPropertyManagers)
    {
        impl_->cursorEditorFactory_->removePropertyManager(manager);
    }
    const auto flagPropertyManagers = manager->findChildren<FlagPropertyManager*>();
    for (auto&& manager : flagPropertyManagers)
    {
        impl_->checkBoxFactory_->removePropertyManager(manager->subBoolPropertyManager());
    }
}
