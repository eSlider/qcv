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
      m_preFilter_b (                         false ),

      m_numFeatures_i (                         4096 ),
      m_pyrLevels_i (                              8 ),
      m_minDistance_i (                            4 ),
      m_checkCollisions_b (                     true ),
      m_maxSqDist4Collision_f (                  9.f ),
      m_kernelSize_i (                             3 ),
      m_usePrediction_b (                       true ),

      m_minEigenvalue_f (                     1e-05f ),
      m_useSubPix_b (                           true ),
      m_subPixBlockSize_i (                        3 ), 
      m_subPixIterNum_i (                        100 ),
      m_subPixEPS_f (                          0.001 )
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
      
      ADD_FLOAT_PARAMETER( "Min Eigenvalue",
                           "Min eigenvalue to consider for detection.",
                           m_minEigenvalue_f,
                           this,
                           MinEigenvalue,
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
   m_prevImg = m_currImg.clone();

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

         cv::boxFilter( img, imgf, ddepth, cv::Size(19,19), cv::Point(-1,-1), true, cv::BORDER_DEFAULT );
         img.convertTo ( imgf2, CV_32F, 1., 0);
         imgf -= imgf2;
         imgf.convertTo ( m_currImg, CV_8U, 5, 127);
         stopClock ("Pre-filtering");            
      }
      else
         m_currImg = img.clone();

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
                           /// Eliminate the younger feature.
                           //if ( m_featureVector[i].t < m_featureVector[j].t )
                           /// Eliminate the feature with the weakest texture
                           
                           if ( m_featureVector[i].e < m_featureVector[j].e )
                           {
                              m_featureVector[i].state = SFeature::FS_UNINITIALIZED;
                           }
                           else
                           {
                              m_featureVector[j].state = SFeature::FS_UNINITIALIZED;
                           }                                    
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

         SRigidMotion *   motion_p = getInput<SRigidMotion>  ( "Predicted Motion" );
         CStereoCamera *  camera_p = getInput<CStereoCamera> ( "Rectified Camera" );

         bool predict_b = m_usePrediction_b && camera_p && motion_p;
         
         for (size_t i = 0; i < m_numFeatures_i; ++i)
         {
            if ( m_featureVector[i].state == SFeature::FS_TRACKED || 
                 m_featureVector[i].state == SFeature::FS_NEW )
            {
               featprev_ocv.push_back ( cv::Point2f(m_featureVector[i].u, m_featureVector[i].v) );
               cv::Point2f curr (m_featureVector[i].u, m_featureVector[i].v);
               
               if ( m_featureVector[i].d > 0 && predict_b )
               {
                  printf("Predicting image position for feature %i with disparity %f\n",
                         i, m_featureVector[i].d);
                  C3DVector prediction ( m_featureVector[i].u,
                                         m_featureVector[i].v,
                                         m_featureVector[i].d );

                  C3DVector p;
                  if ( camera_p->image2Local ( prediction,
                                               p ) )
                  {
                     C3DVector p2 = motion_p->rotation * p + motion_p->translation;
                     if ( camera_p->local2Image ( p2, p ) )
                     {
                        prediction = p;
                        curr = cv::Point2f(p.x(), p.y());
                     }
                  }
               }
               
               featcurr_ocv.push_back ( curr );
               mapping_v.push_back(i);
            }
         }
         stopClock ("Build tracking list");                
         
         // KLT tracker using OpenCV
         startClock ("OpenCV KLT");

         // set window size and criteria
         cv::TermCriteria criteria_ocv;
         cv::Size winsize_ocv;
         winsize_ocv.width     = m_kernelSize_i;
         winsize_ocv.height    = m_kernelSize_i;
         criteria_ocv.epsilon  = 1; // \todo:make parameter
         criteria_ocv.maxCount = 100;// \todo:make parameter
         criteria_ocv.type = (CV_TERMCRIT_EPS | CV_TERMCRIT_ITER);

         int flags = (m_usePrediction_b)?cv::OPTFLOW_USE_INITIAL_FLOW:0;
         std::vector<uint8_t> status_ocv;
         std::vector<float> error_ocv;
         cv::calcOpticalFlowPyrLK(m_prevImg, m_currImg, 
                                  featprev_ocv, featcurr_ocv,
                                  status_ocv, error_ocv, 
                                  winsize_ocv, m_pyrLevels_i,
                                  criteria_ocv, flags);
         
         for (int i = 0; i < featprev_ocv.size(); ++i) 
         {
            int j = mapping_v[i];
            if (status_ocv[i] )
            {
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
      
      registerOutput<CFeatureVector>(m_featPointVector_str, &m_featureVector );      
   }
   
   return COperator::cycle();
}

/// Select good features to track.
void
CKltTrackerOp::selectGoodFeatures()
{     
   startClock ("Select Good Features - cvcornerMinEigenVal");

   cv::cornerMinEigenVal( m_currImg,
                          m_eigenImg,
                          m_kernelSize_i,
                          m_kernelSize_i);
        
   stopClock ("Select Good Features - cvcornerMinEigenVal");
        
   startClock ("Select Good Features - Occupancy Mask Update");
   /// First pass: update occupancy mask.
   m_featureMask = cv::Mat(m_currImg.size(), CV_8U, cv::Scalar(0));

   for (int i = 0; i < m_featureVector.size(); ++i)
   {
      if ( m_featureVector[i].state == SFeature::FS_TRACKED || 
           m_featureVector[i].state == SFeature::FS_NEW )
      {    
         int fx = int(m_featureVector[i].u + .5);
         int fy = int(m_featureVector[i].v + .5);
                
         /// Mark region in mask image.
         int minK = std::max(fy - m_minDistance_i, 0);
         int maxK = std::min(fy + m_minDistance_i, m_featureMask.rows-1);
         int minL = std::max(fx - m_minDistance_i, 0);
         int maxL = std::min(fx + m_minDistance_i, m_featureMask.cols-1);
                
         for (int k = minK; k<=maxK; ++k)
         {
            unsigned char *ptr = &m_featureMask.at<uint8_t>(k,minL);
            for (int l = minL; l<=maxL; ++l, ++ptr)
               *ptr = 255;
         }
      }
   }

   stopClock ("Select Good Features - Occupancy Mask Update");
        
   startClock ("Select Good Features - Sort Eigenvalues");
   /// Order eigenvalues now.
   m_eigenvalueVector.clear();

   for (int i = 2; i < (int)m_eigenImg.rows - 2; ++i)
   {
      float * eigenvalue_p   = &m_eigenImg.at<float>(i,2);
      unsigned char * mask_p = &m_featureMask.at<uint8_t>(i,2);
           
      for ( int j = 2; j < (int)m_eigenImg.cols - 2; ++j, ++eigenvalue_p, ++mask_p )
      {
         if ( *eigenvalue_p > m_minEigenvalue_f &&
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
            /// Mark region in mask image.
            int minK = std::max(eigenvalue.y - m_minDistance_i, 0);
            int maxK = std::min(eigenvalue.y + m_minDistance_i, (int)m_featureMask.rows-1);
            int minL = std::max(eigenvalue.x - m_minDistance_i, 0);
            int maxL = std::min(eigenvalue.x + m_minDistance_i, (int)m_featureMask.cols-1);
                    
            for (int k = minK; k<=maxK; ++k)
            {
               unsigned char *ptr = &m_featureMask.at<uint8_t>(k,minL);
               for (int l = minL; l<=maxL; ++l, ++ptr)
               {
                  *ptr = 255;
               }
            }

            m_featureVector[i].u     = eigenvalue.x;
            m_featureVector[i].v     = eigenvalue.y;
            m_featureVector[i].d     = -1;
            m_featureVector[i].f     = imgNr_u;
            m_featureVector[i].t     = 0;
            m_featureVector[i].state = SFeature::FS_NEW;
         }
         else
         {
            /// there are no more eigenvalues in the vector.
            break;
         }
      }
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
   CDrawingList * list_p = getDrawingList ("Current Image");
   list_p -> clear();

   if ( list_p -> isVisible())
   {
      list_p->addImage ( m_currImg, 0, 0, m_currImg.cols, m_currImg.rows);   
   }

   list_p = getDrawingList ("Previous Image");
   list_p -> clear();

   if ( list_p -> isVisible())
   {
      list_p->addImage ( m_prevImg, 0, 0, m_prevImg.cols, m_prevImg.rows);   
   }

   list_p = getDrawingList ("Matches");
   list_p -> clear();

   if ( list_p -> isVisible())
   {
      list_p->addImage ( m_currImg, 0, 0, m_currImg.cols, m_currImg.rows);   
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

            list_p -> addFilledCircle ( m_featureVector[i].u, 
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
            
            list_p -> addCircle ( m_featureVector[i].u, 
                                  m_featureVector[i].v,
                                  1 );
         }
         else if (m_featureVector[i].state == SFeature::FS_LOST )
         {
            SRgb color = SRgb(0,255,255);
            list_p -> setLineColor (color);
            list_p -> setLineWidth (1);
            
            list_p -> addCircle ( m_featureVector[i].u, 
                                  m_featureVector[i].v,
                                  1 );
         }
      }
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
CKltTrackerOp::mouseMoved ( CMouseEvent * /*f_event_p */ )
{
}

