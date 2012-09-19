/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 3. See the GNU LGPL version 3 for details.
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
* @file imgScaler.cpp
*
* \class CImageScalerOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "imgScaler.h"

#include "paramMacros.h"
#include "drawingList.h"
#include "paramIOFile.h"
#include "ceParameter.h"

using namespace QCV;

/// Constructors.
CImageScalerOp::CImageScalerOp ( COperatorBase * const f_parent_p,
                                 const std::string f_name_str)
    : COperator<cv::Mat, cv::Mat>
      (                       f_parent_p, f_name_str ),
      m_compute_b (                            false ),
      m_scaleMode_e (                      SM_FACTOR ),
      m_scaleFactor (                     0.5f, 0.5f ),
      m_scaleSize (                         320, 240 )
{
    registerDrawingLists();
    registerParameters();
}

void
CImageScalerOp::registerDrawingLists()
{
    registerDrawingList ("Input Image",
                         S2D<int> (0, 0),
                         false);
    registerDrawingList ("Scaled Image",
                         S2D<int> (1, 0),
                         false);
}

void
CImageScalerOp::registerParameters()
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

    END_PARAMETER_GROUP;

    BEGIN_PARAMETER_GROUP("Display", false, SRgb(220,0,0));

      addDrawingListParameter ( "Input Image" );
      addDrawingListParameter ( "Scaled Image" );

    END_PARAMETER_GROUP;


}

/// Virtual destructor.
CImageScalerOp::~CImageScalerOp ()
{
}

/// Cycle event.
bool
CImageScalerOp::cycle()
{
    if ( m_compute_b &&
         m_img.size().width  > 0 && 
         m_img.size().height > 0 )
    {
        cv::Size size;
        
        if ( m_scaleMode_e == SM_FACTOR )
        {
            size = m_img.size();
            size.width  *= m_scaleFactor.x;
            size.height *= m_scaleFactor.y;
        }
        else
        {
            size = m_scaleSize;
        }
        
        if (size == m_img.size())
            m_scaledImg = m_img;
        else
            cv::resize(m_img, m_scaledImg, size);
    }
    else
        m_scaledImg = cv::Mat(0,0, CV_8UC1);

    return COperatorBase::cycle();
}

/// Show event.
bool CImageScalerOp::show()
{
    CDrawingList *list_p  = getDrawingList ( "Input Image");

    list_p -> clear();    
    if ( list_p -> isVisible() ) // No preview but faster.
        list_p->addImage ( m_img );

    list_p = getDrawingList ( "Scaled Image");
    list_p -> clear();    
    if ( list_p -> isVisible() ) // No preview but faster.
        list_p->addImage ( m_scaledImg );

    return COperatorBase::show();
}

/// Init event.
bool CImageScalerOp::initialize()
{
    return COperatorBase::initialize();
}

/// Reset event.
bool CImageScalerOp::reset()
{
    return COperatorBase::reset();
}

bool CImageScalerOp::exit()
{
    return COperatorBase::exit();
}

/// Set the input of this operator
bool
CImageScalerOp::setInput  ( const cv::Mat & f_input )
{
    m_img = f_input;
    return true;
}

/// Gets the output of this operator
bool
CImageScalerOp::getOutput ( cv::Mat & f_output ) const
{
    if ( m_scaledImg.cols == 0 ||
         m_scaledImg.rows == 0 )
        return false;

    f_output = m_scaledImg;

    return true;
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


 
