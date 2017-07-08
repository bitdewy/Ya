/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "lineeditfactory_p.h"
#include "editorfactory/lineeditfactory.h"
#include <QtGui/QRegExpValidator>

void LineEditFactoryPrivate::slotPropertyChanged(Property* property,
    const QString& value)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.constEnd())
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        if (editor->text() != value)
        {
            editor->setText(value);
        }
    }
}

void LineEditFactoryPrivate::slotRegExpChanged(Property* property,
    const QRegExp& regExp)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.constEnd())
    {
        return;
    }
    auto manager = lineEditFactoryPtr_->propertyManager(property);
    if (!manager)
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        const auto oldValidator = editor->validator();
        QValidator* newValidator = 0;
        if (regExp.isValid())
        {
            newValidator = new QRegExpValidator(regExp, editor);
        }
        editor->setValidator(newValidator);
        if (oldValidator)
        {
            delete oldValidator;
        }
        editor->blockSignals(false);
    }
}

void LineEditFactoryPrivate::slotSetValue(const QString& value)
{
    auto object = lineEditFactoryPtr_->sender();
    const auto ecend = editorToProperty_.constEnd();
    for (auto itEditor = editorToProperty_.constBegin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            auto property = itEditor.value();
            auto manager = lineEditFactoryPtr_->propertyManager(property);
            if (!manager)
            {
                return;
            }
            manager->setValue(property, value);
            return;
        }
    }
}
