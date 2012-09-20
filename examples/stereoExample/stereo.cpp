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

#include "imgScaler.h"
#include "matVector.h"

using namespace QCV;

/// Constructors.
CStereoOp::CStereoOp ( COperatorBase * const f_parent_p,
                       const std::string     f_name_str )
    : COperator<TInpImgFromFileVector, TOutputType>
      (                       f_parent_p, f_name_str ),
      m_alg_e (                                SA_BM ),
      m_sgbm (                                       ),
      m_sbm (                                        ),
      m_leftImg (                                    ),
      m_rightImg (                                   ),
      m_dispImg (                                    ),
      m_dispCE (   CColorEncoding::CET_BLUE2GREEN2RED,
                               S2D<float> ( 0, 400 ) ),
      m_scale_i (                                  2 )
{
    registerDrawingLists();
    registerParameters();

    addChild ( new CImageScalerOp ( this, "Left image scaler", 2) );
    
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
    registerDrawingList ("Colored Disparity Image",
                         S2D<int> (1, 1),
                         false);
    registerDrawingList ("B/W Disparity Image",
                         S2D<int> (0, 1),
                         true);
}

void
CStereoOp::registerParameters()
{
    CEnumParameter<EStereoAlgorithm> * algParam_p = static_cast<CEnumParameter<EStereoAlgorithm> * > (
        ADD_ENUM_PARAMETER( "Algorithm",
                            "Stereo algorithm to use",
                            EStereoAlgorithm,
                            m_alg_e,
                            this,
                            StereoAlgorithm,
                            CStereoOp ) );
    
    algParam_p -> addDescription ( SA_SGBM, "Semi global block matching (SGBM)" );
    algParam_p -> addDescription ( SA_BM,   "Block matching (BM)" );
    

    ADD_INT_PARAMETER ( "Downscale factor",
                        "Downscale factor for left and right images",
                        m_scale_i,
                        this,
                        Downscale,
                        CStereoOp );

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
                          9,
                          &m_sgbm,
                          SADWindowSize,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Uniqueness Ratio",
                          "The margin in percents by which the best (minimum) computed cost function\n"
                          "value should \"win\" the second best value to consider the found match\n"
                          "correct. Normally, some value within 5-15 range is good enough",
                          5,
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
                          100,
                          &m_sgbm,
                          P1,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "P2",
                          "Parameters that control disparity smoothness. Cost parameter P2 of SGM",
                          1000,
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
                          0,
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
                          9,
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
                          5,
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
                          9,
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

        addDrawingListParameter ( "B/W Disparity Image", "Fast rendering of disparity image" );

        addDrawingListParameter ( "Colored Disparity Image", "Colorfull but slow" );

        m_dispCE.setColorEncodingType ( CColorEncoding::CET_GREEN2RED );
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
         m_leftImg.size() == m_rightImg.size() &&
         m_leftImg.type() == m_rightImg.type() )
    {
        TMatVector vec = m_leftImg;
        vec.push_back(m_rightImg);
        
        if ( getChild<CImageScalerOp *>("Left image scaler") -> compute ( vec, vec ) )
        {
            m_leftImg  = vec[0];
            m_rightImg = vec[1];
        }        

        cv::Mat auxImg;

        unsigned int numberOfDisparities = ceil(m_sgbm.numberOfDisparities / (16.0*m_scale_i)) * 16;
        m_sgbm.numberOfDisparities = numberOfDisparities;

        numberOfDisparities = ceil(m_sbm.state->numberOfDisparities / (16.0*m_scale_i)) * 16;
        m_sbm.state->numberOfDisparities = numberOfDisparities;

        cv::Size size = m_leftImg.size();
    
        cv::Mat tmpLeft, tmpRight;

        if (m_scale_i > 1) 
        {
            size.width  /= m_scale_i;
            size.height /= m_scale_i;
            
            cv::resize(m_leftImg,  tmpLeft, size);
            cv::resize(m_rightImg, tmpRight, size);
            auxImg = cv::Mat(size, CV_16S );
        }
        else
        {
            tmpLeft  = m_leftImg;
            tmpRight = m_rightImg;
        }
        
        if ( m_alg_e == SA_SGBM ) 
        {
            if (m_scale_i > 1)
                m_sgbm(tmpLeft, tmpRight, auxImg);
            else
                m_sgbm(tmpLeft, tmpRight, m_dispImg);
        }
        else
        {
            static int oldScale_i = -1;
            
            if (oldScale_i != m_scale_i )
                m_sbm.init(CV_STEREO_BM_BASIC, m_sbm.state->numberOfDisparities, m_sbm.state->SADWindowSize);

            oldScale_i = m_scale_i;

            // Transform image to CV_8UC1 if not already in that
            // format. BM works only with in 8 bit grayscale
            // images.
            
            cv::Mat l,r;
            
            if ( tmpLeft.type () != CV_8UC1 )
            {
                if ( tmpLeft.type () == CV_8UC3 )
                {
                    {
                        IplImage src = (IplImage)tmpLeft;
                        l = cv::Mat( tmpLeft.size(), CV_8UC1);
                        IplImage dst = (IplImage)l;
                        cvCvtColor ( &src, &dst, CV_RGB2GRAY);
                    }
                    
                    {
                        IplImage src = (IplImage)tmpRight;
                        r = cv::Mat( tmpLeft.size(), CV_8UC1);
                        IplImage dst = (IplImage)r;
                        cvCvtColor ( &src, &dst, CV_RGB2GRAY);
                    }
                }
                else
                {
                    return COperatorBase::cycle();
                }
                
                if (m_scale_i > 1)
                    m_sbm(l, r, auxImg);
                else
                    m_sbm(l, r, m_dispImg);
            }
            else
            {
                if (m_scale_i > 1)
                    m_sbm(tmpLeft, tmpRight, auxImg);
                else
                    m_sbm(l, r, m_dispImg);
            }
        }
        
        if (m_scale_i != 1)
        {
            cv::resize(auxImg, m_dispImg, m_leftImg.size(), cv::INTER_NEAREST );
            m_dispImg *= m_scale_i;
        }
    }

    return COperatorBase::cycle();
}

/// Show event.
bool CStereoOp::show()
{
    CDrawingList *list_p  = getDrawingList ( "Left Image");
    
    /// Set the screen size if thei is the parent operator.
    if ( getParentOp() == NULL )
        setScreenSize ( m_leftImg.size() );    

    list_p -> clear();    
    list_p->addImage ( m_leftImg );

    list_p = getDrawingList ( "Right Image");
    list_p -> clear();    
    list_p->addImage ( m_rightImg );

    list_p = getDrawingList ( "Colored Disparity Image");
    list_p -> clear();    
    list_p->addColorEncImage ( &m_dispImg, m_dispCE, 0, 0, m_dispImg.size().width, m_dispImg.size().height );

    list_p = getDrawingList ( "B/W Disparity Image");
    list_p -> clear();    
    list_p->addImage ( m_dispImg, 0, 0, m_dispImg.size().width, m_dispImg.size().height, 100);

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
    return COperatorBase::exit();
}

void 
CStereoOp::keyPressed ( CKeyEvent * f_event_p )
{
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
    return true;
}

/// Gets the output of this operator
bool
CStereoOp::getOutput ( TOutputType & f_output ) const
{
    f_output = m_dispImg;
    return true;
}

