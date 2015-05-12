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

#ifndef __DRAWINGLIST_H
#define __DRAWINGLIST_H

/**
 *******************************************************************************
 *
 * @file drawingList.h
 *
 * \class CDrawingList
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief This class maintains a list of primitives drawings and draw them on
 * a the current opengl context.
 *
 * The drawing list class provides a drawing tool for displaying or storing
 * drawings. Drawing primitives are rectangles, ellipses, lines, images, etc.
 *
 *******************************************************************************/

/* INCLUDES */
#include "displayImageList.h"
#include "colors.h"
#include "lineList.h"
#include "rectList.h"
#include "ellipseList.h"
#include "triangleList.h"
#include "polygonList.h"
#include "displayCEImageList.h"
#include "textList.h"

#include "colorEncoding.h"

#include "standardTypes.h"

#include <string>

/* CONSTANTS */
#define MAX_OVERLAY_LEVELS 3

/* DATA TYPES */


/* PROTOTYPES */


namespace QCV
{
    class CDrawingList
    {
    public:
   /// Construction, Destruction
    public:
        CDrawingList( std::string f_name_str = "Unnamed Drawing List" );
        virtual ~CDrawingList();
    
        /// Get the name of the drawing list.
        virtual std::string getName ( ) const { return m_name_str; }

        /// Add line.
        virtual bool        addDrawingList ( const CDrawingList &f_other );

        ////// Add primitive drawings.
        /// Add image.
        virtual bool        addImage ( cv::Mat            f_img,
                                       const float        f_u_f        = 0.f,
                                       const float        f_v_f        = 0.f,
                                       float              f_width_f    = -1.f,
                                       float              f_height_f   = -1.f,
                                       const float        f_scale_f    = 1.f,
                                       const float        f_offset_f   = 0.f,
                                       const float        f_alpha_f    = 1.f,
                                       const bool         f_makeCopy_b = false);
        
        /// Add image.
        virtual bool        addColorEncImage ( cv::Mat *            f_img_p,
                                               const CColorEncoding f_encoder,
                                               const float          f_u_f        = 0.,
                                               const float          f_v_f        = 0.,
                                               float                f_width_f    = -1,
                                               float                f_height_f   = -1,
                                               const float          f_alpha_f    = 1.f,
                                               const bool           f_makeCopy_b = false);
        
        /// Add line.
        virtual bool        addLine ( const float f_u1_f, const float f_v1_f,
                                      const float f_u2_f, const float f_v2_f );
 
        /// Add line.
        virtual bool        addLine ( S2D<float> f_point1, 
                                      S2D<float> f_point2 );

        /// Add clipped line.
        virtual bool        addClippedLine ( float f_u1_f,  float f_v1_f,
                                             float f_u2_f,  float f_v2_f,
                                             const float f_cu1_f, const float f_cv1_f,
                                             const float f_cu2_f, const float f_cv2_f );
 
        /// Add clipped line.
        virtual bool        addClippedLine ( S2D<float> f_point1, 
                                             S2D<float> f_point2,
                                             const S2D<float> f_cp1, 
                                             const S2D<float> f_cp2 );

        /// Add rectangle.
        virtual bool        addRectangle ( const float f_u1_f, const float f_v1_f,
                                           const float f_u2_f, const float f_v2_f );

        /// Add rectangle.
        virtual bool        addRectangle ( const S2D<float> f_tl, 
                                           const S2D<float> f_br );

        /// Add filled rectangle.
        virtual bool        addFilledRectangle ( const float f_u1_f, const float f_v1_f,
                                                 const float f_u2_f, const float f_v2_f );
        
        /// Add filled rectangle.
        virtual bool        addFilledRectangle ( const S2D<float> f_tl, 
                                                 const S2D<float> f_bt );

        /// Add square.
        virtual bool        addSquare ( const float f_u_f, const float f_v_f,
                                        const float f_size__f );
        
        /// Add square.
        virtual bool        addSquare ( const S2D<float>  f_center, 
                                        const float       f_size_f );
        
        /// Add filled square.
        virtual bool        addFilledSquare ( const float f_u_f, const float f_v_f,
                                              const float f_size_f );
        
        /// Add filled square.
        virtual bool        addFilledSquare ( const S2D<float>  f_center, 
                                              const float       f_size );

        /// Add line.
        virtual bool        addTriangle ( const float f_u1_f, const float f_v1_f,
                                          const float f_u2_f, const float f_v2_f,
                                          const float f_u3_f, const float f_v3_f );

        /// Add triangle.
        virtual bool        addTriangle ( const S2D<float> f_vertex1, 
                                          const S2D<float> f_vertex2, 
                                          const S2D<float> f_vertex3 );

        /// Add filled triangle.
        virtual bool        addFilledTriangle ( const float f_u1_f, const float f_v1_f,
                                                const float f_u2_f, const float f_v2_f,
                                                const float f_u3_f, const float f_v3_f );

        /// Add filled triangle.
        virtual bool        addFilledTriangle ( const S2D<float> f_vertex1, 
                                                const S2D<float> f_vertex2, 
                                                const S2D<float> f_vertex3 );

        /// Add circle.
        virtual bool        addCircle ( const float f_u_f, const float f_v_f,
                                        const float f_radius_f );

        /// Add circle.
        virtual bool        addCircle ( const S2D<float> f_center,
                                        const float f_radius_f );
        
        /// Add filled circle.
        virtual bool        addFilledCircle ( const float f_u_f, const float f_v_f,
                                              const float f_radius_f );

        /// Add circle.
        virtual bool        addFilledCircle ( const S2D<float> f_center,
                                              const float f_radius_f );

        /// Add ellipse.
        virtual bool        addEllipse ( const float f_u_f,    const float f_v_f,
                                         const float f_radU_f, const float f_radV_f,
                                         const float f_rotation_f = 0.f );

        /// Add ellipse.
        virtual bool        addEllipse ( const S2D<float> f_center,
                                         const S2D<float> f_radius,
                                         const float f_rotation_f = 0.f );

        /// Add filled ellipse.
        virtual bool        addFilledEllipse ( const float f_u_f,    const float f_v_f,
                                               const float f_radU_f, const float f_radV_f,
                                               const float f_rotation_f = 0.f );

        /// Add ellipse.
        virtual bool        addFilledEllipse ( const S2D<float> f_center,
                                               const S2D<float> f_radius,
                                               const float f_rotation_f = 0.f );

        /// Add polygon
        virtual bool        addPolygon ( const std::vector< S2D<float> > &vertex_v );

        /// Add filled polygon
        virtual bool        addFilledPolygon ( const std::vector< S2D<float> > &vertex_v );

        /// Add Quadrilateral.
        virtual bool        addQuadrilateral ( const S2D<float> f_v0, 
                                               const S2D<float> f_v1,
                                               const S2D<float> f_v2,
                                               const S2D<float> f_v3 );
        
        /// Add filled Quadrilateral.
        virtual bool        addFilledQuadrilateral ( const S2D<float> f_v0, 
                                                     const S2D<float> f_v1,
                                                     const S2D<float> f_v2,
                                                     const S2D<float> f_v3 );

        /// Add Pentagon.
        virtual bool        addPentagon ( const S2D<float> f_v0, 
                                          const S2D<float> f_v1,
                                          const S2D<float> f_v2,
                                          const S2D<float> f_v3,
                                          const S2D<float> f_v4 );

        /// Add filled Pentagon.
        virtual bool        addFilledPentagon ( const S2D<float> f_v0, 
                                                const S2D<float> f_v1,
                                                const S2D<float> f_v2,
                                                const S2D<float> f_v3,
                                                const S2D<float> f_v4 );
        
        /// Add Hexagon.
        virtual bool        addHexagon ( const S2D<float> f_v0, 
                                         const S2D<float> f_v1,
                                         const S2D<float> f_v2,
                                         const S2D<float> f_v3,
                                         const S2D<float> f_v4,
                                         const S2D<float> f_v5 );

        /// Add Filled Hexagon.
        virtual bool        addFilledHexagon ( const S2D<float> f_v0, 
                                               const S2D<float> f_v1,
                                               const S2D<float> f_v2,
                                               const S2D<float> f_v3,
                                               const S2D<float> f_v4,
                                               const S2D<float> f_v5 );

        /// Add cross.
        virtual bool        addCross ( const float f_u1_f, const float f_v1_f,
                                       const float f_u2_f, const float f_v2_f );

        /// Add cross.
        virtual bool        addCross ( const float f_u1_f, const float f_v1_f,
                                       const float f_radius_f );

        /// Add cross.
        virtual bool        addCross ( const S2D<float> f_tl, 
                                       const S2D<float> f_br );

        /// Add cross.
        virtual bool        addText ( std::string f_text,
                                      float       f_u_f, 
                                      float       f_v_f,
                                      float       f_fontSize_f,
                                      bool        f_fixSize_b = false );
        
        
    ////// Set/Get drawing state.
    public:
        
        /// Set/Get line color.
        virtual bool          setLineColor ( SRgb  f_color );

        /// Set/Get line color.
        virtual bool          setLineColor ( int f_r_i,
                                             int f_g_i,
                                             int f_b_i,
                                             int f_a_i = -1);
        
        /// Set/Get line color with opacity.
        virtual bool          setLineColor ( SRgba f_color );
        virtual SRgba         getLineColor ( ) const { return m_lineColor; }        

        /// Set/Get fill color.
        virtual bool          setFillColor ( SRgb  f_color );

        /// Set/Get fill color.
        virtual bool          setFillColor (  int f_r_i,
                                              int f_g_i,
                                              int f_b_i,
                                              int f_a_i = -1);
        
        /// Set/Get fill color with opacity.
        virtual bool          setFillColor ( SRgba f_color );
        virtual SRgba         getFillColor ( ) const { return m_fillColor; }

        /// Set/Get line width.
        virtual bool          setLineWidth ( float f_width_f );
        virtual float         getLineWidth ( ) const { return m_lineWidth_f; }
        
        /// Set/Get display state.
        virtual bool          setDisplayState ( SDisplayState f_dState );
        virtual SDisplayState getDisplayState ( ) const;

        /// Set/Get fill color with opacity.
        virtual bool          setOverlayLevel ( int f_level_i );
        virtual int           getOverlayLevel ( ) const { return m_overlayLevel_i; }

       /// Set/Get position.
        virtual bool          setPosition ( S2D<int> f_newPos );
        virtual S2D<int>      getPosition ( ) const { return m_position; }

        /// Set/Get visibility.
        virtual void          setVisibility ( bool f_state_b ){ m_visible_b = f_state_b; }
        virtual bool          isVisible ( ) const { return m_visible_b; }

        /// Get the number of elements in the drawing list.
        virtual int           getElementsCount() const;

        /// Sets the color mask to apply.
        virtual void          setColorMask( bool f_r_b, bool f_g_b, bool f_b_b, bool f_alpha_b );


        /// Scale
        virtual bool          setScaleX( double f_scale_d ) { m_scaleX_d = f_scale_d; return true;};
        virtual double        getScaleX( ) const { return m_scaleX_d; }
        virtual bool          setScaleY( double f_scale_d ) { m_scaleY_d = f_scale_d; return true;}
        virtual double        getScaleY( ) const { return m_scaleY_d; }        
        virtual bool          setScale( double f_scale_d) { m_scaleX_d = m_scaleY_d = f_scale_d; return true;}
        virtual bool          setScale( double f_scaleX_d, 
                                        double f_scaleY_d ) { m_scaleX_d = f_scaleX_d; m_scaleY_d = f_scaleY_d; return true;}

        virtual bool          setOffsetX( double f_offset_d ) { m_offsetX_d = f_offset_d; return true; }
        virtual double        getOffsetX( ) const { return m_offsetX_d; }
        virtual bool          setOffsetY( double f_offset_d ) { m_offsetY_d = f_offset_d; return true; }
        virtual double        getOffsetY( ) const { return m_offsetY_d; }        
        virtual bool          setOffset( double f_offset_d) { m_offsetX_d = m_offsetY_d = f_offset_d; return true;}
        virtual bool          setOffset( double f_offsetX_d, 
                                         double f_offsetY_d ) { m_offsetX_d = f_offsetX_d; m_offsetY_d = f_offsetY_d; return true;}

        virtual bool          setRotation( double f_angle_d ) { m_rotAngle_d = f_angle_d; return true; }
        virtual double        getRotation( ) const { return m_rotAngle_d; }

        virtual bool          setScreenSize ( const S2D<unsigned int> f_size );

        virtual S2D<unsigned int>   
                              getScreenSize (  ) const;

        virtual unsigned int getScreenWidth ( )  const;
        virtual unsigned int getScreenHeight ( ) const;

    //// Actions
    public:
        /// Show all.
        virtual bool          show ();        
        
        /// Show all.
        virtual bool          clear ();

    public:

        /// Write in file.
        virtual bool          write ( std::string          f_path_str,
                                      const int            f_width_i,
                                      const int            f_height_i,
                                      const float          f_offsetX_f = 0.0,
                                      const float          f_offsetY_f = 0.0,
                                      const std::string    f_prefix_str = "");
    private:

        /// Name of this list.
        std::string                m_name_str;
        
        /// Line width.
        S2D<int>                   m_position;

        /// Line width.
        bool                       m_visible_b;

        /// Current line color.
        SRgba                      m_lineColor;

        /// Current fill color.
        SRgba                      m_fillColor;

        /// Line width.
        float                      m_lineWidth_f;

        /// Overlay leve.
        int                        m_overlayLevel_i;

        /// Screen Size
        S2D<unsigned int>          m_screenSize;
    /// Drawing elements
    private:

        /// List of lines.
        std::vector<CDrawingElementList *>  m_drawElems_v;

        /// List of lines.
        CLineList                           m_lines;

        /// List of rectangles.
        CRectangleList                      m_rectangles;

        /// List of ellipses.
        CEllipseList                        m_ellipses;

        /// List of triangles.
        CTriangleList                       m_triangles;

        /// List of color encoded images.
        CPolygonList                        m_polygons;

        /// List of images.
        CDisplayImageList                   m_images;

        /// List of color encoded images.
        CDisplayColorEncImageList           m_colorEncImages;

        /// List of color encoded images.
        CTextList                           m_strings;

        /// Color mask
        bool                                m_colorMask_p[4];

        /// Scale X 
        double                              m_scaleX_d;

        /// Scale Y
        double                              m_scaleY_d;

        /// Offset X
        double                              m_offsetX_d;

        /// Offset Y
        double                              m_offsetY_d;

        /// Offset Y
        double                              m_rotAngle_d;
        
    };

    inline bool 
    CDrawingList::setLineColor ( int f_r_i,
                                 int f_g_i,
                                 int f_b_i,
                                 int f_a_i  ) 
    { 
        if ( f_a_i < 0 )
            return setLineColor ( SRgb ( f_r_i, f_g_i, f_b_i ) );
        else
            return setLineColor ( SRgba ( f_r_i, f_g_i, f_b_i, f_a_i ) );
    }

    inline bool 
    CDrawingList::setLineColor ( SRgb  f_color ) 
    { 
        m_lineColor = f_color; 
        return true; 
    }
    
    /// Set line color with opacity.
    inline bool 
    CDrawingList::setLineColor ( SRgba f_color )
    { 
        m_lineColor = f_color;
        return true;
    }
    
    /// Set fill color.
    inline bool 
    CDrawingList::setFillColor ( int f_r_i,
                                 int f_g_i,
                                 int f_b_i,
                                 int f_a_i  ) 
    { 
        if ( f_a_i < 0 )
            return setFillColor ( SRgb ( f_r_i, f_g_i, f_b_i ) );
        else
            return setFillColor ( SRgba ( f_r_i, f_g_i, f_b_i, f_a_i ) );
    }

    /// Set fill color.
    inline bool 
    CDrawingList::setFillColor ( SRgb  f_color )
    { 
        m_fillColor = f_color; 
        return true; 
    }
    
    /// Set fill color with opacity.
    inline bool 
    CDrawingList::setFillColor ( SRgba f_color ) 
    { 
        m_fillColor = f_color;
        return true;
    }

    /// Set line width.
    inline bool 
    CDrawingList::setLineWidth ( float f_width_f )  
    { 
        m_lineWidth_f = f_width_f;
        return true;
    }

    /// Set new position.
    inline bool 
    CDrawingList::setPosition ( S2D<int> f_newPos )
    {
        m_position = f_newPos;
        return true;
    }
    
    /// Set new display state.
    inline bool 
    CDrawingList::setDisplayState ( SDisplayState f_dState )
    {
        m_position  = f_dState.position;
        m_visible_b = f_dState.visible_b;
        return true;
    }

    /// Get new display state.
    inline SDisplayState 
    CDrawingList::getDisplayState ( ) const
    {
        return SDisplayState (m_position, m_visible_b);
    }
    

    inline bool
    CDrawingList::setOverlayLevel ( int f_level_i )
    {
        m_overlayLevel_i = std::min(std::max(f_level_i, 0), MAX_OVERLAY_LEVELS-1);
        return true;
    }
    
} // Namespace QCV

#endif // __DRAWINGLIST_H
