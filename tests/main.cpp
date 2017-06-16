/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "ya_test.h"
#include <QtCore/QCoreApplication>
#include <QtTest>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    YaTest tc;
    return QTest::qExec(&tc, argc, argv);
}
