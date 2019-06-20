/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QtGui>
#include <QtWidgets>

#include <medCoreLegacyExport.h>

class medToolBoxBodyPrivate;
class medToolBoxTab;

class MEDCORELEGACY_EXPORT medToolBoxBody : public QFrame
{
    Q_OBJECT

public:
     medToolBoxBody(QWidget *parent = nullptr);
    ~medToolBoxBody() override;

     void addWidget(QWidget *widget);
     void removeWidget(QWidget *widget);

    void setTabWidget(medToolBoxTab * tab);
    void clear();

    void setOrientation(Qt::Orientation orientation);

private:
    medToolBoxBodyPrivate *d;
};


