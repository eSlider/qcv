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
 * \file  drawingListPreview.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */

#include <QtGui>
#include <QtOpenGL>

#include "drawingListPreview.h"
#include "displayTreeNode.h"

using namespace QCV;

CDrawingListPreview::CDrawingListPreview( QWidget   *  f_associated_p,
                                          QGLWidget *  f_sharedGLWidget_p )
        : QGLWidget(                         NULL, 
                                             f_sharedGLWidget_p, 
                                             Qt::ToolTip /* |
                                             Qt::FramelessWindowHint | 
                                             Qt::WindowSystemMenuHint |
                                             Qt::WindowStaysOnTopHint */ ),
          m_previewNode_p (                                      NULL ),
          m_screenSize (                                     800, 600 ),
          m_associatedWidget_p (                       f_associated_p )
{
    /// Enable focus on this widget.
    //setFocusPolicy(Qt::StrongFocus);
    
    setAcceptDrops(false);
    setMouseTracking(true);

    m_showTimer_p = new QTimer ( this );
    /// 2 seconds.
    m_showTimer_p -> setInterval ( 2000 );

    connect(m_showTimer_p, SIGNAL(timeout()), this, SLOT(hide()));
}

CDrawingListPreview::~CDrawingListPreview()
{
}

QSize CDrawingListPreview::minimumSizeHint() const
{
    return QSize(m_screenSize.width/5, 
                 m_screenSize.height/5 );
}

QSize CDrawingListPreview::sizeHint() const
{
    return QSize( m_screenSize.width/5, 
                  m_screenSize.height/5 );
}

void CDrawingListPreview::initializeGL()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}

void CDrawingListPreview::hide()
{
    m_showTimer_p -> stop();
    QGLWidget::hide();
}   

void CDrawingListPreview::show()
{
    m_showTimer_p -> start();
    QGLWidget::show();
}   

void CDrawingListPreview::paintGL()
{
    if (!m_previewNode_p) 
        return;
    
    // Reset modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clear screen using the current background color
    glClearColor( 0.f, 0.f, 0.f, 1.);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f( 1.f, 1.f, 1.f, 1.f);

    /// This line can help flip views or set aspect ratios.
    glScalef(2.f, -2.f, 1.f);
    // Move center (0,0) to upper left corner.
    glTranslatef( -.5, -.5, 0.0);

    // Scale to use absolut pixel coordinates considering current zoom factor.
    glScalef( 1.f / m_screenSize.width,
              1.f / m_screenSize.height, 1.0);
    
    //// PAINT HERE FIRST THE IMAGES
    
    if (m_previewNode_p)
    {
        CDrawingList *  list_p  = m_previewNode_p -> getDrawingList();
        
        if (list_p)
        {
            list_p -> show();
        }
    }
}

void 
CDrawingListPreview::resizeGL(const int f_width_i, const int f_height_i)
{
    glViewport( 0, 0, f_width_i, f_height_i );
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
}

void
CDrawingListPreview::mouseMoveEvent ( QMouseEvent *  f_event_p )
{
    QWidget *parent_p = m_associatedWidget_p;

    if (parent_p)
    {   
        QPoint parentPos  = parent_p->pos();
        QSize  parentSize = parent_p->size();
        
        if ( f_event_p -> globalPos().x() > parentPos.x() && 
             f_event_p -> globalPos().x() < parentPos.x() + parentSize.width() && 
             f_event_p -> globalPos().y() > parentPos.y() && 
             f_event_p -> globalPos().y() < parentPos.y() + parentSize.height() )
            move( f_event_p -> globalPos() + QPoint(2, 2) );
        else
            hide();
    }
}

