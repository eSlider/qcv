/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 2.1. See the GNU LGPL version 2.1 for details.
 * QCV is distributed "AS IS" without ANY WARRANTY, without even the
 * implied warranty of merchantability or fitness for a particular
 * purpose. 
 *
 * In no event shall the authors or contributors be liable
 * for any direct, indirect, incidental, special, exemplary, or
 * consequential damages arising in any way out of the use of this
 * software.
 *
 * By downloading, copying, installing or using the software you agree
 * to this license. Do not download, install, copy or use the
 * software, if you do not agree to this license.
 */

/*@@@**************************************************************************
* \file  displayTreeView
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <QtGui>
#include <QMimeData>
#include "displayTreeView.h"
#include "drawingListPreview.h"
#include "displayTreeNode.h"

using namespace QCV;

CDisplayTreeView::CDisplayTreeView( QWidget *  f_parent_p,
                                    QGLWidget *  f_sharedGLWidget_p )
        : QTreeView (                  f_parent_p ),
          m_previewTimer_p (                 NULL ),
          m_previewWidget_p (                NULL ),
          m_sharedGLWidget_p ( f_sharedGLWidget_p )
{
    setSelectionMode (    QAbstractItemView::ExtendedSelection );
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setDragEnabled (         true );
    setAcceptDrops (         true );
    setDropIndicatorShown (  true );
    setMouseTracking (       true );

    m_previewWidget_p = new CDrawingListPreview ( static_cast<QWidget *>(parent()), 
                                                  f_sharedGLWidget_p );
    m_previewTimer_p = new QTimer ( this );
    /// Set interval to 1 second.
    m_previewTimer_p -> setInterval ( 300 );
    
    connect(m_previewTimer_p, SIGNAL(timeout()), this, SLOT(showPreview()));    

    setMouseTracking(true);
    
}

void 
CDisplayTreeView::loadParameters()
{
    QSettings qSettings;

    setColumnWidth ( 0,
            qSettings.value(QString("DisplayTreeView/Column0_Width"), 
                            260).toInt() );
    setColumnWidth ( 1,
            qSettings.value(QString("DisplayTreeView/Column1_Width"), 
                            75).toInt() );
    setColumnWidth ( 2,
            qSettings.value(QString("DisplayTreeView/Column2_Width"), 
                            75).toInt() );
}

CDisplayTreeView::~CDisplayTreeView ()
{
    saveParameters();
}

void 
CDisplayTreeView::saveParameters()
{
    QSettings qSettings;

    printf("Saving parameters\n");
    
    qSettings.setValue(QString("DisplayTreeView/Column0_Width"), columnWidth(0));
    qSettings.setValue(QString("DisplayTreeView/Column1_Width"), columnWidth(1));
    qSettings.setValue(QString("DisplayTreeView/Column2_Width"), columnWidth(2));
}

void CDisplayTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    event->ignore();
}

void 
CDisplayTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    event->ignore();
}

void
CDisplayTreeView::dropEvent(QDropEvent *event)
{
    event->ignore();
}

void
CDisplayTreeView::startDrag(Qt::DropActions /*supportedActions*/)
{
    QModelIndexList items = selectedIndexes();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    int columns_i = model()->columnCount();
    qint32 size_ui = items.size() / columns_i;
    
    dataStream << size_ui;

    for (int i = 0; i < size_ui * columns_i; ++i)
    {
        // The selected items are all indexes including row and column.
        // Since we want only want rows and the items are not sorted
        // we just consider only those items which lie at column 0.
        const QModelIndex &modelIndex =  items.at(i);
        if ( modelIndex.column() == 0)
        {    
            quint64 ptr_ui = (quint64) modelIndex.internalPointer();
            dataStream << ptr_ui;
        }
        
    }

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("display/gl-display-list", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    //drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    //drag->setPixmap(pixmap);

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
    {
        //delete takeItem(row(item));
    }    
}

void
CDisplayTreeView::mouseMoveEvent ( QMouseEvent *  f_event_p )
{
    m_index = indexAt ( f_event_p->pos() );

    if ( !m_index.isValid() ) 
    {
        m_previewTimer_p -> stop();
        m_previewWidget_p -> hide ();
        return;
    }

    m_lastMousePos = f_event_p -> globalPos();
    m_previewTimer_p -> stop();
    m_previewTimer_p -> start();


    QTreeView::mouseMoveEvent ( f_event_p );    
}

void 
CDisplayTreeView::leaveEvent ( QEvent * /*f_event_p*/ )
{
    m_previewTimer_p -> stop();
    m_previewWidget_p -> hide ();
}

void
CDisplayTreeView::showPreview()
{
    m_previewTimer_p -> stop();

    if ( !m_index.isValid() ) 
    {
        //printf("%i %i\n", m_index.column(), m_index.row());
        m_previewWidget_p -> hide ();
        return;
    }

    CDisplayTreeNodeAbstract * node_p = static_cast<CDisplayTreeNodeAbstract *>
        ( m_index.internalPointer() );

    if ( node_p && node_p -> getNodeType() == CDisplayTreeNodeAbstract::NT_DISPLAY )
    {
        CDisplayNode * dnode_p = static_cast<CDisplayNode *> (node_p);
        m_previewWidget_p -> setNode ( dnode_p );
        m_previewWidget_p -> setScreenSize( dnode_p->getDrawingList()->getScreenSize() );
        m_previewWidget_p -> move ( m_lastMousePos + QPoint(2, 2) );
        m_previewWidget_p -> show();
        m_previewWidget_p -> updateGL();
    }
    else
    {
        m_previewWidget_p -> setNode ( NULL );
        m_previewWidget_p -> hide ();
    }
}

void CDisplayTreeView::dataChanged ( const QModelIndex & /*f_topLeft*/, 
                                     const QModelIndex & f_bottomRight )
{
    bool visibilityChanged_b = f_bottomRight.column() == 2;
    
    if (visibilityChanged_b && m_sharedGLWidget_p)
    {
        m_sharedGLWidget_p -> updateGL();
    }
}

void 
CDisplayTreeView::updateTree()
{
    expandAll();
}

void
CDisplayTreeView::focusInEvent(QFocusEvent * /*f_event_p*/ )
{
    updateTree();    
}

