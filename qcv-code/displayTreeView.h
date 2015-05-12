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

#ifndef __DISPLAYTREEVIEW_H
#define __DISPLAYTREEVIEW_H

/*@@@**************************************************************************
 ** \file  displayTreeView
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <QTreeView>

/* CONSTANTS */

/* PROTOTYPES */

class QGLWidget;
class QTimer;

namespace QCV
{
    class CDrawingListPreview;

    class CDisplayTreeView : public QTreeView
    {
        Q_OBJECT
        
        public:
        CDisplayTreeView ( QWidget *    f_parent_p = 0,
                           QGLWidget *  f_sharedGLWidget_p = 0 );
        
        virtual ~CDisplayTreeView ();

        void     saveParameters();
        void     loadParameters();

    /// Public slots.
    public slots:
        
        void showPreview();

        void updateTree();
        
    /// Inherited virtual methods
    protected:
        virtual void dragEnterEvent (QDragEnterEvent * f_event_p);
        virtual void dragMoveEvent  (QDragMoveEvent * f_event_p);
        virtual void dropEvent      (QDropEvent * f_event_p);
        virtual void startDrag      (Qt::DropActions supportedActions_e );

        virtual void mouseMoveEvent ( QMouseEvent * f_event_p );
        virtual void leaveEvent ( QEvent * f_event_p );

        virtual void dataChanged ( const QModelIndex & f_topLeft, 
                                   const QModelIndex & f_bottomRight ) ;
        
        virtual void focusInEvent(QFocusEvent *f_event_p);

    /// Help methods.
    protected:
        
    /// Members
    protected:

        /// Timer for detecting that the mouse do not move 
        /// during the time interval in order to show the preview
        /// as a tool tip.
        QTimer *                m_previewTimer_p;

        /// Preview widget.
        CDrawingListPreview *   m_previewWidget_p;

        /// Index under mouse.
        QGLWidget *             m_sharedGLWidget_p;

        /// Preview widget.
        QPoint                  m_lastMousePos;

        /// Start drag position.
        QPoint                  m_dragStartPosition;

        /// Index under mouse.
        QModelIndex             m_index;
    };
}


#endif // __DISPLAYTREEVIEW_H
