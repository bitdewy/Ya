/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "yatest.h"
#include <QtTest>

namespace Ya
{

YaTest::YaTest()
{
}

void YaTest::initTestCase()
{
}

void YaTest::cleanupTestCase()
{
}

void YaTest::testCast1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

void YaTest::testCast1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

}  // namespace Ya
