#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <QFrame>

#include <medCoreLegacyExport.h>

class QUuid;
class medAbstractView;
class medAbstractData;
class medDataIndex;
class medToolBox;
class medViewContainerSplitter;
class medAbstractParameterL;

class medViewContainerPrivate;
class MEDCORELEGACY_EXPORT medViewContainer: public QFrame
{
    Q_OBJECT

public:
    enum ClosingMode
    {
        CLOSE_CONTAINER,
        CLOSE_VIEW,
        CLOSE_BUTTON_HIDDEN
    };

    enum DropArea
    {
        AREA_OUT = 0,
        AREA_TOP,
        AREA_BOTTOM,
        AREA_LEFT,
        AREA_RIGHT,
        AREA_CENTER
    };

public:
    medViewContainer(medViewContainerSplitter* parent = nullptr);
    ~medViewContainer() override;

    medAbstractView* view() const;
    QUuid uuid() const;
    void setContainerParent(medViewContainerSplitter* splitter);

    bool isSelected() const;
    bool isMaximized() const;
    bool isUserSplittable() const;
    ClosingMode closingMode() const;
    bool isMultiLayered() const;
    bool isUserOpenable() const;

    medViewContainer* splitVertically();
    medViewContainer* splitHorizontally();
    medViewContainer* split(Qt::AlignmentFlag alignement = Qt::AlignRight);

    void setDefaultWidget(QWidget *defaultWidget);
    QWidget* defaultWidget() const;

    void addColorIndicator(QColor color, QString description="");
    void removeColorIndicator(QColor color);

public slots:
    void setView(medAbstractView* view);
    void removeView();
    void addData(medAbstractData* data);
    void addData(medDataIndex index);

    void setSelected(bool selected);
    void setUnSelected(bool unSelected);
    void toggleMaximized();
    void setUserSplittable(bool splittable);
    void setClosingMode(enum ClosingMode mode);
    void setMultiLayered(bool multiLayer);
    void setUserOpenable(bool openable);

    void highlight(QString color = "#FFAA88");
    void unHighlight();

    void splitContainer(unsigned int numY, unsigned int numX);

    void checkIfStillDeserveToLiveContainer();

signals:
    void maximized(QUuid uuid, bool maximized);
    void maximized(bool maximized);
    void containerSelected(QUuid uuid);
    void containerUnSelected(QUuid uuid);
    void currentLayerChanged();
    void vSplitRequest();
    void hSplitRequest();
    void splitRequest(medDataIndex, Qt::AlignmentFlag);
    void viewChanged();
    void viewContentChanged();
    void dataAdded(medAbstractData *);
    void viewRemoved();


protected:
    void focusInEvent(QFocusEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    bool dropEventFromDataBase(QDropEvent * event);
    bool dropEventFromFile(QDropEvent * event);

    void closeEvent(QCloseEvent * event) override;

    void recomputeStyleSheet();

protected slots:
    void openFromSystem();
    void updateToolBar();
    void dataReady(medDataIndex index, QUuid uuid);
    void droppedDataReady(medDataIndex index, QUuid uuid);

private slots:
    void removeInternView();
    DropArea computeDropArea(int x, int y);
    void popupMenu();

private:
    medViewContainerPrivate *d;

};
