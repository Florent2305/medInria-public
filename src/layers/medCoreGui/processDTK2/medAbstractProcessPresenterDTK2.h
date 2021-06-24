#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <QObject>

#include <medAbstractProcessDTK2.h>
#include <medProcessPresenterFactory.h>
#include <medCoreGuiExport.h>

class QWidget;
class QPushButton;
class medViewContainerSplitter;

class medAbstractProcessDTK2PresenterPrivate;
class MEDCOREGUI_EXPORT medAbstractProcessDTK2Presenter : public QObject
{
    Q_OBJECT

public:
    medAbstractProcessDTK2Presenter(medAbstractProcessDTK2 *parent = nullptr);
    virtual ~medAbstractProcessDTK2Presenter();

    virtual medAbstractProcessDTK2 *process() const = 0;

    virtual QWidget *buildToolBoxWidget() = 0;
    virtual medViewContainerSplitter *buildViewContainerSplitter() = 0;

    QPushButton *buildRunButton();
    QPushButton *buildCancelButton();

protected slots:
    void _runProcessFromThread();

private:
    const QScopedPointer<medAbstractProcessDTK2PresenterPrivate> d;
};
MED_DECLARE_PROCESS_PRESENTER_FACTORY(medAbstractProcessDTK2, MEDCOREGUI_EXPORT)
