#include "extensionfactory_p.h"
#include "extension/extensionfactory.h"

ExtensionFactoryPrivate::ExtensionFactoryPrivate()
{

}

ExtensionFactoryPrivate::~ExtensionFactoryPrivate()
{

}

QObject* ExtensionFactoryPrivate::extension(ExtensionFactory *factory, QObject *object, const QString &iid)
{
    if (!object)
    {
        return nullptr;
    }
    auto key = qMakePair(iid, object);

    auto it = extensions_.find(key);
    if (it == extensions_.end())
    {
        if (auto ext = factory->createExtension(object, iid, factory))
        {
            QObject::connect(ext, &QObject::destroyed, [this](QObject* o){ objectDestroyed(o); });
            it = extensions_.insert(key, ext);
        }
    }

    if (!extended_.contains(object))
    {
        QObject::connect(object, &QObject::destroyed, [this](QObject* o){ objectDestroyed(o); });
        extended_.insert(object, true);
    }

    if (it == extensions_.end())
    {
        return nullptr;
    }

    return it.value();
}

void ExtensionFactoryPrivate::objectDestroyed(QObject* object)
{
    for (auto it = extensions_.begin(); it != extensions_.end(); ++it)
    {
        auto o = it.key().second;
        if (o == object || object == it.value())
        {
            it = extensions_.erase(it);
        }
    }
    extended_.remove(object);
}
