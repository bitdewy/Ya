/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

class VariantPropertyManager;
class VariantPropertyPrivate
{
public:
    VariantPropertyPrivate(VariantPropertyManager *m);
    VariantPropertyPrivate(const VariantPropertyPrivate&) = delete;
    VariantPropertyPrivate& operator=(const VariantPropertyPrivate&) = delete;

    VariantPropertyManager* manager;
};
