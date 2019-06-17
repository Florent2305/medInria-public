/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractProcess.h>

#include <dtkCore>

#include <medCoreExport.h>
#include <medDiffusionGradientReader.h>

class medAbstractImageData;
class medAbstractDiffusionModelImageData;
class medAbstractDiffusionModelEstimationProcess;
class medAbstractDWIMaskingProcess;

class medDiffusionModelEstimationMetaProcessPrivate;
class MEDCORE_EXPORT medDiffusionModelEstimationMetaProcess : public medAbstractProcess
{
    Q_OBJECT

public:
    medDiffusionModelEstimationMetaProcess(QObject *parent = nullptr);
    ~medDiffusionModelEstimationMetaProcess() override;

    void setInput(medAbstractImageData* data);
    medAbstractImageData* input() const;

    medAbstractDiffusionModelImageData* output() const;

    medAbstractJob::medJobExitStatus run() override;
    void cancel() override;

    QString caption() const override;
    QString description() const override;

    QStringList dwiMaskingPlugins() const;
    QStringList modelEstimationPlugins() const;

    bool chooseDWIMaskingPlugin(QString processName);
    bool chooseModelEstimationPlugin(QString processName);

    medAbstractDWIMaskingProcess *dwiMaskCalculatorProcess();
    medAbstractDiffusionModelEstimationProcess *modelEstimationProcess();

public slots:
    void updateMaskCalculatorProgression(int value);
    void updateModelEstimationProgression(int value);

protected:
    void setOutput(medAbstractDiffusionModelImageData* data);
    QString outputNameAddon() const  override {return "models";}

private:
    const QScopedPointer<medDiffusionModelEstimationMetaProcessPrivate> d;
};
