/* medDatabaseNavigator.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Dec 15 09:38:39 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Mon Jun 28 14:21:37 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 59
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "medDatabaseNavigator.h"

#include <dtkCore/dtkGlobal.h>

#include <QtCore>
#include <QtGui>

#include <medCore/medAbstractDbController.h>
#include <medCore/medDataManager.h>
#include <medCore/medMetaDataKeys.h>
#include <medCore/medStorage.h>

#include "medDatabaseNavigatorController.h"
#include "medDatabaseNavigatorItem.h"
#include "medDatabaseNavigatorItemGroup.h"
#include "medDatabaseNavigatorScene.h"
#include "medDatabaseNavigatorView.h"


namespace  {

    // These small classes are used to determine if patients from different DBs are the same.
    // At present it is just based on the name.
    struct PatientDataKey{
        QString name;
        bool operator==(const PatientDataKey & other) const { return this->name == other.name; }
        bool operator!=(const PatientDataKey & other) const { return !this->operator==(other); }
    };

    struct StudyDataKey{
        QString name;
        bool operator==(const StudyDataKey & other) const { return this->name == other.name; }
        bool operator!=(const StudyDataKey & other) const { return !this->operator==(other); }
        bool operator<(const StudyDataKey & other) const { return this->name < other.name; }
    };
} // namespace

class medDatabaseNavigatorPrivate
{
public:
    medDatabaseNavigatorScene *scene;
    medDatabaseNavigatorView *view;

    Qt::Orientation orientation;
};

medDatabaseNavigator::medDatabaseNavigator(QWidget *parent) : QFrame(parent), d(new medDatabaseNavigatorPrivate)
{
    d->orientation = medDatabaseNavigatorController::instance()->orientation();
  
    d->scene = new medDatabaseNavigatorScene(this);
    d->scene->setOrientation (d->orientation);

    d->view = new medDatabaseNavigatorView(this);
    d->view->setOrientation (d->orientation);
    d->view->setScene(d->scene);
    d->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->view);
    
    // medDatabaseNavigatorController::instance()->setOrientation(Qt::Vertical);
    medDatabaseNavigatorController::instance()->setItemSize(128, 128);
    d->orientation == Qt::Horizontal
        ? this->setFixedHeight(medDatabaseNavigatorController::instance()->groupHeight() + medDatabaseNavigatorController::instance()->itemSpacing() + 36) // 26 pixels for the scroller
        : this->setFixedWidth(medDatabaseNavigatorController::instance()->groupWidth() + medDatabaseNavigatorController::instance()->itemSpacing() + 36); // 26 pixels for the scroller
}

medDatabaseNavigator::~medDatabaseNavigator(void)
{
    delete d;

    d = NULL;
}

void medDatabaseNavigator::reset(void)
{
    d->scene->reset();
}

void medDatabaseNavigator::onItemClicked(const medDataIndex& index)
{
    if ( index.isValidForPatient() )
        this->onPatientClicked(index);
}

void medDatabaseNavigator::onPatientClicked(const medDataIndex& index)
{
    this->reset();

    if  (!index.isValidForPatient()) {
        return;
    }

    typedef QSet<medDataIndex> IndexSet;
    typedef QList<int> IntList;
    typedef QList<medDataIndex> IndexList;

    medDataManager *dataManager = medDataManager::instance();

    IntList dataSources = dataManager->dataSourceIds();

    QMap<StudyDataKey, medDatabaseNavigatorItemGroup*> groupMap;

    medAbstractDbController *dbc = dataManager->controllerForDataSource(index.dataSourceId());
    if ( !dbc ) 
        return;
    PatientDataKey referencePatientKey;
    referencePatientKey.name = dbc->metaData(index,medMetaDataKeys::PatientName);

    IndexSet addedStudies;

    foreach (const int dataSourceId, dataSources ) {

        medAbstractDbController *dbc = dataManager->controllerForDataSource(dataSourceId);
        if ( !dbc ) 
            continue;

        IndexList patientsForSource;
        if ( dataSourceId == index.dataSourceId() ) {
            patientsForSource.push_back(index);
        } else {
            patientsForSource = dbc->patients();
        }

        foreach (const medDataIndex& patient, patientsForSource ) {

            IndexList studiesForSource = dbc->studies(patient);
            QString patientName = dbc->metaData(patient,medMetaDataKeys::PatientName);
            PatientDataKey patientKey;
            patientKey.name = patientName;
            if ( patientKey != referencePatientKey ) {
                continue;
            }

            foreach (const medDataIndex& study, studiesForSource ) {

                QString studyName = dbc->metaData(study,medMetaDataKeys::StudyDescription);
                StudyDataKey studyKey;
                studyKey.name = studyName;

                medDatabaseNavigatorItemGroup *group = NULL;
                if ( groupMap.contains(studyKey) ) {
                    group = groupMap.find(studyKey).value();
                } else {
                    group = new medDatabaseNavigatorItemGroup;
                    group->setOrientation (d->orientation);
                    group->setName(studyName);
                    groupMap[studyKey] = group;
                }

                IndexList seriesForSource = dbc->series(study);

                foreach (const medDataIndex& serie, seriesForSource ) {

                    medDatabaseNavigatorItem *item = new medDatabaseNavigatorItem( medDataIndex(serie) );

                    connect(item, SIGNAL(itemClicked(const medDataIndex&)), this, SIGNAL(itemClicked(const medDataIndex&)));

                    group->addItem(item);

                }
            }
        }
    }

    foreach(medDatabaseNavigatorItemGroup *group, groupMap)
        d->scene->addGroup(group);
}

void medDatabaseNavigator::onStudyClicked(const medDataIndex& id)
{
    qDebug() << DTK_PRETTY_FUNCTION << id;
}

void medDatabaseNavigator::onSeriesClicked(const medDataIndex& id)
{
    qDebug() << DTK_PRETTY_FUNCTION << id;
}

void medDatabaseNavigator::onImageClicked(const medDataIndex& id)
{
    qDebug() << DTK_PRETTY_FUNCTION << id;
}

void medDatabaseNavigator::setOrientation (Qt::Orientation orientation)
{
    d->orientation = orientation;
    if (d->orientation == Qt::Horizontal) {
        this->setFixedHeight(medDatabaseNavigatorController::instance()->groupHeight() + medDatabaseNavigatorController::instance()->itemSpacing() + 36); // 26 pixels for the scroller
	this->setFixedWidth(QWIDGETSIZE_MAX);
    }
    else {  
        this->setFixedWidth(medDatabaseNavigatorController::instance()->groupWidth() + medDatabaseNavigatorController::instance()->itemSpacing() + 36); // 26 pixels for the scroller
	this->setFixedHeight(QWIDGETSIZE_MAX);
    }

    d->view->setOrientation (d->orientation);
    d->scene->setOrientation (d->orientation);
}

Qt::Orientation medDatabaseNavigator::orientation (void) const
{
  return d->orientation;
}
