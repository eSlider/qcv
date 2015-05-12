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
* @file sobelOp.cpp
*
* \class CSobelOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "sobelOp.h"

#include "paramMacros.h"
#include "drawingList.h"
//#include "ceParameter.h"

using namespace QCV;

/// Constructors.
CSobelOp::CSobelOp ( COperator * const f_parent_p,
                     const std::string f_name_str )
    : COperator (             f_parent_p, f_name_str ),
      m_compute_b (                             true ),
      m_ksize_i (                                  3 ),
      m_applyGauss_b (                         false ),
      m_img (                                        ),
      m_gradImgs_v (                                 )
{
    registerDrawingLists(  );
    registerParameters (  );
}

void
CSobelOp::registerDrawingLists(  )
{
    registerDrawingList ( "Input Image",
                          S2D<int> (0, 0),
                          !getParentOp());

    registerDrawingList ( "X Gradient",
                          S2D<int> (0, 1),
                          !getParentOp());

    registerDrawingList ( "Y Gradient",
                          S2D<int> (1, 1),
                          !getParentOp());
}

void
CSobelOp::registerParameters(  )
{

    BEGIN_PARAMETER_GROUP("Computation", false, SRgb(220,0,0));
    ADD_BOOL_PARAMETER ( "Compute",
                         "Perform the scaling?.",
                         m_compute_b,
                         this,
                         Compute,
                         CSobelOp );

    CEnumParameter<int> * algParam_p = static_cast<CEnumParameter<int> * > (
        ADD_ENUM_PARAMETER( "Filter Size",
                            "Sobel kernel filter size",
                            int,
                            m_ksize_i,
                            this,
                            KernelSize,
                            CSobelOp ) );
      
    algParam_p -> addDescription ( 1, "1x3" );
    algParam_p -> addDescription ( 3, "3x3" );    
    algParam_p -> addDescription ( 5, "5x5" );    
    algParam_p -> addDescription ( 7, "7x7" );    

    ADD_BOOL_PARAMETER ( "Apply Gauss",
                         "Apply a gauss 3x3 filter before computing gradients.",
                         m_applyGauss_b,
                         this,
                         ApplyGauss,
                         CSobelOp );

    END_PARAMETER_GROUP;

    BEGIN_PARAMETER_GROUP("Display", false, SRgb(220,0,0));

      addDrawingListParameter ( "Input Image" );
      addDrawingListParameter ( "X Gradient" );
      addDrawingListParameter ( "Y Gradient" );

    END_PARAMETER_GROUP;
}

/// Virtual destructor.
CSobelOp::~CSobelOp ()
{
}

/// Cycle event.
bool
CSobelOp::cycle()
{
    if ( m_compute_b )
    {
        m_img = getInput<cv::Mat>("Image 0", cv::Mat());
        
        if ( m_img.size().width > 0 )
        {
            m_gradImgs_v.resize(2);
            
            /// Gauss filter.
            if ( m_applyGauss_b )
                cv::GaussianBlur( m_img, m_gaussImg, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
            
            const cv::Mat &srcImg = m_applyGauss_b?m_gaussImg:m_img;
            
            /// Sobel Vert
            cv::Sobel( srcImg, m_gradImgs_v[ID_GRADX],
                       CV_16S, 1, 0, m_ksize_i, 1, 0, cv::BORDER_DEFAULT );
            
            cv::Sobel( srcImg, m_gradImgs_v[ID_GRADY],
                       CV_16S, 0, 1, m_ksize_i, 1, 0, cv::BORDER_DEFAULT );

            registerOutput<CMatVector>( "Image Gradients", &m_gradImgs_v );
        }
    }
    
    return COperator::cycle();
}
    
/// Show event.
bool CSobelOp::show()
{
    if (m_compute_b)
    {
        /// Set the screen size if this is the parent operator.
        setScreenSize ( m_img.size() );

        CDrawingList * list_p = getDrawingList("Input Image" );
        
        list_p -> clear();
        
        if ( list_p -> isVisible() && m_img.size().width > 0  )
        {
            list_p->addImage ( m_img );
        }
        
        cv::Size size = m_img.size();
        
        float offset_f = 0.5;
        float scale_f  = 16.f;
        
        list_p = getDrawingList("X Gradient" );
        list_p -> clear();
        
        if ( list_p -> isVisible() && m_gradImgs_v[ID_GRADX].size().width > 0 )
        {
            list_p->addImage ( m_gradImgs_v[ID_GRADX], 0, 0, size.width, size.height, scale_f, offset_f  );
        }
        
        list_p = getDrawingList("Y Gradient" );
        list_p -> clear();
        
        if ( list_p -> isVisible() && m_gradImgs_v[ID_GRADY].size().width > 0 )
        {
            list_p->addImage ( m_gradImgs_v[ID_GRADY], 0, 0, size.width, size.height, scale_f, offset_f  );
        }
    }

    return COperator::show();
}

/// Init event.
bool CSobelOp::initialize()
{
    m_img = getInput<cv::Mat>("Image 0", cv::Mat());

    setScreenSize ( m_img.size() );

    m_gradImgs_v.resize(2);

    return COperator::initialize();
}

/// Reset event.
bool CSobelOp::reset()
{
    return COperator::reset();
}

bool CSobelOp::exit()
{
    return COperator::exit();
}


