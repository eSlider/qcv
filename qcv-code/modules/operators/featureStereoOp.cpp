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

/* INCLUDES */
#include <limits>

#include "featureStereoOp.h"
#include "drawingList.h"
#include "feature.h"
#include "stereoCamera.h"

#if defined HAVE_QGLVIEWER
#include "glViewer.h"
#endif

/// Dependeing on the parameters might be better not to use OPENMP here.
/// When using large masks, it is better to use openmp. 
#include <omp.h>
//undef _OPENMP

#include "ceParameter.h"

using namespace QCV;

static const float INVALID_DISP=-std::numeric_limits<float>::max();

/// Constructors.
CFeatureStereoOp::CFeatureStereoOp ( COperator * const f_parent_p, 
                                     const std::string f_name_str  )
   : COperator (                 f_parent_p, f_name_str ),
     m_lImg (                                           ),
     m_rImg (                                           ),
     m_compute_b (                                 true ),
     m_idLeftImage_str (                      "Image 0" ),
     m_idRightImage_str (                     "Image 1" ),
     m_featPointVector_str (           "Feature Vector" ),
     m_dispRange (                                0, 80 ),
     m_maskSize (                                11, 11 ),
     m_checkVars_b (                              false ),
     m_minVar_f (                                     0 ),
     m_maxZssd_f (                                 1.e7 ),
     m_maxSsd_f (                                  1.e7 ),
     m_dispOffset_f (                               0.f ),
     m_pyrLeft (                                      2 ),
     m_pyrRight (                                     2 ),
     m_pyrDisp (                                      2 ),
     m_levels_ui (                                    2 ),
     m_deltaDisp_i (                                  1 ),
     m_ceDistance ( CColorEncoding::CET_GREEN2RED,
                                   S2D<float>(0.f,40.f) ),
     m_show3DPoints_b (                           false ),
     m_preFilter_b (                              false ),
     m_showLeftImage_b (                           true ),
     m_showRightImage_b (                          true )

{
   registerDrawingLists();
   registerParameters();
}


void
CFeatureStereoOp::registerDrawingLists()
{
   registerDrawingList ("Left Image Features",
                        S2D<int> (0, 0),
                        false);

   registerDrawingList ("Right Image Features",
                        S2D<int> (1, 0),
                        true);

   registerDrawingList ("Left Image Pyramid",
                        S2D<int> (0, 0),
                        false);

   registerDrawingList ("Right Image Pyramid",
                        S2D<int> (0, 0),
                        false);
}

void
CFeatureStereoOp::registerParameters()
{
    BEGIN_PARAMETER_GROUP("Input/Output", false, SRgb(220,0,0));
      ADD_STR_PARAMETER( "Input Feature Vector Id", 
                         "Id of feature point vector. Must be a CFeatureVector type.",
                         m_featPointVector_str,
                         this,
                         FeaturePointVectorId, 
                         CFeatureStereoOp );
      
      ADD_STR_PARAMETER( "Left Image Id", 
                         "Id of left image.",
                         m_idLeftImage_str,
                         this,
                         IdLeftImageStr, 
                         CFeatureStereoOp );
      
      ADD_STR_PARAMETER( "Right Image Id", 
                         "Id of right image.",
                         m_idRightImage_str,
                         this,
                         IdRightImageStr, 
                         CFeatureStereoOp );
    END_PARAMETER_GROUP;     

    BEGIN_PARAMETER_GROUP("Computation", false, SRgb(220,0,0));
      ADD_BOOL_PARAMETER( "Compute", 
                          "Compute Feature Stereo?",
                          m_compute_b,
                          this,
                          Compute, 
                          CFeatureStereoOp );

      ADD_BOOL_PARAMETER( "Pre-Filter?", 
                          "Apply mask normalization to input images.",
                          m_preFilter_b,
                          this,
                          PreFilter, 
                          CFeatureStereoOp );

      ADD_INT2D_PARAMETER( "Disparity Range", 
                           "Disparity range [px].",
                           m_dispRange,
                           "Min", "Max",
                           this,
                           DisparityRange, 
                           CFeatureStereoOp );

      ADD_UINT_PARAMETER ( "Pyramid Levels",
                           "0 is the first level (original size)",
                           m_levels_ui,
                           this,
                           Levels,
                           CFeatureStereoOp );

      ADD_INT_PARAMETER ( "Disparity correction",
                          "+- correction in pyramid transfer [px].",
                          m_deltaDisp_i,
                          this,
                          DeltaDisp,
                          CFeatureStereoOp );

      ADD_INT2D_PARAMETER( "Mask Size", 
                           "Correlation mask size [px].",
                           m_maskSize,
                           "W", "H",
                           this,
                           MaskSize, 
                           CFeatureStereoOp );

      ADD_BOOL_PARAMETER( "Check Variances?", 
                          "Check the variances of the masks?",
                          m_checkVars_b,
                          this,
                          CheckVariances, 
                          CFeatureStereoOp );

      ADD_FLOAT_PARAMETER( "Min Variance", 
                           "Min allowed variance for left and right windows [luminance^2].",
                           m_minVar_f,
                           this,
                           MinVariance, 
                           CFeatureStereoOp );

      ADD_FLOAT_PARAMETER( "Max ZSSD", 
                           "Max allowed zero-mean sum of square difference [luminance^2].",
                           m_maxZssd_f,
                           this,
                           MaxZSSD, 
                           CFeatureStereoOp );

      ADD_FLOAT_PARAMETER( "Max SSD", 
                           "Max allowed sum of square difference [luminance^2].",
                           m_maxSsd_f,
                           this,
                           MaxSSD, 
                           CFeatureStereoOp );

      BEGIN_PARAMETER_GROUP("Offset", true, SRgb(220,0,0));
      ADD_FLOAT_PARAMETER( "Disparity Offset", 
                           "Disparity offset [px].",
                           m_dispOffset_f,
                           this,
                           DispOffset, 
                           CFeatureStereoOp );
      END_PARAMETER_GROUP;
    END_PARAMETER_GROUP;


    BEGIN_PARAMETER_GROUP("Display", false, SRgb(220,0,0));
      addDrawingListParameter ( "Left Image Features" );
      ADD_BOOL_PARAMETER( "Show left image?", 
                          "Show left image as background for feature display.",
                          m_showLeftImage_b,
                          this,
                          ShowLeftImage, 
                          CFeatureStereoOp );
      
      addDrawingListParameter ( "Right Image Features" );
      ADD_BOOL_PARAMETER( "Show right image?", 
                          "Show right image as background for feature display.",
                          m_showRightImage_b,
                          this,
                          ShowRightImage, 
                          CFeatureStereoOp );
      
      addDrawingListParameter ( "Left Image Pyramid" );
      
      addDrawingListParameter ( "Right Image Pyramid" );
      
      addColorEncodingParameter (  m_ceDistance,
                                   getParameterSet(),
                                   "CE Disparity",
                                   "Color encoding for the disparity features" );

      ADD_BOOL_PARAMETER ( "Show 3D Points",
                           "Show points in 3D display.",
                           m_show3DPoints_b,
                           this,
                           Show3DPoints,
                           CFeatureStereoOp );
    END_PARAMETER_GROUP;

}

/// Virtual destructor.
CFeatureStereoOp::~CFeatureStereoOp ()
{
}

/// Cycle event.
bool
CFeatureStereoOp::cycle()
{//
   if ( m_compute_b )
   {      
      CFeatureVector * featureVector_p = getInput<CFeatureVector> ( m_featPointVector_str );      
    
      if ( featureVector_p )
      {
         m_rejCause_v.clear();
         m_rejCause_v.resize( featureVector_p->size(), ERC_NONE );

         if (m_preFilter_b)
         {
            startClock ("Pre-filtering");            
            getInput<cv::Mat>( m_idLeftImage_str,  cv::Mat() ).copyTo(m_lImg);
            getInput<cv::Mat>( m_idRightImage_str, cv::Mat() ).copyTo(m_rImg);
            int ddepth = CV_32F;
            cv::Mat imgf,imgf2;
            cv::Mat *imgs_p[2] = {&m_lImg, &m_rImg};

            for (int i = 0; i < 2; ++i)
            {
               cv::boxFilter( *imgs_p[i], imgf, ddepth, cv::Size(19,19), cv::Point(-1,-1), true, cv::BORDER_DEFAULT );
               imgs_p[i]->convertTo ( imgf2, CV_32F, 1., 0);
               imgf -= imgf2;
               imgf.convertTo ( *imgs_p[i], CV_8U, 5, 127);
            }
            stopClock ("Pre-filtering");            
         }
         else
         {
            m_lImg = getInput<cv::Mat>( m_idLeftImage_str,  cv::Mat() );
            m_rImg = getInput<cv::Mat>( m_idRightImage_str, cv::Mat() );
         }

         
        if (m_lImg.cols > 0 && m_lImg.rows > 0 && m_lImg.size() == m_rImg.size() &&
             m_lImg.type() == m_rImg.type() && m_lImg.type() == CV_8UC1 )
         {
           m_dispImg = cv::Mat(m_lImg.size(), CV_32FC1);
            
            startClock("Pyramid construction");
            
            setPyramidParams ( m_levels_ui );
        
            /// 1.- Compute Gaussian Pyramids.
           m_pyrLeft.compute  ( m_lImg );
           m_pyrRight.compute ( m_rImg );
        
            stopClock("Pyramid construction");
            
            startClock("First Correlation");
            
            int startLevel_i = m_levels_ui-1;
            /// 2.- Compute first result.
            computeFirstCorrelation(startLevel_i);
            
            stopClock("First Correlation");
            
            char clockId_str[256];
            
           /// 3.- Iterate for all levels.
            for (int i = startLevel_i-1; i >=0; --i)
            {
               sprintf(clockId_str, "Refinement level %i", i);               
               startClock(clockId_str);
               /// a.- Transfer level.
               transferLevel( i );
               stopClock(clockId_str);
            }
            
            startClock("Disparity Image Generation");
            generateDispImage(featureVector_p);
            stopClock("Disparity Image Generation");
            
            /*
              startClock("3D Point List Generation");
              generate3DPointList(featureVector_p);
              stopClock("3D Point List Generation");
            */
            
         }
      }
   }        

   return COperator::cycle();
}

void 
CFeatureStereoOp::generateDispImage( const CFeatureVector *f_vec_p )
{
   const CFeatureVector &vec = *f_vec_p;
    
   m_dispImg = cv::Mat::zeros(m_dispImg.size(), CV_32FC1);

   for (int f = 0; f < (int)vec.size(); ++f)
   {
      if ( vec[f].d >= 0. )
      {
         int i = (int)(vec[f].v + .5);
         int j = (int)(vec[f].u + .5);
         
         if (i >= 0 && j >= 0 && i < m_dispImg.rows && j < m_dispImg.cols)
            m_dispImg.at<float>(i,j) = vec[f].d;
      }
   }
   
   registerOutput ( "Sparse Disparity Image",  &m_dispImg );
}

/*
  void 
  CFeatureStereoOp::generate3DPointList( const CFeatureVector *f_vec_p )
  {
  m_3dPoints.clear();

  CStereoCamera *     camera_p = 
  dynamic_cast<CStereoCamera *>(getInput ( "Rectified Camera" ) );

  if ( camera_p )
  {
  const CFeatureVector &vec = *f_vec_p;
    
  for (int f = 0; f < (int)vec.size(); ++f)
  {
  if ( vec[f].d > 0.5 )
  {
  C3DVector p;
  camera_p->image2World( vec[f].u, vec[f].v, vec[f].d, p );
  m_3dPoints.push_back(p);
  }
  }
  }

  registerOutput ( "Stereo Triangulated 3D Points",  &m_3dPoints );
  }
*/

/// Cycle event.
bool
CFeatureStereoOp::computeFirstCorrelation( int f_fromLevel_i )
{
   CFeatureVector * featureVector_p = getInput<CFeatureVector> ( m_featPointVector_str );
    
   if (!featureVector_p) 
   {
      printf("%s:%i No valid feature vector\n", __FILE__, __LINE__ );
      return false;  
   }

   if (f_fromLevel_i < 0)
      f_fromLevel_i = m_levels_ui-1;    

   m_pyrDisp.clear();
    
   cv::Mat & imgL = m_pyrLeft.getLevelImage(f_fromLevel_i);
   cv::Mat & imgR = m_pyrRight.getLevelImage(f_fromLevel_i);
   cv::Mat & disp = m_pyrDisp.getLevelImage(f_fromLevel_i);

   disp = cv::Mat::zeros(imgL.size(), CV_32FC1);

   int scale_i = 1<<((int)f_fromLevel_i);

   S2D<int> mask = m_maskSize;
   mask.x /= scale_i; mask.x += 1-(mask.x%2);
   mask.y /= scale_i; mask.y += 1-(mask.y%2);

   if (mask.x < 3) mask.x = 3;
   if (mask.y < 3) mask.y = 3;

   const int maskSize_i = mask.width * mask.height;
                            
   S2D<int> imgSize ( imgL.cols, imgL.rows );
   S2D<int> hMask (mask.width/2, mask.height/2);
    
   CFeatureVector &vec = *featureVector_p;
    
#if not defined ( _OPENMP )
   float * const scores_p = m_scoresACTUAL_p[0] + FSO_MAX_WIDTH;
#else
   const unsigned int numThreads_ui = std::min(omp_get_max_threads(), FSO_MAX_CORES);
#pragma omp parallel for num_threads(numThreads_ui) schedule(dynamic)
#endif
   for (int f = 0; f < (int)vec.size(); ++f)
   {
#if defined ( _OPENMP )
      const unsigned int threadNum_ui = omp_get_thread_num();
      float * const scores_p = m_scoresACTUAL_p[threadNum_ui] + FSO_MAX_WIDTH;
#endif
      S2D<int> featPos ( vec[f].u / scale_i + .5, 
                         vec[f].v / scale_i + .5 );
        
      if ( ( vec[f].state == SFeature::FS_NEW || 
             vec[f].state == SFeature::FS_TRACKED ) &&
           ( featPos.x >= 0 && featPos.y >= 0 &&
             featPos.x < imgSize.width && featPos.y < imgSize.height ) )
      {
         if ( !disp.at<float>(featPos.y,featPos.x) )
         {
            vec[f].d = INVALID_DISP;
              
            /// Row limits.
            S2D<int> vlimit ( featPos.y - hMask.height, 
                              featPos.y + hMask.height );
              
            if ( vlimit.min >= 0 && vlimit.max < imgSize.height )
            {
               S2D<int> uLlimit ( featPos.x - hMask.width, 
                                  featPos.x + hMask.width );
                 
               if ( uLlimit.min >= 0 && uLlimit.max < imgSize.width )
               {
                  int subPixel_i = (m_levels_ui == 1)?1:0;
                  /// Disparity limit.
                  S2D<int> rlimit ( std::max(featPos.x - m_dispRange.max/scale_i - subPixel_i, hMask.width), 
                                    std::min(featPos.x - m_dispRange.min/scale_i + subPixel_i, imgSize.width - 1 - hMask.width ) );

                  if (rlimit.min < rlimit.max)
                  {
                     S2D<int> dlimit ( featPos.x - rlimit.max,
                                       featPos.x - rlimit.min );
                                
                     float minZssdScore_f = std::numeric_limits<float>::max();
                     float ssdScore_f     = std::numeric_limits<float>::max();
                     int   bestDisp_i     = INVALID_DISP;
                     float maskVar_f      = std::numeric_limits<float>::max();
                                
                     for (int colrPos = rlimit.min; colrPos <= rlimit.max; ++colrPos)
                     {
                        int d = featPos.x - colrPos;
                        /// Column limits for current disparity.
                        S2D<int> uRlimit ( colrPos - hMask.width, 
                                           colrPos + hMask.width );

                        /// The following should never happen.
                        if ( uRlimit.min < 0 ||
                             uRlimit.max >= imgSize.width )
                        {
                           printf("%s:%i Invalid right limits\n", __FILE__, __LINE__);
                           continue;
                        }                            
                            
                        float sumL_f    = 0.f;
                        float sumR_f    = 0.f;
                        float sumSqR_f  = 0.f;
                        float sumSq_f   = 0.f;
                        float varR_f    = 0.f;
                                  
                       /// Correlate windows now.
                        for (int i = vlimit.min; i <= vlimit.max; ++i)
                        {
                           uint8_t *ptrL_p = &imgL.at<uint8_t>(i,uLlimit.x);
                           uint8_t *ptrR_p = &imgR.at<uint8_t>(i,uRlimit.x);;
                                
                           for (int j = 0; j < mask.width; ++j, ++ptrL_p, ++ptrR_p)
                           {
                              float diff_f = (float)*ptrL_p - (float)*ptrR_p;
                              sumL_f  += *ptrL_p;
                              sumR_f  += *ptrR_p;
                              sumSq_f += diff_f * diff_f;
                                    
                               if ( m_checkVars_b )
                                 sumSqR_f += (float)( *ptrR_p * *ptrR_p);
                           }
                        }

                        if ( m_checkVars_b )
                        {
                           float sqR_f  = sumR_f * sumR_f / maskSize_i;
                           varR_f = (sumSqR_f - sqR_f) / maskSize_i;
                        }

                        float avgDiff_f   = (sumL_f - sumR_f);
                        float sqDiff_f    = avgDiff_f * avgDiff_f / maskSize_i;
                        float scoreZssd_f = sumSq_f - sqDiff_f;

                        /// Store scoreZssd in vector
                        scores_p[d] = scoreZssd_f;

                        if (scoreZssd_f < minZssdScore_f)
                        {
                           minZssdScore_f = scoreZssd_f;
                           ssdScore_f     = sumSq_f;
                           bestDisp_i     = d;

                           maskVar_f      = varR_f;
                        } 
                     }

                     minZssdScore_f /= maskSize_i;
                     ssdScore_f     /= maskSize_i; 

                     if ( bestDisp_i     > dlimit.min - (1-subPixel_i) && 
                          bestDisp_i     < dlimit.max + (1-subPixel_i) &&
                          minZssdScore_f < m_maxZssd_f && 
                          ssdScore_f     < m_maxSsd_f   &&
                          ( !m_checkVars_b || 
                            ( maskVar_f > m_minVar_f ) ) )
                     {
                        vec[f].d = bestDisp_i;

                        if ( subPixel_i == 1 )
                        {
                           // Calculate sub-pixel estimation.
                           const float * const cost_p = scores_p+bestDisp_i;
                           float nom_f    = 0.5 * ( cost_p[1] - cost_p[-1] );
                           float denom_f  = 2.0 * cost_p[0] - ( cost_p[1] + cost_p[-1] );           
                           float correction_f = nom_f/denom_f;

                           if (correction_f <= 0.5 && correction_f >= -0.5)
                           {
                              vec[f].d += nom_f / denom_f;
                           }
                        }

                        disp.at<float>(featPos.y,featPos.x) = vec[f].d;
                     }
                     else
                     {
                        if (!(bestDisp_i     > dlimit.min - (1-subPixel_i) &&
                              bestDisp_i     < dlimit.max + (1-subPixel_i)) )
                           m_rejCause_v[f] = ERC_LIMIT;
                        else if (!(minZssdScore_f < m_maxZssd_f))
                           m_rejCause_v[f] = ERC_LARGEZSSD;
                        else if (!(ssdScore_f < m_maxSsd_f))
                           m_rejCause_v[f] = ERC_LARGESSD;
                        else 
                           m_rejCause_v[f] = ERC_LOWVARIANCE;

                        vec[f].d = INVALID_DISP;
                     }
                  }
                  else
                  {
                     m_rejCause_v[f] = ERC_OTHER;
                     vec[f].d = INVALID_DISP;
                  }
               }
               else // Correlation not possible because feature is at border of image
               {
                  vec[f].d = INVALID_DISP;
                  m_rejCause_v[f] = ERC_ATBORDER;
               }                
            }
            else // Correlation not possible because feature is at border of image
            {
               vec[f].d  = INVALID_DISP;
               m_rejCause_v[f] = ERC_ATBORDER;
            }                
         }                       
         else // Correlation has already been computed.
         {
            vec[f].d = disp.at<float>(featPos.y,featPos.x);
         }                
      }
      else // Outside bounds.
      {
         m_rejCause_v[f] = ERC_OTHER;
         vec[f].d  = INVALID_DISP;
      }                
   }
   
   return true;
}

/// Cycle event.
bool
CFeatureStereoOp::transferLevel( int f_level_i )
{
   
   CFeatureVector * featureVector_p = getInput<CFeatureVector> ( m_featPointVector_str );

   if ( ! featureVector_p ) return false;
   
   cv::Mat &imgL = m_pyrLeft.getLevelImage(f_level_i);
   cv::Mat &imgR = m_pyrRight.getLevelImage(f_level_i);
   cv::Mat &disp = m_pyrDisp.getLevelImage(f_level_i);

   disp = cv::Mat::zeros(imgL.size(), CV_32FC1);

   int scale_i = 1<<f_level_i;

   S2D<int> mask = m_maskSize;
   mask.x /= scale_i; mask.x += 1-(mask.x%2);
   mask.y /= scale_i; mask.y += 1-(mask.y%2);

   if (mask.x < 3) mask.x = 3;
   if (mask.y < 3) mask.y = 3;

   const int maskSize_i = mask.width * mask.height;
                            
   S2D<int> imgSize ( imgL.cols, imgL.rows );
   S2D<int> hMask (mask.width/2, mask.height/2);

   CFeatureVector &vec = *featureVector_p;


#if not defined ( _OPENMP )
   float * const scores_p = m_scoresACTUAL_p[0] + FSO_MAX_WIDTH;
#else
   const unsigned int numThreads_ui = std::min(omp_get_max_threads(), FSO_MAX_CORES);
#pragma omp parallel for num_threads(numThreads_ui) schedule(dynamic)
#endif
   for (int f = 0; f < (int)vec.size(); ++f)
   {
#if defined ( _OPENMP )
      const unsigned int threadNum_ui = omp_get_thread_num();
      float * const scores_p = m_scoresACTUAL_p[threadNum_ui] + FSO_MAX_WIDTH;
#endif        
      S2D<int> featPos ( vec[f].u / scale_i + .5, 
                         vec[f].v / scale_i + .5 );

      if ( vec[f].d >= 0 )
      {
         if ( featPos.x >= 0 && featPos.y >= 0 &&
              featPos.x < imgSize.width && featPos.y < imgSize.height )
         {
            if ( disp.at<float>(featPos.y,featPos.x) <= 0 )
            {
               /// Row limits.
               S2D<int> vlimit ( featPos.y - hMask.height, 
                                 featPos.y + hMask.height );
               
               if ( vlimit.min >= 0 && vlimit.max < imgSize.height )
               {
                  S2D<int> uLlimit ( featPos.x - hMask.width, 
                                     featPos.x + hMask.width );
                    
                  if ( uLlimit.min >= 0 && uLlimit.max < imgSize.width )
                  {
                     int subPixel_i = ( f_level_i == 0)?1:0;

                     /// Hard-encode left limit to 1
                     const int leftDeltaDisp_i = 1;
                     
                     S2D<int> rlimit ( std::max(featPos.x - ((int)vec[f].d)*2 - leftDeltaDisp_i - subPixel_i, hMask.width), 
                                       std::min(featPos.x - ((int)vec[f].d)*2 + m_deltaDisp_i + subPixel_i, imgSize.width - 1 - hMask.width ) );

                     if (rlimit.max - rlimit.min > 1)
                     {
                        S2D<int> dlimit ( featPos.x - rlimit.max,
                                          featPos.x - rlimit.min );

                        float minZssdScore_f = std::numeric_limits<float>::max();
                        float ssdScore_f     = std::numeric_limits<float>::max();
                        int   bestDisp_i     = INVALID_DISP;
                        float maskVar_f      = std::numeric_limits<float>::max();
                        
                        for (int colrPos_i = rlimit.min; colrPos_i <= rlimit.max; ++colrPos_i)
                        {
                           int d = featPos.x - colrPos_i;
                           /// Column limits for current disparity.
                           S2D<int> uRlimit ( colrPos_i - hMask.width, 
                                              colrPos_i + hMask.width );
                            
                           if ( uRlimit.min < 0 ||
                                uRlimit.max >= imgSize.width )
                           {
                              //getchar();
                           }                            
                            
                           float sumL_f    = 0.f;
                           float sumR_f    = 0.f;
                           float sumSqR_f  = 0.f;
                           float sumSq_f   = 0.f;
                           float varR_f    = 0.f;
                            
                           /// Correlate windows now.
                           for (int i = vlimit.min; i <= vlimit.max; ++i)
                           {
                              uint8_t *ptrL_p = &imgL.at<uint8_t>(i,uLlimit.x);
                              uint8_t *ptrR_p = &imgR.at<uint8_t>(i,uRlimit.x);
                                
                              for (int j = 0; j < mask.width; ++j, ++ptrL_p, ++ptrR_p)
                              {
                                 float diff_f = (float)*ptrL_p - (float)*ptrR_p;
                                 sumL_f  += *ptrL_p;
                                 sumR_f  += *ptrR_p;
                                 sumSq_f += diff_f * diff_f;
                                    
                                 if ( m_checkVars_b )
                                    sumSqR_f += (float)( *ptrR_p * *ptrR_p);
                              }
                           }
                            
                           if ( m_checkVars_b )
                           {
                              float sqR_f  = sumR_f * sumR_f / maskSize_i;
                              varR_f = (sumSqR_f - sqR_f) / maskSize_i;
                           }

                           float avgDiff_f   = (sumL_f - sumR_f);
                           float sqDiff_f    = avgDiff_f * avgDiff_f / maskSize_i;
                           float scoreZssd_f = sumSq_f - sqDiff_f;
                            
                           /// Store scoreZssd in vector
                           scores_p[d] = scoreZssd_f;
                            
                           if (scoreZssd_f < minZssdScore_f)
                           {
                              minZssdScore_f = scoreZssd_f;
                              ssdScore_f     = sumSq_f;
                              bestDisp_i     = d;
                              maskVar_f      = varR_f;
                           }

                           // If already evaluated at least 3 positions and 
                           // local minima found not at right border, then break.
                           if ( colrPos_i >= rlimit.min+2 && bestDisp_i != d)
                              break;
                        }

                        minZssdScore_f /= maskSize_i;
                        ssdScore_f     /= maskSize_i;                               

                        if ( bestDisp_i     > dlimit.min - (1-subPixel_i) &&
                             bestDisp_i     < dlimit.max + (1-subPixel_i) &&
                             minZssdScore_f < m_maxZssd_f  && 
                             ssdScore_f     < m_maxSsd_f   && 
                             ( !m_checkVars_b || 
                               ( maskVar_f > m_minVar_f ) ) )
                        {
                           vec[f].d = bestDisp_i;

                           if ( f_level_i == 0 )
                           {
                              // Calculate sub-pixel estimation.
                                
                              const float * const cost_p = scores_p+bestDisp_i;
                              float nom_f    = 0.5 * ( cost_p[1] - cost_p[-1] );
                              float denom_f  = 2.0 * cost_p[0] - ( cost_p[1] + cost_p[-1] );           
                              float correction_f = nom_f/denom_f;
                                
                              if (correction_f <= 0.5 && correction_f >= -0.5)
                              {
                                 vec[f].d += correction_f + m_dispOffset_f;
                              }
                              else
                              {
                                 m_rejCause_v[f] = ERC_OTHER;
                                 vec[f].d = INVALID_DISP;
                              }                              
                           }

                           disp.at<float>(featPos.y,featPos.x) = vec[f].d;     
                        }
                        else
                        {

                         vec[f].d = INVALID_DISP;
                           if (!( bestDisp_i     > dlimit.min - (1-subPixel_i) &&
                                  bestDisp_i     < dlimit.max + (1-subPixel_i)) )
                              m_rejCause_v[f] = ERC_LIMIT;
                           else if (!(minZssdScore_f < m_maxZssd_f))
                              m_rejCause_v[f] = ERC_LARGEZSSD;
                           else if (!(ssdScore_f < m_maxSsd_f))
                              m_rejCause_v[f] = ERC_LARGESSD;
                           else 
                              m_rejCause_v[f] = ERC_LOWVARIANCE;
                        }
                     }
                     else
                     {   
                        m_rejCause_v[f] = ERC_INVALIDDISP;
                        vec[f].d = INVALID_DISP;
                     }  

                  }
                  else // Correlation not possible because feature is at border of image
                  {
                     m_rejCause_v[f] = ERC_ATBORDER;
                     vec[f].d = INVALID_DISP;
                  }                
               }
               else // Correlation not possible because feature is at border of image
               {
                  vec[f].d = INVALID_DISP;
                  m_rejCause_v[f] = ERC_ATBORDER;
               }                
            }                       
            else // Correlation has already been computed.
            {
               vec[f].d = disp.at<float>(featPos.y,featPos.x);
            }   
         }
         else // Outside bounds.
         {
            m_rejCause_v[f] = ERC_OTHER;
            vec[f].d  = INVALID_DISP;
         }                
      }
   }


   return true;
}

/// Show event.
bool CFeatureStereoOp::show()
{
   if ( m_compute_b )
   {
      CFeatureVector * featureVector_p = getInput<CFeatureVector> ( m_featPointVector_str );
      
      CDrawingList *list_p;    

      const float srcWidth_f  = getScreenSize().width;
      const float srcHeight_f = getScreenSize().height;

      if ( m_lImg.cols > 0 && featureVector_p )
      {
         CFeatureVector &vec = *featureVector_p;

         if (m_rejCause_v.size() < vec.size())
            m_rejCause_v.resize( vec.size(), ERC_NONE );

         for (int im = 0; im < 2; ++im)
         {
            if (im == 0 )
               list_p = getDrawingList ("Left Image Features");
            else
               list_p = getDrawingList ("Right Image Features");   

            list_p -> clear();
            
            if ( list_p->isVisible() )
            {
               CDrawingList *imgdl = getInput<CDrawingList>((im == 0?m_idLeftImage_str:m_idRightImage_str) + " Drawing List");
               if ( ( (im == 0 && m_showLeftImage_b) ||
                      (im == 1 && m_showRightImage_b) ) )
               {
               if (imgdl)
                  list_p->addDrawingList( *imgdl );
                   else
                       list_p->addImage(getInput<cv::Mat>(im==0?m_idLeftImage_str:m_idRightImage_str, cv::Mat()), 0, 0, srcWidth_f, srcHeight_f);
               }

               list_p -> setLineColor ( SRgb(255,255,0 ) );
                
               float scaleX_f = srcWidth_f  / m_lImg.cols;
               float scaleY_f = srcHeight_f / m_lImg.rows;
               S2D<float> p0, p1, q1;
                
               C3DVector p;
               const float refFeatureSize_f = 0.1;

               SRgb color;
               for (int i = 0; i < (int)vec.size(); ++i)
               {
                  S2D<float> pr ( vec[i].u * scaleX_f,
                                  vec[i].v * scaleY_f );
                  float featureSize_f = 20 * refFeatureSize_f; //vec[i].d * refFeatureSize_f;
                  
                  if (im == 1)
                     pr.x = pr.x - scaleX_f * vec[i].d;
                  
                  if ( vec[i].d >= 0 )
                  {
                     m_ceDistance.colorFromValue ( vec[i].d,
                                                   color );

                     list_p -> setLineWidth ( 1 );
                     list_p -> setLineColor ( color );
                     list_p -> setFillColor ( SRgba(color, 120 ) );
                     list_p -> addFilledSquare ( pr, featureSize_f );
                  }
                  else
                  {
                     color = ( (m_rejCause_v[i] == ERC_NONE       )?SRgb(255,255,255): // White
                               (m_rejCause_v[i] == ERC_LIMIT      )?SRgb(255,000,000): // Red
                               (m_rejCause_v[i] == ERC_ATBORDER   )?SRgb(000,000,255): // Blue
                               (m_rejCause_v[i] == ERC_INVALIDDISP)?SRgb(000,255,000): // Green
                               (m_rejCause_v[i] == ERC_LOWVARIANCE)?SRgb(255,000,255): // Magenta/Pink
                               (m_rejCause_v[i] == ERC_LARGEZSSD  )?SRgb(255,255,000): // Yellow
                               (m_rejCause_v[i] == ERC_LARGESSD   )?SRgb(000,255,255): // Cyan
                               SRgb(000,000,000));

                     list_p -> setLineWidth ( 2 );
                     list_p -> setLineColor ( color );
                     list_p -> addCross ( pr.x, pr.y, featureSize_f );
                  }
                     
               }
            }
         }
      }
    
      CImagePyramid *pyrs_p[2] = { &m_pyrLeft, &m_pyrRight };

      for (int l = 0; l < 2; ++l)
      {
         float scaleX_f = srcWidth_f  / pyrs_p[0]->getWidth();
         float scaleY_f = srcHeight_f / pyrs_p[0]->getHeight();

         if (l == 0)
            list_p = getDrawingList ("Left Image Pyramid");
         else
            list_p = getDrawingList ("Right Image Pyramid");
            
         list_p -> clear();
        
         if ( list_p -> isVisible() )
         {
            float x = 0.;
            
            for (unsigned int i = 1; i < m_levels_ui; ++i)
            {
               cv::Mat img = pyrs_p[l] -> getLevelImage(i);
               float w_f = img.cols * scaleX_f;
               float h_f = img.rows * scaleY_f;

               list_p -> addImage ( img, 
                                    x, 0, 
                                    w_f,
                                    h_f );
               x += w_f;
            }
         }
      }

      if ( m_3dViewer_p && m_show3DPoints_b && m_compute_b)
         show3D();
   }
   
   return COperator::show();
}

void CFeatureStereoOp::show3D()
{
#if defined HAVE_QGLVIEWER

   CStereoCamera *     camera_p = getInput<CStereoCamera> ( "Rectified Camera" );
   
   cv::Mat m_lImg = getInput<cv::Mat>( m_idLeftImage_str, cv::Mat() );

   if ( !camera_p || m_lImg.cols == 0 || m_lImg.data == 0)
      return;

   m_3dViewer_p -> setPointSize(3);
    
   int w_i = m_dispImg.cols;
   int h_i = m_dispImg.rows;

   m_3dViewer_p -> clear();

   for (int i = 0; i < h_i; ++i)
   {
      float *      disp_p = &m_dispImg.at<float>(i, 0);
      uint8_t *    limg_p = &m_lImg.at<uint8_t>(i, 0);
	
      for ( int j = 0; j < w_i; ++j, ++disp_p, ++limg_p )
      {
         if (*disp_p > 0  )
         {
            C3DVector point;
            camera_p -> image2World ( j, i, *disp_p, point );
            SRgb color(*limg_p, *limg_p, *limg_p );
            m_3dViewer_p -> addPoint ( point, color );
         }
      }
   }        
#endif
}


/// Init event.
bool CFeatureStereoOp::initialize()
{
   return COperator::initialize();
}

/// Reset event.
bool CFeatureStereoOp::reset()
{
   return COperator::reset();
}

bool CFeatureStereoOp::exit()
{
   return COperator::exit();
}

void 
CFeatureStereoOp::keyPressed ( CKeyEvent * f_event_p )
{
   return COperator::keyPressed ( f_event_p );    
}

bool
CFeatureStereoOp::setPyramidParams ( unsigned int f_levels_ui )
{
   if ( f_levels_ui != m_pyrLeft.getLevels() )
   {
      m_levels_ui = f_levels_ui;
      
      m_pyrLeft.setLevels ( f_levels_ui );
      m_pyrRight.setLevels ( f_levels_ui );
      m_pyrDisp.setLevels ( f_levels_ui );
      
      m_pyrLeft.clear();
      m_pyrRight.clear();
   }

   return true;
}

bool
CFeatureStereoOp::setLevels ( unsigned int f_levels_ui )
{
   return setPyramidParams ( f_levels_ui );
}

unsigned int
CFeatureStereoOp::getLevels ( ) const
{
   return m_levels_ui;
}

