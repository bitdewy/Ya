/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "metaenumwrapper.h"

MetaEnumWrapper::MetaEnumWrapper(QObject *parent) :
    QObject(parent)
{

}

QSizePolicy::Policy MetaEnumWrapper::policy() const
{
    return QSizePolicy::Ignored;
}
