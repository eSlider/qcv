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
* \class CKltTrackerOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "kltTrackerOp.h"
#include "paramMacros.h"
#include "drawingList.h"
#include "ceParameter.h"
#include "stereoCamera.h"
#include "rigidMotion.h"

#ifdef USE_GPU
#include "opencv2/gpu/gpu.hpp"
#include "opencv2/nonfree/gpu.hpp"
#endif
using namespace QCV;

/// Constructors.
CKltTrackerOp::CKltTrackerOp ( COperator * const f_parent_p,
                             const std::string f_name_str )
    : COperator (             f_parent_p, f_name_str ),
      m_inpImageId_str (                   "Image 0" ),
      m_featPointVector_str (       "Feature Vector" ),
      m_compute_b (                             true ),
      m_respCE (   CColorEncoding::CET_BLUE2GREEN2RED,
                               S2D<float> ( 0, 200 ) ),
      m_preFilter_b (                          false ),
      m_pFMaskSize_i (                            19 ),
      m_pFClampScale_f (                        10.f ),

      m_numFeatures_i (                         4096 ),
      m_pyrLevels_i (                              8 ),
      m_minDistance_i (                            4 ),
      m_adaptiveDistance_b (                   false ),
      m_checkCollisions_b (                     true ),
      m_maxSqDist4Collision_f (                  9.f ),
      m_kernelSize_i (                             3 ),
      m_pyrLKEpsilon_f (                          1. ),
      m_pyrLKMaxCount_i (                        100 ),
      m_usePrediction_b (                       true ),

      m_minEigenvalue_f (                     1e-05f ),
      m_useSubPix_b (                           true ),
      m_subPixBlockSize_i (                        3 ), 
      m_subPixIterNum_i (                        100 ),
      m_subPixEPS_f (                          0.001 ),
      m_detectGFTT_b (                          true ),
      m_harrisK_d (                             0.04 ),
      m_minHarrisResponse_d (                  0.001 ),
      m_selectedIdx_i (                           -1 )

{
    registerDrawingLists(  );
    registerParameters (  );
}

void
CKltTrackerOp::registerDrawingLists(  )
{
    bool showImages_b = (getParentOp() == NULL);
    
    registerDrawingList ( "Current Image",
                          S2D<int> (0, 0),
                          showImages_b);    

    registerDrawingList ( "Previous Image",
                          S2D<int> (0, 0),
                          showImages_b);    

    registerDrawingList ( "Keypoints",
                          S2D<int> (0, 0),
                          showImages_b);    

    registerDrawingList ( "Matches",
                          S2D<int> (0, 0),
                          true);    

    registerDrawingList ( "Feature Mask",
                          S2D<int> (0, 0),
                          showImages_b);
}

void
CKltTrackerOp::registerParameters( )
{
    BEGIN_PARAMETER_GROUP("Input/Output", false, SRgb(220,0,0));
      ADD_STR_PARAMETER( "Left Image Id", 
                         "String Id of left image.",
                         m_inpImageId_str,
                         this,
                         InputImageIdStr, 
                         CKltTrackerOp );
      
      ADD_STR_PARAMETER( "Output Feature Vector Id", 
                         "Id of feature point vector.",
                         m_featPointVector_str,
                         this,
                         FeaturePointVectorId, 
                         CKltTrackerOp );

      ADD_BOOL_PARAMETER( "Pre-Filter?", 
                          "Apply mask normalization to input image.",
                          m_preFilter_b,
                          this,
                          PreFilter, 
                          CKltTrackerOp );

      ADD_INT_PARAMETER( "Pre-Filter Mask Size", 
                         "Mask size for box-filter normalization [px]",
                         m_pFMaskSize_i,
                         this,
                         PreFilterMaskSize, 
                         CKltTrackerOp );

      ADD_FLOAT_PARAMETER( "Pre-Filter Clamp Scale", 
                           "Pre-filter clamp scale",
                           m_pFClampScale_f,
                           this,
                           PreFilterClampScale, 
                           CKltTrackerOp );
    END_PARAMETER_GROUP;     
     
    BEGIN_PARAMETER_GROUP("Computation", false, SRgb(220,0,0));
      ADD_BOOL_PARAMETER ( "Compute",
                           "Compute SURF features on the input image?.",
                           m_compute_b,
                           this,
                           Compute,
                           CKltTrackerOp );
      
      ADD_INT_PARAMETER( "Maximum Number of Features",
                         "Maximum Number of Features to ouptut.",
                         m_numFeatures_i,
                         this,
                         NumFeatures,
                         CKltTrackerOp );

    BEGIN_PARAMETER_GROUP("Tracking", false, SRgb(220,0,0));
      
      ADD_INT_PARAMETER( "KLT Pyramid levels",
                         "Pyramid levels for klt tracker.",
                         m_pyrLevels_i,
                         this,
                         PyramidLevels,
                         CKltTrackerOp );

      ADD_BOOL_PARAMETER ( "Check Collissions",
                           "Check for feature collissions.",
                           m_checkCollisions_b,
                           this,
                           CheckCollisions,
                           CKltTrackerOp );

      ADD_FLOAT_PARAMETER ( "Sq. collision distance",
                            "Squared distance between two tracked features to detect collision [px].",
                            m_maxSqDist4Collision_f,
                            this,
                            MaxSqDist4Collision,
                            CKltTrackerOp );

     ADD_INT_PARAMETER( "Kernel Size",
                         "Kernel size parameter for KLT tracking",
			 m_kernelSize_i,
                         this,
                         KernelSize,
                         CKltTrackerOp );

     ADD_FLOAT_PARAMETER( "Pyr LK Epsilon",
                          "Epsilon for iterative LK algorithm",
                          m_pyrLKEpsilon_f,
                          this,
                          PyrLKEpsilon,
                          CKltTrackerOp );

     ADD_INT_PARAMETER( "Pyr LK Max Count",
                        "Max number of iteratinos for LK algorithm",
                        m_pyrLKMaxCount_i,
                        this,
                        PyrLKMaxCount,
                        CKltTrackerOp );

      ADD_BOOL_PARAMETER( "Use Prediction",
                          "Use prediction from ego-motion to estimate future feature position",
                          m_usePrediction_b,
                          this,
                          UsePrediction,
                          CKltTrackerOp );

    END_PARAMETER_GROUP;
    BEGIN_PARAMETER_GROUP("Detection", false, SRgb(220,0,0));

      ADD_INT_PARAMETER( "Min Feature Distance",
                         "Min distance between detected features.",
                         m_minDistance_i,
                         this,
                         MinDistance,
                         CKltTrackerOp );

      ADD_BOOL_PARAMETER( "Adaptive Distance",
                          "Min distance between detected features.",
                          m_adaptiveDistance_b,
                          this,
                          AdaptiveDistance,
                          CKltTrackerOp );
      
      ADD_BOOL_PARAMETER( "Detect GFTT",
                           "If true, detects thresholding min eigenvalue. Otherwise Harris.",
                           m_detectGFTT_b,
                           this,
                           DetectGFTT,
                           CKltTrackerOp );      

      ADD_DOUBLE_PARAMETER( "Harris K",
                            "K parameter value for Harris corner detector.",
                            m_harrisK_d,
                            this,
                            HarrisK,
                            CKltTrackerOp );      

      ADD_FLOAT_PARAMETER( "Min Eigenvalue",
                           "Min eigenvalue to consider for detection.",
                           m_minEigenvalue_f,
                           this,
                           MinEigenvalue,
                           CKltTrackerOp );      

      ADD_DOUBLE_PARAMETER( "Min Harris Response",
                            "Min Harris response to sort values.",
                            m_minHarrisResponse_d,
                            this,
                            MinHarrisResponse,
                            CKltTrackerOp );      

      ADD_BOOL_PARAMETER( "Apply SPE after GFTT",
                          "Use Sub Pixel Estimation after GoodFeaturesToTrack",
                          m_useSubPix_b,
                          this,
                          UseSubPix,
                          CKltTrackerOp );
      
      ADD_INT_PARAMETER( "SPE Block Size",
                         "Block Size parameter of OpenCV cornerSubPix",
                         m_subPixBlockSize_i,
                         this,
                         SubPixBlockSize,
                         CKltTrackerOp );      

      ADD_INT_PARAMETER( "SPE Max Iterations",
                         "Maximum number of iterations for Sub Pixel Estimation",
                         m_subPixIterNum_i,
                         this,
                         SubPixIterNum,
                         CKltTrackerOp );
      
      ADD_FLOAT_PARAMETER( "SPE epsilon",
                           "Epsilon to stop estimation",
                           m_subPixEPS_f,
                           this,
                           SubPixEPS,
                           CKltTrackerOp );      
    END_PARAMETER_GROUP;
      
    BEGIN_PARAMETER_GROUP("Display", false, SRgb(220,0,0));

      addDrawingListParameter ( "Previous Image" );

      addDrawingListParameter ( "Current Image" );

      addDrawingListParameter ( "Keypoints" );

      addDrawingListParameter ( "Matches" );

      addDrawingListParameter ( "Feature Mask" );

      addColorEncodingParameter (  m_respCE,
                                   getParameterSet(),
                                   "Vector Length",
                                   "Color encoding for the optical vector length." );

    END_PARAMETER_GROUP;
}

/// Virtual destructor.
CKltTrackerOp::~CKltTrackerOp ()
{
}

/// Cycle event.
bool CKltTrackerOp::cycle()
{
   if (m_currImg.cols > 0)
      m_currImg.copyTo(m_prevImg);

   cv::Mat img;

   if ( m_compute_b )
      img = getInput<cv::Mat>(m_inpImageId_str, cv::Mat() ); 

   if ( m_compute_b && img.cols > 0)
   {
      size_t imgNr_u = getInput<int> ("Frame Number", 0 );     

      if (m_preFilter_b)
      {
         startClock ("Pre-filtering");            
         int ddepth = CV_32F;
         cv::Mat imgf,imgf2;
         //cv::Sobel( img,  m_currImg,  ddepth, 1, 1, 5, 255.f/500.f, 127.5f, cv::BORDER_DEFAULT );

         cv::boxFilter( img, imgf, ddepth, cv::Size(m_pFMaskSize_i,m_pFMaskSize_i), cv::Point(-1,-1), true, cv::BORDER_DEFAULT );
         img.convertTo ( imgf2, CV_32F, 1., 0);
         imgf -= imgf2;
         imgf.convertTo ( m_currImg, CV_8U, m_pFClampScale_f, 127);
         stopClock ("Pre-filtering");
      }
      else
         img.copyTo(m_currImg);

      startClock ("Copy Feature Vector");

      m_prevFeatureVector = m_featureVector;

      stopClock ("Copy Feature Vector");

      int halfMaskSize_i = m_kernelSize_i / 2;
      /// Get input from parent.

      if ( m_prevImg.cols > 0  )
      {
         startClock ("Collision Detection");            
           
         for (int i = 0; i < m_numFeatures_i; ++i)
         {
            if ( m_featureVector[i].state == SFeature::FS_TRACKED || 
                 m_featureVector[i].state == SFeature::FS_NEW )
            {
               if (m_checkCollisions_b)
               {
                  int j;
                  for (j = i+1; j < m_numFeatures_i; ++j)
                  {
                     if ( m_featureVector[j].state == SFeature::FS_TRACKED || 
                          m_featureVector[j].state == SFeature::FS_NEW )
                     {
                        float diffx_f = m_featureVector[i].u - m_featureVector[j].u;
                        float diffy_f = m_featureVector[i].v - m_featureVector[j].v;
                        float sqDiff_f = diffx_f * diffx_f + diffy_f * diffy_f;
                        
                        if ( sqDiff_f <= m_maxSqDist4Collision_f )
                        {
			  m_featureVector[i].clear();
                          
                          const bool removeByAge_b = true;
                          
                          if ( (removeByAge_b && m_featureVector[i].t < m_featureVector[j].t) ||
                               (!removeByAge_b && m_featureVector[i].e < m_featureVector[j].e ) )
                             m_featureVector[i].clear();
                          else
                             m_featureVector[j].clear();

                          break;
                       }
                     }
                  }
               }
            }
         }
         stopClock ("Collision Detection");

         startClock ("Build tracking list");
         std::vector<cv::Point2f> featprev_ocv;
         std::vector<cv::Point2f> featcurr_ocv;
         std::vector<size_t>      mapping_v;

         SRigidMotion *   motion_p     = getInput<SRigidMotion>  ( "Predicted Motion" );
         CStereoCamera *  stCamera_p   = getInput<CStereoCamera> ( "Rectified Camera" );
         CCamera *        monoCamera_p = NULL;

         if (!stCamera_p)
            monoCamera_p = getInput<CCamera> ( "Rectified Camera" );

         bool predict_b = m_usePrediction_b && (stCamera_p || monoCamera_p) && motion_p;
         
         std::vector<cv::Point2f> featpred; // FOR PAPER
         for (size_t i = 0; i < m_numFeatures_i; ++i)
         {
            CStereoCamera cam;
            if (monoCamera_p)
            {
               ((CCamera)cam) = *monoCamera_p;
               cam.setBaseline(1);
            }
            else
               cam = *stCamera_p;
            
            if ( m_featureVector[i].state == SFeature::FS_TRACKED || 
                 m_featureVector[i].state == SFeature::FS_NEW )
            {
               featprev_ocv.push_back ( cv::Point2f(m_featureVector[i].u, m_featureVector[i].v) );
               cv::Point2f curr (m_featureVector[i].u, m_featureVector[i].v);
               
               if ( m_featureVector[i].d > 0 && predict_b )
               {
                  C3DVector prediction ( m_featureVector[i].u,
                                         m_featureVector[i].v,
                                         m_featureVector[i].d );

                  C3DVector p;
                  if ( cam.image2Local ( prediction,
                                               p ) )
                  {
                     C3DVector p2 = motion_p->rotation * p + motion_p->translation;
                     if ( cam.local2Image ( p2, p ) )
                     {
                        prediction = p;
                        curr = cv::Point2f(p.x(), p.y());
                     }
                  }
               }
               featpred.push_back (curr); // FOR PAPER
               featcurr_ocv.push_back ( curr );
               mapping_v.push_back(i);
            }
         }
         stopClock ("Build tracking list");                
         
         // KLT tracker using OpenCV
         startClock ("OpenCV KLT");

         // set window size and criteria
         std::vector<uint8_t> status_ocv;
         std::vector<float> error_ocv;

#ifdef USE_GPU
         cv::Mat tmpFeatPrev = cv::Mat(1, featprev_ocv.size(), CV_32FC2, &featprev_ocv[0]);
         cv::Mat tmpFeatCurr = cv::Mat(1, featcurr_ocv.size(), CV_32FC2, &featcurr_ocv[0]);

         cv::gpu::GpuMat gpuPrevImg(m_prevImg);
         cv::gpu::GpuMat gpuCurrImg(m_currImg);
         cv::gpu::GpuMat gpuFeatPrev(tmpFeatPrev);
         cv::gpu::GpuMat gpuFeatCurr(tmpFeatCurr);
         cv::gpu::GpuMat gpuStatus;
         cv::gpu::GpuMat gpuErrors;


         // sparse lk optical flow
         cv::gpu::PyrLKOpticalFlow    gpuPyrLK;
         
         gpuPyrLK.winSize.width  = m_kernelSize_i;
         gpuPyrLK.winSize.height = m_kernelSize_i;
         gpuPyrLK.maxLevel       = m_pyrLevels_i;
         gpuPyrLK.iters          = m_pyrLKMaxCount_i;
         gpuPyrLK.derivLambda    = m_pyrLKEpsilon_f;
         gpuPyrLK.useInitialFlow = m_usePrediction_b;
         
         cv::gpu::GpuMat d_nextPts;
         cv::gpu::GpuMat d_status;
         cv::gpu::GpuMat d_err;
         
         gpuPyrLK.sparse(gpuPrevImg, 
                         gpuCurrImg, 
                         gpuFeatPrev, 
                         gpuFeatCurr, 
                         gpuStatus, 
                         &gpuErrors);
         
         tmpFeatPrev = cv::Mat(gpuFeatPrev);
         tmpFeatCurr = cv::Mat(gpuFeatCurr);
         
         for (int i = 0 ; i < tmpFeatPrev.cols; ++i)
         {
             featprev_ocv[i] = tmpFeatPrev.at<cv::Point2f>(i);
         }
         
         for (int i = 0 ; i < tmpFeatCurr.cols; ++i)
         {
             featcurr_ocv[i] = tmpFeatCurr.at<cv::Point2f>(i);
         }
         
         cv::Mat tmpStatus ( gpuStatus );
         status_ocv.resize(gpuStatus.cols);
         for (int i = 0 ; i < tmpStatus.cols; ++i)
         {
             status_ocv[i] = tmpStatus.at<uint8_t>(i);
         }

         cv::Mat tmpError ( gpuErrors );
         error_ocv.resize(gpuErrors.cols);
         for (int i = 0 ; i < tmpError.cols; ++i)
         {
             error_ocv[i] = tmpError.at<float>(i);
         }
#else
         cv::TermCriteria criteria_ocv;
         cv::Size winsize_ocv;
         winsize_ocv.width     = m_kernelSize_i;
         winsize_ocv.height    = m_kernelSize_i;
         criteria_ocv.epsilon  = m_pyrLKEpsilon_f;
         criteria_ocv.maxCount = m_pyrLKMaxCount_i; 
         criteria_ocv.type = (CV_TERMCRIT_EPS | CV_TERMCRIT_ITER);

         int flags = (m_usePrediction_b)?cv::OPTFLOW_USE_INITIAL_FLOW:0;
         cv::calcOpticalFlowPyrLK(m_prevImg, m_currImg, 
                                  featprev_ocv, featcurr_ocv,
                                  status_ocv, error_ocv, 
                                  winsize_ocv, m_pyrLevels_i,
                                  criteria_ocv, flags);
         
#endif    
         for (int i = 0; i < featprev_ocv.size(); ++i) 
         {
            int j = mapping_v[i];
            if (status_ocv[i] )
            {
 // FOR PAPER
               C3DVector d1(featcurr_ocv[i].x-featpred[i].x, 
                            featcurr_ocv[i].y-featpred[i].y,0.);
               C3DVector d2(featcurr_ocv[i].x-featprev_ocv[i].x, 
                            featcurr_ocv[i].y-featprev_ocv[i].y,0.);
               //if (predict_b)
               //   printf("Measurement  for %i is %f %f preddist %f prevdist %f \n", i, featcurr_ocv[i].x, featcurr_ocv[i].y, d1.magnitude(), d2.magnitude() );

               int j = mapping_v[i];
               m_featureVector[j].u      = featcurr_ocv[i].x;
               m_featureVector[j].v      = featcurr_ocv[i].y;
               m_featureVector[j].d      = -1;
               m_featureVector[j].f      = imgNr_u;
               m_featureVector[j].e      = error_ocv[i];
               m_featureVector[j].state  = SFeature::FS_TRACKED;
               ++m_featureVector[j].t;
            }
            else
               m_featureVector[j].state  = SFeature::FS_LOST;               
         }
      }
      stopClock ("OpenCV KLT");

      startClock ("Select Good Features");
      selectGoodFeatures();
      stopClock ("Select Good Features");
      
      registerOutput<cv::Mat>("KltTrackerOp Previous Image", &m_prevImg );      
      registerOutput<cv::Mat>("KltTrackerOp Current Image",  &m_currImg );
      registerOutput<CFeatureVector>(m_featPointVector_str, &m_featureVector );      
   }
   
   return COperator::cycle();
}

/// Select good features to track.
void
CKltTrackerOp::selectGoodFeatures()
{
    int downScale_i = 2; // \todo: make parameter
   startClock ("Select Good Features - Occupancy Mask Update");
   /// First pass: update occupancy mask.
    m_featureMask = cv::Mat(m_currImg.rows/downScale_i, m_currImg.cols/downScale_i, CV_8U, cv::Scalar(0));

   for (int i = 0; i < m_featureVector.size(); ++i)
   {
      if ( m_featureVector[i].state == SFeature::FS_TRACKED || 
           m_featureVector[i].state == SFeature::FS_NEW )
        {
            int minDist_i;
            if  ( m_adaptiveDistance_b )
      {
         C3DVector p (m_featureVector[i].u - m_currImg.cols/2.,
                      m_featureVector[i].v - m_currImg.rows/2., 0.);

            minDist_i = std::min(std::max(1, int(m_minDistance_i * p.magnitude()/(m_currImg.cols/2.))), m_minDistance_i);
            }
         else
            minDist_i = m_minDistance_i;
         
            minDist_i /= downScale_i;

            int fx = int(m_featureVector[i].u + .5)/downScale_i;
            int fy = int(m_featureVector[i].v + .5)/downScale_i;
                
         /// Mark region in mask image.
         int minK = std::max(fy - minDist_i, 0);
         int maxK = std::min(fy + minDist_i, m_featureMask.rows-1);
         int minL = std::max(fx - minDist_i, 0);
         int maxL = std::min(fx + minDist_i, m_featureMask.cols-1);
                
         for (int k = minK; k<=maxK; ++k)
         {
            unsigned char *ptr = &m_featureMask.at<uint8_t>(k,minL);
            for (int l = minL; l<=maxL; ++l, ++ptr)
               *ptr = 255;
         }
      }
   }

   stopClock ("Select Good Features - Occupancy Mask Update");
    startClock ("Select Good Features - Scale Image");
    cv::Mat currImg;
    if (downScale_i > 1)
    {
        cv::resize(m_currImg, currImg, m_featureMask.size() );
    }
    else
        currImg = m_currImg;
        
   startClock ("Select Good Features - Build corner reponse image");

   if (m_detectGFTT_b)
   {
        cv::cornerMinEigenVal( currImg,
                             m_eigenImg,
                             m_kernelSize_i,
                             m_kernelSize_i);
   }
   else
   {
        cv::cornerHarris( currImg,
                        m_eigenImg,
                        m_kernelSize_i,
                        3,
                        m_harrisK_d);
   }
      
   stopClock ("Select Good Features - Build corner reponse image");

   startClock ("Select Good Features - Sort Eigenvalues");
   /// Order eigenvalues now.
   m_eigenvalueVector.clear();

   float threshold_f = m_detectGFTT_b?m_minEigenvalue_f:m_minHarrisResponse_d;

   for (int i = 2; i < (int)m_eigenImg.rows - 2; ++i)
   {
      float * eigenvalue_p   = &m_eigenImg.at<float>(i,2);
      unsigned char * mask_p = &m_featureMask.at<uint8_t>(i,2);
           
      for ( int j = 2; j < (int)m_eigenImg.cols - 2; ++j, ++eigenvalue_p, ++mask_p )
      {
         if ( *eigenvalue_p > threshold_f &&
              ! (*mask_p) )
         { 
            m_eigenvalueVector.push_back ( SEigenvalue( j, i, *eigenvalue_p ) );                    
         }
      }            
   }
        
   sort (m_eigenvalueVector.begin(), m_eigenvalueVector.end());

   stopClock ("Select Good Features - Sort Eigenvalues");

   startClock ("Select Good Features - New Feature Selection");

   if (m_featureVector.size() != m_numFeatures_i)
      m_featureVector.resize(m_numFeatures_i);   

   const size_t imgNr_u = getInput<int> ("Frame Number", 0 );     

   /// Second pass: fill empty vector spaces with new features.
   std::vector<cv::Point2f> addedPts_v;
   std::vector<size_t>      addedIdx_v;
   
   int i;
   for (i = 0; i < m_numFeatures_i; ++i)
   {
      if ( m_featureVector[i].state == SFeature::FS_LOST || 
           m_featureVector[i].state == SFeature::FS_UNINITIALIZED )
      {
         /// Check for min distance constraint.
         SEigenvalue eigenvalue;
         eigenvalue.x=-1;
         eigenvalue.y=-1;
                
         unsigned char mask_i = 255;
                
         while (m_eigenvalueVector.size() > 0)
         {
            eigenvalue = m_eigenvalueVector.back();
            mask_i = m_featureMask.at<uint8_t>(eigenvalue.y,eigenvalue.x);
            m_eigenvalueVector.pop_back();
                    
            if ( !mask_i )
               break;
         }

         if ( !mask_i )
         {
                C3DVector p (eigenvalue.x - currImg.cols/2.,
                             eigenvalue.y - currImg.rows/2., 0.);
            
            int minDist_i;
            if  ( m_adaptiveDistance_b )
                    minDist_i = std::min(std::max(1, int(m_minDistance_i * p.magnitude()/(currImg.cols/2.))), m_minDistance_i);
            else
               minDist_i = m_minDistance_i;
                minDist_i /= downScale_i;

            /// Mark region in mask image.
            int minK = std::max(eigenvalue.y - minDist_i, 0);
            int maxK = std::min(eigenvalue.y + minDist_i, (int)m_featureMask.rows-1);
            int minL = std::max(eigenvalue.x - minDist_i, 0);
            int maxL = std::min(eigenvalue.x + minDist_i, (int)m_featureMask.cols-1);
                    
            for (int k = minK; k<=maxK; ++k)
            {
               unsigned char *ptr = &m_featureMask.at<uint8_t>(k,minL);
               for (int l = minL; l<=maxL; ++l, ++ptr)
               {
                  *ptr = 255;
               }
            }

                m_featureVector[i].u     = eigenvalue.x*downScale_i;
                m_featureVector[i].v     = eigenvalue.y*downScale_i;
            m_featureVector[i].d     = -1;
            m_featureVector[i].f     = imgNr_u;
            m_featureVector[i].t     = 0;
            m_featureVector[i].state = SFeature::FS_NEW;

                addedPts_v.push_back(cv::Point2f(m_featureVector[i].u, m_featureVector[i].v));
            addedIdx_v.push_back (i);
         }
         else
         {
            /// there are no more eigenvalues in the vector.
            break;
         }
      }
   }
   
    // Sub-pixel estimation at the original level
   if(m_useSubPix_b && !addedPts_v.empty())
   {
      startClock ("Select Good Features - Sub-pixel");
      cv::cornerSubPix (m_currImg, 
                        addedPts_v,
                        cv::Size (m_subPixBlockSize_i, 
                                  m_subPixBlockSize_i), 
                        cv::Size (-1, -1), 
                        cv::TermCriteria ( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 
                                           m_subPixIterNum_i,
                                           m_subPixEPS_f) );
      
      for(size_t i = 0; i < addedPts_v.size(); ++i) 
      {
         m_featureVector[addedIdx_v[i]].u = addedPts_v[i].x;
         m_featureVector[addedIdx_v[i]].v = addedPts_v[i].y;
      }
      stopClock ("Select Good Features - Sub-pixel");
   }
   
   // Mark the rest of the vector that could not be filled
   // with new features with uninitialized states.
   for (; i < m_numFeatures_i; ++i)
   {
      if ( m_featureVector[i].state == SFeature::FS_LOST || 
           m_featureVector[i].state == SFeature::FS_UNINITIALIZED )
      {
         m_featureVector[i].state = SFeature::FS_UNINITIALIZED;
      }
   }
   stopClock ("Select Good Features - New Feature Selection");
}    


/// Show event.
bool CKltTrackerOp::show()
{
   const bool annotate_b = false;
   
   CDrawingList * list_p = getDrawingList ("Current Image");
   list_p -> clear();

   if ( list_p -> isVisible())
   {
      list_p->addImage ( m_currImg, 0, 0, m_currImg.cols, m_currImg.rows);   
   }

   CDrawingList *list2_p = getDrawingList ("Previous Image");
   list2_p -> clear();

   list_p = getDrawingList ("Matches");
   list_p -> clear();

   if ( list_p -> isVisible())
   {
      for(size_t i = 0; i < m_featureVector.size(); i++) 
      {
         if ( m_selectedIdx_i == i && 
              ( m_featureVector[i].state != SFeature::FS_TRACKED ) )
            m_selectedIdx_i = -1;

         if (m_featureVector[i].state == SFeature::FS_TRACKED)
         {
            double dx = m_featureVector[i].u - m_prevFeatureVector[i].u;
            double dy = m_featureVector[i].v - m_prevFeatureVector[i].v;
            
            double sqDist_d = dx*dx+dy*dy;
            
            if (i == m_selectedIdx_i)
            {
               list_p -> setLineWidth (1);
               list_p -> setLineColor (SRgb(255,255,255));
               list_p -> addCross ( m_featureVector[i].u, 
                                    m_featureVector[i].v,
                                    m_kernelSize_i/2. );
               list_p -> setLineWidth (5);
            }
            else
            {
               list_p -> setLineWidth (2);
            }
            
            SRgb color;
            m_respCE.colorFromValue ( sqrt(sqDist_d),
                                      color );
            
            list_p -> setLineColor (color);
            list_p -> setFillColor (SRgba(color,120));
         
         
            list_p -> addSquare ( m_featureVector[i].u, 
                                  m_featureVector[i].v,
                                  m_kernelSize_i/2. );

            list_p -> addLine( m_featureVector[i].u, 
                               m_featureVector[i].v,
                               m_prevFeatureVector[i].u, 
                               m_prevFeatureVector[i].v );

            if (list2_p->isVisible())
            {
               list2_p -> setLineColor (color);
               list2_p -> setFillColor (SRgba(color,120));
               list2_p -> setLineWidth (2);
               list2_p -> addSquare ( m_prevFeatureVector[i].u, 
                                      m_prevFeatureVector[i].v,
                                      m_kernelSize_i/2. );
               list2_p -> addLine( m_featureVector[i].u, 
                                   m_featureVector[i].v,
                                   m_prevFeatureVector[i].u, 
                                   m_prevFeatureVector[i].v );
            }
            
         }
      }

      list_p->setLineColor ( SRgb(255,0,0));   
      if (annotate_b)
      {  
         list_p -> setLineWidth(10);
         list_p->setLineColor ( SRgb(0,0,0));   
         list_p->addText("Current Image", 20, 420, 20, false);
         list_p->setLineColor ( SRgb(255,0,0));   
         list_p->addText("Current Image", 21, 421, 20, false);
      }
   }
   

   if ( list2_p -> isVisible())
   {
      list2_p->addImage ( m_prevImg, 0, 0, m_prevImg.cols, m_prevImg.rows);   
      if (annotate_b)
      {  
         list2_p -> setLineWidth(10);
         list2_p->setLineColor ( SRgb(0,0,0));   
         list2_p->addText("Previous Image", 20, 420, 20, false);
         list2_p->setLineColor ( SRgb(255,0,0));   
         list2_p->addText("Previous Image", 21, 421, 20, false);
      }
   }

   list_p = getDrawingList ("Feature Mask");
   list_p -> clear();

   if ( list_p -> isVisible())
   {
      list_p->addImage ( m_featureMask, 0, 0, m_currImg.cols, m_currImg.rows);   
   }


   return COperator::show();
}

/// Init event.
bool CKltTrackerOp::initialize()
{
   cv::Mat img = getInput<cv::Mat>(m_inpImageId_str, cv::Mat() ); 

    /// Set the screen size if this is the parent operator.
    if ( img.size().width > 0 &&
         !getParentOp() )
    {
        setScreenSize ( img.size() );
    }

    m_featureVector.clear();
    m_featureVector.resize( m_numFeatures_i );
    m_prevFeatureVector = m_featureVector;

    m_currImg = cv::Mat();
    m_prevImg = cv::Mat();
    
    return COperator::initialize();
}

/// Reset event.
bool CKltTrackerOp::reset()
{
    return COperator::reset();
}

bool CKltTrackerOp::exit()
{
    return COperator::exit();
}

void 
CKltTrackerOp::keyPressed ( CKeyEvent * f_event_p )
{
   if (f_event_p->qtKeyEvent_p -> key() == Qt::Key_U)
   {
      m_selectedIdx_i = -1;
      show();
      updateDisplay();
   }
   return COperator::keyPressed ( f_event_p );    
}

void 
CKltTrackerOp::mouseMoved ( CMouseEvent * f_event_p )
{
   if ( m_currImg.cols <= 0) return COperator::mouseMoved ( f_event_p );

   const double dispWidth_d  = getScreenSize().width;
   const double dispHeight_d = getScreenSize().height;
   
   const CDrawingList *  drawList_p   = getDrawingList ("Matches");
   
   bool mouseOnDL_b   = ( f_event_p -> displayScreen == drawList_p->getPosition() && 
                          drawList_p -> isVisible() );
   
   if ( mouseOnDL_b )
   {
        double imgPosU_d = f_event_p -> posInScreen.x;
        double imgPosV_d = f_event_p -> posInScreen.y;

        if ( (f_event_p -> qtMouseEvent_p -> buttons() & Qt::LeftButton) )
        {
           double minDist_d = 1.e6;

           m_selectedIdx_i = -1;

           for(size_t i = 0; i < m_featureVector.size(); i++) 
           {
              if ( m_featureVector[i].state == SFeature::FS_TRACKED || 
                   m_featureVector[i].state == SFeature::FS_NEW )
              {
                 double dx = m_featureVector[i].u - imgPosU_d;
                 double dy = m_featureVector[i].v - imgPosV_d;

                 double dist_d = sqrt(dx*dx+dy*dy);
                 
                 if (dist_d < 16 && dist_d < minDist_d)
                 {
                    minDist_d = dist_d;
                    m_selectedIdx_i = i;
                 }
              }
           }
        }
        show();
        updateDisplay();
   }
   return COperator::mouseMoved ( f_event_p );
}

