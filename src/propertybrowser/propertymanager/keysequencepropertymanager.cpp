/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "keysequencepropertymanager.h"
#include "private/propertymanagerutils_p.h"
#include "private/keysequencepropertymanager_p.h"
#include "property.h"
#include <QtGui/QKeySequence>

KeySequencePropertyManager::KeySequencePropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new KeySequencePropertyManagerPrivate)
{
    impl_->keySequencePropertyManagerPtr_ = this;
}

KeySequencePropertyManager::~KeySequencePropertyManager()
{
    clear();
}

QKeySequence KeySequencePropertyManager::value(const Property* property) const
{
    return impl_->values_.value(property, QKeySequence());
}

QString KeySequencePropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    return it.value().toString(QKeySequence::NativeText);
}

void KeySequencePropertyManager::setValue(Property* property, const QKeySequence& val)
{
    setSimpleValue<const QKeySequence&, QKeySequence, KeySequencePropertyManager>(impl_->values_, this,
        &KeySequencePropertyManager::propertyChanged,
        &KeySequencePropertyManager::valueChanged,
        property, val);
}

void KeySequencePropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = {};
}

void KeySequencePropertyManager::uninitializeProperty(Property *property)
{
    impl_->values_.remove(property);
}
