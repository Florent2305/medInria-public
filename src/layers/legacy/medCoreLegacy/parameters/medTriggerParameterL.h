/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractParameterL.h>

#include <medCoreLegacyExport.h>

class QIcon;
class QPushButton;
class QSize;
class QWidget;

class medTriggerParameterLPrivate;
class MEDCORELEGACY_EXPORT medTriggerParameterL : public medAbstractTriggerParameterL
{
    Q_OBJECT

public:
    medTriggerParameterL(QString name = "Unknow trigger parameter", QObject* parent = nullptr);
    ~medTriggerParameterL() override;

    void setButtonIcon(QIcon& icon);
    void setButtonIconSize(QSize& size);
    void setButtonText(QString text);
    QPushButton* getPushButton();

    QWidget* getWidget() override;
private slots:
    void removeInternPushButton();

private:
    medTriggerParameterLPrivate* d;
};
