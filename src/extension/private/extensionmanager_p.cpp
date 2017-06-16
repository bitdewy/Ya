#include "extensionmanager_p.h"
#include "extension/abstractextensionfactory.h"

ExtensionManagerPrivate::ExtensionManagerPrivate()
{

}

ExtensionManagerPrivate::~ExtensionManagerPrivate()
{

}

void ExtensionManagerPrivate::registerExtensions(AbstractExtensionFactory* factory, const QString& iid)
{
    if (iid.isEmpty())
    {
        globalExtension_.prepend(factory);
        return;
    }

    auto it = extensions_.find(iid);
    if (it == extensions_.end())
    {
        it = extensions_.insert(iid, FactoryList());
    }

    it.value().prepend(factory);
}

void ExtensionManagerPrivate::unregisterExtensions(AbstractExtensionFactory* factory, const QString& iid)
{
    if (iid.isEmpty())
    {
        globalExtension_.removeAll(factory);
        return;
    }

    auto it = extensions_.find(iid);
    if (it == extensions_.end())
    {
        return;
    }

    auto&& factories = it.value();
    factories.removeAll(factory);

    if (factories.isEmpty())
    {
        extensions_.erase(it);
    }
}

QObject* ExtensionManagerPrivate::extension(QObject* object, const QString& iid) const
{
    auto it = extensions_.constFind(iid);
    if (it != extensions_.constEnd())
    {
        auto fcend = it.value().constEnd();
        for (auto fit = it.value().constBegin(); fit != fcend; ++fit)
        {
            if (auto ext = (*fit)->extension(object, iid))
            {
                return ext;
            }
        }
    }
    auto gfcend =  globalExtension_.constEnd();
    for (auto git = globalExtension_.constBegin(); git != gfcend; ++git)
    {
        if (auto ext = (*git)->extension(object, iid))
        {
            return ext;
        }
    }
    return nullptr;
}
