/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medPluginManager.h>
#include <medPluginLegacy.h>

#include <dtkCoreSupport/dtkAbstractData.h>

// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

QStringList medPluginManagerPathSplitter(QString path)
{
    QString paths = path;

#ifdef Q_OS_WIN
    QStringList pathList;
    QRegExp pathFilterRx("(([a-zA-Z]:|)[^:]+)");

    int pos = 0;

    while ((pos = pathFilterRx.indexIn(paths, pos)) != -1) 
    {

        QString pathItem = pathFilterRx.cap(1);
        pathItem.replace("\\", "/");

        if (!pathItem.isEmpty())
        {
            pathList << pathItem;
        }

        pos += pathFilterRx.matchedLength();
    }

#else
    QStringList pathList = paths.split(":", QString::SkipEmptyParts);
#endif

    return pathList;
}

// /////////////////////////////////////////////////////////////////
// medPluginManagerPrivate
// /////////////////////////////////////////////////////////////////

class medPluginManagerPrivate
{
public:
    bool check(const QString& path);

public:
    QHash<QString, QVariant> names;
    QHash<QString, QVariant> versions;
    QHash<QString, QVariantList> dependencies;

    QHash<QString, QStringList> handlers;
    QStringList loadErrors;

public:
    QString path;

    QHash<QString, QPluginLoader *> loaders;

    bool verboseLoading;
    int argc;
    char *argv;
};

/**
 * @brief Gets an instance of the Plugin Manager.
 *
 *
 * @param void
 * @return medPluginManager * a pointer to an instance of the singleton.
*/

medPluginManager *medPluginManager::instance()
{
    if (!s_instance)
    {
        s_instance = new medPluginManager;

        qRegisterMetaType<dtkAbstractObject>("dtkAbstractObject");
        qRegisterMetaType<dtkAbstractObject *>("dtkAbstractObject*");
        qRegisterMetaType<dtkAbstractData>("dtkAbstractData");
        qRegisterMetaType<dtkAbstractData *>("dtkAbstractData*");
    }

    return s_instance;
}

bool medPluginManagerPrivate::check(const QString& path)
{
    bool status = true;

    for(QVariant item: this->dependencies.value(path))
    {
        QVariantMap mitem = item.toMap();
        QVariant na_mitem = mitem.value("name");
        QVariant ve_mitem = mitem.value("version");
        QString key = this->names.key(na_mitem);

        if (!this->names.values().contains(na_mitem))
        {
            qWarning() << "  Missing dependency:" << na_mitem.toString() << "for plugin" << path;
            status = false;
            continue;
        }

        if (this->versions.value(key) != ve_mitem)
        {
            qWarning() << "    Version mismatch:" << na_mitem.toString() << "version" << this->versions.value(this->names.key(na_mitem)).toString() << "but" << ve_mitem.toString() << "required for plugin" << path;
            status = false;
            continue;
        }

        if (!check(key))
        {
            qWarning() << "Corrupted dependency:" << na_mitem.toString() << "for plugin" << path;
            status = false;
            continue;
        }
    }

    return status;
}

// /////////////////////////////////////////////////////////////////
// medPluginManager
// /////////////////////////////////////////////////////////////////

void medPluginManager::initializeApplication()
{

}

void medPluginManager::initialize()
{
    if (d->path.isNull())
        this->readSettings();

    QStringList pathList = medPluginManagerPathSplitter(d->path);

    const QString appDir = qApp->applicationDirPath();

    for(QString path: pathList)
    {

        QDir dir(appDir);

        if (dir.cd(path))
        {
            dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

            for(QFileInfo entry: dir.entryInfoList())
                scan(entry.absoluteFilePath());

            for(QFileInfo entry: dir.entryInfoList())
                loadPlugin(entry.absoluteFilePath());
        }
        else
        {
            qWarning() << "Failed to load plugins from path " << path << ". Could not cd to directory.";
        }
    }

    emit allPluginsLoaded();
}


/**
 * @brief Uninitialize the manager.
 *
 * @param void
*/
void medPluginManager::uninitialize()
{
    this->writeSettings();

    foreach(QString path, d->loaders.keys())
        unloadPlugin(path);
}

void medPluginManager::uninitializeApplication()
{
    delete qApp;
}


void medPluginManager::scan(const QString& path)
{
    if (!QLibrary::isLibrary(path))
        return;

    QPluginLoader *loader = new QPluginLoader(path);

    d->names.insert(path, loader->metaData().value("MetaData").toObject().value("name").toVariant());
    d->versions.insert(path, loader->metaData().value("MetaData").toObject().value("version").toVariant());
    d->dependencies.insert(path, loader->metaData().value("MetaData").toObject().value("dependencies").toArray().toVariantList());

    delete loader;
}

//! Load a specific plugin designated by its name.
/*! The path is retrieved through the plugin manager settings.
 *
 * \param name The name of the plugin, without platform specific prefix (.e.g lib) and suffix (e.g. .so or .dylib or .dll)
 */

void medPluginManager::load(const QString& name)
{
    if (d->path.isNull())
    {
        this->readSettings();
    }

    QStringList pathList = medPluginManagerPathSplitter(d->path);

    const QString appDir = qApp->applicationDirPath();

    foreach(QString path, pathList) {

        QDir dir(appDir);

        if (dir.cd(path)) {
            dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

            foreach(QFileInfo entry, dir.entryInfoList())
                if (entry.fileName().contains(name))
                    loadPlugin(entry.absoluteFilePath());
        }
        else {
            qWarning() << "Failed to load plugins from path " << path << ". Could not cd to directory.";
        }
    }
}

//! Unload a specific plugin designated by its name.
/*! The path is retrieved through the plugin manager settings.
 *
 * \param name The name of the plugin, without platform specific prefix (.e.g lib) and suffix (e.g. .so or .dylib or .dll)
 */

void medPluginManager::unload(const QString& name)
{
    if (d->path.isNull())
        this->readSettings();

    QStringList pathList = medPluginManagerPathSplitter(d->path);

    const QString appDir = qApp->applicationDirPath();

    for(QString path: pathList)
    {
        QDir dir(appDir);

        if (dir.cd(path))
        {
            dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

            for(QFileInfo entry: dir.entryInfoList())
                if (entry.fileName().contains(name))
                    if (this->plugin(name))
                        this->unloadPlugin(entry.absoluteFilePath());
        }
        else
        {
            qWarning() << "Failed to load plugins from path " << path << ". Could not cd to directory.";
        }
    }
}

void medPluginManager::writeSettings()
{

}

void medPluginManager::printPlugins()
{
    foreach(QString path, d->loaders.keys())
        qDebug() << path;
}

void medPluginManager::setVerboseLoading(bool value)
{
    d->verboseLoading = true;
}

bool medPluginManager::verboseLoading() const
{
    return d->verboseLoading;
}

medPluginLegacy *medPluginManager::plugin(const QString& name)
{
    for(QPluginLoader *loader: d->loaders)
    {
        medPluginLegacy *plugin = qobject_cast<medPluginLegacy *>(loader->instance());

        if (plugin->name() == name)
        {
            return plugin;
        }
    }

    return nullptr;
}

QList<medPluginLegacy *> medPluginManager::plugins()
{
    QList<medPluginLegacy *> list;

    for(QPluginLoader *loader: d->loaders)
        list << qobject_cast<medPluginLegacy *>(loader->instance());

    return list;
}

void medPluginManager::setPath(const QString& path)
{
    d->path = path;
}

QString medPluginManager::path() const
{
    return d->path;
}


/*!
    \brief      Loads the plugin from the given filename.
                Derived classes may override to prevent certain plugins being loaded,
                or provide additional functionality. In most cases they should still
                call the base implementation (this).
    \param      path : Path to plugin file to be loaded.
*/

void medPluginManager::loadPlugin(const QString& path)
{
    if (!d->check(path))
    {
        QString error = "Check failure for plugin file " + path;

        if (d->verboseLoading)
        {
            qWarning() << error;
        }

        return;
    }

    QPluginLoader *loader = new QPluginLoader(path);

    loader->setLoadHints(QLibrary::ExportExternalSymbolsHint);

    if (!loader->load())
    {
        QString error = "Unable to load ";
        error += path;
        error += " - ";
        error += loader->errorString();

        if (d->verboseLoading)
        {
            qWarning() << error;
        }

        emit loadError(error);
        delete loader;
        return;
    }

    medPluginLegacy *plugin = qobject_cast<medPluginLegacy *>(loader->instance());

    if (!plugin)
    {
        QString error = "Unable to retrieve ";
        error += path;

        if (d->verboseLoading)
        {
            qWarning() << error;
        }

        emit loadError(error);
        return;
    }

    if (!plugin->initialize())
    {
        QString error = "Unable to initialize ";
        error += plugin->name();
        error += " plugin";

        if (d->verboseLoading) {
            qWarning() << error;
        }

        emit loadError(error);
        return;
    }

    d->loaders.insert(path, loader);

    if (d->verboseLoading) {
        qDebug() << "Loaded plugin " << plugin->name() << " from " << path;
    }

    emit loaded(plugin->name());
}

//! Unloads the plugin previously loaded from the given filename.
/*! Derived classes may override to prevent certain plugins being
 *  unloaded, or provide additional functionality. In most
 *  cases they should still call the base implementation
 *  (this).
 *
 * \param path Path to plugin file to be unloaded.
 */
void medPluginManager::unloadPlugin(const QString& path)
{
    medPluginLegacy *plugin = qobject_cast<medPluginLegacy *>(d->loaders.value(path)->instance());

    if (!plugin)
    {
        if (d->verboseLoading)
        {
            qDebug() << "medPluginManager - Unable to retrieve " << plugin->name() << " plugin";
        }

        return;
    }

    if (!plugin->uninitialize())
    {
        if (d->verboseLoading)
        {
            qDebug() << "Unable to uninitialize " << plugin->name() << " plugin";
        }

        return;
    }

    QPluginLoader *loader = d->loaders.value(path);

    if (!loader->unload())
    {
        if (d->verboseLoading)
        {
            qDebug() << "medPluginManager - Unable to unload plugin: " << loader->errorString();
        }

        return;
    }

    delete loader;

    d->loaders.remove(path);
 }




/**
 * @brief 
 *
 */
void medPluginManager::readSettings()
{
    QDir plugins_dir;
    QString defaultPath;
#ifdef Q_OS_MAC
    plugins_dir = qApp->applicationDirPath() + "/../PlugIns";
#elif defined(Q_OS_WIN)
    plugins_dir = qApp->applicationDirPath() + "/plugins_legacy";
#else
    plugins_dir = qApp->applicationDirPath() + "/plugins_legacy";
#endif
    defaultPath = plugins_dir.absolutePath();

    const char PLUGIN_PATH_VAR_NAME[] = "MEDINRIA_PLUGINS_DIR_LEGACY";
    QByteArray pluginVarArray = qgetenv(PLUGIN_PATH_VAR_NAME);

    if ( !pluginVarArray.isEmpty() )
    {
        setPath( QString(pluginVarArray.constData()));
    }
    else
    {
        setPath(defaultPath);
    }

    if(path().isEmpty())
    {
        qWarning() << "Your config does not seem to be set correctly.";
        qWarning() << "Please set "                  << QString(PLUGIN_PATH_VAR_NAME);
        qWarning() << "Default directory would be: " << defaultPath;
        qWarning() << "Actual directory is: "        << path();
    }
}

/**
 * @brief Gets a list of plugins belonging to 'category'
 *
 * @param category The category to use as a filter
 * @return QStringList list of plugin names
*/
QStringList medPluginManager::handlers(const QString& category)
{
    if (d->handlers.contains(category))
    {
        return d->handlers.value(category);
    }
    return QStringList();
}

/**
 * @brief Adds the plugin to the handlers.
 *
 * @param name Name of the loaded plugin
*/
void medPluginManager::onPluginLoaded(const QString& name)
{
    qDebug() << " Loading plugin: " << name;

    medPluginLegacy *plug = plugin(name);

    QStringList categories;

    if (plug->hasMetaData("category"))
    {
        categories = plug->metaDataValues("category");
    }

    for(QString category : categories)
    {
        d->handlers[category] << plug->types();
    }
}

/**
 * @brief Hidden constructor because of the singleton.
 *
 * Use instance() instead.
 * @param void
*/
medPluginManager::medPluginManager(void) : d(new medPluginManagerPrivate)
{
    d->verboseLoading = false;
    d->argv = nullptr;

    connect(this, SIGNAL(loaded(const QString&)), this, SLOT(onPluginLoaded(const QString&)));
    connect(this, SIGNAL(loadError(QString)), this, SLOT(onLoadError(QString)));
}

/**
 * @brief Hidden destructor because of the singleton.
 *
 * @param void
*/
medPluginManager::~medPluginManager()
{
    delete d;
    d = nullptr;
}

medPluginManager *medPluginManager::s_instance = nullptr;


void medPluginManager::onLoadError(const QString &errorMessage)
{
    qDebug() << "add error message to pluginManager:";
    qDebug() << "\t" << errorMessage;
    d->loadErrors << errorMessage;
}

QStringList medPluginManager::loadErrors()
{
    return d->loadErrors;
}
