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
#include <QtGui>

#include <medCoreLegacyExport.h>
class QUuid;
class QListWidgetItem;
class QAction;

class medToolBox;

class medTabbedViewContainers;
class medDataIndex;
class medViewContainer;


/**
 * @brief A Workspace holds medToolBoxes, medViewContainers and their relations.
 *
 *The main role of a workspace is to provide a coherent set of toolboxes and containers that interact with each other.
 *
 * A workspace is usually instantiated by a factory.
 * It owns several medViewContainers in a medTabbedViewContainers.
 * It also owns toolboxes, but does not place them, the medWorkspaceArea does it when
 * medWorkspaceArea::setupWorkspace is called.
 *
*/

class medAbstractParameterGroupL;
class medViewParameterGroupL;
class medLayerParameterGroupL;

class medAbstractWorkspaceLegacyPrivate;
class MEDCORELEGACY_EXPORT medAbstractWorkspaceLegacy : public QObject
{
    Q_OBJECT

public:

    medAbstractWorkspaceLegacy(QWidget *parent = nullptr);
    ~medAbstractWorkspaceLegacy() override;

    virtual QString identifier() const = 0;
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual QString category() const =0;

    QList <medToolBox*> toolBoxes() const;
    medToolBox* selectionToolBox() const;
    void setDatabaseVisibility(bool visibility);
    bool isDatabaseVisible() const;
    void setToolBoxesVisibility(bool value);
    bool areToolBoxesVisible() const;
    virtual void setupTabbedViewContainer() = 0;
    medTabbedViewContainers * tabbedViewContainers() const;

    void setUserLayerPoolable(bool poolable);
    void setUserViewPoolable(bool poolable);
    void setUserLayerClosable(bool Closable);
    bool isUserLayerPoolable() const;
    bool isUserViewPoolable() const;
    bool isUserLayerClosable() const;

public slots:
    virtual void clear();
    virtual void addNewTab();
    void updateNavigatorsToolBox();
    void updateMouseInteractionToolBox();
    void updateLayersToolBox();
    void updateInteractorsToolBox();
    void clearWorkspaceToolBoxes();
    void addToolBox(medToolBox *toolbox);
    void removeToolBox(medToolBox *toolbox);

    virtual void open(const medDataIndex& index);

protected slots:
    void changeCurrentLayer(int row);
    void removeLayer();

    void addViewGroup(QString group);
    void addLayerGroup(QString group);
    void addViewGroup(medViewParameterGroupL *group);
    void addLayerGroup(medLayerParameterGroupL *group);
    void setViewGroups(QList<medViewParameterGroupL*> groups);
    void setLayerGroups(QList<medLayerParameterGroupL*> groups);

private slots:
    void buildTemporaryPool();

    void addViewstoGroup(QString group);
    void removeViewsFromGroup(QString group);

    void addLayerstoGroup(QString group);
    void removeLayersFromGroup(QString group);

    void removeViewGroup(QString group);
    void removeLayerGroup(QString group);

    void changeViewGroupColor(QString group, QColor color);
    void changeLayerGroupColor(QString group, QColor color);

private:
    QWidget* buildViewLinkMenu();
    QWidget* buildLayerLinkMenu(QList<QListWidgetItem*> selectedlayers);

private:
    medAbstractWorkspaceLegacyPrivate *d;
};

#define MED_WORKSPACE_INTERFACE(_name,_desc,_cate) \
public:\
    static QString staticIdentifier() {return QString(staticMetaObject.className());}\
    static QString staticName() {return QString::fromUtf8(_name);}\
    static QString staticDescription() {return QString::fromUtf8(_desc);}\
    static QString staticCategory() {return QString::fromUtf8(_cate);}\
    virtual QString identifier() const {return staticIdentifier();}\
    virtual QString name() const {return staticName();}\
    virtual QString description() const {return staticDescription();}\
    virtual QString category() const {return staticCategory();}
