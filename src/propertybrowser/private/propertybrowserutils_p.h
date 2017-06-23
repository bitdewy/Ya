/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtGui/QPixmap>

class PropertyBrowserUtils
{
public:
    static QPixmap brushValuePixmap(const QBrush& b);
    static QIcon brushValueIcon(const QBrush& b);
    static QString colorValueText(const QColor& c);
    static QPixmap fontValuePixmap(const QFont& f);
    static QIcon fontValueIcon(const QFont& f);
    static QString fontValueText(const QFont& f);
    static QString dateFormat();
    static QString timeFormat();
    static QString dateTimeFormat();
};
