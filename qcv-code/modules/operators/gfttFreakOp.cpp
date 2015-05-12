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
* \class CGfttFreakOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "gfttFreakOp.h"
#include "paramMacros.h"
#include "drawingList.h"
#include "ceParameter.h"
#include "stereoCamera.h"
#include "rigidMotion.h"

#if defined ( _OPENMP )
#include <omp.h>
#endif

using namespace QCV;

/// Constructors.
CGfttFreakOp::CGfttFreakOp ( COperator * const f_parent_p,
                             const std::string f_name_str )
    : COperator (             f_parent_p, f_name_str ),
      m_inpImageId_str (                   "Image 0" ),
      m_featPointVector_str (       "Feature Vector" ),
      m_compute_b (                             true ),
      m_numFeatures_i (                         4096 ),
      m_img (                                        ),
      m_respCE (   CColorEncoding::CET_BLUE2GREEN2RED,
                               S2D<float> ( 0, 200 ) ),
      m_gftt_p (                                NULL ),
      m_freak_p (                               NULL ),
      m_matcher_p (                             NULL ),
      m_cnt_i (                                    0 ),

      m_subPixBlockSize_i (                        3 ), 
      m_useSubPix_b (                           true ),
      m_subPixIterNum_i (                        100 ),
      m_subPixEPS_f (                          0.001 ),
      m_maxDistance_f (                      10000.f ),
      m_maxDistanceForPred_f (               10000.f ),
      m_qualityLevel_d (                     0.00005 ),
      m_qualityFactor_d (                  0.001/50. ),
      m_minDistance_d (                            4 ),
      m_blockSize_i (                              3 ),
      m_useHarris_b (                           true ),
      m_maxDescriptorDistance_f (               1000 ),
      m_correlation_b (                        false ),
      m_normalizedCorr_b (                     false ),
      m_maxFeatPerTile_i (                         2 )
{
    registerDrawingLists(  );
    registerParameters (  );
}

void
CGfttFreakOp::registerDrawingLists(  )
{
    bool showImages_b = (getParentOp() == NULL);
    
    registerDrawingList ( "Keypoints",
                          S2D<int> (0, 0),
                          showImages_b);    

    registerDrawingList ( "Matches",
                          S2D<int> (0, 0),
                          true);    
}

void
CGfttFreakOp::registerParameters( )
{
    BEGIN_PARAMETER_GROUP("Input/Output", false, SRgb(220,0,0));
      ADD_STR_PARAMETER( "Left Image Id", 
                         "String Id of left image.",
                         m_inpImageId_str,
                         this,
                         InputImageIdStr, 
                         CGfttFreakOp );
      
      ADD_STR_PARAMETER( "Output Feature Vector Id", 
                         "Id of feature point vector.",
                         m_featPointVector_str,
                         this,
                         FeaturePointVectorId, 
                         CGfttFreakOp );
    END_PARAMETER_GROUP;     
     
    BEGIN_PARAMETER_GROUP("Computation", false, SRgb(220,0,0));
      ADD_BOOL_PARAMETER ( "Compute",
                           "Compute SURF features on the input image?.",
                           m_compute_b,
                           this,
                           Compute,
                           CGfttFreakOp );

      ADD_BOOL_PARAMETER ( "Correlation",
                           "Use correlation instead of freak.",
                           m_correlation_b,
                           this,
                           Correlation,
                           CGfttFreakOp );

      ADD_BOOL_PARAMETER ( "Normalized Correlation",
                           "Normalized patches for correlation.",
                           m_normalizedCorr_b,
                           this,
                           NormalizedCorr,
                           CGfttFreakOp );
      
      ADD_INT_PARAMETER( "Maximum Number of Features",
                         "Maximum Number of Features to ouptut.",
                         m_numFeatures_i,
                         this,
                         NumFeatures,
                         CGfttFreakOp );

      ADD_INT_PARAMETER( "Max Features Per Tile",
                         "Maximum Number of Features per tile.",
                         m_maxFeatPerTile_i,
                         this,
                         MaxFeaturesPerTile,
                         CGfttFreakOp );

      ADD_FLOAT_PARAMETER( "Squared max distance",
                           "Squared max distance between key points",
                           m_maxDistance_f,
                           this,
                           MaxDist,
                           CGfttFreakOp );

      ADD_FLOAT_PARAMETER( "Squared max distance 4 Pred",
                           "Squared max distance between key points for predicted points",
                           m_maxDistanceForPred_f,
                           this,
                           MaxDistForPrediction,
                           CGfttFreakOp );

      ADD_BOOL_PARAMETER( "Apply SPE after GFTT",
                          "Use Sub Pixel Estimation after GoodFeaturesToTrack",
                          m_useSubPix_b,
                          this,
                          UseSubPix,
                          CGfttFreakOp );
      
      ADD_INT_PARAMETER( "OpenCV Sub Pixel Estimation Block Size",
                         "Block Size parameter of OpenCV cornerSubPix",
                         m_subPixBlockSize_i,
                         this,
                         SubPixBlockSize,
                         CGfttFreakOp );
      
      ADD_INT_PARAMETER( "SPE Max Iterations",
                         "Maximum number of iterations for Sub Pixel Estimation",
                         m_subPixIterNum_i,
                         this,
                         SubPixIterNum,
                         CGfttFreakOp );
      
      ADD_FLOAT_PARAMETER( "OpenCV Sub Pixel Estimation EPS",
                           "Epsilon to stop estimation",
                           m_subPixEPS_f,
                           this,
                           SubPixEPS,
                           CGfttFreakOp );
      
      ADD_DOUBLE_PARAMETER( "GFTT quality level",
                            "quality level parameter of OpenCV GoodFeaturesToTrack",
                            m_qualityLevel_d,
                            this,
                            QualityLevel,
                            CGfttFreakOp );

      ADD_DOUBLE_PARAMETER( "GFTT quality factor",
                            "Factor to multiply the patch std dev [1/luminance]",
                            m_qualityFactor_d,
                            this,
                            QualityFactor,
                            CGfttFreakOp );
      
      ADD_DOUBLE_PARAMETER( "GFTT Min Distance",
                            "Min Distance parameter of OpenCV GoodFeaturesToTrack",
                            m_minDistance_d,
                            this,
                            MinDistance,
                            CGfttFreakOp );
      
      ADD_INT_PARAMETER( "GFTT Block Size",
                         "Block Size parameter of OpenCV GoodFeaturesToTrack",
			 m_blockSize_i,
                         this,
                         BlockSize,
                         CGfttFreakOp );
      
      ADD_BOOL_PARAMETER( "GFTT Use Harris",
                          "Use Harris corner for OpenCV GoodFeaturesToTrack",
                          m_useHarris_b,
                          this,
                          UseHarris,
                          CGfttFreakOp );

      ADD_FLOAT_PARAMETER( "Max Descriptor Distance",
                           "Max distance between two descriptors",
                           m_maxDescriptorDistance_f,
                           this,
                           MaxDescriptorDistance,
                           CGfttFreakOp );

    END_PARAMETER_GROUP;
      
    BEGIN_PARAMETER_GROUP("Display", false, SRgb(220,0,0));

      addDrawingListParameter ( "Keypoints" );

      addDrawingListParameter ( "Matches" );

      addColorEncodingParameter (  m_respCE,
                                   getParameterSet(),
                                   "Vector Length",
                                   "Color encoding for the optical vector length." );

    END_PARAMETER_GROUP;
}

/// Virtual destructor.
CGfttFreakOp::~CGfttFreakOp ()
{
}

/// Cycle event.
bool
CGfttFreakOp::cycle()
{   
   m_img.copyTo(m_prevImg);

   if ( m_compute_b )
      m_img = getInput<cv::Mat>(m_inpImageId_str, cv::Mat() ); 

   if ( m_compute_b && m_img.cols > 0)
   {
      ++m_cnt_i;
      
      size_t imgNr_u = getInput<int> ("Frame Number", 0 );     
      
      /// \todo: check if image is grayscale
      /// \todo: apply gaussian filter if required.
      
      SFeatureData &currFeatures = m_featData[m_cnt_i%2];
      SFeatureData &prevFeatures = m_featData[1-(m_cnt_i%2)];

      /// Iterate through output feature vector to see if flag active and recompute descriptor in such case.
      if (prevFeatures.idx_feature_rev_v.size() > 0)
      {
         std::vector<cv::KeyPoint>                modKeypoints_v;
         cv::Mat                                  modDescriptors;
         std::vector<int> modIdxs_v;

         startClock ("Extractor - FREAK");
  
         // extract new descriptors
         if (!m_correlation_b)
         {
            m_freak_p->compute( m_prevImg, 
                                modKeypoints_v, 
                                modDescriptors );
         }
         else
         {
            modDescriptors = cv::Mat::zeros ( modKeypoints_v.size(), 7*7, CV_8U );
            
            for (int i = 0; i < modKeypoints_v.size(); ++i)
            {
               if ( modKeypoints_v[i].pt.x > 3 &&
                    modKeypoints_v[i].pt.y > 3 &&
                    modKeypoints_v[i].pt.x < m_prevImg.cols - 3 &&
                    modKeypoints_v[i].pt.y < m_prevImg.rows - 3 )
               {
                  cv::Rect roi ( modKeypoints_v[i].pt.x-3,
                                 modKeypoints_v[i].pt.y-3,
                                 7, 7 );
                  cv::Mat img;
                  
                  if (m_normalizedCorr_b)
                     cv::normalize(m_prevImg(roi), img, 0, 255, cv::NORM_MINMAX, CV_8UC1);
                  else
                     m_prevImg(roi).copyTo(img);
                  
                  memcpy(&modDescriptors.at<uint8_t>(i, 0),
                         img.data,
                         7*7);
               }
            }
         }
      
         // Rewrite descriptors for modified features
         for (int i = 0; i < modIdxs_v.size(); ++i)
         {
            memcpy(&prevFeatures.descriptors.at<uint8_t>(modIdxs_v[i], 0),
                   &modDescriptors.at<uint8_t>(i, 0),
                   modDescriptors.cols );
         }         
         
         stopClock ("Extractor - FREAK");  

      }
 
      startClock ("Detector");
      {         
         startClock ("Detector - GFTT");
         currFeatures.keypoints_v.clear();

         const int maxFeatPerTile_i = std::min(std::max(1, m_maxFeatPerTile_i), m_numFeatures_i);
      
         const int tiles_i = sqrt(m_numFeatures_i/maxFeatPerTile_i);
         
         int verTiles_i = int(m_img.rows/(float)tiles_i)*tiles_i+1;
         int horTiles_i = int(m_img.cols/(float)tiles_i)*tiles_i+1;
         
         cv::GoodFeaturesToTrackDetector gftt (maxFeatPerTile_i,
                                               m_qualityLevel_d,
                                               m_minDistance_d, 
                                               m_blockSize_i, 
                                               m_useHarris_b);
         
         std::vector<cv::KeyPoint> points ( maxFeatPerTile_i*2 );
         
         for (int i = 0 ; i < verTiles_i; i += m_img.rows / tiles_i )
         {
            for (int j = 0 ; j < horTiles_i; j += m_img.cols / tiles_i )
            {
               cv::Rect roi ( std::max(0, j-m_blockSize_i), 
                              std::max(0, i-m_blockSize_i), 
                              m_img.cols / tiles_i + m_blockSize_i,
                              m_img.rows / tiles_i + m_blockSize_i );
               
               if (roi.width  + roi.x >= m_img.cols || j == horTiles_i-1) roi.width  = m_img.cols - roi.x - 1;
               if (roi.height + roi.y >= m_img.rows || i == verTiles_i-1) roi.height = m_img.rows - roi.y - 1;
               
               points.clear();
               gftt.detect(m_img(roi), points );
               
               for (int k = 0; k < points.size(); ++k)
               {
                  points[k].pt.x+=j;
                  points[k].pt.y+=i;
               }
               
               currFeatures.keypoints_v.insert( currFeatures.keypoints_v.end(), points.begin(), points.end() );
            }
         }
         stopClock ("Detector - GFTT");
      }
      
      // sub-pixel estimation
      if(m_useSubPix_b && !currFeatures.keypoints_v.empty())
      {
         startClock ("Detector - Sub-pixel");
         cv::vector<cv::Point2f> points;
         points.clear();
         for(size_t i = 0; i < currFeatures.keypoints_v.size(); i++) 
            points.push_back(currFeatures.keypoints_v[i].pt);
         
         cv::cornerSubPix (m_img, 
                           points,
                           cv::Size (m_subPixBlockSize_i, 
                                     m_subPixBlockSize_i), 
                           cv::Size (-1, -1), 
                           cv::TermCriteria ( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 
                                              m_subPixIterNum_i,
                                              m_subPixEPS_f) );
         
         for(size_t i = 0; i < points.size(); i ++) 
         {
            currFeatures.keypoints_v[i].pt = points[i];
         }
         stopClock ("Detector - Sub-pixel");
      }
      stopClock ("Detector");
     
     /// Clear mappings
      currFeatures.idx_feature_v.clear();
      currFeatures.idx_feature_v.resize(m_numFeatures_i, -1 );
      currFeatures.idx_feature_rev_v.clear();
      currFeatures.idx_feature_rev_v.resize(m_numFeatures_i, -1 );

      startClock ("Extractor");  
      
      startClock ("Extractor - FREAK");  
      // extract new descriptors
      if (!m_correlation_b)
      {
         m_freak_p->compute( m_img, 
                             currFeatures.keypoints_v, 
                             currFeatures.descriptors );
      }
      else
      {
         currFeatures.descriptors = cv::Mat::zeros ( currFeatures.keypoints_v.size(), 7*7, CV_8U );

         for (int i = 0; i < currFeatures.keypoints_v.size(); ++i)
         {
            if ( currFeatures.keypoints_v[i].pt.x > 3 && 
                 currFeatures.keypoints_v[i].pt.y > 3 &&
                 currFeatures.keypoints_v[i].pt.x < m_img.cols - 3 &&
                 currFeatures.keypoints_v[i].pt.y < m_img.rows - 3 )
            {
               cv::Rect roi ( currFeatures.keypoints_v[i].pt.x-3, 
                              currFeatures.keypoints_v[i].pt.y-3, 
                              7, 7 );
               cv::Mat img;

               if (m_normalizedCorr_b)
                  cv::normalize(m_img(roi), img, 0, 255, cv::NORM_MINMAX, CV_8UC1);
               else
                  m_img(roi).copyTo(img);

               memcpy(&currFeatures.descriptors.at<uint8_t>(i, 0),
                      img.data,
                      7*7);
            }
         }
      }

      stopClock ("Extractor - FREAK");

      stopClock ("Extractor");

      // For visualization
      m_prevFeatureVector = m_featureVector;
      
      if ( imgNr_u == 0 )
      {
         // register new features
         for(size_t i = 0; i < currFeatures.keypoints_v.size(); i++) 
         {
            m_featureVector[i].state = SFeature::FS_NEW;
            m_featureVector[i].u     = currFeatures.keypoints_v[i].pt.x;
            m_featureVector[i].v     = currFeatures.keypoints_v[i].pt.y;
            m_featureVector[i].d     = -1;
            m_featureVector[i].f     = imgNr_u;
            m_featureVector[i].t     = 0;
            m_featureVector[i].idx   = -1;

            // Update mappings
            currFeatures.idx_feature_v[i]     = i;
            currFeatures.idx_feature_rev_v[i] = i;
         }   
      }
      else
      {
         startClock ("Matcher");
         
         startClock ("Matcher - Create Mask");

         // Create mask based on distances
         cv::Mat match_mask ( prevFeatures.keypoints_v.size(),
                              currFeatures.keypoints_v.size(),
                              CV_8UC1,
                              cv::Scalar(0) );
      
         // use integer value for reducing the processing time of generating masks
         size_t prevSize_ui = prevFeatures.keypoints_v.size();
         size_t currSize_ui = currFeatures.keypoints_v.size();	      

         SRigidMotion *   motion_p = getInput<SRigidMotion>  ( "Predicted Motion" );
         CStereoCamera *  camera_p = getInput<CStereoCamera> ( "Rectified Camera" );
         
         // \todo: do parameter
         float xyRatio_f = camera_p?camera_p->getFu()/camera_p->getFv():1;
         xyRatio_f *= m_img.cols/(float)m_img.rows;

         SRigidMotion motion;
         if (motion_p)
            motion = *motion_p;         

#if defined ( _OPENMP )
         const unsigned int numThreads_ui = omp_get_max_threads();
#pragma omp parallel for num_threads(numThreads_ui) schedule(dynamic)
#endif
         for(unsigned i = 0; i < prevSize_ui; i ++)
         {
            int idx_i = prevFeatures.idx_feature_v[i];

            int    maxDist_i   = static_cast<int>(m_maxDistance_f);

            C3DVector prediction ( m_prevFeatureVector[idx_i].u,
                                   m_prevFeatureVector[idx_i].v,
                                   m_prevFeatureVector[idx_i].d );

            if (camera_p && prediction.z() > 0 && motion_p)
            {
               C3DVector p;
               camera_p->image2Local ( prediction,
                                       p );
               C3DVector p2 = motion.rotation * p + motion.translation;
               camera_p->local2Image ( p2, p );
               prediction = p;
               maxDist_i  = m_maxDistanceForPred_f;
            }

            cv::KeyPoint *k1 = &prevFeatures.keypoints_v[i];
            uchar *p1 = &match_mask.data[i*match_mask.step];
            cv::Point2f refPnt = k1->pt;
            cv::KeyPoint *k2 = &currFeatures.keypoints_v[0];

            for(unsigned j = 0; j < currSize_ui; j++, ++p1, ++k2) 
            {
               int dx=static_cast<int>(prediction.x() - k2->pt.x);
               int dy=static_cast<int>((prediction.y() - k2->pt.y)*xyRatio_f);
               *p1 = (dx*dx+dy*dy < maxDist_i);
            }
         }
         stopClock ("Matcher - Create Mask");

         // Matching from the previous frame to the current frame
         startClock ("Forward Matching");
         m_matcher_p->knnMatch( prevFeatures.descriptors, 
                                currFeatures.descriptors, 
                                prevFeatures.radius_matches_v,
                                1,
                                match_mask);//m_featureMatchTh_f);
         stopClock ("Forward Matching");

      
         // Matching from the current frame to the previous frame
         startClock ("Backward Matching");
         m_matcher_p->knnMatch( currFeatures.descriptors, 
                                prevFeatures.descriptors, 
                                currFeatures.radius_matches_v,
                                1,
                                match_mask.t());//m_featureMatchTh_f);
         stopClock ("Backward Matching");
      
         startClock ("Check consistency");      
         // Check the consistency of the matches.
         for(size_t i = 0; i < currFeatures.radius_matches_v.size(); i ++) 
         {
            if(currFeatures.radius_matches_v[i].size() != 0)  
            {
               if( prevFeatures.radius_matches_v[currFeatures.radius_matches_v[i].at(0).trainIdx].empty() ||
                   prevFeatures.radius_matches_v[currFeatures.radius_matches_v[i].at(0).trainIdx].at(0).trainIdx != (signed)i) 
               {
                  currFeatures.radius_matches_v[i].clear();
               }

            }
         }
         stopClock ("Check consistency");
         
         stopClock ("Matcher");
         
         startClock ("Update feature list");
         std::vector<size_t> newIndices_v;
         
         // Update Feature Vector for tracked features first
         for(size_t i = 0; i < currFeatures.radius_matches_v.size(); ++i) 
         {
           bool associated_b = false;
           if(! currFeatures.radius_matches_v[i].empty() ) 
           {
              /// Get mappings between freak features and maintained feature list.
              int idx_prev    = currFeatures.radius_matches_v[i].at(0).trainIdx;
              int idx_curr    = currFeatures.radius_matches_v[i].at(0).queryIdx;
              int idx_featvec = prevFeatures.idx_feature_v[idx_prev];

              if ( idx_featvec != -1 && currFeatures.radius_matches_v[i].at(0).distance < m_maxDescriptorDistance_f )
              {
                 /// DEBUG
                 if ( m_featureVector[idx_featvec].state == SFeature::FS_TRACKED && 
                      ( prevFeatures.keypoints_v[idx_prev].pt.x !=  (float)m_featureVector[idx_featvec].u ||
                        prevFeatures.keypoints_v[idx_prev].pt.y !=  (float)m_featureVector[idx_featvec].v ) ) 
                 {
                    printf("This should not happen\n"                       
                           "Fature doesn't match: keypoint(%d):(%f,%f), vec(%d):(%f,%f)\n", 
                           idx_prev, prevFeatures.keypoints_v[idx_prev].pt.x, prevFeatures.keypoints_v[idx_prev].pt.y,
                           idx_featvec, m_featureVector[idx_featvec].u,m_featureVector[idx_featvec].v);
                    ::exit(1);
                 }
                 else
                 {
                    // Update mappings
                    currFeatures.idx_feature_v[idx_curr]        = idx_featvec;
                    currFeatures.idx_feature_rev_v[idx_featvec] = idx_curr;
                    
                    /// If associated point is too far, let's stop tracking and
                    /// create a new feature at that position
                    if ( m_featureVector[idx_featvec].state == SFeature::FS_LOST )
                    {
                       associated_b = true;
                       m_featureVector[idx_featvec].state = SFeature::FS_NEW;
                       m_featureVector[idx_featvec].u = currFeatures.keypoints_v[idx_curr].pt.x;
                       m_featureVector[idx_featvec].v = currFeatures.keypoints_v[idx_curr].pt.y;
                       m_featureVector[idx_featvec].d = -1;
                       m_featureVector[idx_featvec].f = imgNr_u;
                       m_featureVector[idx_featvec].t = 0; 
                    }
                    else 
                    {
                       /// Point was associated. Lets update the
                       /// tracking information.
                       associated_b = true;
                       m_featureVector[idx_featvec].state = SFeature::FS_TRACKED;
                       m_featureVector[idx_featvec].u = currFeatures.keypoints_v[idx_curr].pt.x;
                       m_featureVector[idx_featvec].v = currFeatures.keypoints_v[idx_curr].pt.y;
                       m_featureVector[idx_featvec].d = -1;
                       m_featureVector[idx_featvec].f = imgNr_u;
                       ++m_featureVector[idx_featvec].t;
                    }
                 }
              }
              else
                 currFeatures.idx_feature_v[idx_curr] = -1;
           }

           /// Create list of not associated entries so that we can
           /// include those as new detections later.
           if (!associated_b)
              newIndices_v.push_back(i);
	}

        /// Remove features that were not found now.
        for(int i = 0; i < m_numFeatures_i; i++) 
        {
           if( currFeatures.idx_feature_rev_v[i] == -1 || 
               m_featureVector[i].f              != imgNr_u ) 
           {
              if ( currFeatures.idx_feature_rev_v[i] != -1 )
              {
                 currFeatures.idx_feature_v[currFeatures.idx_feature_rev_v[i]] = -1;
                 currFeatures.idx_feature_rev_v[i] = -1;
              }

              if(m_featureVector[i].state == SFeature::FS_NEW ||
                 m_featureVector[i].state == SFeature::FS_TRACKED ) 
                 m_featureVector[i].state = SFeature::FS_LOST;
              else
                 m_featureVector[i].state = SFeature::FS_UNINITIALIZED;
           }
        }

        // Add features newly found or not matched on the current frame
        for(int i = 0; i < m_numFeatures_i && !newIndices_v.empty(); i++) 
        {
           if ( m_featureVector[i].state == SFeature::FS_LOST || 
                m_featureVector[i].state == SFeature::FS_UNINITIALIZED )
           {
              int idx_match_list = newIndices_v.back(); newIndices_v.pop_back();

              currFeatures.idx_feature_v[idx_match_list] = i;
              currFeatures.idx_feature_rev_v[i] = idx_match_list;
              
              m_featureVector[i].state = SFeature::FS_NEW;
              m_featureVector[i].u = currFeatures.keypoints_v[idx_match_list].pt.x;
              m_featureVector[i].v = currFeatures.keypoints_v[idx_match_list].pt.y;
              m_featureVector[i].d = -1;
              m_featureVector[i].f = imgNr_u;
              m_featureVector[i].t = 0;
           }
        }

        stopClock ("Update feature list");
     } /// imgNr > 0

      registerOutput<CFeatureVector>(m_featPointVector_str, &m_featureVector );
   }
   
   return COperator::cycle();
}

/// Show event.
bool CGfttFreakOp::show()
{
   CDrawingList * list_p = getDrawingList ("Matches");
   list_p -> clear();

   CDrawingList *imgdl = getInput<CDrawingList>(m_inpImageId_str + " Drawing List");
   if ( list_p -> isVisible())
   {
      if (imgdl)
         list_p->addDrawingList( *imgdl );
      else
         list_p->addImage ( m_img, 0, 0, m_img.cols, m_img.rows);   

      for(size_t i = 0; i < m_featureVector.size(); i++) 
      {
         if (m_featureVector[i].state == SFeature::FS_TRACKED)
         {
            double dx = m_featureVector[i].u - m_prevFeatureVector[i].u;
            double dy = m_featureVector[i].v - m_prevFeatureVector[i].v;
            
            double sqDist_d = dx*dx+dy*dy;
            
            SRgb color;
            m_respCE.colorFromValue ( sqrt(sqDist_d),
                                      color );
            
            list_p -> setLineColor (color);
            list_p -> setFillColor (SRgba(color,120));
            list_p -> setLineWidth (2);

            list_p -> addFilledSquare ( m_featureVector[i].u, 
                                        m_featureVector[i].v,
                                        4 );
            
            list_p -> addLine( m_featureVector[i].u, 
                               m_featureVector[i].v,
                               m_prevFeatureVector[i].u, 
                               m_prevFeatureVector[i].v );
         }
         else if (m_featureVector[i].state == SFeature::FS_NEW)
         {
            SRgb color = SRgb(255,0,0);
            list_p -> setLineColor (color);
            list_p -> setLineWidth (1);
            
            list_p -> addSquare ( m_featureVector[i].u, 
                                  m_featureVector[i].v,
                                  1 );
         }
         else if (m_featureVector[i].state == SFeature::FS_LOST )
         {
            SRgb color = SRgb(0,255,255);
            list_p -> setLineColor (color);
            list_p -> setLineWidth (1);
            
            list_p -> addSquare ( m_featureVector[i].u, 
                                  m_featureVector[i].v,
                                  1 );
         }
      }
   }
   

    list_p = getDrawingList("Keypoints" );
    list_p->clear();
    SFeatureData &currFeatures = m_featData[m_cnt_i%2];

    if ( list_p->isVisible() )
    {
      if (imgdl)
         list_p->addDrawingList( *imgdl );
      else
         list_p->addImage ( m_img, 0, 0, m_img.cols, m_img.rows);   

       for (unsigned int i = 0; i < currFeatures.keypoints_v.size(); ++i)
       {
          SRgb color;
          
          m_respCE.colorFromValue ( currFeatures.keypoints_v[i].response,
                                    color );
          
          list_p->setLineWidth ( 1. );
          list_p->setLineColor ( color );
          list_p->setFillColor ( SRgba(color, 30 ) );
          
          list_p->addFilledSquare ( currFeatures.keypoints_v[i].pt.x, 
                                    currFeatures.keypoints_v[i].pt.y, 
                                    currFeatures.keypoints_v[i].size/2 );
          
          cv::Vec2d p( currFeatures.keypoints_v[i].size/2,
                       0 );
          
          if (currFeatures.keypoints_v[i].angle != -1) /// -1 means not applicable
          {
             list_p->setLineWidth ( 2. );
             // Show orientation:
             double sin_d = sin(currFeatures.keypoints_v[i].angle);
             double cos_d = cos(currFeatures.keypoints_v[i].angle);
             
             cv::Vec2d q(p[0] * cos_d - p[1] * sin_d,
                         p[0] * sin_d + p[1] * cos_d );
             
             list_p->addLine ( currFeatures.keypoints_v[i].pt.x, 
                               currFeatures.keypoints_v[i].pt.y, 
                               currFeatures.keypoints_v[i].pt.x+q[0],
                               currFeatures.keypoints_v[i].pt.y+q[1] );
          }
       }        
    }    
    
    return COperator::show();
}

/// Init event.
bool CGfttFreakOp::initialize()
{
    if ( getInput<cv::Mat>(m_inpImageId_str) )
        m_img = *getInput<cv::Mat>(m_inpImageId_str);

    /// Set the screen size if this is the parent operator.
    if ( m_img.size().width > 0 &&
         !getParentOp() )
    {
        setScreenSize ( m_img.size() );
    }

    m_featData[0].clear();
    m_featData[1].clear();
    
    if (m_freak_p) delete m_freak_p;
    m_freak_p = new cv::FREAK (false, false, 18.0, 0);
   
    if (m_matcher_p) delete m_matcher_p;
    m_matcher_p = new cv::BFMatcher(m_correlation_b?cv::NORM_L2:cv::NORM_HAMMING, false); //

    m_featureVector.clear();
    m_featureVector.resize( m_numFeatures_i );
    m_prevFeatureVector = m_featureVector;
    
    return COperator::initialize();
}

/// Reset event.
bool CGfttFreakOp::reset()
{
    return COperator::reset();
}

bool CGfttFreakOp::exit()
{
    return COperator::exit();
}

void 
CGfttFreakOp::mouseMoved ( CMouseEvent * f_event_p )
{
    CDrawingList *list2_p = getDrawingList("Descriptor Overlay" );    
    list2_p -> clear();    

    if ( !(f_event_p -> qtMouseEvent_p -> buttons() & Qt::LeftButton) ) return;
   
    SFeatureData &currFeatures = m_featData[m_cnt_i%2];

    CDrawingList *list1_p = getDrawingList("Keypoints" );

    list2_p -> setPosition ( f_event_p -> displayScreen );

    if ( currFeatures.descriptors.rows == 0 ||
         f_event_p -> displayScreen != list1_p->getPosition() ||
         !list1_p->isVisible() )
        return;

    float minSqDist_f = 10000.f*10000.f;
    int   idx_i = -1;
    
    for (unsigned int i = 0; i < currFeatures.keypoints_v.size(); ++i)
    {
        float dx_f = (currFeatures.keypoints_v[i].pt.x - f_event_p->posInScreen.x);
        float dy_f = (currFeatures.keypoints_v[i].pt.y - f_event_p->posInScreen.y);
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
        m_respCE.colorFromValue ( currFeatures.keypoints_v[idx_i].response,
                                  color );
        
        list2_p->setLineWidth ( 3. );
        list2_p->setLineColor ( color );
        list2_p->setFillColor ( SRgba(color, 30 ) );
            
        list2_p->addFilledSquare ( currFeatures.keypoints_v[idx_i].pt.x, 
                                  currFeatures.keypoints_v[idx_i].pt.y, 
                                  currFeatures.keypoints_v[idx_i].size/2 );
        
        /// Build image with descriptor and show it.
        int descSize_i =  currFeatures.descriptors.cols;
        int width_i = descSize_i/8;
        
        cv::Mat descrImg ( 8, width_i, CV_32FC1, &currFeatures.descriptors.at<float>(idx_i,0) );
        
        int dispSize_i = m_img.size().width / 10;

        cv::Rect rect( currFeatures.keypoints_v[idx_i].pt.x + currFeatures.keypoints_v[idx_i].size/2 + 5,
                       currFeatures.keypoints_v[idx_i].pt.y - dispSize_i / 2.f,
                       dispSize_i,   // width
                       dispSize_i ); // height

        list2_p->addImage ( descrImg,
                            rect.x, rect.y, rect.width, rect.height,
                            descSize_i ); // scale
        
        list2_p->addRectangle ( rect.tl(), rect.br() );

        if ( idx_i < (signed)currFeatures.idx_feature_v.size() && 
             currFeatures.idx_feature_v[idx_i] < (signed)m_featureVector.size())
        {
           const SFeature &feat = m_featureVector[currFeatures.idx_feature_v[idx_i]];
           printf("Feature idx %i\n"
                  "  u:     %.2f\n"
                  "  v:     %.2f\n"
                  "  d:     %.2f\n"
                  "  age:   %i\n"
                  "  frame: %zi\n"
                  "  state: %s\n",
                  currFeatures.idx_feature_v[idx_i],
                  feat.u, feat.v, feat.d, feat.t, feat.f, SFeature::FeatureStateName_v[(int)feat.state]);
        }
        
    }

    /// Let's tell QCV to update the screen
    updateDisplay();

}

