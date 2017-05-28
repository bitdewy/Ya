/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QObject>

namespace Ya
{

class YaTest : public QObject
{
    Q_OBJECT

public:
    YaTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCast1_data();
    void testCast1();
};

}  // namespace Ya
