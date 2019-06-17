/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <QLabel>

#include <medCoreLegacyExport.h>

class medStringListParameterL;

class medPoolIndiactorPrivate;
class MEDCORELEGACY_EXPORT medPoolIndicatorL: public QWidget
{
    Q_OBJECT

public:
    medPoolIndicatorL(QWidget* parent = nullptr);
    ~medPoolIndicatorL() override;

    void addColorIndicator(QColor color, QString description);
    void removeColorIndicator(QColor color);

public slots:
    void replaceColorIndicator(QColor oldColor, QColor newColor);

private:
    medPoolIndiactorPrivate *d;
};
