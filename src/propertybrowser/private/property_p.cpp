/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "property_p.h"

PropertyPrivate::PropertyPrivate(AbstractPropertyManager* manager) :
    enabled_(true),
    modified_(false),
    manager_(manager)
{

}
