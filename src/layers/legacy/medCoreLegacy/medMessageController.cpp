/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <dtkCoreSupport/dtkGlobal.h>

#include <medMessageController.h>

#include <dtkLog/dtkLog.h>

// /////////////////////////////////////////////////////////////////
// medMessage
// /////////////////////////////////////////////////////////////////

medMessage::medMessage( QWidget *parent,
                        const QString& text, 
                        unsigned int timeout) : QWidget(parent)
{
    this->timeout = timeout;

    this->setFixedWidth(400);
    icon = new QLabel(this);

    info = new QLabel(this);
    info->setText(text);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);
    layout->addWidget(icon);
    layout->addWidget(info);


    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(remove()));

    this->setLayout(layout);
}

medMessage::~medMessage() = default;

void medMessage::startTimer()
{
    if (timeout>0)
        timer->start(timeout);
}

void medMessage::stopTimer()
{
    if (timeout>0)
        timer->stop();
}

void medMessage::remove()
{
    medMessageController::instance()->remove(this);
}


// /////////////////////////////////////////////////////////////////
// medMessageInfo
// /////////////////////////////////////////////////////////////////

medMessageInfo::medMessageInfo(
        const QString& text, QWidget *parent,unsigned int timeout) :
        medMessage(parent,text, timeout)
{
    icon->setPixmap(QPixmap(":/icons/information.png"));
    this->setFixedWidth(200);
}

medMessageInfo::~medMessageInfo() = default;

// /////////////////////////////////////////////////////////////////
// medMessageError
// /////////////////////////////////////////////////////////////////

medMessageError::medMessageError(
        const QString& text, QWidget *parent,unsigned int timeout) :
        medMessage(parent, text, timeout)
{
    icon->setPixmap(QPixmap(":/icons/exclamation.png"));
    this->setFixedWidth(350);
}

medMessageError::~medMessageError() = default;

// /////////////////////////////////////////////////////////////////
// medMessageProgress
// /////////////////////////////////////////////////////////////////


medMessageProgress::medMessageProgress(
        const QString& text, QWidget *parent) :
        medMessage(parent, text, 0)
{
    progress = new QProgressBar(this);
    progress->setMinimum(0);
    progress->setMaximum(100);
    progress->setValue(100);
    this->layout()->addWidget(progress);
}

medMessageProgress::~medMessageProgress() = default;

void medMessageProgress::setProgress(int value)
{
    progress->setValue(value);
}

void medMessageProgress::success()
{
    progress->setValue(100);
    progress->setProperty("success",true);
    progress->style()->unpolish(progress);
    progress->style()->polish(progress);
    progress->update();
    this->associateTimer();
    info->setText("Operation succeeded");
}

void medMessageProgress::failure()
{
    progress->setProperty("failure",true);
    progress->style()->unpolish(progress);
    progress->style()->polish(progress);
    progress->update();
    this->associateTimer();

    info->setText("Operation failed");
}

void medMessageProgress::associateTimer()
{
    this->timer = new QTimer(this);
    timeout = 2000;
    connect(timer, SIGNAL(timeout()), this, SLOT(remove()));
}

void medMessageProgress::paintEvent ( QPaintEvent * event)
{
    if (timeout > 0)
    {
        timer->start(timeout);
    }
}

// /////////////////////////////////////////////////////////////////
// medMessageController
// /////////////////////////////////////////////////////////////////


medMessageController *medMessageController::instance()
{
    if(!s_instance)
        s_instance = new medMessageController;

    return s_instance;
}

void medMessageController::showInfo(const QString& text,unsigned int timeout)
{
    if ( dynamic_cast<QApplication *>(QCoreApplication::instance()) ) 
    {
        // GUI
        medMessageInfo *message = new medMessageInfo(text, nullptr, timeout);
        emit addMessage(message);
    } 
    else
    {
        dtkTrace() << text;
    }
}

void medMessageController::showError(const QString& text,unsigned int timeout)
{
    if ( dynamic_cast<QApplication *>(QCoreApplication::instance()) ) 
    {
        // GUI
        medMessageError *message = new medMessageError(text, nullptr, timeout);
        emit addMessage(message);

    } 
    else 
    {
        dtkError() << text;
    }
}

medMessageProgress* medMessageController::showProgress(const QString& text)
{
    if (dynamic_cast<QApplication *>(QCoreApplication::instance()))
    {
        // GUI
        medMessageProgress *message = new medMessageProgress(text);

        emit addMessage(message);
        return message;
    } 
    return nullptr;
}

void medMessageController::remove(medMessage *message)
{
    if(message != nullptr)
    {
        emit removeMessage(message);
        message->deleteLater();
    }
}

medMessageController::medMessageController() = default;

medMessageController::~medMessageController() = default;

medMessageController *medMessageController::s_instance = nullptr;
