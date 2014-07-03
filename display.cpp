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
 * \file  CDisplay
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */

#include <QtGui>
#include <QtOpenGL>
#include <QGLContext>

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else // __APPLE__
#include <GL/glut.h>
#endif // __APPLE__

#include "display.h"
#include "displayTreeNode.h"

using namespace QCV;

const QGLContext * g_QGLContext_p = NULL;


CDisplay::CDisplay( CDisplayOpNode * const f_rootNode_p,
                    QWidget *              f_parent_p )
        : QGLWidget(                    f_parent_p, 0 ),
          m_rootNode_p (                 f_rootNode_p ),
          m_zoomTL (                              0,0 ),
          m_zoomFactor_f (                        1.f ),
	  m_initialized_b (                     false ),
          m_highlightScreen (                  -1, -1 )
{
    /// Just now temporal.
    m_screenSize.width  = 640;
    m_screenSize.height = 480;

    m_screenCount.width  = 3;
    m_screenCount.height = 3;

    /// Enable focus on this widget.
    setFocusPolicy(Qt::StrongFocus);

    m_roiSize.width  = width();
    m_roiSize.height = height();
    m_roiPos.x = 0;
    m_roiPos.y = 0;
    
    setMouseTracking(true);
    
    setAcceptDrops(true);

    setMouseTracking(true);

    g_QGLContext_p = context();
}

CDisplay::~CDisplay()
{
}

QSize CDisplay::minimumSizeHint() const
{
    return QSize(400, 400);
}

QSize CDisplay::sizeHint() const
{
    return QSize(random()/(RAND_MAX/100)+100, 200);
}

void CDisplay::initializeGL()
{
    // First disable depth test. 
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    int argc=QCoreApplication::argc();
    static bool s_initialized_b;
    
    if (!s_initialized_b)
        glutInit(&argc, QCoreApplication::argv());

    s_initialized_b = true;
}

void CDisplay::paintGL()
{
    // printf("g_QGLContext_p = %p thiscontext = %p\n", 
    //        g_QGLContext_p, context());

    // Can't repaint if window hasn't been displayed yet.
    if ( !m_initialized_b ) return;


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
    glScalef( m_zoomFactor_f / m_roiSize.width,
              m_zoomFactor_f / m_roiSize.height, 1.0);
    
    glTranslatef( m_roiPos.x-m_zoomTL.x,
                  m_roiPos.y-m_zoomTL.y, 0.0);

    //// PAINT HERE FIRST THE IMAGES

    // Enable transparency
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    ////// Paint now the drawing lists.
    for (int l = 0; l < MAX_OVERLAY_LEVELS; ++l)
        displayScreens ( m_rootNode_p, l );

    /// Highlight screen if in dropping a drawing list.
    highlightScreen();
    
    // Disable transparency again
    //glDisable(GL_BLEND);

    ////// PAINT FINALLY HERE THE SELECTION AND ZOOM RECTANGLES.


    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    emit glPainted();
    
    //printf("paintGL called \n");
}

void CDisplay::resizeGL(const int f_width_i, const int f_height_i)
{
    // Can't repaint if window hasn't been displayed yet.
    if ( !m_initialized_b ) return;

    glViewport( 0, 0, f_width_i, f_height_i );
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();

    // Scale to use absolut pixel coordinates considering current zoom factor.
    const float totalWidth_f  = m_screenSize.width  * (float) m_screenCount.width;
    const float totalHeight_f = m_screenSize.height * (float) m_screenCount.height;

    // Centrate displays in window.
    float displayAspectRatio_f = totalWidth_f / (float)totalHeight_f;
    float widgetAspectRatio_f = f_width_i / (float)f_height_i;
    float aspectRel_f         = displayAspectRatio_f / widgetAspectRatio_f;
    
    if ( aspectRel_f < 1 )
    {
        float rel_f = totalHeight_f / f_height_i;
        
        m_roiSize.height = totalHeight_f;
        m_roiPos.y       = 0;
        
        m_roiSize.width  = rel_f * f_width_i;
        m_roiPos.x       = (m_roiSize.width - totalWidth_f)/2;
    }
    else
    {
        float rel_f = totalWidth_f / f_width_i;
        
        m_roiSize.width  = totalWidth_f;
        m_roiPos.x       = 0;
        
        m_roiSize.height = rel_f * f_height_i;
        m_roiPos.y       = (m_roiSize.height - totalHeight_f)/2;
    }

    if (isOneScreenMode())
        zoomToScreen ( m_fsScreen.x, m_fsScreen.y, false);
}

QImage
CDisplay::renderGL ()
{
    
    /// Must reset all PixelTransferf methods to default value
    /// otherwise th rendering is very slow.
    glPixelTransferf ( GL_RED_SCALE,   1. );
    glPixelTransferf ( GL_RED_BIAS,    0. );
    glPixelTransferf ( GL_GREEN_SCALE, 1.);
    glPixelTransferf ( GL_GREEN_BIAS,  0.);
    glPixelTransferf ( GL_BLUE_SCALE,  1.);
    glPixelTransferf ( GL_BLUE_BIAS,   0.);
    
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 8);
    
    int offset_i = 0;
    cv::Size size ( width(), height() );
    
    if ( size.width % 4 )
    {
        offset_i = (size.width % 4)/2;
        size.width -= size.width % 4;
    }
    
    if ( m_snapshot.size() != size )
        
    {
        m_snapshot.create(size, CV_8UC3);
        m_aux.create(size, CV_8UC3);
        
    }

    // printf("allocate array and read pixels into it.\n")
    // allocate array and read pixels into it.

    glReadPixels(offset_i, 0, size.width, size.height, GL_RGB, GL_UNSIGNED_BYTE, m_aux.data);
        
    /// printf("Flip image vertically.\n")
    /// Flip image vertically.
    for (int i =0; i < size.height; ++i)

    {
        memcpy( &m_snapshot.at<SRgb>(i,0),
                &m_aux.at<SRgb>(size.height-i-1,0),
                sizeof(SRgb) * size.width);

    }

    QImage img( (unsigned char *)m_snapshot.data, size.width, size.height, QImage::Format_RGB888);
    return img.copy();
}

void CDisplay::mousePressEvent( QMouseEvent * f_event_p )
{
    m_prevZoomTL = m_zoomTL;
    m_prevMousePos.x = f_event_p -> x();
    m_prevMousePos.y = f_event_p -> y();

    emit mousePressed ( getMouseEventData(f_event_p) );
}

CMouseEvent *CDisplay::getMouseEventData( QMouseEvent * f_event_p )
{
    static CMouseEvent me;
    me.qtMouseEvent_p = f_event_p;
    
    S2D<float> mousePos;
    /// normalize position.
    mousePos.x = m_zoomTL.x - m_roiPos.x + 
        (f_event_p->x() / (float)width()  * m_roiSize.width) / m_zoomFactor_f;
    
    mousePos.y = m_zoomTL.y - m_roiPos.y + 
        f_event_p->y() / (float)height() * m_roiSize.height / m_zoomFactor_f;
    
    me.posInDisplay = mousePos;
    
    S2D<int> screen ( (int)(mousePos.x / m_screenSize.width),
                      (int)(mousePos.y / m_screenSize.height) );

    if (mousePos.x < 0) --screen.x;
    if (mousePos.y < 0) --screen.y;
    
    me.displayScreen = screen;
    me.posInScreen = S2D<float>( mousePos.x - 
                                 screen.x * m_screenSize.width,
                                 mousePos.y - 
                                 screen.y * m_screenSize.height );
    return &me;
}

void CDisplay::mouseMoveEvent( QMouseEvent * f_event_p )
{
    m_highlightScreen.x = m_highlightScreen.y = -1;

    // Check if mouse clicked.
    if ( m_prevMousePos.isValid() && 
         (f_event_p -> buttons() & Qt::LeftButton) &&
         m_zoomFactor_f > 1 &&
         f_event_p->modifiers() & Qt::ControlModifier )
    {
        S2D<float> deltaPos ( m_prevMousePos.x - f_event_p -> x(),
                              m_prevMousePos.y - f_event_p -> y() );    
        
        // Scale to use absolut pixel coordinates.
        //const float totalWidth_f  = m_screenSize.width  * (float) m_screenCount.width;
        //const float totalHeight_f = m_screenSize.height * (float) m_screenCount.height;

        deltaPos.x *= m_roiSize.width  / width();
        deltaPos.y *= m_roiSize.height / height();
        
        m_zoomTL.x = m_prevZoomTL.x + deltaPos.x / (m_zoomFactor_f);
        m_zoomTL.y = m_prevZoomTL.y + deltaPos.y / (m_zoomFactor_f);

        //printf("zoomFactor: %f pos x: %f pos y: %f\n",
        //       m_zoomFactor_f, m_zoomTL.x, m_zoomTL.y );

        exitOneScreenMode();

        updateGL();
    }

    emit mouseMoved ( getMouseEventData(f_event_p) );
}

void CDisplay::mouseReleaseEvent(QMouseEvent * f_event_p )
{
    // Invalidate mouse pos.
    m_prevMousePos = S2D<float>();
    
    emit clicked();
    emit mouseReleased( getMouseEventData(f_event_p) );
}

void CDisplay::wheelEvent ( QWheelEvent *f_event_p )
{
    if ( f_event_p->orientation() == Qt::Vertical || 
         ( f_event_p->orientation() != Qt::Vertical &&
           (bool)(f_event_p -> modifiers() & Qt::AltModifier) )  )
    {
        int numDegrees_i = f_event_p->delta() / 8;
        int numSteps_i   = numDegrees_i / 15;
        
        float newZoomFactor_f = m_zoomFactor_f;

        if ( (bool)(f_event_p -> modifiers() & Qt::ControlModifier) )
        {
            if (numSteps_i > 0)
                newZoomFactor_f *= (abs(numSteps_i)+1);
            else
                newZoomFactor_f /= (abs(numSteps_i)+1);
        }
        else if ( (bool)(f_event_p -> modifiers() & Qt::ShiftModifier) )
        {

            newZoomFactor_f += numSteps_i/97.f;
        }
        else if ( (bool)(f_event_p -> modifiers() & Qt::AltModifier) )
        {
            newZoomFactor_f += numSteps_i/11.f;
        }
        else
            newZoomFactor_f += numSteps_i;
        
        newZoomFactor_f = std::max(1.f, newZoomFactor_f);
        
        exitOneScreenMode();
        
        zoom ( f_event_p -> x(),
               f_event_p -> y(),
               newZoomFactor_f,
               true );
    }

    f_event_p->accept();
    static CWheelEvent s_event;
    s_event.qtWheelEvent_p = f_event_p;
    emit wheelTurned ( &s_event );
}


/// Progressive zoom.
void 
CDisplay::zoom ( float f_x_f, 
                 float f_y_f, 
                 float f_newZoomFactor_f,
                 bool f_updateGL_b )
{
    if (f_newZoomFactor_f == 1.f)
    {
        m_zoomFactor_f = f_newZoomFactor_f;
        m_zoomTL.x = 0;
        m_zoomTL.y = 0;
    }
    else
    {
        // Scale to use absolut pixel coordinates.
        S2D<float> mousePos;

        /// normalize position.
        mousePos.x = f_x_f / (float)width()  * m_roiSize.width; 
        mousePos.y = f_y_f / (float)height() * m_roiSize.height;
    
        /// Compute position of mouse in the scaled coordinate.
        float x_f = m_zoomTL.x + mousePos.x / m_zoomFactor_f;
        float y_f = m_zoomTL.y + mousePos.y / m_zoomFactor_f;

        m_zoomFactor_f = f_newZoomFactor_f;

        /// Compute position top/left corner in the new zoomed
        /// coordinate.
        m_zoomTL.x = x_f - ( mousePos.x ) / m_zoomFactor_f;
        m_zoomTL.y = y_f - ( mousePos.y ) / m_zoomFactor_f;
    
        //printf("zoomFactor: %f pos x: %f pos y: %f\n",
        //       m_zoomFactor_f, m_zoomTL.x, m_zoomTL.y );    
    }

    if ( f_updateGL_b )
        updateGL();
}

void CDisplay::keyPressEvent ( QKeyEvent *   f_event_p )
{
    static Qt::Key prevKey_e;
    static int numKeyCount_i;
    
    bool emitEvent_b = true;
    
    if (f_event_p -> key() == Qt::Key_F )
    {
        emit fullScreenSwitched();
    }
    else if ( f_event_p -> key() == Qt::Key_Escape )
    {
        emit exitFullScreen();
    }
    else if ( f_event_p -> key() >= Qt::Key_F1 && 
              f_event_p -> key() <= Qt::Key_F35 )
    {
        int key_i = f_event_p -> key() - Qt::Key_F1;
        int sx_i = (int)(key_i % m_screenCount.width );
        int sy_i = (int)(key_i / m_screenCount.width );

        /// Center on other screen but with the same zoom.
        if (f_event_p -> modifiers() & Qt::ControlModifier )
        {
            int xScreen_i = ( m_zoomTL.x ) / m_screenSize.width;
            int yScreen_i = ( m_zoomTL.y ) / m_screenSize.height;
            
            float offsetx_f = ((sx_i - xScreen_i) * (int)m_screenSize.width);
            
            float offsety_f = ((sy_i - yScreen_i) * (int)m_screenSize.height);

            m_zoomTL.x += offsetx_f;
            m_zoomTL.y += offsety_f;

            updateGL();
        }
        else
        {   
            /// Zoom a screen.
            zoomToScreen ( sx_i, sy_i, true );
        }
    }
    else if ( ( f_event_p -> key() == Qt::Key_Left || 
                f_event_p -> key() == Qt::Key_Right || 
                f_event_p -> key() == Qt::Key_Up || 
                f_event_p -> key() == Qt::Key_Down )  )
    {
        if ( f_event_p -> modifiers() & Qt::ControlModifier )
        {            
            int xScreen_i = 0;
            int yScreen_i = 0;
            
            if ( f_event_p -> key() == Qt::Key_Left )
                ++xScreen_i;
            else if ( f_event_p -> key() == Qt::Key_Right )
                --xScreen_i;
            else if ( f_event_p -> key() == Qt::Key_Up )
                ++yScreen_i;
            else if ( f_event_p -> key() == Qt::Key_Down )
                --yScreen_i;            
            
            float offsetx_f = ((-xScreen_i) * (int)m_screenSize.width);
            
            float offsety_f = ((-yScreen_i) * (int)m_screenSize.height);
        
            m_zoomTL.x += offsetx_f;
            m_zoomTL.y += offsety_f;
            
            exitOneScreenMode();
            
            updateGL();

            emitEvent_b = false;
        }
    }
    else if ( f_event_p -> key() == Qt::Key_Less )
    {
        zoomOut();
    }
    else if ( f_event_p -> key() == Qt::Key_Greater )
    {
        zoomIn();
    }
    else if ( 0 && f_event_p ->key() == Qt::Key_G )
    {
//         static int imgNr_i = 0;

//         if (0)
//         {
//             QImage saveImage = renderGL();
            
//             char fileName_p[256];
            
//             sprintf(fileName_p, "grabbedDisplayImg_%05i.png", imgNr_i );
         
//             printf("Saving image %s\n", fileName_p );
            
//             saveImage.save( fileName_p );
            
//         }
//         ++imgNr_i;
    }
    
    if ( f_event_p -> key() >= Qt::Key_0 && 
         f_event_p -> key() <= Qt::Key_9 && 
         f_event_p -> modifiers() & Qt::AltModifier )
    {
        ++numKeyCount_i;

        /// Were two numeric keys pressed consecutively while presing the alt key?
        if ( prevKey_e >= Qt::Key_0 && 
             prevKey_e <= Qt::Key_9 && 
             (numKeyCount_i % 2) == 0 )
        {
            /// Move zoom to corresponding screen.
            int sx_i = (int)( ((int)prevKey_e) - ((int)Qt::Key_0));
            int sy_i = (int)( ((int)f_event_p -> key()) - ((int)Qt::Key_0));
            
            int xScreen_i = ( m_zoomTL.x ) / m_screenSize.width;
            int yScreen_i = ( m_zoomTL.y ) / m_screenSize.height;
            
            m_zoomTL.x += (((int)sx_i) - xScreen_i) * m_screenSize.width;
            m_zoomTL.y += (((int)sy_i) - yScreen_i) * m_screenSize.height;

            updateGL();
        }
    }
    else
        numKeyCount_i = 0;
    
    
    prevKey_e = (Qt::Key)f_event_p -> key();

    static CKeyEvent ke;
    ke.qtKeyEvent_p = f_event_p;

    if ( emitEvent_b )
        emit keyPressed ( &ke );

}

void CDisplay::displayScreens ( CDisplayOpNode * const f_parent_p, 
                                const int              f_level_f )
{
    if ( f_parent_p == NULL ) return;
    
    for (uint32_t i = 0; i < f_parent_p -> getDisplayCount(); ++i)
    {
        CDisplayNode *  dispNode_p = f_parent_p-> getDisplayChild (i);
        CDrawingList *  list_p     = dispNode_p -> getDrawingList();

        if ( list_p -> QCV::CDrawingList::isVisible() &&
             list_p -> getOverlayLevel() == f_level_f )
        {
            S2D<int> pos = list_p -> getPosition ( );

            if ( ( pos.x < (int)m_screenCount.width && 
                   pos.y < (int)m_screenCount.height && 
                   m_zoomFactor_f == 1) || m_zoomFactor_f != 1)
            {
                if ( !isOneScreenMode() ||
                     ( pos.x == m_fsScreen.x &&
                       pos.y == m_fsScreen.y ) )
                {
                    glPushMatrix();
                    
                    glTranslatef( pos.x * m_screenSize.width, 
                                  pos.y * m_screenSize.height, 
                                  0.0 );                    

                    glScalef( list_p -> getScaleX(),
                              list_p -> getScaleY(),
                              1.0f );
                    
                    glTranslatef( list_p -> getOffsetX(),
                                  list_p -> getOffsetY(), 
                                  0.0f );
                    
                    if (fabs(list_p->getRotation()) > 1.e-4)
                    {
                        glTranslatef( m_screenSize.width/2.f,
                                      m_screenSize.height/2.f,
                                      0.0f );

                        glRotated( list_p -> getRotation(), 0, 0, 1. );

                        glTranslatef( -m_screenSize.width/2.f,
                                      -m_screenSize.height/2.f,
                                      0.0f );
                    }

                    list_p -> show();

                    glPopMatrix();
                }
            }
        }
    }

    for (unsigned int i = 0; i < f_parent_p -> getOpCount(); ++i)
    {
        displayScreens ( f_parent_p-> getOpChild (i), f_level_f );
    }
}

void
CDisplay::highlightScreen()
{
    if ( m_highlightScreen.x > 0 && 
         m_highlightScreen.y > 0 )
        
    glPushMatrix();
    
    glTranslatef( m_highlightScreen.x * m_screenSize.width, 
                  m_highlightScreen.y * m_screenSize.height, 
                  0.0 );

    //glLineWidth( 4 );
    glColor4ub( 200, 255, 200, 20);
    
    glBegin(GL_LINE_LOOP);
    glVertex2f ( 0, 0 );
    glVertex2f ( m_screenSize.width, 0 );
    glVertex2f ( m_screenSize.width, m_screenSize.height );
    glVertex2f ( 0, m_screenSize.height );

    glEnd();
   
    glPopMatrix();   
}


void
CDisplay::show ( )
{
    m_initialized_b = true;
    QGLWidget::show();
}

void
CDisplay::showEvent ( QShowEvent * /*f_event_p*/ )
{
    m_initialized_b = true;
    resizeGL(width(), height());
}

bool CDisplay::showAllScreens ( )
{
    m_zoomTL.x = 0;
    m_zoomTL.y = 0;    
    m_zoomFactor_f = 1;

    updateGL();
    return true;
}

bool CDisplay::zoomToScreen ( const S2D<float>  screen,
                                 const bool f_update_b )
{
    return zoomToScreen ( screen.x, screen.y, f_update_b );
}

bool CDisplay::zoomToScreen ( const unsigned int f_screenX_ui, 
                              const unsigned int f_screenY_ui,
                              const bool         f_update_b )
{
    if ( f_screenX_ui < (unsigned) m_screenCount.width && 
         f_screenY_ui < (unsigned) m_screenCount.height )
    {
        m_fsScreen.x = f_screenX_ui;
        m_fsScreen.y = f_screenY_ui;

        S2D<float> aspect( m_roiSize.width  /  m_screenSize.width,
                           m_roiSize.height /  m_screenSize.height );

        if ( aspect.width < aspect.height )
        {
            m_zoomFactor_f = aspect.width;
            m_zoomTL.x = 0;
            float val_f = (m_screenSize.width ) / (float)width();
            m_zoomTL.y = -(height() * val_f - m_screenSize.height)/2;
        }
        else
        {
            m_zoomFactor_f = aspect.height;
            float val_f = (m_screenSize.height ) / (float)height();
            m_zoomTL.x = -(width() * val_f - m_screenSize.width)/2;
            m_zoomTL.y = 0;
        }

        m_zoomTL.x += f_screenX_ui * m_screenSize.width  + m_roiPos.x;
        m_zoomTL.y += f_screenY_ui * m_screenSize.height + m_roiPos.y;

        if (f_update_b)
            updateGL();

        return true;        
    }

    return false;
}

void CDisplay::zoomIn ( )
{
    QPoint cursorPos = QCursor::pos();
    QPoint winPos = pos();
    winPos = mapToGlobal(winPos);

    cursorPos.rx() -= winPos.x() - pos().x();
    cursorPos.ry() -= winPos.y() - pos().y();

    exitOneScreenMode();

    zoom ( cursorPos.x(),
           cursorPos.y(),
           std::max(m_zoomFactor_f*2, 1.f),
           true );
}

void CDisplay::zoomOut ( )
{
    QPoint cursorPos = QCursor::pos();
    QPoint winPos = pos();    
    winPos = mapToGlobal(winPos);

    cursorPos.rx() -= winPos.x() - pos().x();    
    cursorPos.ry() -= winPos.y() - pos().y();

    exitOneScreenMode();

    zoom ( cursorPos.x(),
           cursorPos.y(),
           std::max(m_zoomFactor_f/2, 1.f),
           true );
}

/// DRAG/DROP OPERATIONS
void CDisplay::dragEnterEvent(QDragEnterEvent *event)
{
    m_highlightScreen.x = m_highlightScreen.y = -1;
    //printf("Drag Enter\n");
    
    if (event->mimeData()->hasFormat("display/gl-display-list"))
        event->accept();
    else
        event->ignore();
}

void CDisplay::dragMoveEvent(QDragMoveEvent *f_event_p)
{
    // Some cool display should be made here.
    //printf("Drag Move\n");
    m_highlightScreen.x = m_highlightScreen.y = -1;
    
    if (f_event_p->mimeData()->hasFormat("display/gl-display-list")) 
    {
        f_event_p->setDropAction(Qt::MoveAction);
        f_event_p->accept();
       
        //printf("Drop F_Event_P\n");
        if (f_event_p->mimeData()->hasFormat("display/gl-display-list")) 
        {
            QByteArray pieceData = f_event_p->mimeData()->data("display/gl-display-list");
            QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
            //QPixmap pixmap;
            //QPoint location;
            QModelIndexList items;
           
            qint32 size_i;
            quint64 ptr_ui;
           
            dataStream >> size_i;
            //printf("Size is %i\n", size_i);
           
            for (int i = 0; i < size_i; ++i)
            {
                dataStream >> ptr_ui;
               
                /// normalize position.
                QPoint pos = f_event_p -> pos ();
                S2D<float> mousePos;
                mousePos.x = pos.x() / (float)width()  * m_roiSize.width  - m_roiPos.x;
                mousePos.y = pos.y() / (float)height() * m_roiSize.height - m_roiPos.y;
               
                S2D<int> newPos;
                newPos.x =  std::min( std::max( 0,
                                                (int)(mousePos.x / m_screenSize.width) ),
                                      (int)(m_screenCount.width  - 1 ) );
                newPos.y =  std::min( std::max( 0, 
                                                (int)(mousePos.y / m_screenSize.height) ), 
                                      (int)(m_screenCount.height - 1 ) );
               
                m_highlightScreen = newPos;
            }
           
            f_event_p->setDropAction(Qt::MoveAction);
            f_event_p->accept();
           
            updateGL();
        } 
    }
    else
        f_event_p->ignore();
}

void CDisplay::dropEvent(QDropEvent *f_event_p)
{
    m_highlightScreen.x = m_highlightScreen.y = -1;
    //printf("Drop F_Event_P\n");
    if (f_event_p->mimeData()->hasFormat("display/gl-display-list")) 
    {
        QByteArray pieceData = f_event_p->mimeData()->data("display/gl-display-list");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        //QPixmap pixmap;
        //QPoint location;
        QModelIndexList items;
        
        qint32 size_i;
        quint64 ptr_ui;
        
        dataStream >> size_i;
        //printf("Size is %i\n", size_i);
        
        for (int i = 0; i < size_i; ++i)
        {
            dataStream >> ptr_ui;

            CDisplayNode *  node_p = static_cast<CDisplayNode *>((void*)ptr_ui);
            
            /// normalize position.
            QPoint pos = f_event_p -> pos ();
            S2D<float> mousePos;
            mousePos.x = pos.x() / (float)width()  * m_roiSize.width  - m_roiPos.x;
            mousePos.y = pos.y() / (float)height() * m_roiSize.height - m_roiPos.y;
            
            S2D<int> newPos;
            newPos.x =  std::min( std::max( 0,
                                            (int)(mousePos.x / m_screenSize.width) ),
                                  (int)(m_screenCount.width  - 1 ) );
            newPos.y =  std::min( std::max( 0, 
                                            (int)(mousePos.y / m_screenSize.height) ), 
                                  (int)(m_screenCount.height - 1 ) );
            
            node_p -> getDrawingList() -> setPosition(newPos);
            node_p -> setVisibility(true);
        }
        
        f_event_p->setDropAction(Qt::MoveAction);
        f_event_p->accept();
        
        updateGL();
    } 
    else
        f_event_p->ignore();
}

bool
CDisplay::setScreenCount ( S2D<unsigned int> f_count )
{
    m_screenCount = f_count;
    resizeGL(width(), height());
    return true;
}

bool
CDisplay::setScreenSize ( const S2D<unsigned int> f_size )
{
    if ( (int)f_size.width  != m_screenSize.width ||
         (int)f_size.height != m_screenSize.height )
    {
        m_screenSize.width  = f_size.width;
        m_screenSize.height = f_size.height;
        
        resizeGL(width(), height());
    }
    return true;
}

S2D<unsigned int> 
CDisplay::getScreenSize ( ) const
{
    return S2D<unsigned int>(m_screenSize.width, m_screenSize.height);
}

void
CDisplay::timerEvent ( QTimerEvent * /*f_event_p*/ )
{
    /// activate it and implement here the deactivation of the drag 
    
}
