/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QtCore>
#include <medCoreLegacyExport.h>

class medPluginLegacy;
class medPluginManagerPrivate;



/**
 * @brief Load and unload plugins for medInria.
 *
*/
class MEDCORELEGACY_EXPORT medPluginManager : public QObject
{
    Q_OBJECT

public:
    static medPluginManager *instance();


    void initializeApplication();
    void initialize();
    void uninitialize();
    void uninitializeApplication();


    void   scan(const QString& path);
    void   load(const QString& name);
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
    void   loadPlugin(const QString& path);
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


