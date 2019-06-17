/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QtGui>
#include <QtWidgets>

#include <medCoreLegacyExport.h>

class medAbstractData;
class medAbstractImageView;


// /////////////////////////////////////////////////////////////////
// medClutEditorVertex
// /////////////////////////////////////////////////////////////////

class medClutEditorVertexPrivate;

// TODO use QGraphicsObjactItem noobs.
class medClutEditorVertex : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    medClutEditorVertex(QPointF value, QPointF coord, QColor color = Qt::white,
            QGraphicsItem *parent = nullptr);
    medClutEditorVertex( const medClutEditorVertex & other,
                         QGraphicsItem *parent = nullptr);
    ~medClutEditorVertex() override;

    const QPointF & value() const;
    void shiftValue( qreal amount, bool forceConstraints = true );

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QRectF boundingRect() const override;
    QColor color() const;
    void setColor(QColor color);

    void initiateMove();
    void finalizeMove();
    void forceGeometricalConstraints( const QRectF & limits,
                      bool inManhattan = false );

    void interpolate( medClutEditorVertex * prev, medClutEditorVertex * next );

    void updateCoordinates();
    void updateValue();

    static bool LessThan(const medClutEditorVertex *v1,
                         const medClutEditorVertex *v2) {
        return (v1->x() < v2->x());
    }

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private :
    void setAlpha();
    medClutEditorVertexPrivate * d;
};


// /////////////////////////////////////////////////////////////////
// medClutEditorTable
// /////////////////////////////////////////////////////////////////
class medClutEditorTablePrivate;

class medClutEditorTable : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    //medClutEditorTable(QGraphicsItem *parent = 0);
    medClutEditorTable(const medClutEditorTable & table);
    medClutEditorTable(const QString & title = "Unknown", QGraphicsItem *parent = nullptr);
    ~medClutEditorTable() override;

    const QString & title() const;
    void setTitle(const QString & title);
    void setMinSize( const QSizeF & size );
    const QSizeF & minSize() const;
    void setMinRange( qreal min, qreal max );
    void setSize( const QSizeF & size );
    const QSizeF & size() const;
    // void setRange( qreal min, qreal max );

    void addVertex(medClutEditorVertex *vertex, bool interpolate = false);
    QList<medClutEditorVertex *> & vertices();
    const QList<medClutEditorVertex *> & vertices() const;

    void initiateMoveSelection();
    void constrainMoveSelection( medClutEditorVertex * driver,
                 bool inManhattan = false );
    void finalizeMoveSelection();
    void updateCoordinates();

    QRectF boundingRect() const override;
    void range( qreal & min, qreal & max ) const;

    void setSelectedAllVertices( bool isSelected );
    void deleteSelection();
    void setColorOfSelection( const QColor & color );
    void setColorOfSelection();

    void scaleWindowWidth( qreal factor );
    void shiftWindowCenter( qreal amount );

    void setup(float min, float max, int size, int *table);
    void getTransferFunction(QList<double> &scalars, QList<QColor> &colors );
    void setTransferFunction(QList<double> &scalars, QList<QColor> &colors );
    void simplifyTransferFunction();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void changeDisplayAlpha( qreal step );
    void resetDisplayAlpha();

    void triggerVertexChanged();
    // void keyPressEvent(QKeyEvent *event);
    // void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

signals:
    void vertexChanged();
    void vertexRemoved();
    void vertexAdded();

// public slots:
//     void onDeleteVertex(medClutEditorVertex * v);

private:
    void deleteAllVertices();

    medClutEditorTablePrivate *d;
};


// /////////////////////////////////////////////////////////////////
// medClutEditorHistogram
// /////////////////////////////////////////////////////////////////
class medClutEditorHistogramPrivate;

class medClutEditorHistogram : public QGraphicsItem
{
public:
     medClutEditorHistogram(QGraphicsItem *parent = nullptr);
    ~medClutEditorHistogram() override;

    // QSizeF size() const;
    // void setSize( QSizeF s );

    void range( qreal & min, qreal & max ) const;
    qreal getRangeMin() const;
    qreal getRangeMax() const;
    void updateCoordinates();

    const QMap< qreal, qreal > & values();
    void setValues(const QMap<qreal, qreal> & bins);
    void addValue(qreal intensity, qreal number);
    void adjustScales();
    // QPointF valueToCoordinate( QPointF value ) const;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QRectF boundingRect() const override;

private:
    medClutEditorHistogramPrivate * d;
};


// /////////////////////////////////////////////////////////////////
// medClutEditorScene
// /////////////////////////////////////////////////////////////////
class medClutEditorView;
class medClutEditorScenePrivate;

class medClutEditorScene : public QGraphicsScene
{
public:
     medClutEditorScene(QObject *parent = nullptr);
    ~medClutEditorScene() override;

    medClutEditorTable * table();
    medClutEditorHistogram * histogram();
    medClutEditorView * view();

    QRectF  plotArea();
    QPointF coordinateToValue( QPointF coord );
    QPointF valueToCoordinate( QPointF value );

    void updateCoordinates();
    void setSize( QSizeF size );
    void setRange( qreal min, qreal max );
    void adjustRange();
    void scaleRange( qreal factor );
    void shiftRange( qreal amount );
    // void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
private:
    medClutEditorScenePrivate * d;
};


// /////////////////////////////////////////////////////////////////
// medClutEditorView
// /////////////////////////////////////////////////////////////////

class medClutEditorView : public QGraphicsView
{
public:
     medClutEditorView(QWidget *parent = nullptr);
    ~medClutEditorView() override;

protected:
    medClutEditorTable * table();

    void resizeEvent	   ( QResizeEvent * event ) override;
    void wheelEvent 	   ( QWheelEvent  * event ) override;
    void keyReleaseEvent   ( QKeyEvent    * event ) override;
    void keyPressEvent     ( QKeyEvent    * event ) override;
    void mousePressEvent   ( QMouseEvent  * event ) override;
    void mouseReleaseEvent ( QMouseEvent  * event ) override;
};



// /////////////////////////////////////////////////////////////////
// medClutEditor
// /////////////////////////////////////////////////////////////////

class medClutEditorPrivate;

class MEDCORELEGACY_EXPORT medClutEditor : public QWidget
{
    Q_OBJECT

public:
     medClutEditor(QWidget *parent = nullptr);
    ~medClutEditor() override;

    void setData(medAbstractData *data);
    void setView(medAbstractImageView *view, bool force = false);
    void applyTable();

protected:
    void initializeTable();
    void deleteTable();

    void mousePressEvent(QMouseEvent *event) override;

protected slots:
    void onNewTableAction();
    void onLoadTableAction();
    void onSaveTableAction();
    // void onDeleteTableAction();
    void onApplyTablesAction();
    // void onColorAction();
    // void onDeleteAction();
    void onVertexMoved();
    void onToggleDirectUpdateAction();
private:
    medClutEditorPrivate *d;
};


