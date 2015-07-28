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
* \file  glViewer.cpp
* \author Hernan Badino
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
          m_sceneRadius_f (        10.f )
{
    setWindowTitle("3D GL Viewer");
    setObjectName ( windowTitle() );
}


void CGLViewer::draw()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glScalef( -1, 1, 1);

    camera()->setZClippingCoefficient(10);
    
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

    /// Points
    m_lineList.show();
    
    /// Meshes
    m_meshList.show();

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

void CGLViewer::addPoint ( const C3DVector  f_point,
                           const SRgb       f_color,
                           const float      f_size_f,
                           const C3DVector  f_normal )
{
    float pointSize_f = f_size_f<0?m_pointSize_f:f_size_f;  
    m_pointList.add( f_point, f_color, pointSize_f, f_normal );    
}

void CGLViewer::addLine  ( const C3DVector  f_point1,
                           const C3DVector  f_point2,
                           const SRgb       f_color,
                           const float      f_lineWidth_f )
{
    float lineWidth_f = f_lineWidth_f<0?m_lineWidth_f:f_lineWidth_f;  
    m_lineList.add( f_point1, f_point2, f_color, lineWidth_f );
}

void CGLViewer::addMesh ( cv::Mat     f_vectorImg,
                          cv::Mat     f_dispTexture,
                          const float f_maxDist_f,
                          const float f_maxInvDist_f  )
{
    m_meshList.add( f_vectorImg, f_dispTexture,  f_maxDist_f, f_maxInvDist_f );
}

void CGLViewer::clear ()
{
    m_pointList.clear();
    m_lineList.clear();
    m_meshList.clear();
}
