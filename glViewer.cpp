/*@@@**************************************************************************
* \file  glViewer
* \author Hernan Badino
* \date  Tue Sep  8 13:34:41 EDT 2009
* \notes 
*******************************************************************************
*****          (C) COPYRIGHT Hernan Badino - All Rights Reserved          *****
******************************************************************************/

/* INCLUDES */
#include "glViewer.h"

using namespace QCV;

CGLViewer::CGLViewer()
        : QGLViewer (                    ),
          m_bgColor (         96, 96, 96 ),
          m_lineWidth_f (            0.f ),

          m_gridLength_f (         100.f ),
          m_gridWidth_f (          100.f ),
          m_sceneRadius_f (         15.f )
{
    setWindowTitle("3D GL Viewer");
    setObjectName ( windowTitle() );
}


void CGLViewer::draw()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    camera()->setSceneRadius( m_sceneRadius_f );

    // Reset modelview matrix
    QGLViewer::setBackgroundColor( QColor( m_bgColor.r, m_bgColor.g, m_bgColor.b ));

    glLineWidth( m_lineWidth_f );

    glDisable( GL_TEXTURE_GEN_Q );
    glDisable( GL_TEXTURE_GEN_S );
    glDisable( GL_TEXTURE_GEN_R );
    glDisable( GL_TEXTURE_GEN_T );

    glDisable( GL_TEXTURE_1D );
    glDisable( GL_TEXTURE_2D );

    glDisable( GL_COLOR_MATERIAL );

    glDisable( GL_LIGHTING );
    
    /////////// Draw figures /////////

    /// Points
    m_pointList.show();
    
    /// Meshes
    //m_meshList.draw();

    /////////// End drawing figures /////////

    glPopMatrix();  
}

void
CGLViewer::preDraw()
{
    QGLViewer::preDraw();
}

void
CGLViewer::postDraw()
{
    glPushMatrix();
    camera()->loadModelViewMatrix();
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    glDisable( GL_TEXTURE_GEN_Q );
    glDisable( GL_TEXTURE_GEN_S );
    glDisable( GL_TEXTURE_GEN_R );
    glDisable( GL_TEXTURE_GEN_T );

    glDisable( GL_TEXTURE_1D );
    glDisable( GL_TEXTURE_2D );

    glDisable( GL_COLOR_MATERIAL );

    // Draw all visual hints (revolve around point, zoom and so on).
    drawVisualHints();

    if ( gridIsDrawn() )
        drawGrid( m_gridWidth_f, m_gridLength_f );

    glDisable( GL_LIGHTING );

    if ( axisIsDrawn() )
    {
        glLineWidth( 3 );
        drawAxis( 1. );
    }

    // Restore GL state
    glPopAttrib();
    glPopMatrix();
}


void CGLViewer::init()
{
    /// From f1 to f3 different camera poses.
    camera()->setPosition( qglviewer::Vec( 0.0, 0.0, 0.0 ) );
    camera()->lookAt(      qglviewer::Vec( 0.0, 0.0, 1.0 ) );
    camera()->setUpVector( qglviewer::Vec( 0.0, 1.0, 0.0 ) );
    camera()->setRevolveAroundPoint( qglviewer::Vec( 0.0,  0.0, 1.0 ) );
    camera()->addKeyFrameToPath( 1 );
    
    camera()->setPosition( qglviewer::Vec( 0.0, 75.0, 25.0 ) );
    camera()->lookAt(      qglviewer::Vec( 0.0,  0.0, 25.0 ) );
    camera()->setUpVector( qglviewer::Vec( 0.0,  0.0,  1.0 ) );
    camera()->setRevolveAroundPoint( qglviewer::Vec( 0.0,  0.0, 25.0 ) );
    camera()->addKeyFrameToPath( 2 );
    
    camera()->setPosition( qglviewer::Vec( -50.0, 1.0, 25.0 ) );
    camera()->lookAt(      qglviewer::Vec( 0.0,   1.0, 25.0 ) );
    camera()->setUpVector( qglviewer::Vec( 0.0,  1.0,  0.0 ) );
    camera()->setRevolveAroundPoint( qglviewer::Vec( 0.0,  1.0, 25.0 ) );

    camera()->addKeyFrameToPath( 3 );
}

QString CGLViewer::helpString() const
{
    QString text("<h2>3D Viewer</h2>");
    text += "Documentation must still be added.";
    return text;
}

void CGLViewer::addPoint ( const C3DVector & f_point,
                           const SRgb      & f_color,
                           const float       f_size_f,
                           const C3DVector & f_normal )
{
    m_pointList.add( f_point, f_color, f_size_f, f_normal );    
}

void CGLViewer::clear ()
{
    m_pointList.clear();
    //m_meshList.clear();
}

/* ////////////  Version History ///////////////
 *  $Log: glViewer.cpp,v $
 *  Revision 1.3  2010/01/18 17:12:29  badino
 *  General updates and added new classes.
 *
 *  Revision 1.2  2009/11/18 15:50:15  badino
 *  badino: global changes.
 *
 *//////////////////////////////////////////////
