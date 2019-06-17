/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QLabel>

#include <medDataIndex.h>

#include <medCoreLegacyExport.h>

class medDropSitePrivate;

class MEDCORELEGACY_EXPORT medDropSite : public QLabel
{
    Q_OBJECT

public:
     medDropSite(QWidget *parent = nullptr);
    ~medDropSite() override;

    QSize sizeHint() const override;

    void setCanAutomaticallyChangeAppereance(bool can);

public:
    medDataIndex index() const;

public slots:
    void clear();

signals:

    /**
     * This signal is emitted when an object containing
     * a valid medDataIndex is dropped.
     */
    void objectDropped (const medDataIndex& index);

    /** Signal emitted when the user clicks on the medDropSite. */
    void clicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    medDropSitePrivate *d;
};


