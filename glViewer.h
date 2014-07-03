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

#ifndef __GLVIEWER_H
#define __GLVIEWER_H

/**
 *******************************************************************************
 *
 * @file glViewer.h
 *
 * \class CGLViewer
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Interface class for the QGLViewer.
 *
 ******************************************************************************/

/* INCLUDES */

#include <QGLViewer/qglviewer.h>

#include "3DRowVector.h"
#include "3DPointList.h"
#include "3DLineList.h"
#include "3DMeshList.h"
#include "colors.h"

namespace QCV
{
    class CGLViewer : public QGLViewer
    {

    public:
        CGLViewer();

    public:
        /// Clear the list of drawings.
        void                   clear ();

        /// Add a 3D point.
        void                   addPoint ( const C3DVector  f_point,
                                          const SRgb       f_color  = SRgb ( 255, 255, 255 ),
                                          const float      f_size_f = -1.f,
                                          const C3DVector  f_normal = C3DVector (0,0,0) );
        
        /// Add a 3D point.
        void                   addLine  ( const C3DVector   f_point1,
                                          const C3DVector   f_point2,
                                          const SRgb        f_color  = SRgb ( 255, 255, 255 ),
                                          const float       f_lineWidth_f = -1.f );
        
        /// Add a 3D mesh.
        void                   addMesh ( cv::Mat     f_vectorImg,
                                         cv::Mat     f_dispTexture,
                                         const float f_maxDist_f,
                                         const float f_maxInvDist_f  );
        
        bool                   setBackgroundColor ( SRgb f_bgColor ) { m_bgColor = f_bgColor; return true; }
        SRgb                   getBackgroundColor (  ) const { return m_bgColor; }
        
        bool                   setPointSize ( float f_pointSize_f ) { m_pointSize_f = f_pointSize_f; return true; }
        float                  getPointSize (  ) const { return m_pointSize_f; }
        
        bool                   setLineWidth ( float f_lineWidth_f ) { m_lineWidth_f = f_lineWidth_f; return true; }
        float                  getLineWidth (  ) const { return m_lineWidth_f; }

        bool                   setGridLength ( float f_gridLength_f ) { m_gridLength_f = f_gridLength_f; return true; }
        float                  getGridLength (  ) const { return m_gridLength_f; }

        bool                   setGridWidth ( float f_gridWidth_f ) { m_gridWidth_f = f_gridWidth_f; return true; }
        float                  getGridWidth (  ) const { return m_gridWidth_f; }

        bool                   setRadiusOfScene ( float f_sceneRadius_f ) { m_sceneRadius_f = f_sceneRadius_f; return true; }
        float                  getRadiusOfScene (  ) const { return m_sceneRadius_f; }

    protected :
        /// Draw
        virtual void           preDraw();

        /// Draw
        virtual void           draw();

        /// Initialize
        virtual void           init();

        /// Help string for project.
        virtual QString        helpString() const;

        /// Post draw virtual method
        virtual void           postDraw();
        

    private:
        void                   drawMeshes();
        
    private:
        /// 3D point list
        C3DPointList               m_pointList;

        /// 3D mesh list
        C3DLineList                m_lineList;

        /// 3D mesh list
        C3DMeshList                m_meshList;

        /// Background color.
        SRgb                       m_bgColor;

        /// Point size.
        float                      m_pointSize_f;

        /// Line width.
        float                      m_lineWidth_f;

        /// Grid length
        float                      m_gridLength_f;

        /// Grid width
        float                      m_gridWidth_f;

        /// Scene radius
        float                      m_sceneRadius_f;
        
    };
}


#endif // __GLVIEWER_H
 

/* ////////////  Version History ///////////////
 *  $Log: glViewer.h,v $
 *  Revision 1.3  2010/01/18 17:12:29  badino
 *  General updates and added new classes.
 *
 *  Revision 1.2  2009/11/18 15:51:01  badino
 *  badino: documentation added. Some other global changes.
 *
 *//////////////////////////////////////////////


