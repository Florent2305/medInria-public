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
class medPluginManagerPrivate;

using eCategory = enum
{
    CAT_IODATA = 1,
    CAT_DB,
    CAT_FEATURE,
    CAT_AREA,
    CAT_WORKSPACE,
    CAT_PROCESS
};

/**
 * @brief Load and unload plugins for medInria.
 *
*/
class MEDCORELEGACY_EXPORT medPluginManager : public QObject
{
    Q_OBJECT

public:
    static medPluginManager *instance();

    void loadPluginFromDirectories(QStringList pluginDirs);//scanPlugins scanForPlugins scanPluginsDirs
   // void loadPlugin(QString const & pi_roPluginPath); //createPluginLoader



    void setValidFileExtensions(QStringList const &pi_roExts);
private:

    QStringList m_oExtensions;
    std::list<std::tuple<int, QString, QPluginLoader*, QString, medPluginLegacy*> > m_lPlugins;





    //void registerPlugins();































public:
    void initializeApplication();
    void initialize();
    void uninitialize();
    void uninitializeApplication();


    void   scan(const QString& path);
    //void   load(const QString& name);
    void unload(const QString& name);

    void  readSettings();
    void writeSettings();

    void printPlugins();

    void setVerboseLoading(bool value);
    bool verboseLoading() const;

    medPluginLegacy   *plugin(const QString& name);
    QList<medPluginLegacy *> plugins();

    void setPath(const QString& path);

    QStringList handlers(const QString& category);

    QStringList loadErrors();

public slots:
    //void   loadPlugin(const QString& path);
    void unloadPlugin(const QString& path);
    void onPluginLoaded(const QString& name);

protected:
     medPluginManager();
    ~medPluginManager();

    QString path() const;

protected slots:
     void onLoadError(const QString& errorMessage);

signals:
     void allPluginsLoaded();
     void   loaded(const QString& plugin);
     void unloaded(const QString& plugin);
     void   loadError(const QString& errorMessage);

private:
    static medPluginManager *s_instance;

private:
    medPluginManagerPrivate *d;
};
