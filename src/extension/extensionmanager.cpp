#include "extensionmanager.h"
#include "private/extensionmanager_p.h"

ExtensionManager::ExtensionManager(QObject* parent) :
    QObject(parent),
    impl_(new ExtensionManagerPrivate)
{
}

ExtensionManager::~ExtensionManager()
{
}

void ExtensionManager::registerExtensions(AbstractExtensionFactory* factory, const QString& iid)
{
    impl_->registerExtensions(factory, iid);
}

void ExtensionManager::unregisterExtensions(AbstractExtensionFactory* factory, const QString& iid)
{
    impl_->unregisterExtensions(factory, iid);
}

QObject* ExtensionManager::extension(QObject* object, const QString& iid) const
{
    return impl_->extension(object, iid);
}

