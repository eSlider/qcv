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

#ifndef __CVGLDRAWINGLIST_H
#define __CVGLDRAWINGLIST_H

/**
 *******************************************************************************
 *
 * @file drawingListPreview.h
 *
 * \class CDrawingListPreview
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief This class implements a preview of the content of a drawing list.
 *
 * This class implements a preview of the content of a drawing list to be used
 * as a tooltip display in the CDisplayTreeDlg class.
 *
 *******************************************************************************/

/* INCLUDES */
#include <QGLWidget>
#include "drawingList.h"

namespace QCV
{

    /* PROTOTYPES */
    class CDisplayNode;

    class CDrawingListPreview : public QGLWidget
    {
        Q_OBJECT

        public:
        CDrawingListPreview( QWidget   *  parent = 0,
                             QGLWidget *  f_sharedGLWidget_p = 0 );

        ~CDrawingListPreview();

        QSize minimumSizeHint() const;
        QSize sizeHint() const;

    /// Inherited slots.
    public:
        virtual void hide ();
        virtual void show ();
        

    /// Set node to preview
    public:

        /// Set node to preview.
        void setNode       ( CDisplayNode * f_node_p ) { m_previewNode_p = f_node_p; }

        /// Set screen size.
        void setScreenSize ( S2D<unsigned int> f_size ) { m_screenSize = f_size; }

    /// Inherited members.
    protected:
        
        virtual void mouseMoveEvent ( QMouseEvent *  f_event_p );
        virtual void initializeGL();
        virtual void paintGL();
        virtual void resizeGL ( int width, int height );

    /// Own help methods.
    protected:

    /// Data members.
    private:

        /// Root node of display lists tree.
        CDisplayNode *             m_previewNode_p;

        /// Size of each screen.
        S2D<unsigned int>          m_screenSize;

        /// Timer for showing the preview.
        QTimer *                   m_showTimer_p;

        /// Associated widget.
        QWidget *                  m_associatedWidget_p;
    };
}


#endif // #define __CVGLDRAWINGLIST_H
