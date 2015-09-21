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

/**
*******************************************************************************
*
* @file imgScalerOp.cpp
*
* \class CImageScalerOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "imgScalerOp.h"

#include "paramMacros.h"
#include "drawingList.h"
#include "ceParameter.h"

using namespace QCV;

/// Constructors.
CImageScalerOp::CImageScalerOp ( COperator * const f_parent_p,
                                 const std::string f_name_str,
                                 const int f_preferedNumImgs_i )
    : COperator (             f_parent_p, f_name_str ),
      m_inputId_str (                 "Input Images" ),
      m_outputId_str (               "Scaled Images" ),
      m_compute_b (                            false ),
      m_scaleMode_e (                      SM_FACTOR ),
      m_scaleFactor (                     0.5f, 0.5f ),
      m_scaleSize (                         320, 240 ),
      m_img_v (                                      ),
      m_scaledImgs_v (                               ),
      m_interpolMode_i (            cv::INTER_LINEAR )
{
    registerDrawingLists( f_preferedNumImgs_i );
    registerParameters ( f_preferedNumImgs_i );
}

void
CImageScalerOp::registerDrawingLists( int f_numReg_i )
{
    char str[256];
    
    for (int i = 0; i < f_numReg_i; ++i)
    {
        sprintf(str, "Input Image %i", i);        
        registerDrawingList ( str,
                              S2D<int> (0, 0),
                              i==0 && !getParentOp());

        sprintf(str, "Output Image %i", i);        
        registerDrawingList ( str,
                              S2D<int> (1, 0),
                              i==0 && !getParentOp());
    }
}

void
CImageScalerOp::registerParameters( int f_numReg_i )
{

    BEGIN_PARAMETER_GROUP("Computation", false, SRgb(220,0,0));
    ADD_BOOL_PARAMETER ( "Compute",
                         "Perform the scaling?.",
                         m_compute_b,
                         this,
                         Compute,
                         CImageScalerOp );
    

    CEnumParameter<EScaleMode> * algParam_p = static_cast<CEnumParameter<EScaleMode> * > (
        ADD_ENUM_PARAMETER( "Algorithm",
                            "ImageScaler algorithm to use",
                            EScaleMode,
                            m_scaleMode_e,
                            this,
                            ScaleMode,
                            CImageScalerOp ) );
      
    algParam_p -> addDescription ( SM_FACTOR, "Scale factor" );
    algParam_p -> addDescription ( SM_SIZE,   "Size" );    
      
    ADD_FLT2D_PARAMETER ( "Factors",
                          "Scale factors for scaling the image.",
                          m_scaleFactor,
                          "X", "Y",
                          this,
                          ScaleFactor,
                          CImageScalerOp );

    ADD_UINT2D_PARAMETER ( "Size",
                           "Fix size to scale the input image.",
                           m_scaleSize,
                           "width", "height",
                           this,
                           ScaleSize,
                           CImageScalerOp );

    CEnumParameter<int> * scaleMode_p = static_cast<CEnumParameter<int> * > (
        ADD_ENUM_PARAMETER( "Interpolation",
                            "Interpolation type to use.",
                            int,
                            m_interpolMode_i,
                            this,
                            InterpolationMode,
                            CImageScalerOp ) );
    
    scaleMode_p -> addDescription ( cv::INTER_NEAREST,  "Nearest Neighbor" );
    scaleMode_p -> addDescription ( cv::INTER_LINEAR,   "Bilinear interpolation" );
    scaleMode_p -> addDescription ( cv::INTER_AREA,     "Resampling using pixel area relation" );
    scaleMode_p -> addDescription ( cv::INTER_CUBIC,    "Bicubic interpolation" );
    scaleMode_p -> addDescription ( cv::INTER_LANCZOS4, "Lanczos" );
    

    END_PARAMETER_GROUP;

    BEGIN_PARAMETER_GROUP("Display", false, SRgb(220,0,0));

    for (int i = 0; i < f_numReg_i; ++i)
    {
        char str[256];
        sprintf(str, "Input Image %i", i);        
        addDrawingListParameter ( str );
        
        sprintf(str, "Output Image %i", i);        
        addDrawingListParameter ( str );
    }

    END_PARAMETER_GROUP;
}

/// Virtual destructor.
CImageScalerOp::~CImageScalerOp ()
{
}

/// Compute
bool
CImageScalerOp::compute ( const CMatVector & f_input_v, 
                          CMatVector       & fr_output_v )
{
    if (m_compute_b)
    {
        m_img_v = f_input_v;
        resize();
        fr_output_v = m_scaledImgs_v;
    }
    else
    {
        fr_output_v = f_input_v;
    }

    return true;
}

/// Cycle event.
bool
CImageScalerOp::cycle()
{
    if ( m_compute_b )
    {
        getInputs();
        resize();
    }
    else
        m_scaledImgs_v = m_img_v;
    
    return COperator::cycle();
}

/// Resize.
void
CImageScalerOp::resize()
{
    m_scaledImgs_v.resize(m_img_v.size());
        
    for ( unsigned int i = 0; i < m_img_v.size(); ++i )
    {
        if ( m_img_v[i].size().width  > 0 && 
             m_img_v[i].size().height > 0 )
        {
            cv::Size size;
                 
            if ( m_scaleMode_e == SM_FACTOR )
            {
                size = m_img_v[i].size();
                size.width  *= m_scaleFactor.x;
                size.height *= m_scaleFactor.y;
            }
            else
            {
                size = m_scaleSize;
            }
                
            if (size == m_img_v[i].size())
                m_scaledImgs_v[i] = m_img_v[i];
            else
            {
                cv::resize(m_img_v[i], m_scaledImgs_v[i], size, 0, 0, m_interpolMode_i);
            }
        }
        else
            m_scaledImgs_v[i] = cv::Mat(0, 0, CV_8UC1);
    }
    
    registerOutput<CMatVector> ( m_outputId_str, &m_scaledImgs_v );
}

/// Show event.
bool
CImageScalerOp::show()
{
    for ( unsigned int i = 0; i < m_scaledImgs_v.size(); ++i )
    {
        char name_str[256];
        if ( m_img_v[i].size().width  > 0 && 
             m_img_v[i].size().height > 0 )
        {
            sprintf(name_str, "Input Image %i", i );
            CDrawingList *list_p  = getDrawingList ( name_str );
            list_p -> clear();    
            if ( list_p -> isVisible() ) // No preview but faster.
                list_p->addImage ( m_img_v[i], 0, 0, getScreenSize().width, getScreenSize().height );
        }

        if ( m_scaledImgs_v[i].size().width  > 0 && 
             m_scaledImgs_v[i].size().height > 0 )
        {
            sprintf(name_str, "Output Image %i", i );
            CDrawingList *list_p  = getDrawingList ( name_str );
            list_p -> clear();    
            if ( list_p -> isVisible() ) // No preview but faster.
                list_p->addImage ( m_scaledImgs_v[i], 0, 0, getScreenSize().width, getScreenSize().height );
        }
    }

    return COperator::show();
}

bool 
CImageScalerOp::getInputs()
{
    m_img_v = getInput<CMatVector>( "Device Images", CMatVector());

    if (m_img_v.size() == 0)
    {   
        char str[256];
        for (int i = 0; ; ++i)
        {
            sprintf(str, "Image %i", i);
            cv::Mat img = getInput<cv::Mat>(str, cv::Mat() );
            if (img.size().width > 0)
                m_img_v.push_back(img);
            else
                break;
        }
    }

    return m_img_v.size() > 0;
}

/// Init event.
bool CImageScalerOp::initialize()
{
    getInputs();
    /// Set the screen size if this is the parent operator.
    if ( m_img_v.size() > 0 &&
         !getParentOp() )
    {
        setScreenSize ( m_img_v[0].size() );
        getDrawingList ( "Input Image 0" ) -> setVisibility(true);
    }

    return COperator::initialize();
}

/// Reset event.
bool CImageScalerOp::reset()
{
    return COperator::reset();
}

bool CImageScalerOp::exit()
{
    return COperator::exit();
}

bool
CImageScalerOp::setScaleFactor ( S2D<float> f_factors )
{
    if (f_factors == m_scaleFactor) return true;

    if ( f_factors.x <= 0 || f_factors.y <= 0 ||
         f_factors.x > 100 || f_factors.y > 100 )
        return false;
    
    m_scaleFactor = f_factors;

    return true;
}

S2D<float>
CImageScalerOp::getScaleFactor ( ) const
{
    return m_scaleFactor;
}

bool
CImageScalerOp::setScaleSize ( S2D<unsigned int> f_size )
{
    if (f_size == m_scaleSize) return true;

    if ( f_size.x <= 0 || f_size.y <= 0 )
        return false;

    m_scaleSize = f_size;

    return true;
}

S2D<unsigned int>
CImageScalerOp::getScaleSize ( ) const
{
    return m_scaleSize;
}


 
