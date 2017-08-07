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
#include <QtCOre/QString>

#define TYPEID(IFace) QLatin1String(IFace##_iid)

class QString;
class AbstractExtensionFactory;

class AbstractExtensionManager
{
public:
    virtual ~AbstractExtensionManager() {}

    virtual void registerExtensions(AbstractExtensionFactory* factory, const QString& iid) = 0;
    virtual void unregisterExtensions(AbstractExtensionFactory* factory, const QString& iid) = 0;

    virtual QObject* extension(QObject* object, const QString& iid) const = 0;
};

Q_DECLARE_INTERFACE(AbstractExtensionManager, "bitdewy.Ya.AbstractExtensionManager")

template <typename T>
inline T extension(AbstractExtensionManager*, QObject*)
{
    return nullptr;
}

#define Y_DECLARE_EXTENSION_INTERFACE(IFace, IId) \
const char* const IFace##_iid = IId; \
Q_DECLARE_INTERFACE(IFace, IId) \
template <> inline IFace* extension<IFace*>(AbstractExtensionManager* manager, QObject* object) \
{ \
    QObject* extension = manager->extension(object, TYPEID(IFace)); \
    return extension ? static_cast<IFace*>(extension->qt_metacast(IFace##_iid)) : static_cast<IFace*>(nullptr); \
}
