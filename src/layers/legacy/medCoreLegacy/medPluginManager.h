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
#include <tuple>

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

/**
 * @brief Load and unload plugins for the application
 *
*/
class MEDCORELEGACY_EXPORT medPluginManager : public QObject
{
    Q_OBJECT

public:
    static medPluginManager *instance();

    void loadPluginFromDirectories(QStringList pluginDirs);
    QStringList getValidPluginPathList(QStringList pluginDirs);
    void savePluginAndMetadata(QStringList pluginsPaths);
    void loadPluginsByCategory();

    void initialize();
    void uninitialize();

    void unload(const QString& name);

    void readSettings();

    void printPlugins();

    medPluginLegacy   *plugin(const QString& name);
    QList<medPluginLegacy *> plugins();

    void setPath(const QString& path);

    QStringList loadErrors();

public slots:
    void unloadPlugin(const QString& path);
    void onPluginLoaded(const QString& name);

protected:
     medPluginManager();
     QString path() const;

protected slots:
     void onLoadError(const QString& errorMessage);

signals:
     void allPluginsLoaded();
     void loaded(const QString& plugin);
     void unloaded(const QString& plugin);
     void loadError(const QString& errorMessage);

private:
    static medPluginManager *s_instance;
    QStringList loadErrorsList;
    QString pathSettings;
    QList<std::tuple<int, QString, QPluginLoader*, QString, medPluginLegacy*> > m_lPlugins;

    int getCategoryFromTuple(std::tuple<int, QString, QPluginLoader*, QString, medPluginLegacy*> tuple);
    QString getPathFromTuple(std::tuple<int, QString, QPluginLoader*, QString, medPluginLegacy*> tuple);
    QPluginLoader* getPluginLoaderFromTuple(std::tuple<int, QString, QPluginLoader*, QString, medPluginLegacy*> tuple);
    QString getNameFromTuple(std::tuple<int, QString, QPluginLoader*, QString, medPluginLegacy*> tuple);
    medPluginLegacy* getMedPluginFromTuple(std::tuple<int, QString, QPluginLoader*, QString, medPluginLegacy*> tuple);
};
