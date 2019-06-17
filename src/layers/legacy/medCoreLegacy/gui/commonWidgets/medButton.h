/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QWidget>

#include <medCoreLegacyExport.h>

class medButtonPrivate;

/**
 * @brief General custom button that loads shows an icon.
 */
class MEDCORELEGACY_EXPORT medButton : public QWidget
{
    Q_OBJECT

public:

    medButton(QWidget *parent, QString resourceLocation, QString toolTip);
    medButton(QWidget *parent, QPixmap pixmap, QString toolTip);

    ~medButton() override;

    QSize sizeHint() const override;

    void setIcon(QPixmap icon);

signals:
    //!  Emitted when the button is clicked.
    void triggered();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    medButtonPrivate * d;
};

