#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractProcessPresenterDTK2.h>
#include <medDiffusionModelEstimationMetaProcess.h>
#include <medProcessPresenterFactory.h>
#include <medCoreGuiExport.h>

class QWidget;
class QPushButton;

class medViewContainerSplitter;
class medAbstractData;

class medDiffusionModelEstimationMetaProcessPresenterPrivate;
class MEDCOREGUI_EXPORT medDiffusionModelEstimationMetaProcessPresenter : public medAbstractProcessDTK2Presenter
{
    Q_OBJECT

public:
    medDiffusionModelEstimationMetaProcessPresenter(medDiffusionModelEstimationMetaProcess *parent = nullptr);
    virtual ~medDiffusionModelEstimationMetaProcessPresenter();

    virtual QWidget *buildToolBoxWidget();
    virtual medViewContainerSplitter *buildViewContainerSplitter();

    virtual medDiffusionModelEstimationMetaProcess *process() const;

    const QScopedPointer<medDiffusionModelEstimationMetaProcessPresenterPrivate> d;

private slots:
    void _importOutput(medAbstractJob::medJobExitStatus jobExitStatus);

    void _setInputFromContainer(medAbstractData *data);

    void chooseDWIMaskingPlugin(int processIndex);
    void chooseEstimationPlugin(int processIndex);

signals:
    void _outputImported(medAbstractData *);
};
