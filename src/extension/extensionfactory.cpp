#include "extensionfactory.h"
#include "extension/extensionmanager.h"
#include "extension/private/extensionfactory_p.h"

ExtensionFactory::ExtensionFactory(ExtensionManager* parent) :
    QObject(parent),
    impl_(new ExtensionFactoryPrivate)
{
}

ExtensionFactory::~ExtensionFactory()
{

}

QObject *ExtensionFactory::extension(QObject *object, const QString &iid) const
{
    return impl_->extension(const_cast<ExtensionFactory*>(this), object, iid);
}

ExtensionManager *ExtensionFactory::extensionManager() const
{
    return static_cast<ExtensionManager*>(parent());
}

QObject* ExtensionFactory::createExtension(QObject* object, const QString& iid, QObject* parent) const
{
    Q_UNUSED(object);
    Q_UNUSED(iid);
    Q_UNUSED(parent);
    return nullptr;
}
