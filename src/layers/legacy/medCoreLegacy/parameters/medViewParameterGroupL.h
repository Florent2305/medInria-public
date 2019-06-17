/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractParameterGroupL.h>

#include <medCoreLegacyExport.h>

class medViewParameterGroupLPrivate;

class MEDCORELEGACY_EXPORT medViewParameterGroupL : public medAbstractParameterGroupL
{
    Q_OBJECT

public:
    medViewParameterGroupL(QString name = "", QObject *parent = nullptr, QString workspace = "");
    ~medViewParameterGroupL() override;

    void addImpactedView(medAbstractView *view);
    void removeImpactedView(medAbstractView *view);
    QList<medAbstractView*> impactedViews();

    void setLinkAllParameters(bool linkAll) override;

protected:
    void updatePool() override;
    void updateParameterToLinkList(medAbstractView *view);

private slots:
    void removeImpactedView();
    void updateGroupIndicators(QColor oldColor, QColor newColor);

private:
    medViewParameterGroupLPrivate *d;

};

