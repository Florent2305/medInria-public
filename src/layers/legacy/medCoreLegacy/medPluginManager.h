#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <QtCore>
#include <medCoreLegacyExport.h>

#include <list>

class medPluginLegacy;

enum eCategory
{
    CAT_IODATA = 1,
    CAT_DB,
    CAT_FEATURE,
    CAT_AREA,
    CAT_WORKSPACE,
    CAT_PROCESS
};

struct sPluginEntry
{
    eCategory         type;
    QString           path;
    QPluginLoader*    handler;
    QString           name;
    medPluginLegacy*  object;
};

/**
 * @brief Load and unload plugins for the application
 *
 */
class MEDCORELEGACY_EXPORT medPluginManager : public QObject
{
    Q_OBJECT

public:
    static medPluginManager *instance();
    static QStringList medPluginManagerPathSplitter(QString paths);
    static QString readSettings(char const *pi_pchPluginPathVarName = nullptr);


    void loadPluginFromDirectories(QStringList pluginDirs);
    void unload(const QString& name);



    medPluginLegacy   *plugin(const QString& name);
    QList<medPluginLegacy *> plugins();



    void printPlugins();
    QStringList loadErrors();

private:
    QStringList getValidPluginPathList(QStringList pluginDirs);
    void savePluginAndMetadata(QStringList pluginsPaths);
    void loadPluginsByCategory();
    


public slots:
    void unloadPlugin(const QString& path);
    void onPluginLoaded(const QString& name);

protected:
     medPluginManager();

protected slots:
     void onLoadError(const QString& errorMessage);

signals:
     void allPluginsLoaded();
     void loaded(const QString &plugin);
     void unloaded(const QString &plugin);
     void loadError(const QString &errorMessage);

private:
    static medPluginManager *s_instance;
    std::list<sPluginEntry> m_lPlugins;
    QStringList m_loadErrorsList;
};
