/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QImage>

#include <medCoreLegacyExport.h>

class medDataIndex;
class QLabel;

class medDatabasePreviewStaticScenePrivate;
class MEDCORELEGACY_EXPORT medDatabasePreviewStaticScene: public QGraphicsScene
{
    Q_OBJECT
public:
    medDatabasePreviewStaticScene(QObject *parent = nullptr);
    ~medDatabasePreviewStaticScene() override;

    void addImage(const medDataIndex &index);
    void setImage(const medDataIndex &index);
    medDataIndex& currentDataIndex() const;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
signals:
    void openRequest(const medDataIndex& index);

private :
    medDatabasePreviewStaticScenePrivate *d;
};


class medDatabasePreviewDynamicScenePrivate;
class MEDCORELEGACY_EXPORT medDatabasePreviewDynamicScene: public medDatabasePreviewStaticScene
{
    Q_OBJECT
public:
    medDatabasePreviewDynamicScene(const QList<QPair<medDataIndex, QString> > & seriesDescriptionDataIndexList, QObject * parent = nullptr);
    ~medDatabasePreviewDynamicScene() override;
    void previewMouseMoveEvent(QMouseEvent *event, int width);

signals:
    void updateLabel(const QString & description);

private:
    medDatabasePreviewDynamicScenePrivate *d;

};


class medDatabasePreviewPrivate;
class MEDCORELEGACY_EXPORT medDatabasePreview: public QGraphicsView
{
    Q_OBJECT
public :
    medDatabasePreview(QWidget *parent = nullptr);
    ~medDatabasePreview() override;

    enum medDataType
    {
        PATIENT,
        STUDY,
        SERIES
    };

    QLabel* label() const;

public slots:
    void showSeriesPreview(const medDataIndex &index);
    void showStudyPreview(const medDataIndex &index);
    void showPatientPreview(const medDataIndex &index);

    void setLabel(const QString & text);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void openRequest(const medDataIndex& index);

private:
    medDatabasePreviewPrivate *d;
};
