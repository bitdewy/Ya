/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "editorfactory_p.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QLayout>

void setupTreeViewEditorMargin(QLayout* lt)
{
    enum { DecorationMargin = 4 };
    if (QApplication::layoutDirection() == Qt::LeftToRight)
    {
        lt->setContentsMargins(DecorationMargin, 0, 0, 0);
    }
    else
    {
        lt->setContentsMargins(0, 0, DecorationMargin, 0);
    }
}
