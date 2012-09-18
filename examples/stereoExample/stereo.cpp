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
* @file featureStereoOp.cpp
*
* \class CStereoOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "stereo.h"
#include "paramMacros.h"
#include "drawingList.h"
#include "paramIOFile.h"
#include "ceParameter.h"

using namespace QCV;

/// Constructors.
CStereoOp::CStereoOp ( COperatorBase * const f_parent_p )
    : COperator<TInpImgFromFileVector, TOutputType>
      (               f_parent_p, "OpenCV Stereo" ),
      m_sgbm (                                    ),
      m_sbm (                                     ),
      m_leftImg (                                 ),
      m_rightImg (                                ),
      m_dispImg (                                 )
{
    registerDrawingLists();
    registerParameters();

    CParamIOFile pio ( "parameters.xml" );
    getParameterSet()->load ( pio );
}


void
CStereoOp::registerDrawingLists()
{
    registerDrawingList ("Left Image",
                         S2D<int> (0, 0),
                         true);
    registerDrawingList ("Right Image",
                         S2D<int> (1, 0),
                         true);
    registerDrawingList ("Disparity Image",
                         S2D<int> (2, 0),
                         true);
}

void
CStereoOp::registerParameters()
{
    CEnumParameter<EStereoAlgorithm> * algParam_p = static_cast<CEnumParameter<EStereoAlgorithm> * > (
        ADD_ENUM_PARAMETER( "Algorithm",
                            "Stereo algorithm to use",
                            EStereoAlgorithm,
                            SA_SGBM,
                            this,
                            StereoAlgorithm,
                            CStereoOp ) );
    
    algParam_p -> addDescription ( SA_SGBM, "Semi global block matching (SGBM)" );
    algParam_p -> addDescription ( SA_BM,   "Block matching (BM)" );
    

    BEGIN_PARAMETER_GROUP("SGBM", false, SRgb(220,0,0));

      ADD_INT_PARAMETER ( "Number Of Disparities",
                          "This is maximum disparity minus minimum disparity. Always greater than 0.",
                          64,
                          &m_sgbm,
                          NumberOfDisparities,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Window Size",
                          "The matched block size. Must be an odd number >=1 . Normally, it should be\n"
                          "somewhere in 3..11 range.",
                          15,
                          &m_sgbm,
                          SADWindowSize,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Uniqueness Ratio",
                          "The margin in percents by which the best (minimum) computed cost function\n"
                          "value should \"win\" the second best value to consider the found match\n"
                          "correct. Normally, some value within 5-15 range is good enough",
                          1,
                          &m_sgbm,
                          UniquenessRatio,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Disp LR Max Diff",
                          "Maximum allowed difference (in integer pixel units) in the left-right\n"
                          "disparity check. Set it to non-positive value to disable the check.",
                          1,
                          &m_sgbm,
                          Disp12MaxDiff,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "P1",
                          "Parameters that control disparity smoothness. Cost parameter P1 of SGM",
                          10,
                          &m_sgbm,
                          P1,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "P2",
                          "Parameters that control disparity smoothness. Cost parameter P2 of SGM",
                          200,
                          &m_sgbm,
                          P2,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Pre Filter Cap",
                          "Truncation value for the prefiltered image pixels. The algorithm first\n"
                          "computes x-derivative at each pixel and clips its value by [-preFilterCap,\n"
                          "preFilterCap] interval. The result values are passed to the Birchfield-Tomasi\n"
                          "pixel cost function.",
                          0,
                          &m_sgbm,
                          PreFilterCap,
                          CMyStereoSGBM );      

      ADD_INT_PARAMETER ( "Specke Window Size",
                          "Maximum size of smooth disparity regions to consider them noise speckles and\n"
                          "invdalidate. Set it to 0 to disable speckle filtering. Otherwise, set it\n"
                          "somewhere in 50-200 range",
                          1,
                          &m_sgbm,
                          SpeckleWindowSize,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Speckle Range",
                          "Maximum disparity variation within each connected component. If you do speckle\n"
                          "filtering, set it to some positive value, multiple of 16. Normally, 16 or 32 is\n"
                          "good enough.",
                          1,
                          &m_sgbm,
                          SpeckleRange,
                          CMyStereoSGBM );

      ADD_BOOL_PARAMETER ( "Full DP",
                           "Set it to true to run full-scale 2-pass dynamic programming algorithm. It will\n"
                           "consume O(W*H*numDisparities) bytes, which is large for 640x480 stereo and huge\n"
                           "for HD-size pictures.",
                           false,
                           &m_sgbm,
                           FullDP,
                           CMyStereoSGBM );

    END_PARAMETER_GROUP;


    BEGIN_PARAMETER_GROUP("SBM", false, SRgb(220,0,0));

      ADD_INT_PARAMETER ( "Number Of Disparities",
                          "This is maximum disparity minus minimum disparity. Always greater than 0.",
                          64,
                          &m_sbm,
                          NumberOfDisparities,
                          CMyStereoBM );

      ADD_INT_PARAMETER ( "Disp LR Max Diff",
                          "The maximum allowed difference between the explicitly computed left-to-right\n"
                          "disparity map and the implicitly (by ValidateDisparity ) computed right-to-left\n"
                          "disparity. If for some pixel the difference is larger than the specified threshold,\n"
                          "the disparity at the pixel is invalidated. Disable it with -1.",
                          1,
                          &m_sbm,
                          Disp12MaxDiff,
                          CMyStereoBM );

      ADD_INT_PARAMETER ( "SAD Window Size",
                         "Could be 5x5..21x21 or higher, but with 21x21 or smaller windows the processing speed\n"
                          "is much higher.",
                          5,
                          &m_sbm,
                          SADWindowSize,
                          CMyStereoBM );

      ADD_INT_PARAMETER ( "Texture Threshold",
                          "The textureness threshold. That is, if the sum of absolute values of x-derivatives\n"
                          "computed over SADWindowSize by SADWindowSize pixel neighborhood is smaller than the\n"
                          "parameter, no disparity is computed at the pixel.",
                          1,
                          &m_sbm,
                          TextureThreshold,
                          CMyStereoBM );
    

      ADD_INT_PARAMETER ( "Uniqueness Ratio",
                          "The minimum margin in percents between the best (minimum) cost function value and the\n"
                          "second best value to accept the computed disparity.",
                          1,
                          &m_sbm,
                          UniquenessRatio,
                          CMyStereoBM );    
      
      CEnumParameter<int> * param_p = static_cast<CEnumParameter<int> * > (
          ADD_ENUM_PARAMETER( "Pre-Filter Type",
                              "Type of the prefilter. CV_STEREO_BM_XSOBEL recommended",
                              int,
                              CV_STEREO_BM_XSOBEL,
                              &m_sbm,
                              PreFilterType,
                              CMyStereoBM ) );
      
      param_p -> addDescription ( CV_STEREO_BM_XSOBEL, "CV_STEREO_BM_XSOBEL" );
      param_p -> addDescription ( CV_STEREO_BM_NORMALIZED_RESPONSE, "CV_STEREO_BM_NORMALIZED_RESPONSE" );      

      ADD_INT_PARAMETER ( "Pre-Filter Size",
                          "Size corresponding to the filter type. Typical values are in the range 5x5 to 21x21.",
                          5,
                          &m_sbm,
                          PreFilterSize,
                          CMyStereoBM );
    
      ADD_INT_PARAMETER ( "Pre-Filter Cap",
                          "Truncation value for the prefiltered image pixels.",
                          1,
                          &m_sbm,
                          PreFilterCap,
                          CMyStereoBM );    
    
      ADD_INT_PARAMETER ( "Speckle Window Size",
                          "The maximum area of speckles to remove (set to 0 to disable speckle filtering).",
                          5,
                          &m_sbm,
                          SpeckleWindowSize,
                          CMyStereoBM );
    
      ADD_INT_PARAMETER ( "Speckle Range",
                          "Acceptable range of disparity variation in each connected component.",
                          4,
                          &m_sbm,
                          SpeckleRange,
                          CMyStereoBM );
    
    END_PARAMETER_GROUP;

    BEGIN_PARAMETER_GROUP("Display", false, SRgb(220,0,0));

      addDrawingListParameter ( "Left Image" );
      addDrawingListParameter ( "Right Image" );

      BEGIN_PARAMETER_GROUP("Disparity", false, SRgb(220,0,0));

        addDrawingListParameter ( "Disparity Image" );

        addColorEncodingParameter (  m_dispCE,
                                     getParameterSet(),
                                     "Disparity",
                                     "Color encoding for the disparity image" );
      END_PARAMETER_GROUP;

    END_PARAMETER_GROUP;


}

/// Virtual destructor.
CStereoOp::~CStereoOp ()
{
}

/// Cycle event.
bool
CStereoOp::cycle()
{
    if ( m_leftImg.size().width  > 0 && 
         m_leftImg.size().height > 0 && 
         m_leftImg.size() == m_rightImg.size() )
    {
        cv::Mat auxImg;

        int m_scale_i = 1;
        
        unsigned int numberOfDisparities = ceil(m_sgbm.numberOfDisparities / (16.0*m_scale_i)) * 16;
        m_sgbm.numberOfDisparities = numberOfDisparities;

        numberOfDisparities = ceil(m_sbm.state->numberOfDisparities / (16.0*m_scale_i)) * 16;
        m_sbm.state->numberOfDisparities = numberOfDisparities;

        cv::Size size = m_leftImg.size();
    
        if (m_scale_i > 1) 
        {
            size.width  /=  m_scale_i;
            size.height /= m_scale_i;
        
            cv::Mat tmpLeft, tmpRight;
            cv::resize(m_leftImg, tmpLeft, size);
            cv::resize(m_rightImg, tmpRight, size);

            if ( m_alg_e == SA_SGBM ) 
            {
                m_sgbm(tmpLeft, tmpRight, auxImg);
            }
            else
            {
                // Transform image to CV_8UC1 if not already in that
                // format. BM works only with in 8 bit grayscale
                // images.

                cv::Mat l,r;
                if ( tmpLeft.type () != CV_8UC1 )
                    tmpLeft.convertTo ( l, CV_8UC1, 1, 0);
                else
                    l = tmpLeft;
                if ( tmpRight.type () != CV_8UC1 )
                    tmpRight.convertTo ( r, CV_8UC1, 1, 0);
                else
                    r = tmpRight;
                    
                 printf("l.type = %i leftimg.type = %i CV_8UC1 = %i\n",
                       l.type(), m_leftImg.type(), CV_8UC1 );
                
               m_sbm(l, r, auxImg);
            }

            cv::resize(auxImg, m_dispImg, m_leftImg.size(), cv::INTER_NEAREST );
            m_dispImg *= 2;
        }
        else 
        {
            if ( m_alg_e == SA_SGBM ) 
            {
                m_sgbm(m_leftImg, m_rightImg, m_dispImg);
            }
            else
            {
                // Transform image to CV_8UC1 if not already in that
                // format. BM works only with in 8 bit grayscale
                // images.

                cv::Mat l,r;

                if ( m_leftImg.type () != CV_8UC1 )
                {
                    IplImage src = (IplImage)m_leftImg;
                    l = cv::Mat( m_leftImg.size(), CV_8UC1);
                    IplImage dst = (IplImage)l;
                    cvCvtColor ( &src, &dst, CV_RGB2GRAY);
                    //m_leftImg.convertTo ( l, CV_8UC1, 1, 0);
                }
                else
                    l = m_leftImg;

                if ( m_rightImg.type () != CV_8UC1 )
                {
                    IplImage src = (IplImage)m_rightImg;
                    r = cv::Mat( m_leftImg.size(), CV_8UC1);
                    IplImage dst = (IplImage)r;
                    cvCvtColor ( &src, &dst, CV_RGB2GRAY);
                    //m_rightImg.convertTo ( r, CV_8UC1, 1, 0);
                }
                else
                    r = m_rightImg;
                    
                printf("l.type = %i leftimg.type = %i CV_8UC1 = %i\n",
                       l.type(), m_leftImg.type(), (int)CV_8UC1 );
                
                cv::imwrite("file.bmp", l);
                
                m_sbm(l, r, m_dispImg);
            }
        }
        cv::imwrite("dispimg.pgm", m_dispImg );
    }

    return COperatorBase::cycle();
}

/// Show event.
bool CStereoOp::show()
{
    CDrawingList *list_p  = getDrawingList ( "Left Image");
    setScreenSize ( m_leftImg.size() );    

    printf("%i %i\n",     list_p->getScreenWidth(),    list_p->getScreenHeight() );
    
    list_p -> clear();    
    list_p->addImage ( m_leftImg, 0, 0 );

    list_p = getDrawingList ( "Right Image");
    list_p -> clear();    
    list_p->addImage ( m_rightImg, 0, 0 );

    list_p = getDrawingList ( "Disparity Image");
    list_p -> clear();    
    list_p->addColorEncImage ( &m_dispImg, m_dispCE, 0, 0, m_dispImg.size().width, m_dispImg.size().height );

    return COperatorBase::show();
}

/// Init event.
bool CStereoOp::initialize()
{
    setScreenSize ( m_leftImg.size() );
    return COperatorBase::initialize();
}

/// Reset event.
bool CStereoOp::reset()
{
    return COperatorBase::reset();
}

bool CStereoOp::exit()
{
    printf("Exit Called\n");
    
    CParamIOFile pio;
    
    getParameterSet()->save ( pio );
    pio.save ("parameters.xml");

    return COperatorBase::exit();
}

void 
CStereoOp::keyPressed ( CKeyEvent * f_event_p )
{
    printf("Event received\n");
    show();
    return COperatorBase::keyPressed ( f_event_p );    
}

/// Set the input of this operator
bool
CStereoOp::setInput  ( const TInpImgFromFileVector & f_input_v )
{
    if ( f_input_v.size () < 2 )
    {
        printf("Stereo requires two images to compute\n");
        return false;
    }

    m_leftImg  = f_input_v[0].image;
    m_rightImg = f_input_v[1].image;
}

/// Gets the output of this operator
bool
CStereoOp::getOutput ( TOutputType & f_output ) const
{
    return true;
}

