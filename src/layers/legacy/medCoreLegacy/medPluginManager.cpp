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

medPluginManager *medPluginManager::s_instance = nullptr;


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

QStringList medPluginManager::medPluginManagerPathSplitter(QString paths)
{
    QStringList pathList;
#ifdef Q_OS_WIN
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
    pathList = paths.split(":", QString::SkipEmptyParts);
#endif

    return pathList;
}

QString medPluginManager::readSettings(char const *pi_pchPluginPathVarName)
{
    QString sPathRes;

    QDir pluginsDir;
    QString defaultPath;

#ifdef Q_OS_MAC
    plugins_dir = qApp->applicationDirPath() + "/../PlugIns";
#elif defined(Q_OS_WIN)
    pluginsDir = qApp->applicationDirPath() + "/plugins_legacy";
#endif
    defaultPath = pluginsDir.absolutePath();

    if (pi_pchPluginPathVarName == nullptr)
    {
        pi_pchPluginPathVarName = "MEDINRIA_PLUGINS_DIR_LEGACY";
    }

    QByteArray pluginVarArray = qgetenv(pi_pchPluginPathVarName);

    if (!pluginVarArray.isEmpty())
    {
        sPathRes = pluginVarArray.constData();
    }
    else
    {
        sPathRes = defaultPath;
    }

    if (!QDir(sPathRes).exists())
    {
        qWarning() << "Your config does not seem to be set correctly.";
        qWarning() << "Please set " << pi_pchPluginPathVarName;
        qWarning() << "Default directory would be: " << defaultPath;
        qWarning() << "Actual directory is: " << sPathRes;
    }

    return sPathRes;
}

//---------------------------------------------------------------------------------------------------------------------

void medPluginManager::loadPluginFromDirectories(QStringList pluginDirs)
{
    QStringList pluginsPaths = getValidPluginPathList(pluginDirs);
    savePluginAndMetadata(pluginsPaths);
    loadPluginsByCategory();
    emit allPluginsLoaded();
}

QStringList medPluginManager::getValidPluginPathList(QStringList pluginDirs)
{
    QDir dir;
    QStringList pluginsPaths;
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    for (QString dirPath: pluginDirs)
    {
        dir.setPath(dirPath);
        if (dir.cd(dirPath)) //check if the directory exist
        {
            // Recursive search of files in dirPath
            QDirIterator it(dirPath, QDir::Files|QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
            while (it.hasNext())
            {
                QString currentPath = it.next(); // First next is first file
                if(QLibrary::isLibrary(currentPath))
                {
                    pluginsPaths.push_back(currentPath);
                }
            }
        }
        else
        {
            emit loadError(dirPath + " is not a valid path.");
        }
    }

    return pluginsPaths;
}

void medPluginManager::savePluginAndMetadata(QStringList pluginsPaths)
{
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
                sPluginEntry newLine = { static_cast<eCategory>(iCategory), path, pLoader, QString(), nullptr };
                m_lPlugins.push_back(newLine);
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
}

void medPluginManager::loadPluginsByCategory()
{
    unsigned int uiNbCores = std::thread::hardware_concurrency();

    if (uiNbCores < 4)
        uiNbCores /= 2;






    std::atomic<unsigned int> index = 0;
    std::mutex iomutex;
    std::vector<std::thread> threads(uiNbCores);
    for (unsigned i = 0; i < uiNbCores; ++i)
    {
        threads[i] = std::thread([&iomutex, i] {
            {
                // Use a lexical scope and lock_guard to safely lock the mutex only for
                // the duration of std::cout usage.
                std::lock_guard<std::mutex> iolock(iomutex);
                std::cout << "Thread #" << i << " is running\n";
            }

            // Simulate important work done by the tread by sleeping for a bit...
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        });
    }

    for (auto& t : threads) {
        t.join();
    }








    for (int iCatNum = 1; iCatNum < 7; ++iCatNum)
    {
        auto it = m_lPlugins.begin();
        auto itEnd = m_lPlugins.end();
        for (; it != itEnd; ++it)
        {
            if (it->type == iCatNum)
            {
                QPluginLoader *pLoader = it->handler;
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
                        loadError(it->path + " error. " + pLoader->errorString());
                    }
                }

                if (!name.isEmpty() && ((!bLazy && medPlugin) || bLazy))
                {
                    it->name = name;
                    it->object = medPlugin;

                    emit loaded(name);
                }
                else
                {
                    loadError(it->path + " plugin not kept. " + pLoader->errorString());
                    delete(pLoader);
                    it = m_lPlugins.erase(it);
                }
            }
        }
    }
}




void medPluginManager::printPlugins()
{
    for (auto line: m_lPlugins)
    {
        qDebug() << line.path;
    }
}

medPluginLegacy* medPluginManager::plugin(const QString& name)
{
    medPluginLegacy *pPluginRes = nullptr;


    for (auto line: m_lPlugins)
    {
        // TODO Homepage->Plugins, double click on a plugin: the asked name
        // is the plugin name, not the getNameFromTuple. But plugin(name) is used
        // in medPluginManager with getNameFromTuple.

        if (line.name == name)
        {
            pPluginRes = line.object;
        }
    }

    return pPluginRes;
}

QList<medPluginLegacy *> medPluginManager::plugins()
{
    QList<medPluginLegacy *> list;

    for (auto line: m_lPlugins)
    {
        medPluginLegacy* currentPlugin = line.object;
        if (currentPlugin)
        {
            list << currentPlugin;
        }
    }

    return list;
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
        if (line.path == path)
        {
            plugin = line.object;
            loader = line.handler;
            break;
        }
        cpt++;
    }

    if (!plugin)
    {
        qDebug() << "Unable to retrieve " << QFileInfo(path).fileName() << " plugin.";
        return;
    }

    if (!plugin->uninitialize())
    {
        qDebug() << "Unable to uninitialize " << plugin->name() << " plugin.";
        return;
    }

    if (!loader->unload())
    {
        qDebug() << "Unable to unload plugin. " << loader->errorString();
        return;
    }

    delete loader;
    //m_lPlugins.removeAt(cpt); TODO redo
}

//! Unload a specific plugin designated by its name.
/*! The path is retrieved through the plugin manager settings.
 *
 * \param name The name of the plugin, without platform specific prefix (.e.g lib) and suffix (e.g. .so or .dylib or .dll)
 */
void medPluginManager::unload(const QString& name)
{
  //TODO
}

void medPluginManager::unload()
{
    //TODO
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








/**
 * @brief onLoadError displays an error message into several log systems.
 * It displays the error QString message in the classic logs (terminal and .log file),
 * and also in the 'Failed Plugin' widget on homepage.
 * @param errorMessage
 */
void medPluginManager::onLoadError(const QString &errorMessage)
{
    qDebug() << "Error from Plugin Manager: " << qPrintable(errorMessage);
    m_loadErrorsList << errorMessage;
}

QStringList medPluginManager::loadErrors()
{
    return m_loadErrorsList;
}
