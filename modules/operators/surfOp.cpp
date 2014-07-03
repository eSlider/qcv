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
* @file surfOp.cpp
*
* \class CSurfOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "surfOp.h"

#include "paramMacros.h"
#include "drawingList.h"
#include "ceParameter.h"

using namespace QCV;

/// Constructors.
CSurfOp::CSurfOp ( COperator * const f_parent_p,
                   const std::string f_name_str )
    : COperator (             f_parent_p, f_name_str ),
      m_inpImageId_str (                   "Image 0" ),
      m_compute_b (                             true ),
      m_useProvidedKeypoints_b (               false ),
      m_img (                                        ),
      m_imgAux (                                     ),
      m_keyPoints (                                  ),
      m_descriptors (                                ),
      m_respCE (   CColorEncoding::CET_BLUE2GREEN2RED,
                           S2D<float> ( 3000, 5000 ) ),
      m_computeDescriptors_b (                  true )
{
    registerDrawingLists(  );
    registerParameters (  );
}

void
CSurfOp::registerDrawingLists(  )
{
    bool showImages_b = (getParentOp() == NULL);
    
    registerDrawingList ( "Input Image",
                          S2D<int> (0, 0),
                          showImages_b);
    
    registerDrawingList ( "Keypoints",
                          S2D<int> (0, 0),
                          showImages_b);    
}

void
CSurfOp::registerParameters( )
{

    BEGIN_PARAMETER_GROUP("Computation", false, SRgb(220,0,0));
      ADD_BOOL_PARAMETER ( "Compute",
                           "Compute SURF features on the input image?.",
                           m_compute_b,
                           this,
                           Compute,
                           CSurfOp );

      ADD_BOOL_PARAMETER ( "Use input keypoints",
                           "Use the input keypoitns instead of detecting them\n"
                           "from the image?.",
                           m_useProvidedKeypoints_b,
                           this,
                           UseProvidedKeypoints,
                           CSurfOp );

      ADD_BOOL_PARAMETER ( "Compute Descriptors",
                           "Compute descriptors for the detected keypoints.\n"
                           "Keypoints provided by the user will be ignored\n"
                           "if this parameters is false.",
                           m_computeDescriptors_b,
                           this,
                           ComputeDescriptors,
                           CSurfOp );

      ADD_BOOL_PARAMETER ( "Extended",
                           "Compute a descriptor of 128 elements instead of 64.",
                           false,
                           &m_surfOp,
                           Extended,
                           CCvSurf );

#if CV_MAJOR_VERSION > 2 || ( CV_MAJOR_VERSION == 2 && CV_MINOR_VERSION >= 2 )
      ADD_BOOL_PARAMETER ( "Upright",
                           "Upright version of SURF (no orientation).",
                           false,
                           &m_surfOp,
                           Upright,
                           CCvSurf );
#endif
      ADD_DOUBLE_PARAMETER ( "Hessian Threshold",
                             "Hessian Threshold for detection of keypoints.",
                             3000,
                             &m_surfOp,
                             HessianThreshold,
                             CCvSurf );

      ADD_INT_PARAMETER ( "Octaves",
                          "Number of octaves (4 is default).",
                          4,
                          &m_surfOp,
                          Octaves,
                          CCvSurf );

      ADD_INT_PARAMETER ( "Octave Layers",
                          "Number of octave layers (2 is default).",
                          2,
                          &m_surfOp,
                          OctaveLayers,
                          CCvSurf );


    END_PARAMETER_GROUP;

    BEGIN_PARAMETER_GROUP("Display", false, SRgb(220,0,0));

      addDrawingListParameter ( "Input Image" );

      addDrawingListParameter ( "Keypoints" );

      addColorEncodingParameter (  m_respCE,
                                   getParameterSet(),
                                   "Response",
                                   "Color encoding for the response of the filter." );

    END_PARAMETER_GROUP;
}

/// Virtual destructor.
CSurfOp::~CSurfOp ()
{
}

/// Cycle event.
bool
CSurfOp::cycle()
{
    if ( m_compute_b )
    {
        m_img = getInput<cv::Mat>(m_inpImageId_str, cv::Mat());

        m_keyPoints.clear();
        m_descriptors.clear();
        
        if ( m_img.size().width > 0)
        {            
            if ( m_img.type() == CV_8UC3 )
            {            
                m_descriptors.clear();

                IplImage src = (IplImage)m_img;
                
                if (m_imgAux.size() != m_img.size())
                    m_imgAux = cv::Mat( m_img.size(), CV_8UC1);
                
                IplImage dst = (IplImage)m_imgAux;
                
                cvCvtColor ( &src, &dst, CV_RGB2GRAY);
                
                if (m_computeDescriptors_b)
                    m_surfOp(m_imgAux, cv::Mat(), m_keyPoints, m_descriptors, m_useProvidedKeypoints_b );
                else
                    m_surfOp(m_imgAux, cv::Mat(), m_keyPoints );   
            }
            else
            {
                if (m_computeDescriptors_b)
                    m_surfOp(m_img, cv::Mat(), m_keyPoints, m_descriptors, m_useProvidedKeypoints_b );
                else
                    m_surfOp(m_img, cv::Mat(), m_keyPoints );
            }
            
            registerOutput< std::vector<cv::KeyPoint> >("Keypoints",   &m_keyPoints );
            registerOutput< std::vector<float> >       ("Descriptors", &m_descriptors );
        }
        else
            printf("%s:%i Invalid input image %s\n", __FILE__, __LINE__, m_inpImageId_str.c_str() );
    }
    
    return COperator::cycle();
}

/// Show event.
bool CSurfOp::show()
{
    getDrawingList("Descriptor Overlay" )->clear();
    
    CDrawingList * list_p = getDrawingList("Input Image" );

    list_p -> clear();
    
    if ( list_p -> isVisible() && m_img.size().width > 0  )
    {
        list_p->addImage ( m_img );
    }
     
    list_p = getDrawingList("Keypoints" );
    list_p->clear();
    
    if ( list_p->isVisible() && m_keyPoints.size())
    {
        for (unsigned int i = 0; i < m_keyPoints.size(); ++i)
        {
            SRgb color;
            
            m_respCE.colorFromValue ( m_keyPoints[i].response,
                                      color );

            list_p->setLineWidth ( 1. );
            list_p->setLineColor ( color );
            list_p->setFillColor ( SRgba(color, 30 ) );
            
            list_p->addFilledCircle ( m_keyPoints[i].pt.x, 
                                      m_keyPoints[i].pt.y, 
                                      m_keyPoints[i].size/2 );

            cv::Vec2d p( m_keyPoints[i].size/2,
                         0 );
            
            if (m_keyPoints[i].angle != -1) /// -1 means not applicable
            {
                list_p->setLineWidth ( 2. );
                // Show orientation:
                double sin_d = sin(m_keyPoints[i].angle);
                double cos_d = cos(m_keyPoints[i].angle);
                
                cv::Vec2d q(p[0] * cos_d - p[1] * sin_d,
                            p[0] * sin_d + p[1] * cos_d );
                
                list_p->addLine ( m_keyPoints[i].pt.x, 
                                  m_keyPoints[i].pt.y, 
                                  m_keyPoints[i].pt.x+q[0],
                                  m_keyPoints[i].pt.y+q[1] );
            }
        }        
    }    

    return COperator::show();
}

/// Init event.
bool CSurfOp::initialize()
{
    if ( getInput<cv::Mat>("Image 0") )
        m_img = *getInput<cv::Mat>("Image 0");

   /// Set the screen size if this is the parent operator.
    if ( m_img.size().width > 0 &&
         !getParentOp() )
    {
        setScreenSize ( m_img.size() );
    }

    return COperator::initialize();
}

/// Reset event.
bool CSurfOp::reset()
{
    return COperator::reset();
}

bool CSurfOp::exit()
{
    return COperator::exit();
}

void 
CSurfOp::mouseMoved (     CMouseEvent * f_event_p )
{
    CDrawingList *list1_p = getDrawingList("Keypoints" );
    CDrawingList *list2_p = getDrawingList("Descriptor Overlay" );

    list2_p -> setPosition ( f_event_p -> displayScreen );
    list2_p -> clear();    
    
    if ( m_descriptors.size() ||
         f_event_p -> displayScreen != list1_p->getPosition() ||
         !list1_p->isVisible() )
        return;


    float minSqDist_f = 10000.f*10000.f;
    int   idx_i = -1;
    
    for (unsigned int i = 0; i < m_keyPoints.size(); ++i)
    {
        float dx_f = (m_keyPoints[i].pt.x - f_event_p->posInScreen.x);
        float dy_f = (m_keyPoints[i].pt.y - f_event_p->posInScreen.y);
        float sqDist_f = dx_f * dx_f + dy_f * dy_f;        

        if ( sqDist_f < minSqDist_f )
        {
            idx_i = i;
            minSqDist_f = sqDist_f;
        }        
    }

    /// If distance to keypoint is smaller than 6 pixels.
    if ( minSqDist_f < 36 )
    {
        SRgb color;
        
        /// Enhance keypoint circle
        list2_p -> setVisibility ( true );
        m_respCE.colorFromValue ( m_keyPoints[idx_i].response,
                                  color );
        
        list2_p->setLineWidth ( 3. );
        list2_p->setLineColor ( color );
        list2_p->setFillColor ( SRgba(color, 30 ) );
            
        list2_p->addFilledCircle ( m_keyPoints[idx_i].pt.x, 
                                  m_keyPoints[idx_i].pt.y, 
                                  m_keyPoints[idx_i].size/2 );
        
        /// Build image with descriptor and show it.
        int descSize_i = m_surfOp.descriptorSize();
        int width_i = descSize_i/8;
        
        cv::Mat descrImg ( 8, width_i, CV_32FC1, &m_descriptors[idx_i*descSize_i] );
        
        int dispSize_i = m_img.size().width / 10;

        cv::Rect rect( m_keyPoints[idx_i].pt.x + m_keyPoints[idx_i].size/2 + 5,
                       m_keyPoints[idx_i].pt.y - dispSize_i / 2.f,
                       dispSize_i,   // width
                       dispSize_i ); // height

        list2_p->addImage ( descrImg,
                            rect.x, rect.y, rect.width, rect.height,
                            descSize_i ); // scale
        
        list2_p->addRectangle ( rect.tl(), rect.br() );
    }

    /// Let's tell QCV to update the screen
    updateDisplay();
}

