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

#define CATEGORY "category"
#define LAZY "lazy"
#define NAME "name"

// /////////////////////////////////////////////////////////////////
// medPluginManagerPrivate
// /////////////////////////////////////////////////////////////////

class medPluginManagerPrivate
{
public:
    QStringList loadErrors;

public:
    QString path;

    QHash<QString, QPluginLoader *> loaders;

    bool verboseLoading;
};

void medPluginManager::loadPluginFromDirectories(QStringList pluginDirs)
{
    QDir dir;
    QStringList pluginsPaths;
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    dir.setNameFilters(m_oExtensions);

    for (QString dirPath: pluginDirs)
    {
        dir.setPath(dirPath);
        if (dir.cd(dirPath))
        {
            for (QFileInfo dirEntry: dir.entryInfoList())
            {
                pluginsPaths.push_back(dirEntry.absoluteFilePath());
            }
        }
        else
        {
            //TODO WARNING
        }
    }

    for (QString path: pluginsPaths)
    {
        int iCategory = 0;
        QPluginLoader *pLoader = nullptr;

        if (QFileInfo(path).isDir())
        {
            pLoader = new QPluginLoader(QFileInfo(path).baseName() + ".so");
        }
        else
        {
            pLoader = new QPluginLoader(path);
        }
        QJsonObject metaData = pLoader->metaData();
        if (!metaData.empty() && metaData.contains("MetaData") && metaData.value("MetaData").toObject().contains(CATEGORY) && metaData.value("MetaData").toObject().value(CATEGORY).isDouble())
        {
            iCategory = metaData.value("MetaData").toObject().value(CATEGORY).toInt();
            if (iCategory > 0 && iCategory < 7)
            {
                m_lPlugins.push_back(std::make_tuple(iCategory, path, pLoader, QString(), nullptr));
            }
            else
            {
                //TODO ERROR BAD CAT
            }
        }
        else
        {
            loadError("### NOT OK");
            //TODO ERROR IT IS NOT A VALID PLUGIN BECAUSE METADATA ARE MALFORMED
        }
    }

    for (int iCatNum=1; iCatNum <7; ++iCatNum)
    {
        auto it = m_lPlugins.begin();
        auto itEnd = m_lPlugins.end();
        for (; it != itEnd; ++it)
        {
            if (std::get<0>(*it) == iCatNum)
            {
                QPluginLoader *pLoader = std::get<2>(*it);
                QString name;
                medPluginLegacy* medPlugin = nullptr;
                bool bLazy = false;

                QJsonObject metaData = pLoader->metaData();
                if (metaData.value("MetaData").toObject().contains(LAZY) &&
                    metaData.value("MetaData").toObject().value(LAZY).isBool())
                {
                    bLazy = metaData.value("MetaData").toObject().value(LAZY).toBool();
                }


                if (metaData.value("MetaData").toObject().contains(NAME) &&
                    metaData.value("MetaData").toObject().value(NAME).isString())
                {
                    name = metaData.value("MetaData").toObject().value(NAME).toString();
                }

                if (!bLazy)
                {
                    pLoader->setLoadHints(QLibrary::ExportExternalSymbolsHint); //EPLICATIONNNNNNNNN
                    QObject* pInstance = pLoader->instance();
                    if (pInstance)
                    {
                        medPlugin = dynamic_cast<medPluginLegacy*>(pInstance);
                    }
                    else
                    {
                        //pLoader->errorString()
                    }
                }

                if (!name.isEmpty() && ((!bLazy && medPlugin) || bLazy))
                {
                    *it = std::make_tuple(iCatNum, std::get<1>(*it), pLoader, name, medPlugin);

                    d->loaders.insert(std::get<1>(*it), pLoader);
                    emit loaded(name);
                }
                else
                {
                    delete(pLoader);
                    it = m_lPlugins.erase(it);
                    //TODO SEND ERROR
                }
            }
        }
    }
}


/*void medPluginManager::loadPlugin(QString const & pi_roPluginPath)
{

}*/

void medPluginManager::setValidFileExtensions(QStringList const &pi_roExts)
{
    m_oExtensions = pi_roExts;
}

int medPluginManager::getCategoryFromTuple(std::tuple<int, QString, QPluginLoader*, QString, medPluginLegacy*> tuple)
{
    return std::get<0>(tuple);
}

QString medPluginManager::getPathFromTuple(std::tuple<int, QString, QPluginLoader*, QString, medPluginLegacy*> tuple)
{
    return std::get<1>(tuple);
}

QPluginLoader* medPluginManager::getPluginLoaderFromTuple(std::tuple<int, QString, QPluginLoader*, QString, medPluginLegacy*> tuple)
{
    return std::get<2>(tuple);
}

QString medPluginManager::getNameFromTuple(std::tuple<int, QString, QPluginLoader*, QString, medPluginLegacy*> tuple)
{
    return std::get<3>(tuple);
}

medPluginLegacy* medPluginManager::getMedPluginFromTuple(std::tuple<int, QString, QPluginLoader*, QString, medPluginLegacy*> tuple)
{
    return std::get<4>(tuple);
}



























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

/*bool medPluginManagerPrivate::check(const QString& path)
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
}*/

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

    setValidFileExtensions(QStringList(QString("*.dll")) << QString("*.so"));

    QStringList pathList = medPluginManagerPathSplitter(d->path);
    loadPluginFromDirectories(pathList);
    /*const QString appDir = qApp->applicationDirPath();

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
    }*/

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


/*void medPluginManager::scan(const QString& path)
{
    if (!QLibrary::isLibrary(path))
        return;

    QPluginLoader *loader = new QPluginLoader(path);

    d->names.insert(path, loader->metaData().value("MetaData").toObject().value("name").toVariant());
    d->versions.insert(path, loader->metaData().value("MetaData").toObject().value("version").toVariant());
    d->dependencies.insert(path, loader->metaData().value("MetaData").toObject().value("dependencies").toArray().toVariantList());

    delete loader;
}/*

//! Load a specific plugin designated by its name.
/*! The path is retrieved through the plugin manager settings.
 *
 * \param name The name of the plugin, without platform specific prefix (.e.g lib) and suffix (e.g. .so or .dylib or .dll)
 */

/*void medPluginManager::load(const QString& name)
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
}*/

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
    for (auto line: m_lPlugins)
    {
        if (getNameFromTuple(line) == name)
        {
            return getMedPluginFromTuple(line);
        }
    }

    return nullptr;
}

QList<medPluginLegacy *> medPluginManager::plugins()
{
    QList<medPluginLegacy *> list;

    for (auto line: m_lPlugins)
    {
        medPluginLegacy* currentPlugin = getMedPluginFromTuple(line);
        if (currentPlugin)
        {
            list << currentPlugin;
        }
    }

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

/*void medPluginManager::loadPlugin(const QString& path)
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
}*/

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
    medPluginLegacy *plugin = nullptr;
    QPluginLoader *loader = nullptr;
    auto it = m_lPlugins.begin();

    for (auto line: m_lPlugins)
    {
        if (getPathFromTuple(line) == path)
        {
            plugin = getMedPluginFromTuple(line);
            loader = getPluginLoaderFromTuple(line);
        }
        it++;
    }

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

    if (!loader->unload())
    {
        if (d->verboseLoading)
        {
            qDebug() << "medPluginManager - Unable to unload plugin: " << loader->errorString();
        }

        return;
    }

    delete loader;

    m_lPlugins.erase(it);
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
/*QStringList medPluginManager::handlers(const QString& category)
{
    if (d->handlers.contains(category))
    {
        return d->handlers.value(category);
    }
    return QStringList();
}*/

/**
 * @brief Adds the plugin to the handlers.
 *
 * @param name Name of the loaded plugin
*/
void medPluginManager::onPluginLoaded(const QString& name)
{
    qDebug() << " Loading plugin: " << name;

    medPluginLegacy *plug = plugin(name);

    if (plug)
    {
        plug->initialize();

//        QStringList categories;
//        if (plug->hasMetaData("category"))
//        {
//            categories = plug->metaDataValues("category");
//        }
    }

//        foreach(QString category, categories)
//        {
//            d->handlers[category] << plug->types();
//        }
    }
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
    //d->argv = nullptr;

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
