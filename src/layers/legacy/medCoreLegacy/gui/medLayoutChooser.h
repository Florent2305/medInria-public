/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QTableWidget>

#include <medCoreLegacyExport.h>

class medLayoutChooserPrivate;

class MEDCORELEGACY_EXPORT medLayoutChooser : public QTableWidget
{
    Q_OBJECT

public:
     medLayoutChooser(QWidget *parent = nullptr);
    ~medLayoutChooser() override;

    QSize sizeHint() const override;
    int sizeHintForRow(int row) const override;
    int sizeHintForColumn(int column) const override;

signals:
    void selected(unsigned int rows, unsigned int cols);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    medLayoutChooserPrivate *d;
};


