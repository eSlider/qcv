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
* @file stereoOp.cpp
*
* \class CStereoOp
* \author Hernan Badino (hernan.badino@gmail.com)
* \brief Compute stereo using OpenCV stereo algorithms
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "stereoOp.h"
#include "paramMacros.h"
#include "drawingList.h"
#include "ceParameter.h"

#include "imgScalerOp.h"
#include "matVector.h"
#include "stereoCamera.h"

using namespace QCV;

static const char g_scalerName_str[] = "Stereo Image Scaler";

/// Constructors.
CStereoOp::CStereoOp ( COperator * const f_parent_p,
                       const std::string     f_name_str )
    : COperator (             f_parent_p, f_name_str ),
      m_compute_b (                             true ),
      m_leftImgId_str (                    "Image 0" ),
      m_rightImgId_str (                   "Image 1" ),
      m_dispImgId_str (            "Disparity Image" ),
      m_alg_e (                                SA_BM ),
      m_sgbm (                                       ),
      m_sbm (                                        ),
      m_leftImg (                                    ),
      m_rightImg (                                   ),
      m_dispImg (                                    ),
      m_dispCE (   CColorEncoding::CET_BLUE2GREEN2RED,
                               S2D<float> ( 0, 400 ) ),
      m_scale_i (                                  2 ),
      m_convert2Float_b (                      false ),
      m_3DPointImg (                                 ),
      m_show3D_b (                              true )
{
    m_sbm.init(CV_STEREO_BM_BASIC, m_sbm.state->numberOfDisparities, m_sbm.state->SADWindowSize);

    registerDrawingLists();
    registerParameters();

    addChild ( new CImageScalerOp ( this, g_scalerName_str, 2) );
    
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
    
    ADD_BOOL_PARAMETER ( "Compute",
                         "Compute stereo?",
                         m_compute_b,
                         this,
                         Compute,
                         CStereoOp );

    ADD_BOOL_PARAMETER ( "Convert to Float",
                         "Convert output disparity image to float? Output "
                         "id will be \"Float \" + OutputId", 
                         m_convert2Float_b,
                         this,
                         ConvertDispImg2Float,
                         CStereoOp );



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
                        "Downscale factor for left and right images. Disparity image will have "
                        "the original image size.",
                        m_scale_i,
                        this,
                        Downscale,
                        CStereoOp );

    BEGIN_PARAMETER_GROUP("SGBM", false, SRgb(220,0,0));

      ADD_INT_PARAMETER ( "Number Of Disparities SGBM",
                          "This is maximum disparity minus minimum disparity. Always greater than 0.",
                          64,
                          &m_sgbm,
                          NumberOfDisparities,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Window Size",
                          "The matched block size. Must be an odd number >=1 . Normally, it should be "
                          "somewhere in 3..11 range.",
                          9,
                          &m_sgbm,
                          SADWindowSize,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Uniqueness Ratio SGBM",
                          "The margin in percents by which the best (minimum) computed cost function "
                          "value should \"win\" the second best value to consider the found match "
                          "correct. Normally, some value within 5-15 range is good enough",
                          5,
                          &m_sgbm,
                          UniquenessRatio,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Disp LR Max Diff SGBM",
                          "Maximum allowed difference (in integer pixel units) in the left-right "
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
                          "Truncation value for the prefiltered image pixels. The algorithm first "
                          "computes x-derivative at each pixel and clips its value by [-preFilterCap, "
                          "preFilterCap] interval. The result values are passed to the Birchfield-Tomasi "
                          "pixel cost function.",
                          0,
                          &m_sgbm,
                          PreFilterCap,
                          CMyStereoSGBM );      

      ADD_INT_PARAMETER ( "Speckle Window Size SGBM",
                          "Maximum size of smooth disparity regions to consider them noise speckles and "
                          "invdalidate. Set it to 0 to disable speckle filtering. Otherwise, set it "
                          "somewhere in 50-200 range",
                          0,
                          &m_sgbm,
                          SpeckleWindowSize,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Speckle Range SGBM",
                          "Maximum disparity variation within each connected component. If you do speckle "
                          "filtering, set it to some positive value, multiple of 16. Normally, 16 or 32 is "
                          "good enough.",
                          1,
                          &m_sgbm,
                          SpeckleRange,
                          CMyStereoSGBM );

      ADD_BOOL_PARAMETER ( "Full DP",
                           "Set it to true to run full-scale 2-pass dynamic programming algorithm. It will "
                           "consume O(W*H*numDisparities) bytes, which is large for 640x480 stereo and huge "
                           "for HD-size pictures.",
                           false,
                           &m_sgbm,
                           FullDP,
                           CMyStereoSGBM );

    END_PARAMETER_GROUP;


    BEGIN_PARAMETER_GROUP("SBM", false, SRgb(220,0,0));

      ADD_INT_PARAMETER ( "Number Of Disparities SBM",
                          "This is maximum disparity minus minimum disparity. Always greater than 0.",
                          64,
                          &m_sbmState,
                          NumberOfDisparities,
                          CMyStereoBMState );

      ADD_INT_PARAMETER ( "Disp LR Max Diff SBM",
                          "The maximum allowed difference between the explicitly computed left-to-right "
                          "disparity map and the implicitly (by ValidateDisparity ) computed right-to-left "
                          "disparity. If for some pixel the difference is larger than the specified threshold, "
                          "the disparity at the pixel is invalidated. Disable it with -1.",
                          1,
                          &m_sbmState,
                          Disp12MaxDiff,
                          CMyStereoBMState );

      ADD_INT_PARAMETER ( "SAD Window Size",
                         "Could be 5x5..21x21 or higher, but with 21x21 or smaller windows the processing speed "
                          "is much higher.",
                          9,
                          &m_sbmState,
                          SADWindowSize,
                          CMyStereoBMState );

      ADD_INT_PARAMETER ( "Texture Threshold",
                          "The textureness threshold. That is, if the sum of absolute values of x-derivatives "
                          "computed over SADWindowSize by SADWindowSize pixel neighborhood is smaller than the "
                          "parameter, no disparity is computed at the pixel.",
                          1,
                          &m_sbmState,
                          TextureThreshold,
                          CMyStereoBMState );
    

      ADD_INT_PARAMETER ( "Uniqueness Ratio SBM ",
                          "The minimum margin in percents between the best (minimum) cost function value and the "
                          "second best value to accept the computed disparity.",
                          5,
                          &m_sbmState,
                          UniquenessRatio,
                          CMyStereoBMState );    
      
      CEnumParameter<int> * param_p = static_cast<CEnumParameter<int> * > (
          ADD_ENUM_PARAMETER( "Pre-Filter Type",
                              "Type of the prefilter. CV_STEREO_BM_XSOBEL recommended",
                              int,
                              CV_STEREO_BM_XSOBEL,
                              &m_sbmState,
                              PreFilterType,
                              CMyStereoBMState ) );
      
      param_p -> addDescription ( CV_STEREO_BM_XSOBEL, "CV_STEREO_BM_XSOBEL" );
      param_p -> addDescription ( CV_STEREO_BM_NORMALIZED_RESPONSE, "CV_STEREO_BM_NORMALIZED_RESPONSE" );      

      ADD_INT_PARAMETER ( "Pre-Filter Size",
                          "Size corresponding to the filter type. Typical values are in the range 5x5 to 21x21.",
                          5,
                          &m_sbmState,
                          PreFilterSize,
                          CMyStereoBMState );
    
      ADD_INT_PARAMETER ( "Pre-Filter Cap",
                          "Truncation value for the prefiltered image pixels.",
                          1,
                          &m_sbmState,
                          PreFilterCap,
                          CMyStereoBMState );    
    
      ADD_INT_PARAMETER ( "Speckle Window Size SBM",
                          "The maximum area of speckles to remove (set to 0 to disable speckle filtering).",
                          9,
                          &m_sbmState,
                          SpeckleWindowSize,
                          CMyStereoBMState );
    
      ADD_INT_PARAMETER ( "Speckle Range SBM",
                          "Acceptable range of disparity variation in each connected component.",
                          4,
                          &m_sbmState,
                          SpeckleRange,
                          CMyStereoBMState );
    
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

#ifdef HAVE_QGLVIEWER
        ADD_LINE_SEPARATOR;
        
        ADD_BOOL_PARAMETER ( "Show 3D Mesh",
                             "Show 3D mesh of the reconstructed scene.",
                             &m_show3D_b,
                             this,
                             Show3DMesh,
                             CStereoOp );
#endif
      END_PARAMETER_GROUP;

    END_PARAMETER_GROUP;


}

/// Virtual destructor.
CStereoOp::~CStereoOp ()
{
}

/// Validate event.
bool
CStereoOp::validateImages() const
{
    return ( m_leftImg.size().width  > 32 && 
             m_leftImg.size().height > 32 && 
             m_leftImg.size() == m_rightImg.size() &&
             m_leftImg.type() == m_rightImg.type() );    
}

bool
CStereoOp::getInput()
{
    m_leftImg  = COperator::getInput<cv::Mat> ( m_leftImgId_str,  cv::Mat() );
    m_rightImg = COperator::getInput<cv::Mat> ( m_rightImgId_str, cv::Mat() );
    
    if ( !validateImages() )
    {
        CMatVector * vec = COperator::getInput<CMatVector> ( "Images" );
        if (vec && vec->size() >= 2)
        {
            m_leftImg  = (*vec)[0];
            m_rightImg = (*vec)[1];
        }

        return validateImages();
    }

    return true;
}

/// Cycle event.
bool
CStereoOp::cycle()
{
    if ( m_compute_b )
    {
        if ( getInput() )
        {
            CMatVector vec = m_leftImg;
            vec.push_back(m_rightImg);

            /// Scale images
            getChild<CImageScalerOp *>( g_scalerName_str ) -> compute ( vec, vec );

            //unsigned int numberOfDisparities = ceil(m_sgbm.numberOfDisparities / (16.0*m_scale_i)) * 16;
            //m_sgbm.numberOfDisparities = numberOfDisparities;

            //numberOfDisparities = ceil(m_sbm.state->numberOfDisparities / (16.0*m_scale_i)) * 16;
            //m_sbm.state->numberOfDisparities = numberOfDisparities;

            cv::Size size = vec[0].size();
    
            cv::Mat tmpLeft, tmpRight;

            if (m_scale_i > 1) 
            {
                size.width  /= m_scale_i;
                size.height /= m_scale_i;
 
                cv::resize(vec[0], tmpLeft, size);
                cv::resize(vec[1], tmpRight, size);
                m_auxImg = cv::Mat(size, CV_16S );
            }
            else
            {
                tmpLeft  = vec[0];
                tmpRight = vec[1];
            }
        
            if ( m_alg_e == SA_SGBM ) 
            {
                if (m_scale_i > 1)
                    m_sgbm(tmpLeft, tmpRight, m_auxImg);
                else
                    m_sgbm(tmpLeft, tmpRight, m_dispImg);
            }
            else
            {
               //static int oldScale_i = -1;
                m_sbm.state->minDisparity = -1;

                m_sbmState.setParams(m_sbm);

                //oldScale_i = m_scale_i;

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
                        printf("%s:%i Required image format is CV_8UC1 or CV_8UC3\n", __FILE__, __LINE__);
                        return false;
                    }

                    if (m_scale_i > 1)
                        m_sbm(l, r, m_auxImg);
                    else
                        m_sbm(l, r, m_dispImg);
                }
                else
                {
                    if (m_scale_i > 1)
                        m_sbm(tmpLeft, tmpRight, m_auxImg);
                    else
                        m_sbm(tmpLeft, tmpRight, m_dispImg);
                }
            }
        
            if (m_scale_i != 1)
            {
                registerOutput<cv::Mat> ( std::string("Downscaled " + m_dispImgId_str), 
                                          &m_auxImg );

                cv::resize(m_auxImg, m_dispImg, vec[0].size(), 0, 0, cv::INTER_NEAREST );
                m_dispImg *= m_scale_i;
            }
            else // Set the original-sized image as output
                registerOutput<cv::Mat> ( std::string("Downscaled " + m_dispImgId_str), 
                                          &m_dispImg );
               

            if ( m_convert2Float_b )
            {
                /// Convert to float output
                m_dispImgFloat = cv::Mat(m_dispImg.size(), CV_32FC1);
                
                float *t     = (float *)    m_dispImgFloat.data;
                short int *s = (short int *)m_dispImg.data;
                const short int * const e = (short int *)(m_dispImg.data + m_dispImg.step * m_dispImg.size().height);
                for (; s < e; ++s, ++t) *t = *s / 16.f;
            }
            
            registerOutput<cv::Mat> ( m_dispImgId_str, 
                                      &m_dispImg );

            registerOutput<cv::Mat> ( std::string("Float ") + m_dispImgId_str, 
                                      &m_dispImgFloat );        
        }
        else
        {
            printf("%s:%i Invalid input images.\n", __FILE__, __LINE__ );
            return false;
        }    
    }

    return true; //COperator::cycle();
}

/// Show event.
bool CStereoOp::show()
{
    /// Set the screen size if this is the parent operator.
    if ( getParentOp() == NULL )
        setScreenSize ( m_leftImg.size() );
    
    CDrawingList *list_p  = getDrawingList ( "Left Image");    
    list_p -> clear();    
    if (list_p -> isVisible() )
        list_p->addImage ( m_leftImg ); //, 0, 0, getScreenSize().width, getScreenSize().height );

    list_p = getDrawingList ( "Right Image");
    list_p -> clear();    
    if (list_p -> isVisible() )
        list_p->addImage ( m_rightImg );

    list_p = getDrawingList ( "Colored Disparity Image");
    list_p -> clear();    
    list_p->addColorEncImage ( &m_dispImg, m_dispCE, 0, 0, getScreenSize().width, getScreenSize().height );

    list_p = getDrawingList ( "B/W Disparity Image");
    list_p -> clear();    
    if (list_p -> isVisible() )
        list_p->addImage ( m_dispImg, 0, 0, getScreenSize().width, getScreenSize().height, 100);
    

#ifdef HAVE_QGLVIEWER
    if ( m_3dViewer_p && m_show3D_b && m_compute_b)
        show3D();    
#endif // HAVE_QGLVIEWER
    
    return COperator::show();
}

void CStereoOp::show3D()
{
#ifdef HAVE_QGLVIEWER
    if (m_leftImg.cols <= 0) return;
    cv::Mat textureImg = m_leftImg;

    m_3dViewer_p -> clear(); /// This might clear 3D added by other operators.

    m_3DPointImg = cv::Mat::zeros( m_dispImg.size(), CV_64FC3 );
        
    // Let set just some approximate calibration params
    CStereoCamera tcam;    
    tcam.setBaseline(0.15);
    tcam.setFocalLength(800/m_leftImg.cols*m_dispImg.cols);
    tcam.setU0(m_dispImg.rows/2);
    tcam.setV0(m_dispImg.cols/2);

    CStereoCamera cam = COperator::getInput<CStereoCamera> ( "Rectified Camera", tcam );
        
    if ( m_dispImg.cols != m_leftImg.cols )
    {
        CMatVector vec = COperator::getInput<CMatVector> ( "Scaled Images", CMatVector() );
        if (!vec.empty())
            textureImg = vec[0];
    }

    for (int i = 0; i < m_dispImg.rows; ++i)
    {
        short int *p     = &m_dispImg.at<short int>(i, 0);
        C3DVector *img_p = &m_3DPointImg.at<C3DVector>(i, 0);
            
        for (int j = 0; j < m_dispImg.cols; ++j, ++p, ++img_p)
        {
            if (*p > 0)
            {
                cam.image2Local ( j, i, *p/16.f, *img_p);
            }
        }
    }   

    m_3dViewer_p -> addMesh ( m_3DPointImg, textureImg, 2, 1000);
#endif // HAVE_QGLVIEWER
}


/// Init event.
bool CStereoOp::initialize()
{
    if (!getParentOp() && getInput() )
        setScreenSize ( m_leftImg.size() );

    return COperator::initialize();
}

/// Reset event.
bool CStereoOp::reset()
{
    return COperator::reset();
}

bool CStereoOp::exit()
{
    return COperator::exit();
}

void 
CStereoOp::keyPressed ( CKeyEvent * f_event_p )
{
    return COperator::keyPressed ( f_event_p );    
}

/// Set the input of this operator
bool
CStereoOp::setInput  ( const CMatVector & f_input_v )
{
    if ( f_input_v.size () < 2 )
    {
        printf("Stereo requires two images to compute\n");
        return false;
    }

    m_leftImg  = f_input_v[0];
    m_rightImg = f_input_v[1];
    return true;
}

    
