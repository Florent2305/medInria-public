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

void medPluginManager::loadPluginFromDirectories(QStringList pluginDirs)
{
    QDir dir;
    QStringList pluginsPaths;
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    for (QString dirPath: pluginDirs)
    {
        dir.setPath(dirPath);
        if (dir.cd(dirPath))
        {
            // Recursive search of files in dirPath
            QDirIterator it(dirPath, QDir::Files|QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
            while (it.hasNext())
            {
                QString currentPath = it.next(); // First next is first file
                if (m_oExtensions.contains(QString(".")+QFileInfo(currentPath).completeSuffix()))
                {
                    pluginsPaths.push_back(currentPath);
                }
            }
        }
        else
        {
            loadError(dirPath + " is not a valid path.");
        }
    }

    for (QString path: pluginsPaths)
    {
        int iCategory = 0;

        QPluginLoader *pLoader = new QPluginLoader(path);

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
                loadError(QFileInfo(path).fileName() + " configuration file is set with a wrong category.");
            }
        }
        else
        {
            loadError(QFileInfo(path).fileName() + " is not valid, metadata are malformed.");
        }
    }

    for (int iCatNum = 1; iCatNum < 7; ++iCatNum)
    {
        auto it = m_lPlugins.begin();
        auto itEnd = m_lPlugins.end();
        for (; it != itEnd; ++it)
        {
            if (getCategoryFromTuple(*it) == iCatNum)
            {
                QPluginLoader *pLoader = getPluginLoaderFromTuple(*it);
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
                    //You can give some hints on how the symbols are resolved. Usually, the symbols are not resolved at load time,
                    //but resolved lazily, (that is, when resolve() is called). If you set the loadHints to ResolveAllSymbolsHint,
                    //then all symbols will be resolved at load time if the platform supports it.
                    //Setting ExportExternalSymbolsHint will make the external symbols in the library available for resolution in subsequent loaded libraries.
                    pLoader->setLoadHints(QLibrary::ExportExternalSymbolsHint);
                    QObject* pInstance = pLoader->instance();
                    if (pInstance)
                    {
                        medPlugin = dynamic_cast<medPluginLegacy*>(pInstance);
                    }
                    else
                    {
                        loadError(getPathFromTuple(*it) + " error. " + pLoader->errorString());
                    }
                }

                if (!name.isEmpty() && ((!bLazy && medPlugin) || bLazy))
                {
                    *it = std::make_tuple(iCatNum, getPathFromTuple(*it), pLoader, name, medPlugin);

                    emit loaded(name);
                }
                else
                {
                    loadError(getPathFromTuple(*it) + " plugin not kept. " + pLoader->errorString());
                    delete(pLoader);
                    it = m_lPlugins.erase(it);
                }
            }
        }
    }
}

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

// /////////////////////////////////////////////////////////////////
// medPluginManager
// /////////////////////////////////////////////////////////////////

void medPluginManager::initialize()
{
    if (path().isNull())
    {
        this->readSettings();
    }

    setValidFileExtensions(QStringList(QString(".so"))
                           << QString("*.dll")
                           << QString("*.dylib"));

    QStringList pathList = medPluginManagerPathSplitter(path());
    loadPluginFromDirectories(pathList);

    emit allPluginsLoaded();
}

/**
 * @brief Uninitialize the manager.
 *
 * @param void
*/
void medPluginManager::uninitialize()
{
    for (auto line: m_lPlugins)
    {
        unloadPlugin(getPathFromTuple(line));
    }
}

void medPluginManager::printPlugins()
{
    for (auto line: m_lPlugins)
    {
        qDebug() << getPathFromTuple(line);
    }
}

medPluginLegacy *medPluginManager::plugin(const QString& name)
{
    for (auto line: m_lPlugins)
    {
        // TODO Homepage->Plugins, double click on a plugin: the asked name
        //is the plugin name, not the getNameFromTuple. But plugin(name) is used
        // in medPluginManager with getNameFromTuple.
        /*medPluginLegacy* plugin = getMedPluginFromTuple(line);
        if (plugin)
        {
            if (plugin->name() == name)
            {
                return plugin;
            }
        }*/
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
    pathSettings = path;
}

QString medPluginManager::path() const
{
    return pathSettings;
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
    medPluginLegacy *plugin = nullptr;
    QPluginLoader *loader = nullptr;
    int cpt = 0;

    for (auto line: m_lPlugins)
    {
        if (getPathFromTuple(line) == path)
        {
            plugin = getMedPluginFromTuple(line);
            loader = getPluginLoaderFromTuple(line);
            break;
        }
        cpt++;
    }

    if (!plugin)
    {
        qDebug() << "Unable to retrieve " << QFileInfo(path).fileName() << " plugin";
        return;
    }

    if (!plugin->uninitialize())
    {
        qDebug() << "Unable to uninitialize " << plugin->name() << " plugin";
        return;
    }

    if (!loader->unload())
    {
        qDebug() << "Unable to unload plugin. " << loader->errorString();
        return;
    }

    delete loader;
    m_lPlugins.removeAt(cpt);
}

//! Unload a specific plugin designated by its name.
/*! The path is retrieved through the plugin manager settings.
 *
 * \param name The name of the plugin, without platform specific prefix (.e.g lib) and suffix (e.g. .so or .dylib or .dll)
 */
void medPluginManager::unload(const QString& name)
{
    if (path().isNull())
    {
        this->readSettings();
    }

    QStringList pathList = medPluginManagerPathSplitter(path());

    const QString appDir = qApp->applicationDirPath();

    for(QString path: pathList)
    {
        QDir dir(appDir);

        if (dir.cd(path))
        {
            dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

            for(QFileInfo entry: dir.entryInfoList())
            {
                if (entry.fileName().contains(name))
                {
                    if (this->plugin(name))
                    {
                        this->unloadPlugin(entry.absoluteFilePath());
                    }
                }
            }
        }
        else
        {
            qWarning() << "Failed to load plugins from path " << path << ". Could not cd to directory.";
        }
    }
}

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
 * @brief Adds the plugin to the handlers.
 *
 * @param name Name of the loaded plugin
*/
void medPluginManager::onPluginLoaded(const QString& name)
{
    qDebug() << " Loading plugin: " << qPrintable(name);

    medPluginLegacy *plug = plugin(name);

    if (plug)
    {
        plug->initialize();
    }
    }
}

/**
 * @brief Hidden constructor because of the singleton.
 *
 * Use instance() instead.
 * @param void
*/
medPluginManager::medPluginManager()
{
    connect(this, SIGNAL(loaded(const QString&)), this, SLOT(onPluginLoaded(const QString&)));
    connect(this, SIGNAL(loadError(QString)), this, SLOT(onLoadError(QString)));
}

medPluginManager *medPluginManager::s_instance = nullptr;

/**
 * @brief onLoadError displays an error message into several log systems.
 * It displays the error QString message in the classic logs (terminal and .log file),
 * and also in the 'Failed Plugin' widget on homepage.
 * @param errorMessage
 */
void medPluginManager::onLoadError(const QString &errorMessage)
{
    qDebug() << "Error from Plugin Manager: " << qPrintable(errorMessage);
    loadErrorsList << errorMessage;
}

QStringList medPluginManager::loadErrors()
{
    return loadErrorsList;
}
