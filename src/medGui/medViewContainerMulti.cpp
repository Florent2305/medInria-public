/* medViewContainerMulti.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Mar 17 11:01:46 2010 (+0100)
 * Version: $Id$
 * Last-Updated: Wed Mar 17 18:48:11 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 52
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "medViewContainer_p.h"
#include "medViewContainerSingle.h"
#include "medViewContainerMulti.h"

#include <dtkCore/dtkAbstractView.h>

medViewContainer::Type medViewContainerMulti::type(void)
{
    return medViewContainer::Multi;
}

void medViewContainerMulti::split(int rows, int cols)
{
    Q_UNUSED(rows);
    Q_UNUSED(cols);

    // Don't split a multi view container

    return;
}

dtkAbstractView *medViewContainerMulti::view(void)
{
    return NULL;
}

void medViewContainerMulti::setView(dtkAbstractView *view)
{
    QList<QWidget *> content;
    
    for(int i = 0; i < d->layout->rowCount() ; i++) {
        for(int j = 0; j < d->layout->columnCount() ; j++) {
            if(QLayoutItem *item = d->layout->itemAtPosition(i, j)) {
                content << item->widget();
                d->layout->removeItem(item);
            }
        }
    }
    
    medViewContainer *container = new medViewContainerSingle(this);
    container->setAcceptDrops(false);
    container->setView(view);
    content << container;
    
    int row = 0;
    int col = 0, colmax = 0;
    
    for(int i = 0; i < content.count()-1; i++) {
        
        if(((col+1)*(row+1)) <= content.count()-1) {
            
            qreal rratio = qMin(((qreal)this->height()/(qreal)(row+2)), ((qreal)this->width()/(qreal)(col+1)));
            qreal cratio = qMin(((qreal)this->height()/(qreal)(row+1)), ((qreal)this->width()/(qreal)(col+2)));
            
            if(rratio > cratio) {                    
                row++;
                col = 0;
            } else {
                col++;
            }
            
            colmax = qMax(col, colmax);
        }
    }
    
    int layout_row = 0;
    int layout_col = 0;
    
    for(int i = 0; i < content.size(); i++) {
        
        d->layout->addWidget(content.at(i), layout_row, layout_col);
        
        if(layout_col == colmax) {
            layout_row++;
            layout_col = 0;
        } else {
            layout_col++;
        }
    }
    
    d->layout->setContentsMargins(1, 1, 1, 1);    
    d->view = view;
}

void medViewContainerMulti::dragEnterEvent(QDragEnterEvent *event)
{
    medViewContainer::dragEnterEvent(event);
}

void medViewContainerMulti::dragMoveEvent(QDragMoveEvent *event)
{
    medViewContainer::dragMoveEvent(event);
}

void medViewContainerMulti::dragLeaveEvent(QDragLeaveEvent *event)
{
    medViewContainer::dragLeaveEvent(event);
}

void medViewContainerMulti::dropEvent(QDropEvent *event)
{
    if(medViewContainerMulti *container = dynamic_cast<medViewContainerMulti *>(this->parentWidget())) {
        s_current = container;
    } else {
        s_current = this;
    }

    medViewContainer::dropEvent(event);
}

void medViewContainerMulti::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
}

void medViewContainerMulti::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
}