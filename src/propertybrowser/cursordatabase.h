/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QMap>
#include <QtGui/QIcon>

class CursorDatabase
{
public:
    CursorDatabase();
    CursorDatabase(const CursorDatabase&) = default;
    CursorDatabase& operator=(const CursorDatabase&) = default;

    void clear();

    QStringList cursorShapeNames() const;
    QMap<int, QIcon> cursorShapeIcons() const;
    QString cursorToShapeName(const QCursor &cursor) const;
    QIcon cursorToShapeIcon(const QCursor &cursor) const;
    int cursorToValue(const QCursor &cursor) const;
    QCursor valueToCursor(int value) const;

private:
    void appendCursor(Qt::CursorShape shape, const QString& name, const QIcon& icon);
    QStringList cursorNames_;
    QMap<int, QIcon> cursorIcons_;
    QMap<int, Qt::CursorShape> valueToCursorShape_;
    QMap<Qt::CursorShape, int> cursorShapeToValue_;
};
