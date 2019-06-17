/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medSettingsWidget.h>
#include <QWidget>

#include <medCoreLegacyExport.h>

class medDatabaseSettingsWidgetPrivate;

class MEDCORELEGACY_EXPORT medDatabaseSettingsWidget : public medSettingsWidget
{
    Q_OBJECT
    MED_SETTINGS_INTERFACE("Database","Database Settings")
public:
    medDatabaseSettingsWidget(QWidget *parent = nullptr);

signals:

public slots:
    void read() override;

private slots:
    void selectDbDirectory();

protected:
    bool validate() override;

    bool write() override;
private:
    medDatabaseSettingsWidgetPrivate *d;

};
