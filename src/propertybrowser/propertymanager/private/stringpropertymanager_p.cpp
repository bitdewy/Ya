/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "stringpropertymanager_p.h"

StringPropertyManagerPrivate::Data::Data() :
    regExp(QString(QLatin1Char('*')), Qt::CaseSensitive, QRegExp::Wildcard)
{

}
