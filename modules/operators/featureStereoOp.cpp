/**
*******************************************************************************
*
* @file featureStereoOp.cpp
*
* \class CFeatureStereoOp
* \date  Tue Oct 13, 2009
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief Framework operator class for computing ego-motion from 3D point
*        correspondences between current and previous frame.
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "featureStereoOp.h"
#include "drawingList.h"
#include "feature.h"
#include "stereoCamera.h"
#include "glViewer.h"

/// Dependeing on the parameters might be better not to use OPENMP here.
/// When using large masks, it is better to use openmp. 
#include <omp.h>
//undef _OPENMP

#include "ceParameter.h"

using namespace QCV;

static const float INVALID_DISP=std::numeric_limits<float>::min();

/// Constructors.
CFeatureStereoOp::CFeatureStereoOp ( COperator * const f_parent_p, 
                                     const std::string f_name_str  )
   : COperator (                 f_parent_p, f_name_str ),
     m_compute_b (                                false ),
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
     m_show3DPoints_b (                           false )
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
                        S2D<int> (0, 0),
                        false);

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
      
      addDrawingListParameter ( "Right Image Features" );
      
      addDrawingListParameter ( "Left Image Pyramid" );
      
      addDrawingListParameter ( "Right Image Pyramid" );
      
      addColorEncodingParameter (  m_ceDistance,
                                   getParameterSet(),
                                   "Disparity",
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
         cv::Mat imgL = getInput<cv::Mat>( m_idLeftImage_str,  cv::Mat() );
         cv::Mat imgR = getInput<cv::Mat>( m_idRightImage_str, cv::Mat() );
         
        if (imgL.cols > 0 && imgL.rows > 0 && imgL.size() == imgR.size() &&
             imgL.type() == imgR.type() && imgL.type() == CV_8UC1 )
         {
           m_dispImg = cv::Mat(imgL.size(), CV_32FC1);
            
            startClock("Pyramid construction");
            
            setPyramidParams ( m_levels_ui );
        
            /// 1.- Compute Gaussian Pyramids.
           m_pyrLeft.compute  ( imgL );
           m_pyrRight.compute ( imgR );
        
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
                           ::exit(1);
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
                            ( m_checkVars_b && maskVar_f > m_minVar_f ) ) )
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
                        vec[f].d = INVALID_DISP;
                     }
                  }
                  else
                  {
                     vec[f].d = INVALID_DISP;
                  }
               }
            }
            else // Correlation not possible because feature is at border of image
            {
               vec[f].d  = INVALID_DISP;
            }                
         }                       
         else // Correlation has already been computed.
         {
            vec[f].d = disp.at<float>(featPos.y,featPos.x);
         }                
      }
      else // Outside bounds.
      {
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

      if ( vec[f].d != INVALID_DISP )
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

                     /// Disparity limit.
                     S2D<int> rlimit ( std::max(featPos.x - ((int)vec[f].d)*2 - m_deltaDisp_i - subPixel_i, hMask.width), 
                                       std::min(featPos.x - ((int)vec[f].d)*2 + m_deltaDisp_i + subPixel_i, imgSize.width - 1 - hMask.width ) );
                            
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
                        }                       

                        minZssdScore_f /= maskSize_i;
                        ssdScore_f     /= maskSize_i;                               

                        if ( bestDisp_i     > dlimit.min - (1-subPixel_i) &&
                             bestDisp_i     < dlimit.max + (1-subPixel_i) &&
                             minZssdScore_f < m_maxZssd_f  && 
                             ssdScore_f     < m_maxSsd_f   && 
                             ( !m_checkVars_b || 
                               ( m_checkVars_b && maskVar_f > m_minVar_f ) ) )
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
                                 vec[f].d = -1;

                              if (vec[f].d < 0.5) vec[f].d = -1;
                           }

                           disp.at<float>(featPos.y,featPos.x) = vec[f].d;
                                    
                        }
                        else
                        {
                           vec[f].d = -1;
                        }
                     }
                  }
                  else // Correlation not possible because feature is at border of image
                  {
                     vec[f].d = -1;
                  }                
               }
               else // Correlation not possible because feature is at border of image
               {
                  vec[f].d = -1;
               }                
            }                       
            else // Correlation has already been computed.
            {
               vec[f].d = disp.at<float>(featPos.y,featPos.x);
            }   
         }
         else // Outside bounds.
         {
            vec[f].d  = -1;
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
      
      cv::Mat imgL = getInput<cv::Mat>( m_idLeftImage_str, cv::Mat() );
      
      CDrawingList *list_p;    

      const float srcWidth_f  = getScreenSize().width;
      const float srcHeight_f = getScreenSize().height;

      if ( imgL.cols > 0 && featureVector_p )
      {
         CFeatureVector &vec = *featureVector_p;

         for (int im = 0; im < 2; ++im)
         {
            if (im == 0 )
               list_p = getDrawingList ("Left Image Features");   
            else
               list_p = getDrawingList ("Right Image Features");   

            list_p -> clear();
            
            if ( list_p->isVisible() )
            {
               list_p -> setLineColor ( SRgb(255,255,0 ) );
                
               float scaleX_f = srcWidth_f  / imgL.cols;
               float scaleY_f = srcHeight_f / imgL.rows;
               S2D<float> p0, p1, q1;
                
               C3DVector p;
               float featureSize_f = 2;

               SRgb color;
               for (int i = 0; i < (int)vec.size(); ++i)
               {
                  if ( vec[i].d > 0.5 )
                  {
                     S2D<float> pr ( vec[i].u * scaleX_f,
                                     vec[i].v *  scaleY_f );

                     if (im == 1)
                        pr.x = pr.x - scaleX_f * vec[i].d;

                     m_ceDistance.colorFromValue ( vec[i].d,
                                                   color );

                     list_p -> setLineColor ( color );
                     list_p -> setFillColor ( SRgba(color, 120 ) );

                     list_p -> addFilledSquare ( pr, featureSize_f );
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
   CStereoCamera *     camera_p = getInput<CStereoCamera> ( "Rectified Camera" );
   
   cv::Mat imgL = getInput<cv::Mat>( m_idLeftImage_str, cv::Mat() );

   if ( !camera_p || imgL.cols == 0 || imgL.data == 0)
      return;

   m_3dViewer_p -> setPointSize(3);
    
   int w_i = m_dispImg.cols;
   int h_i = m_dispImg.rows;

   m_3dViewer_p -> clear();

   for (int i = 0; i < h_i; ++i)
   {
      float *      disp_p = &m_dispImg.at<float>(i, 0);
      uint8_t *    limg_p = &imgL.at<uint8_t>(i, 0);
	
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

