/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractProcess.h>
#include <medAbstractProcessPresenter.h>
#include <medJobManager.h>

#include <QThreadPool>
#include <QPushButton>

#include <dtkLog>

class medAbstractProcessDTK2PresenterPrivate
{
public:
    medAbstractProcessDTK2 *process;
};

medAbstractProcessDTK2Presenter::medAbstractProcessDTK2Presenter(medAbstractProcessDTK2*parent)
    : QObject(parent), d(new medAbstractProcessDTK2PresenterPrivate)
{
    if(parent == nullptr)
    {
        qWarning() << "Constructing presenter parented to a null process"
                  << this;
    }
    d->process = parent;
}

medAbstractProcessDTK2Presenter::~medAbstractProcessDTK2Presenter()
{

}

QPushButton* medAbstractProcessDTK2Presenter::buildRunButton()
{
    QPushButton *runButton = new QPushButton(tr("Run"));
    runButton->setDisabled(d->process->isRunning());
    connect(runButton, &QPushButton::clicked,
            this, &medAbstractProcessDTK2Presenter::_runProcessFromThread);
    connect(d->process, &medAbstractProcessDTK2::running,
            runButton, &QPushButton::setDisabled,
            Qt::QueuedConnection);

    return runButton;
}

QPushButton* medAbstractProcessDTK2Presenter::buildCancelButton()
{
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setEnabled(d->process->isRunning());
    connect(cancelButton, &QPushButton::clicked,
            d->process, &medAbstractProcessDTK2::cancel,
            Qt::QueuedConnection);
    connect(d->process, &medAbstractProcessDTK2::running,
            cancelButton, &QPushButton::setEnabled,
            Qt::QueuedConnection);

    return cancelButton;
}

void medAbstractProcessDTK2Presenter::_runProcessFromThread()
{
    medJobManager::instance()->startJobInThread(d->process);
}
