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
 * \file  drawingList
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "drawingList.h"
#include "glheader.h"
#include "clipLine.h"
#include <stdio.h>

using namespace QCV;

CDrawingList::CDrawingList( std::string f_name_str )
        : m_name_str (                    f_name_str ),
          m_position (                          0, 0 ),
          m_visible_b (                         true ),
          m_lineColor (                   0, 0, 0, 0 ),
          m_fillColor (             255, 255, 255, 0 ),
          m_lineWidth_f (                          1 ),
          m_overlayLevel_i (                       0 ),
          m_screenSize (                    640, 480 ),
          m_scaleX_d (                            1. ),
          m_scaleY_d (                            1. ),
          m_offsetX_d (                           0. ),
          m_offsetY_d (                           0. ),
          m_rotAngle_d (                          0. )
{
    m_colorMask_p[0] = true;
    m_colorMask_p[1] = true;
    m_colorMask_p[2] = true;
    m_colorMask_p[3] = false;

    m_drawElems_v.push_back( &m_images );
    m_drawElems_v.push_back( &m_colorEncImages );
    m_drawElems_v.push_back( &m_lines );
    m_drawElems_v.push_back( &m_rectangles );
    m_drawElems_v.push_back( &m_polygons );
    m_drawElems_v.push_back( &m_triangles );
    m_drawElems_v.push_back( &m_ellipses );
    m_drawElems_v.push_back( &m_strings );
}

CDrawingList::~CDrawingList()
{
}

/// Add image.
bool
CDrawingList::addImage ( cv::Mat            f_img,
                         const float        f_u_f,
                         const float        f_v_f,
                         float              f_width_f,
                         float              f_height_f,
                         const float        f_scale_f,
                         const float        f_offset_f,
                         const float        f_alpha_f,
                         const bool         f_makeCopy_b )
{
    if (f_img.cols <= 0 || 
	f_img.rows <= 0 ||
	!f_img.data)
        return false;

    if (f_width_f == -1)
        f_width_f = f_img.size().width;

    if (f_height_f == -1)
        f_height_f = f_img.size().height;
    
    return m_images.add ( f_img,
                          f_u_f,
                          f_v_f,
                          f_width_f,
                          f_height_f,
                          f_scale_f,
                          f_offset_f,
                          f_alpha_f,
                          f_makeCopy_b );
}

/// Add image.
bool 
CDrawingList::addColorEncImage ( cv::Mat *            f_img_p,
                                 const CColorEncoding f_encoder,
                                 const float          f_u_f,
                                 const float          f_v_f,
                                 float                f_width_f,
                                 float                f_height_f,
                                 const float          f_alpha_f,
                                 const bool           f_makeCopy_b )
{
    if (f_width_f == -1)
        f_width_f = f_img_p->size().width;

    if (f_height_f == -1)
        f_height_f = f_img_p->size().height;
    
    return m_colorEncImages.add (  f_img_p,
                                   f_encoder,
                                   f_u_f,
                                   f_v_f,
                                   f_width_f,
                                   f_height_f,
                                   f_alpha_f,
                                   f_makeCopy_b );
}


/// Add string.
bool
CDrawingList::addText ( std::string f_text,
                        float f_u_f, float f_v_f,
                        float f_fontSize_f,
                        bool f_fixSize_b )
{
    return m_strings.add ( f_text, 
                           f_u_f, f_v_f,
                           m_lineColor,
                           m_lineWidth_f,
                           f_fontSize_f,
                           f_fixSize_b );
}

/// Add line.
bool
CDrawingList::addLine ( const float f_u1_f, const float f_v1_f,
                        const float f_u2_f, const float f_v2_f )
{
    return m_lines.add ( f_u1_f, f_v1_f, f_u2_f, f_v2_f,
                         m_lineColor,
                         m_lineWidth_f );
}

/// Add line.
bool
CDrawingList::addLine ( const S2D<float> f_point1, 
                        const S2D<float> f_point2 )
{
    return m_lines.add ( f_point1.x, f_point1.y, 
                         f_point2.x, f_point2.y, 
                         m_lineColor,
                         m_lineWidth_f );
}

/// Add line.
bool
CDrawingList::addClippedLine (  float f_u1_f,  float f_v1_f,
                                float f_u2_f,  float f_v2_f,
                                const float f_cu1_f, const float f_cv1_f,
                                const float f_cu2_f, const float f_cv2_f )
{
    if ( clipLine ( f_u1_f, f_v1_f, 
                    f_u2_f, f_v2_f, 
                    f_cu1_f, f_cv1_f, 
                    f_cu2_f, f_cv2_f ) != 0 )
        
        return addLine ( f_u1_f, f_v1_f, 
                         f_u2_f, f_v2_f );

    return false;    
}

/// Add line.
bool
CDrawingList::addClippedLine ( S2D<float> f_point1, 
                               S2D<float> f_point2,
                               const S2D<float> f_cp1, 
                               const S2D<float> f_cp2 )
{
    return addClippedLine ( f_point1.x, f_point1.y, 
                            f_point2.x, f_point2.y, 
                            f_cp1.x, f_cp1.y, 
                            f_cp2.x, f_cp2.y );
}

/// Add rectangle.
bool
CDrawingList::addRectangle ( const float f_u1_f, const float f_v1_f,
                             const float f_u2_f, const float f_v2_f )
{

    return m_rectangles.add ( f_u1_f, f_v1_f, f_u2_f, f_v2_f,
                              m_lineColor,
                              m_lineWidth_f );
}

/// Add rectangle.
bool
CDrawingList::addRectangle ( const S2D<float> f_tl, 
                             const S2D<float> f_br )
{
    return m_rectangles.add ( f_tl.x, f_tl.y, 
                              f_br.x, f_br.y, 
                              m_lineColor,
                              m_lineWidth_f );
}

bool
CDrawingList::addFilledRectangle ( const float f_u1_f, const float f_v1_f,
                                   const float f_u2_f, const float f_v2_f )
{

    return m_rectangles.add ( f_u1_f, f_v1_f, f_u2_f, f_v2_f,
                              m_lineColor,
                              m_fillColor,
                              m_lineWidth_f );
}

/// Add filled rectangle.
bool
CDrawingList::addFilledRectangle ( const S2D<float> f_tl, 
                                   const S2D<float> f_br )
{
    return m_rectangles.add ( f_tl.x, f_tl.y, 
                              f_br.x, f_br.y, 
                              m_lineColor,
                              m_fillColor,
                              m_lineWidth_f );
}

/// Add square.
bool
CDrawingList::addSquare ( const float f_u_f, 
                          const float f_v_f,
                          const float f_halfSize_f )
{
    return m_rectangles.add ( f_u_f - f_halfSize_f, 
                              f_v_f - f_halfSize_f, 
                              f_u_f + f_halfSize_f, 
                              f_v_f + f_halfSize_f, 
                              m_lineColor,
                              m_lineWidth_f );
}

/// Add square.
bool
CDrawingList::addSquare ( const S2D<float> f_center, 
                          const float       f_size_f )
{
    return addSquare ( f_center.x,
                       f_center.y,
                       f_size_f );
}

/// Add filled square.
bool
CDrawingList::addFilledSquare ( const float f_u_f, 
                                const float f_v_f,
                                const float f_halfSize_f )
{
    return m_rectangles.add ( f_u_f - f_halfSize_f, 
                              f_v_f - f_halfSize_f, 
                              f_u_f + f_halfSize_f, 
                              f_v_f + f_halfSize_f, 
                              m_lineColor,
                              m_fillColor,
                              m_lineWidth_f );
}

/// Add square.
bool
CDrawingList::addFilledSquare ( const S2D<float> f_center, 
                                const float       f_size_f )
{
    return addFilledSquare ( f_center.x, 
                             f_center.y,
                             f_size_f );
}

/// Add polygon
bool
CDrawingList::addPolygon (const std::vector< S2D<float> > &vertex_v )
{
    return m_polygons.add ( vertex_v, 
                            m_lineColor,
                            m_lineWidth_f );
}

/// Add filled polygon
bool
CDrawingList::addFilledPolygon ( const std::vector< S2D<float> > &vertex_v )
{
    return m_polygons.add ( vertex_v, 
                            m_lineColor,
                            m_fillColor,
                            m_lineWidth_f );
}

/// Add Quadrilateral.
bool
CDrawingList::addQuadrilateral ( const S2D<float> f_v0, 
                               const S2D<float> f_v1,
                               const S2D<float> f_v2,
                               const S2D<float> f_v3 )
{
    std::vector< S2D<float> > v;

    v.push_back(f_v0);
    v.push_back(f_v1);
    v.push_back(f_v2);
    v.push_back(f_v3);
    
    return m_polygons.add ( v, 
                            m_lineColor,
                            m_lineWidth_f );
}


/// Add filled Quadrilateral.
bool
CDrawingList::addFilledQuadrilateral ( const S2D<float> f_v0, 
                                     const S2D<float> f_v1,
                                     const S2D<float> f_v2,
                                     const S2D<float> f_v3 )
{
    std::vector< S2D<float> > v;

    v.push_back(f_v0);
    v.push_back(f_v1);
    v.push_back(f_v2);
    v.push_back(f_v3);
    
    return m_polygons.add ( v, 
                            m_lineColor,
                            m_fillColor,
                            m_lineWidth_f );
}

/// Add Pentagon.
bool
CDrawingList::addPentagon ( const S2D<float> f_v0, 
                            const S2D<float> f_v1,
                            const S2D<float> f_v2,
                            const S2D<float> f_v3,
                            const S2D<float> f_v4 )
{
    std::vector< S2D<float> > v;

    v.push_back(f_v0);
    v.push_back(f_v1);
    v.push_back(f_v2);
    v.push_back(f_v3);
    v.push_back(f_v4);

    return m_polygons.add ( v, 
                            m_lineColor,
                            m_lineWidth_f );
}

/// Add filled Pentagon.
bool
CDrawingList::addFilledPentagon ( const S2D<float> f_v0, 
                                  const S2D<float> f_v1,
                                  const S2D<float> f_v2,
                                  const S2D<float> f_v3,
                                  const S2D<float> f_v4 )
{
    std::vector< S2D<float> > v;

    v.push_back(f_v0);
    v.push_back(f_v1);
    v.push_back(f_v2);
    v.push_back(f_v3);
    v.push_back(f_v4);
    
    return m_polygons.add ( v, 
                            m_lineColor,
                            m_fillColor,
                            m_lineWidth_f );
}
        
/// Add Hexagon.
bool        
CDrawingList::addHexagon ( const S2D<float> f_v0, 
                           const S2D<float> f_v1,
                           const S2D<float> f_v2,
                           const S2D<float> f_v3,
                           const S2D<float> f_v4,
                           const S2D<float> f_v5 )
{
    std::vector< S2D<float> > v;

    v.push_back(f_v0);
    v.push_back(f_v1);
    v.push_back(f_v2);
    v.push_back(f_v3);
    v.push_back(f_v4);
    v.push_back(f_v5);
    
    return m_polygons.add ( v, 
                            m_lineColor,
                            m_lineWidth_f );
}

/// Add Filled Hexagon.
bool
CDrawingList::addFilledHexagon ( const S2D<float> f_v0, 
                                 const S2D<float> f_v1,
                                 const S2D<float> f_v2,
                                 const S2D<float> f_v3,
                                 const S2D<float> f_v4,
                                 const S2D<float> f_v5 )
{
    std::vector< S2D<float> > v;

    v.push_back(f_v0);
    v.push_back(f_v1);
    v.push_back(f_v2);
    v.push_back(f_v3);
    v.push_back(f_v4);
    v.push_back(f_v5);
    
    return m_polygons.add ( v, 
                            m_lineColor,
                            m_fillColor,
                            m_lineWidth_f );
}

/// Add circle.
bool
CDrawingList::addCircle ( const float f_u_f, const float f_v_f,
                          const float f_radius_f )
{
    return m_ellipses.add ( f_u_f, f_v_f, 
                            f_radius_f, f_radius_f,
                            0.f, // rotation.
                            m_lineColor,
                            m_lineWidth_f );
}

/// Add circle.
bool
CDrawingList::addCircle ( const S2D<float>  f_center,
                          const float f_radius_f )
{
    return m_ellipses.add ( f_center.x, f_center.y,
                            f_radius_f, f_radius_f,
                            0.f, // rotation.
                            m_lineColor,
                            m_lineWidth_f );
}


/// Add filled circle.
bool 
CDrawingList::addFilledCircle ( const float f_u_f, const float f_v_f,
                                const float f_radius_f )
{
    return m_ellipses.add ( f_u_f, f_v_f, 
                            f_radius_f, f_radius_f,
                            0.f, // rotation.
                            m_lineColor,
                            m_fillColor,
                            m_lineWidth_f );
}

/// Add circle.
bool
CDrawingList::addFilledCircle ( const S2D<float> f_center,
                                const float f_radius_f )
{
    return m_ellipses.add ( f_center.x, f_center.y,
                            f_radius_f, f_radius_f,
                            0.f, // rotation.
                            m_lineColor,
                            m_fillColor,
                            m_lineWidth_f );
}

/// Add ellipse.
bool 
CDrawingList::addEllipse ( const float f_u_f,    const float f_v_f,
                           const float f_radU_f, const float f_radV_f,
                           const float f_rotation_f )
{
    return m_ellipses.add ( f_u_f, f_v_f, 
                            f_radU_f, f_radV_f,
                            f_rotation_f,
                            m_lineColor,
                            m_lineWidth_f );
}

/// Add filled ellipse.
bool 
CDrawingList::addEllipse (  const S2D<float> f_center,
                            const S2D<float> f_radius,
                            const float f_rotation_f )
{
    return m_ellipses.add ( f_center.x, f_center.y,
                            f_radius.x, f_radius.y,
                            f_rotation_f,
                            m_lineColor,
                            m_lineWidth_f );
}

/// Add filled ellipse.
bool 
CDrawingList::addFilledEllipse (  const float f_u_f,    const float f_v_f,
                                  const float f_radU_f, const float f_radV_f,
                                  const float f_rotation_f )
{
    return m_ellipses.add ( f_u_f, f_v_f, 
                            f_radU_f, f_radV_f,
                            f_rotation_f,
                            m_lineColor,
                            m_fillColor,
                            m_lineWidth_f );
}

/// Add filled ellipse.
bool 
CDrawingList::addFilledEllipse (  const S2D<float> f_center,
                                  const S2D<float> f_radius,
                                  const float f_rotation_f )
{
    return m_ellipses.add ( f_center.x, f_center.y,
                            f_radius.x, f_radius.y,
                            f_rotation_f,
                            m_lineColor,
                            m_fillColor,
                            m_lineWidth_f );
}

/// Add triangle.
bool
CDrawingList::addTriangle ( const float f_u1_f, const float f_v1_f,
                            const float f_u2_f, const float f_v2_f,
                            const float f_u3_f, const float f_v3_f )
{
    return m_triangles.add ( f_u1_f, f_v1_f, 
                             f_u2_f, f_v2_f, 
                             f_u3_f, f_v3_f,
                             m_lineColor,
                             m_lineWidth_f );
}

/// Add triangle.
bool 
CDrawingList::addTriangle ( const S2D<float> f_vertex1, 
                            const S2D<float> f_vertex2, 
                            const S2D<float> f_vertex3 )
{
    return m_triangles.add ( f_vertex1, 
                             f_vertex2,
                             f_vertex3,
                             m_lineColor,
                             m_lineWidth_f );
}

/// Add filled triangle.
bool
CDrawingList::addFilledTriangle ( const float f_u1_f, const float f_v1_f,
                                  const float f_u2_f, const float f_v2_f,
                                  const float f_u3_f, const float f_v3_f )
{
    return m_triangles.add ( f_u1_f, f_v1_f, 
                             f_u2_f, f_v2_f, 
                             f_u3_f, f_v3_f,
                             m_lineColor,
                             m_fillColor,
                             m_lineWidth_f );
}

/// Add filled triangle.
bool
CDrawingList::addFilledTriangle ( const S2D<float> f_vertex1, 
                                  const S2D<float> f_vertex2, 
                                  const S2D<float> f_vertex3 )
{
    return m_triangles.add ( f_vertex1, 
                             f_vertex2,
                             f_vertex3,
                             m_lineColor,
                             m_fillColor,
                             m_lineWidth_f );
}

/// Add cross.
bool
CDrawingList::addCross ( const float f_u1_f, const float f_v1_f, float f_radius_f )
{

    bool ok_b = m_lines.add ( f_u1_f-f_radius_f, f_v1_f-f_radius_f, f_u1_f+f_radius_f, f_v1_f+f_radius_f,
                              m_lineColor,
                              m_lineWidth_f );

    ok_b &= m_lines.add ( f_u1_f-f_radius_f, f_v1_f+f_radius_f, f_u1_f+f_radius_f, f_v1_f-f_radius_f,
                          m_lineColor,
                          m_lineWidth_f );

    return ok_b;
}

/// Add cross.
bool
CDrawingList::addCross ( const float f_u1_f, const float f_v1_f,
                         const float f_u2_f, const float f_v2_f )
{

    bool ok_b = m_lines.add ( f_u1_f, f_v1_f, f_u2_f, f_v2_f,
                              m_lineColor,
                              m_lineWidth_f );

    ok_b &= m_lines.add ( f_u1_f, f_v2_f, f_u2_f, f_v1_f,
                          m_lineColor,
                          m_lineWidth_f );

    return ok_b;
}

/// Add cross.
bool
CDrawingList::addCross ( const S2D<float> f_tl, 
                         const S2D<float> f_br )
{
    return addCross ( f_tl.x, f_tl.y, 
                      f_br.x, f_br.y );
}

/// Show all.        
bool
CDrawingList::show ()
{
    bool resShow_b;
    bool success_b = true;
    
    std::vector< CDrawingElementList * >::const_iterator 
        last = m_drawElems_v.end();

    glColorMask( m_colorMask_p[0],
                 m_colorMask_p[1],
                 m_colorMask_p[2],
                 m_colorMask_p[3] );

    for (std::vector< CDrawingElementList * >::const_iterator 
             i = m_drawElems_v.begin(); 
         i != last; ++i )
    {
        if ((*i)->isBlendable())
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }

        resShow_b = (*i)->show();
        success_b &= resShow_b;
    }

    glColorMask( true, true, true, false );

    glDisable(GL_BLEND);

    return success_b;
}

/// Clear all.        
bool
CDrawingList::clear ()
{
    bool resClear_b;
    bool success_b = true;
    
    std::vector< CDrawingElementList * >::const_iterator 
        last = m_drawElems_v.end();

    for (std::vector< CDrawingElementList * >::const_iterator 
             i = m_drawElems_v.begin(); 
         i != last; ++i )
    {
        resClear_b = (*i)->clear();
        success_b &= resClear_b;
    }

    return success_b;
}


bool
CDrawingList::addDrawingList ( const CDrawingList &f_other )
{
    m_images.add          ( f_other.m_images );
    m_colorEncImages.add  ( f_other.m_colorEncImages );
    m_lines.add           ( f_other.m_lines );
    m_rectangles.add      ( f_other.m_rectangles );
    m_triangles.add       ( f_other.m_triangles );
    m_ellipses.add        ( f_other.m_ellipses );
    m_strings.add         ( f_other.m_strings );
    m_polygons.add        ( f_other.m_polygons );

    return true;    
}

int
CDrawingList::getElementsCount() const
{
    return ( m_images.getSize () + 
             m_colorEncImages.getSize () + 
             m_lines.getSize () + 
             m_rectangles.getSize () + 
             m_triangles.getSize () + 
             m_ellipses.getSize () + 
             m_strings.getSize () +
	     m_polygons.getSize() 

	     );
}

void
CDrawingList::setColorMask( bool f_r_b, 
                            bool f_g_b, 
                            bool f_b_b, 
                            bool f_a_b )
{
    m_colorMask_p[0] = f_r_b;
    m_colorMask_p[1] = f_g_b;
    m_colorMask_p[2] = f_b_b;
    m_colorMask_p[3] = f_a_b;

}

bool
CDrawingList::write(  std::string          f_path_str,
                      const int            f_width_i,
                      const int            f_height_i,
                      const float          f_offsetX_f,
                      const float          f_offsetY_f,
                      const std::string    f_prefix_str)
{
    FILE* file_p = fopen(f_path_str.c_str(), "w");
    
    if (file_p == NULL) return false;
    
    fprintf(file_p,
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
            "<svg version=\"1.1\"\n"
            "     xmlns=\"http://www.w3.org/2000/svg\"\n"
            "     xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
            "     xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n"
            "     inkscape:version=\"0.45.1\"\n"
            "     width=\"%i\"\n"
            "     height=\"%i\">\n", f_width_i, f_height_i );

    
    bool resWrite_b;
    bool success_b = true;
    
    std::vector< CDrawingElementList * >::const_iterator 
        last = m_drawElems_v.end();

    for (std::vector< CDrawingElementList * >::const_iterator 
             i = m_drawElems_v.begin(); 
         i != last; ++i )
    {
        fprintf( file_p,
                 "    <g inkscape:groupmode=\"layer\"\n"
                 "       inkscape:label=\"%s\"\n"
                 "       id=\"Layer%s\" >\n",
                 (*i)->getGroupName().c_str(),
                 (*i)->getGroupName().c_str() );
        
        resWrite_b = (*i)->write( file_p,
                                  f_offsetX_f,
                                  f_offsetY_f,
                                  f_prefix_str );
        
        fprintf( file_p, "    </g>\n" );

        success_b &= resWrite_b;
    }
    
    // Finish file
    fprintf(file_p, "</svg>\n");
    fclose(file_p );

    return success_b;    
}

bool
CDrawingList::setScreenSize ( const S2D<unsigned int> f_size )
{
    if ( f_size  != m_screenSize )
        m_screenSize = f_size;

    return true;
}

S2D<unsigned int> 
CDrawingList::getScreenSize ( ) const
{
    return m_screenSize;
}

unsigned int
CDrawingList::getScreenWidth ( ) const
{
    return m_screenSize.width;
}

unsigned int
CDrawingList::getScreenHeight ( ) const
{
    return m_screenSize.height;
}
