/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <medCoreLegacyExport.h>

class medMessageControllerPrivate;

// /////////////////////////////////////////////////////////////////
// medMessage
// /////////////////////////////////////////////////////////////////

class MEDCORELEGACY_EXPORT medMessage : public QWidget
{
    Q_OBJECT

public:
     medMessage(QWidget *parent = nullptr, const QString& text=nullptr, unsigned int timeout=0);
    ~medMessage() override;
    void startTimer();
    void stopTimer();


protected:
    QLabel *icon;
    QTimer *timer;
    int timeout;
    QLabel *info;


protected slots:
    void remove();
};



// /////////////////////////////////////////////////////////////////
// medMessageInfo
// /////////////////////////////////////////////////////////////////

class medMessageInfo : public medMessage
{
    Q_OBJECT

public:
     medMessageInfo(const QString& text, QWidget *parent = nullptr, unsigned int timeout=0);
    ~medMessageInfo() override;
};

// /////////////////////////////////////////////////////////////////
// medMessageError
// /////////////////////////////////////////////////////////////////

class medMessageError : public medMessage
{
    Q_OBJECT
public:
     medMessageError(const QString& text, QWidget *parent = nullptr, unsigned int timeout=0);
    ~medMessageError() override;
};

// /////////////////////////////////////////////////////////////////
// medMessageProgress
// /////////////////////////////////////////////////////////////////


class MEDCORELEGACY_EXPORT medMessageProgress : public medMessage
{
    Q_OBJECT

public:
     medMessageProgress(const QString& text, QWidget *parent = nullptr);
    ~medMessageProgress() override;
    void associateTimer();
    void paintEvent ( QPaintEvent * event ) override;

protected:
    QProgressBar *progress;


public slots:
    void setProgress(int value);
    void success();
    void failure();
};


// /////////////////////////////////////////////////////////////////
// medMessageController
// /////////////////////////////////////////////////////////////////

class MEDCORELEGACY_EXPORT medMessageController : public QObject
{
    Q_OBJECT

public:
    static medMessageController *instance();

public slots:
    void     showInfo(const QString& text,unsigned int timeout=0);
    void     showError(const QString& text,unsigned int timeout=0);
    medMessageProgress * showProgress(const QString& text);

    void remove(medMessage *message);

signals:
  void addMessage(medMessage * message);
  void removeMessage(medMessage * message);

protected:
     medMessageController();
    ~medMessageController() override;

protected:
    static medMessageController *s_instance;

private:
    medMessageControllerPrivate *d;
};


