/*
 * Copyright (C) 2015 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of StereoSFM
 *
 * StereoSFM is under the terms of the GNU General Public License v2.
 * See the GNU GPL version 2.0 for details.
 * StereoSFM uses dual licensing. Contact the author to get information
 * for a commercial/proprietary license.
 *
 * StereoSFM is distributed "AS IS" without ANY WARRANTY, without even 
 * the implied warranty of merchantability or fitness for a particular
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

/*

This project is an implementation of the method presented in the paper:

Hernan Badino and Takeo Kanade. A Head-Wearable Short-Baseline Stereo System for the Simultaneous Estimation of Structure and Motion. In IAPR Conference on Machine Vision Applications (MVA), Nara, Japan, June 2011.

*/

/**
*******************************************************************************
*
* @file egoMotionOp.cpp
*
* \class CStereoEgoMotionOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief Framework operator class for computing ego-motion from 3D point
*        correspondences between current and previous frame.
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "stereoEgoMotionOp.h"
#include "drawingList.h"
#include "ceParameter.h"

#include <limits>

#if defined ( _OPENMP )
#include <omp.h>
#endif

/// Declaration of loss function.
static double calculateSqMahalanobis( const double *f_params_p, int  );

#define invert6x6Matrix(inpptr, outptr) {                               \
      cv::Mat matInp ( cv::Size(6,6), CV_64FC1, (void *) inpptr );      \
      cv::Mat matOut ( cv::Size(6,6), CV_64FC1, (void *) outptr );      \
      matOut=matInp.inv(); }
      
#define multiply6xnMatrix(matAptr, n, matBptr, outptr) {                \
      cv::Mat matA(    cv::Size(6,6), CV_64FC1, (void *) matAptr );     \
      cv::Mat matB(    cv::Size(n,6), CV_64FC1, (void *) matBptr );     \
      cv::Mat matOut(  cv::Size(n,6), CV_64FC1, (void *) outptr );      \
      matOut=matA * matB; }


using namespace QCV;

/// Constructors.
CStereoEgoMotionOp::CStereoEgoMotionOp ( COperator * const f_parent_p, 
                                         const std::string f_name_str )
   : COperator (                       f_parent_p, f_name_str ),
     m_cameraId_str (                      "Rectified Camera" ),
     m_leftImgId_str (                              "Image 0" ),
     m_featPointVector_str (         "Unified Feature Vector" ),
     m_compute_b (                                       true ),
     m_maxOptIters_i (                                     20 ),
     m_maxMinIters_i (                                      5 ),
     m_expVar_f (                                       0.25f ),
     m_minReprojRes_f (                                  0.3f ),
     m_varTolerance_f (                                  16.f ),
     m_tolerance_f (                                   1.e-5f ),
     m_forceReevaluation_b (                             true ),
     m_minPoints_i (                                       12 ),
     m_transCompStep_f (                                 0.01 ),
     m_rotCompStep_f (                                 0.0015 ),
     m_camera (                                               ),
     m_pIdx_i (                                             0 ),
     m_cIdx_i (                                             1 ),
     m_predRotAxis (                                  0, 0, 0 ),
     m_predTrans (                                    0, 0, 0 ),
     m_trackHistoryACTUAL_v (                               2 ),
     m_trackHistory_v (                                     2 ),
     m_colorEnc ( CColorEncoding::CET_HUE,S2D<float>(0.f,5.f) ),
     m_inclDispInColorEnc_b (                            true ),
     m_showOutliers_b (                                 false ),
     m_useMotionPrediction_b (                           true ),
     m_integrateEgoMotion_b (                            true ),
     m_sysRotVar_d (                                   0.0001 ),
     m_sysTransVar_d (                                   0.01 ),
     m_initialSpeed_f (                                  0.5f ),
     m_minDisparity_f (                                 0.01f ),
     m_initialSigmaTol_f (                                 8. ),
     m_finalSigmaTolerance_f (                            0.f ),
     m_printoutKitti_b (                                false )
{
   memset(m_covVar_p, 0, sizeof(double)*36);
   for (int i = 0; i < 6; ++i)
   {
      m_covVar_p[i*6+i] = 1.e9;
      m_state_p[i] = 0;
   }
   
   m_currMotion.clear();
   m_integratedMotion.clear();
   
   registerDrawingLists();
   registerParameters();
}

void
CStereoEgoMotionOp::registerDrawingLists()
{
   registerDrawingList ("Estimated Feature Motion",
                        S2D<int> (0, 0),
                        false, 1);
}

void
CStereoEgoMotionOp::registerParameters()
{
   BEGIN_PARAMETER_GROUP("Input/Output", false, SRgb(220,0,0));
   ADD_STR_PARAMETER( "Left Image Id", 
                      "Id for left image",
                      m_leftImgId_str,
                      this,
                      LeftImageId, 
                      CStereoEgoMotionOp );
      
   ADD_STR_PARAMETER( "Camera Id", 
                      "Id for rectified camera",
                      m_cameraId_str,
                      this,
                      CameraId, 
                      CStereoEgoMotionOp );
      
   ADD_STR_PARAMETER( "Input Feature Vector Id", 
                      "Id of feature point vector. Must be a CFeatureVector type.",
                      m_featPointVector_str,
                      this,
                      FeaturePointVectorId, 
                      CStereoEgoMotionOp );
      
   END_PARAMETER_GROUP;
      
   ADD_BOOL_PARAMETER( "Compute", 
                       "Compute Ego-motion?",
                       m_compute_b,
                       this,
                       Compute, 
                       CStereoEgoMotionOp );
      

   BEGIN_PARAMETER_GROUP("Integration", false, CColor::red );
   ADD_BOOL_PARAMETER( "Integrate with KF?",
                       "Integrate motion estimates with KF?",
                       m_integrateEgoMotion_b,
                       this,
                       IntegrateEgoMotion, 
                       CStereoEgoMotionOp );
      
   ADD_DOUBLE_PARAMETER( "System Rotation Variance",
                         "Driving noise: system rotation variance [deg^2].",
                         m_sysRotVar_d,
                         this,
                         SystemRotationVariance, 
                         CStereoEgoMotionOp );

   ADD_DOUBLE_PARAMETER( "System Translation Variance",
                         "Driving noise: system translation variance [m^2].",
                         m_sysTransVar_d,
                         this,
                         SystemTranslationVariance, 
                         CStereoEgoMotionOp );

   END_PARAMETER_GROUP;

   BEGIN_PARAMETER_GROUP("Estimation", false, CColor::red );
   ADD_BOOL_PARAMETER( "Use Motion Prediction", 
                       "Use motion prediction?",
                       m_useMotionPrediction_b,
                       this,
                       UseMotionPrediction, 
                       CStereoEgoMotionOp );

   ADD_INT_PARAMETER( "Max Iterations", 
                      "Maximum number of iterations for optimization.",
                      m_maxOptIters_i,
                      this,
                      MaxOptIterations, 
                      CStereoEgoMotionOp );
      
   ADD_INT_PARAMETER( "Max Minimization Iters", 
                      "Maximum number of minimization iterations.",
                      m_maxMinIters_i,
                      this,
                      MaxMinIterations, 
                      CStereoEgoMotionOp );
      
   ADD_BOOL_PARAMETER( "Force Reevaluation", 
                       "At least two iterations of the optimization step is performed.",
                       m_forceReevaluation_b,
                       this,
                       ForceReevaluation, 
                       CStereoEgoMotionOp );
      
   ADD_FLOAT_PARAMETER( "Initial Sigma Tolerance",
                        "Sigma tolerance to use for initial feature selection [px]",
                        m_initialSigmaTol_f,
                        this,
                        InitialSigmaTolerance, 
                        CStereoEgoMotionOp );

   ADD_INT_PARAMETER( "Min Points", 
                      "Minimum number of points for optimization. If the number of points falls "
                      "below this threshold, the optimization iteration stops",
                      m_minPoints_i,
                      this,
                      MinPoints, 
                      CStereoEgoMotionOp );
      
   ADD_FLOAT_PARAMETER( "Expected Variance",
                        "Max expected variance for. The optimization iteration will stop "
                        "when a residuum smaller than this is obtained [px^2].",
                        m_expVar_f,
                        this,
                        ExpectedVariance, 
                        CStereoEgoMotionOp );

   ADD_FLOAT_PARAMETER( "Min Reproj Residual",
                        "Minimum reprojection residual to use for dynamic weighting.",
                        m_minReprojRes_f,
                        this,
                        MaxVariance4Weighting, 
                        CStereoEgoMotionOp );

   ADD_FLOAT_PARAMETER( "Variance Tolerance",
                        "Variance tolerance for outlier rejection [px^2]",
                        m_varTolerance_f,
                        this,
                        VarianceTolerance, 
                        CStereoEgoMotionOp );
      
   ADD_FLOAT_PARAMETER( "Residuum Tolerance", 
                        "Optimization iteration stops when the score value of two "
                        "consecutive iterations is less than this number [px^2].",
                        m_tolerance_f,
                        this,
                        Tolerance, 
                        CStereoEgoMotionOp );
      
   ADD_FLOAT_PARAMETER( "Initial speed", 
                        "Initial Z translation for first motion estimate [m/frame]",
                        m_initialSpeed_f,
                        this,
                        InitialSpeed, 
                        CStereoEgoMotionOp );

   ADD_FLOAT_PARAMETER( "Translation Comp Step", 
                        "Step for the computation the the translation part of the "
                        "Hesse Matrix [m].",
                        m_transCompStep_f,
                        this,
                        TranslationComputationStep, 
                        CStereoEgoMotionOp );
      
   ADD_FLOAT_PARAMETER( "Rotation Comp Step", 
                        "Step for the computation the rotation part of the "
                        "Hesse Matrix [deg].",
                        m_rotCompStep_f,
                        this,
                        RotationComputationStep, 
                        CStereoEgoMotionOp );

   ADD_FLOAT_PARAMETER( "Min Disparity", 
                        "Min feature disparity to consider for motion estimation",
                        m_minDisparity_f,
                        this,
                        MinDisparity, 
                        CStereoEgoMotionOp );

   ADD_BOOL_PARAMETER( "Print out KITTI format", 
                       "Print out kitti format for evaluation/submission",
                       m_printoutKitti_b,
                       this,
                       PrintoutKITTIFormat, 
                       CStereoEgoMotionOp );

   END_PARAMETER_GROUP;

   BEGIN_PARAMETER_GROUP("Display", false, CColor::red );
    
   addDrawingListParameter ( "Estimated Feature Motion" );
   addDrawingListParameter ( "Integrated Features" );

   addColorEncodingParameter (   m_colorEnc,
                                 getParameterSet(),
                                 "Error Color Encoding", 
                                 "Color encoding for the error between predicted and measured reprojection [px]." );

   ADD_BOOL_PARAMETER( "Include Disp in Color Enc", 
                       "Include the disparity estimate in the error color encoding?",
                       m_inclDispInColorEnc_b,
                       this,
                       InclDispInColorEnc, 
                       CStereoEgoMotionOp );
      
   ADD_BOOL_PARAMETER( "Show Outliers", 
                       "Show outliers as circles?",
                       m_showOutliers_b,
                       this,
                       ShowOutliers, 
                       CStereoEgoMotionOp );
      
   END_PARAMETER_GROUP;
}

/// Virtual destructor.
CStereoEgoMotionOp::~CStereoEgoMotionOp ()
{
}

/// Cycle event.
bool CStereoEgoMotionOp::cycle()
{   
   CFeatureVector * featureVector_p = getInput<CFeatureVector> ( m_featPointVector_str );
    
   CStereoCamera * camera_p = getInput<CStereoCamera> ( m_cameraId_str );

   m_rotation.loadIdentity();
   m_translation.clear();

   int  frameNumber_i = getInput<int> ("Frame Number", -1 );
   bool firstFrame_b = ( getInput<int> ("Frame Number", -1 ) <= 1);

   bool failed_b = false;

   double covVar_p[36];
   if ( m_compute_b && 
        featureVector_p && 
        camera_p )
   {
      m_camera = *camera_p;
      
      float minDisparity_f = m_minDisparity_f;
      
      /// Predict
      C3DVector translationPred = m_predTrans;

      if (firstFrame_b)
      {
         translationPred.at(2) = m_initialSpeed_f;
      }

      C3DMatrix rotationPred;
      rotationPred.loadIdentity( );
        
      C3DVector axis = m_predRotAxis;
        
      double angle_d = axis.magnitude();
        
      axis /= angle_d;
        
      if ( angle_d )
      {
         rotationPred.rotateAxis ( axis, angle_d );
      }        
        
      m_pIdx_i = m_cIdx_i;
      m_cIdx_i++; m_cIdx_i %= 2;
        
      m_trackHistoryACTUAL_v[m_cIdx_i] = *featureVector_p;

      if ( m_trackHistoryACTUAL_v[m_cIdx_i].size() == m_trackHistoryACTUAL_v[m_pIdx_i].size() )
      {
         m_trackHistory_v[m_cIdx_i].clear();
         m_trackHistory_v[m_pIdx_i].clear();
         m_trackPrev3D_v.clear();
         m_weightACTUAL.clear();
           
         for (int i = 0; i < (signed)m_trackHistoryACTUAL_v[m_cIdx_i].size(); ++i)
         {
            if ( ( m_trackHistoryACTUAL_v[m_cIdx_i][i].state == SFeature::FS_TRACKED ||
                   m_trackHistoryACTUAL_v[m_cIdx_i][i].state == SFeature::FS_NEW ) &&
                 m_trackHistoryACTUAL_v[m_pIdx_i][i].state == SFeature::FS_LOST )
            {
               m_trackHistoryACTUAL_v[m_cIdx_i][i].state =  SFeature::FS_NEW;
               m_trackHistoryACTUAL_v[m_cIdx_i][i].t = 0;
            }
            else   
               if ( m_trackHistoryACTUAL_v[m_cIdx_i][i].state == SFeature::FS_TRACKED &&
                    ( m_trackHistoryACTUAL_v[m_pIdx_i][i].state == SFeature::FS_TRACKED ||
                      m_trackHistoryACTUAL_v[m_pIdx_i][i].state == SFeature::FS_NEW ) && 
                    m_trackHistoryACTUAL_v[m_pIdx_i][i].d > minDisparity_f )
               {
                  C3DVector c3d;
                  const SFeature &curr = m_trackHistoryACTUAL_v[m_cIdx_i][i];
                  if (curr.t >= 1 && curr.d > minDisparity_f &&
                      m_camera.image2Local ( curr.u,
                                             curr.v,
                                             curr.d,
                                             c3d ) )
                  {
                     const SFeature &prev = m_trackHistoryACTUAL_v[m_pIdx_i][i];
                     
                     C3DVector p3d;
                     if ( m_camera.image2Local ( prev.u,
                                                 prev.v,
                                                 prev.d,
                                                 p3d ) )
                     {
                        /// Everything all right. Let's add this point correspondence.
                        m_trackHistory_v[m_cIdx_i].push_back ( curr );
                        m_trackHistory_v[m_pIdx_i].push_back ( prev );
                        m_trackPrev3D_v.push_back ( p3d );

                        m_weightACTUAL.push_back ( 1.f );
                     }
                  }
               }
         }
           

         m_initialFeaturesCurr.clear();
         m_initialFeaturesPrev.clear();
         m_initialPrev3D_v.clear();
         m_weight.clear();

         startClock("Initial Feature Selection");

         double newSigmaTol_d = m_initialSigmaTol_f*m_initialSigmaTol_f;

         // Let let it converge the first 10 frames.
         if ( frameNumber_i < 10 )
            newSigmaTol_d = 120*120;

         /// We will first select only those features that we are going to use for
         /// minimization and generate two new vectors only with those features.
         for (unsigned int i = 0 ; i < m_trackHistory_v[m_cIdx_i].size(); ++i)
         {
            const SFeature &curr = m_trackHistory_v[m_cIdx_i][i];
            const SFeature &prev = m_trackHistory_v[m_pIdx_i][i];                  
            
            C3DVector w2;
            C3DVector w3 ( curr.u, curr.v, curr.d );
            
            C3DVector p2 = rotationPred * m_trackPrev3D_v[i] + translationPred;
            
            m_camera.local2Image ( p2,
                                   w2 );
            
            C3DVector diff = w2-w3;
            
            float sqDist_f = diff.sumOfSquares();
            
            if ( sqDist_f  < newSigmaTol_d )
            {
               m_initialFeaturesCurr.push_back( curr );
               m_initialFeaturesPrev.push_back( prev );
               m_initialPrev3D_v.push_back ( m_trackPrev3D_v[i] );
               
               sqDist_f = std::max(sqDist_f, m_minReprojRes_f);
               
               m_weight.push_back( m_weightACTUAL[i]/(sqDist_f) );
            }
         }          
            
         stopClock("Initial Feature Selection");

         m_selectedFeaturesCurr     = m_initialFeaturesCurr;
         m_selectedFeaturesPrev     = m_initialFeaturesPrev;
         m_selectedPrev3D_v         = m_initialPrev3D_v;
            
         if ( (int)m_selectedFeaturesCurr.size() > m_minPoints_i )
         {
            double x_p[6];
            double gradient_p[6];
            double hesse_p[36];

            float prevRet_d     = std::numeric_limits<float>::max();
            double ret_d = 0;                
            int it;

            double expVar_f = firstFrame_b?32:m_expVar_f;
            double maxSigma_d = expVar_f;
            
            double steps_p[6] = { m_rotCompStep_f,
                                  m_rotCompStep_f,
                                  m_rotCompStep_f,
                                  m_transCompStep_f,
                                  m_transCompStep_f,
                                  m_transCompStep_f };

            startClock("Optimization");
                
            if ( m_useMotionPrediction_b )
            {
               x_p[0] = m_predRotAxis.x();
               x_p[1] = m_predRotAxis.y();
               x_p[2] = m_predRotAxis.z();
               x_p[3] = m_predTrans.x();
               x_p[4] = m_predTrans.y();
               x_p[5] = m_predTrans.z();
            }
            else
               memset(x_p, 0, sizeof(double)*6);

            for ( it = 0; ( it < m_maxOptIters_i && 
                            ( (newSigmaTol_d > maxSigma_d ) ||
                              (it < 2 && m_forceReevaluation_b ) ) ); ++it)
            {
               startClock("Newton minimization");

               ret_d = NumericalSolver::solveByNewton ( x_p, 
                                                        gradient_p, 
                                                        hesse_p,
                                                        steps_p,
                                                        6, 6,
                                                        m_maxMinIters_i,
                                                        m_tolerance_f,
                                                        *this);

               stopClock("Newton minimization");

               newSigmaTol_d = m_varTolerance_f * ret_d;

               if ( ( fabs(prevRet_d - ret_d) < m_tolerance_f ||
                      (ret_d < (expVar_f)) ) &&
                    !(it < 1 && m_forceReevaluation_b) )
               {
                  break;
               }
                    
               // Update rotation and translation vectors.
               m_translation.set ( x_p[3], x_p[4], x_p[5] );
               m_rotation.loadIdentity( );
                    
               C3DVector axis (  x_p[0], 
                                 x_p[1], 
                                 x_p[2] );
                    
               double angle_d = axis.magnitude();
                    
               axis /= angle_d;
                    
               if ( angle_d )
               {
                  m_rotation.rotateAxis ( axis, angle_d );
               }            

               startClock("Iterative Feature Selection");

               /// Lets rebuild the list of inliners.
               m_selectedFeaturesCurr.clear();
               m_selectedFeaturesPrev.clear();
               m_selectedPrev3D_v.clear();
               m_weight.clear();

               /// Select new subset of feature to include in
               /// next iteration.
               for (unsigned int i = 0 ; i < m_trackHistory_v[m_cIdx_i].size(); ++i)
               {
                  const SFeature &curr = m_trackHistory_v[m_cIdx_i][i];

                  C3DVector w2;
                  C3DVector w3 ( curr.u, curr.v, curr.d );

                  C3DVector p2 = m_rotation * m_trackPrev3D_v[i] + m_translation;
                            
                  m_camera.local2Image ( p2,
                                         w2 );
                            
                  C3DVector diff = w2-w3;
                               
                  float sqDist_f = diff.sumOfSquares();

                  if ( sqDist_f < newSigmaTol_d )
                  {
                     m_selectedFeaturesCurr.push_back ( m_trackHistory_v[m_cIdx_i][i] );
                     m_selectedFeaturesPrev.push_back ( m_trackHistory_v[m_pIdx_i][i] );
                     m_selectedPrev3D_v.push_back     ( m_trackPrev3D_v[i] );

                     sqDist_f = std::max(sqDist_f, m_minReprojRes_f);
                          
                     m_weight.push_back( m_weightACTUAL[i]/(sqDist_f) );
                  }
               }
                    
               m_finalSigmaTolerance_f = newSigmaTol_d;                    

               stopClock("Iterative Feature Selection");

               prevRet_d = ret_d;
                    
               if(0)
                  printf ("it < m_maxOptIters_i = (%i < %i) - "
                          "newSigmaTol_d > maxSigma_d = ( %f > %f) - "
                          "it < 2 && m_forceReevaluation_b = %i < 2 && %i - "
                          "%i points in the lists\n",
                          it, m_maxOptIters_i, 
                          newSigmaTol_d, maxSigma_d, 
                          it, m_forceReevaluation_b,
                          (int)m_selectedFeaturesCurr.size());
                    
               if ( (int)m_selectedFeaturesCurr.size() < m_minPoints_i )
               {
                  printf("Estimation FAILED!!!\n");
                  failed_b = true;
                  break;
               }
            }
                
            if (failed_b)
            {
               memset(hesse_p, 0, sizeof(double) * 36);
               hesse_p[0]=hesse_p[7]=hesse_p[14]=hesse_p[21]=hesse_p[28]=hesse_p[35]=1.e6;
            }
                
            stopClock("Optimization");
                
            if (0)
            {
               printf("Final Result residuum: %f - (%f %f %f - %f %f %f) ( %i iterations)\n",
                      ret_d,
                      x_p[0], 
                      x_p[1], 
                      x_p[2], 
                      x_p[3], 
                      x_p[4], 
                      x_p[5], it );
                    
               printf("Gradient is: %f %f %f - %f %f %f\n",
                      gradient_p[0], 
                      gradient_p[1], 
                      gradient_p[2], 
                      gradient_p[3], 
                      gradient_p[4], 
                      gradient_p[5] );
                
               printf("Hesse is:\n"
                      "\t%17g %17g %17g %17g %17g %17g\n"
                      "\t%17g %17g %17g %17g %17g %17g\n"
                      "\t%17g %17g %17g %17g %17g %17g\n"
                      "\t%17g %17g %17g %17g %17g %17g\n"
                      "\t%17g %17g %17g %17g %17g %17g\n"
                      "\t%17g %17g %17g %17g %17g %17g\n\n",
                      hesse_p[0*6+0],hesse_p[0*6+1],hesse_p[0*6+2],hesse_p[0*6+3],hesse_p[0*6+4],hesse_p[0*6+5], 
                      hesse_p[1*6+0],hesse_p[1*6+1],hesse_p[1*6+2],hesse_p[1*6+3],hesse_p[1*6+4],hesse_p[1*6+5], 
                      hesse_p[2*6+0],hesse_p[2*6+1],hesse_p[2*6+2],hesse_p[2*6+3],hesse_p[2*6+4],hesse_p[2*6+5], 
                      hesse_p[3*6+0],hesse_p[3*6+1],hesse_p[3*6+2],hesse_p[3*6+3],hesse_p[3*6+4],hesse_p[3*6+5], 
                      hesse_p[4*6+0],hesse_p[4*6+1],hesse_p[4*6+2],hesse_p[4*6+3],hesse_p[4*6+4],hesse_p[4*6+5], 
                      hesse_p[5*6+0],hesse_p[5*6+1],hesse_p[5*6+2],hesse_p[5*6+3],hesse_p[5*6+4],hesse_p[5*6+5] );

               invert6x6Matrix(hesse_p, covVar_p);
                
               printf("Covariance Matrix is:\n"
                      "\t%17g %17g %17g %17g %17g %17g\n"
                      "\t%17g %17g %17g %17g %17g %17g\n"
                      "\t%17g %17g %17g %17g %17g %17g\n"
                      "\t%17g %17g %17g %17g %17g %17g\n"
                      "\t%17g %17g %17g %17g %17g %17g\n"
                      "\t%17g %17g %17g %17g %17g %17g\n\n",
                      covVar_p[0*6+0],covVar_p[0*6+1],covVar_p[0*6+2],covVar_p[0*6+3],covVar_p[0*6+4],covVar_p[0*6+5], 
                      covVar_p[1*6+0],covVar_p[1*6+1],covVar_p[1*6+2],covVar_p[1*6+3],covVar_p[1*6+4],covVar_p[1*6+5], 
                      covVar_p[2*6+0],covVar_p[2*6+1],covVar_p[2*6+2],covVar_p[2*6+3],covVar_p[2*6+4],covVar_p[2*6+5], 
                      covVar_p[3*6+0],covVar_p[3*6+1],covVar_p[3*6+2],covVar_p[3*6+3],covVar_p[3*6+4],covVar_p[3*6+5], 
                      covVar_p[4*6+0],covVar_p[4*6+1],covVar_p[4*6+2],covVar_p[4*6+3],covVar_p[4*6+4],covVar_p[4*6+5], 
                      covVar_p[5*6+0],covVar_p[5*6+1],covVar_p[5*6+2],covVar_p[5*6+3],covVar_p[5*6+4],covVar_p[5*6+5] );
            }

            m_translation.set ( x_p[3], x_p[4], x_p[5] );
            m_rotation.loadIdentity( );

            m_predTrans = m_translation;
            m_predRotAxis.set (  x_p[0], 
                                 x_p[1], 
                                 x_p[2] );

            double angle_d = m_predRotAxis.magnitude();
                
            C3DVector axis = m_predRotAxis / angle_d;

            if ( angle_d )
            {
               m_rotation.rotateAxis ( axis, angle_d );
            }

            static int s_converged_i;

            if ( m_integrateEgoMotion_b )
            {
               double invCovVarMeas_p[36];
               double invCovVarSys_p[36];
               double newInvCovVar_p[36];
               double vecSys_p[6];
               double vecMeas_p[6];
               double covVarMeas_p[36];

               memcpy(vecMeas_p, x_p, sizeof(double) * 6);
                    
               /// Obtain covariance matrix from Hessian
               memcpy(invCovVarMeas_p, hesse_p, 36*sizeof(double));
               invert6x6Matrix ( invCovVarMeas_p, covVarMeas_p );

               double sqMahalanobis_d = 0;

               /// Add driving noise
               for (int i = 0; i < 3; ++i)
                  m_covVar_p[i*6+i] += m_sysRotVar_d;

               for (int i = 3; i < 6; ++i)
                  m_covVar_p[i*6+i] += m_sysTransVar_d;

               if ( s_converged_i < 16 || sqMahalanobis_d < 9 )
               {
                  invert6x6Matrix ( m_covVar_p, invCovVarSys_p);
                       
                  for (int i = 0; i < 36; ++i)
                     newInvCovVar_p[i] = invCovVarSys_p[i] + invCovVarMeas_p[i];
                       
                  invert6x6Matrix ( newInvCovVar_p, m_covVar_p);                    
                       
                  multiply6xnMatrix ( invCovVarSys_p, 
                                      1,
                                      m_state_p, 
                                      vecSys_p );
                       
                  multiply6xnMatrix ( invCovVarMeas_p, 
                                      1,
                                      x_p, 
                                      vecMeas_p );
                       
                       
                  for (int i = 0; i < 6; ++i)
                     vecSys_p[i] += vecMeas_p[i];
                       
                  multiply6xnMatrix ( m_covVar_p, 
                                      1,
                                      vecSys_p, 
                                      m_state_p );
               }
               else
               {
                  // Predict only, ignoring measurement
               }
                    
               /// Ensure it to be symmetric
               for (int i = 0; i < 6; ++i)
               {
                  for (int j = i+1; j < 6; ++j)
                     m_covVar_p[i*6+j] = m_covVar_p[j*6+i];
               }

               m_predTrans.set   ( m_state_p[3], m_state_p[4], m_state_p[5] );
               m_predRotAxis.set ( m_state_p[0], m_state_p[1], m_state_p[2] );

               m_integratedMotion.rotation.loadIdentity();
               double angle_d = m_predRotAxis.magnitude();                    
               C3DVector axis = m_predRotAxis / angle_d;
                    
               C3DMatrix trot; trot.loadIdentity();
               if ( angle_d )
               {
                  m_integratedMotion.rotation.rotateAxis ( axis, angle_d );
               }
                   
               m_integratedMotion.translation = m_predTrans;
            }
         }
         else
         {
            printf("Not enough features to compute ego-motion (%i)\n",
                   (int)m_selectedFeaturesCurr.size() );
         }
      }

      SRigidMotion prevMotion = m_currMotion;
        
      m_currMotion.rotation = m_rotation;
      m_currMotion.translation = m_translation;
    
      registerOutput<SRigidMotion> ( std::string("Ego-Motion Estimation"),             &m_currMotion );
      registerOutput<SRigidMotion> ( std::string("Integrated Ego-Motion Estimation"),  &m_integratedMotion );
      registerOutput<C3DVector>    ( std::string("Ego-Motion Rotation"),               &m_predRotAxis );
      registerOutput<C3DVector>    ( std::string("Ego-Motion Translation"),            &m_predTrans );

/////////////////////
      // Generate KITTI-like output
      {
         double idT_p[]={1,0,0,0,-1,0,0,0,1};

         C3DMatrix cv2kitti(idT_p);

         int frameNumber_i = getInput<int> ("Frame Number", -1 );
         if (frameNumber_i == 0)
         {
            m_intRotation.loadIdentity();
            m_intTranslation.clear();
         }
       
         m_intRotation    = m_integratedMotion.rotation * m_intRotation;
         m_intTranslation = m_integratedMotion.rotation * m_intTranslation + m_currMotion.translation;

         if (m_printoutKitti_b)
         {
            C3DMatrix finalr = cv2kitti * m_intRotation * cv2kitti;
            finalr.transpose();
            C3DVector finalt = m_intRotation.multiplyTransposed( -m_intTranslation );
          
            printf( "Integrated Transformation %.9lf %.9lf %.9lf %.9lf %.9lf %.9lf %.9lf %.9lf %.9lf %.9lf %.9lf %.9lf\n",
                    //frameNumber_i,
                    finalr.at(0,0), finalr.at(0,1), finalr.at(0,2), finalt.x(),
                    finalr.at(1,0), finalr.at(1,1), finalr.at(1,2), -finalt.y(),
                    finalr.at(2,0), finalr.at(2,1), finalr.at(2,2), finalt.z() );
         }
      }
   }
    
   return COperator::cycle();
}

/// Show event.
bool
CStereoEgoMotionOp::show()
{
   CDrawingList *list_p;
   const float scrWidth_f  = getScreenSize().width;
   const float scrHeight_f = getScreenSize().height;

   CFeatureVector &curr = m_selectedFeaturesCurr; // m_trackHistory_v[m_cIdx_i]; //
   CFeatureVector &prev = m_selectedFeaturesPrev; // m_trackHistory_v[m_pIdx_i]; //

   cv::Mat img = getInput<cv::Mat> ( m_leftImgId_str, cv::Mat() );
    
   const float scaleX_f = scrWidth_f  / img.cols;
   const float scaleY_f = scrHeight_f / img.rows;

   list_p = getDrawingList ("Estimated Feature Motion");   
   list_p -> clear();

   if ( list_p -> isVisible () && 
        img.cols > 0 )
   {
      CDrawingList *imgdl = getInput<CDrawingList>(m_leftImgId_str + " Drawing List");
      if (imgdl)
         list_p->addDrawingList( *imgdl );

      for (unsigned int i = 0; i < prev.size(); ++i)
      {
         /// This can be optimized by pregenerating two new feature vectors
         /// with only the elements to be considered in order to avoid checking
         /// this condition every time.
         C3DVector p1;
         C3DVector w1 ( prev[i].u, prev[i].v, prev[i].d );
         C3DVector w2;
         C3DVector w3 ( curr[i].u, curr[i].v, curr[i].d );
           
         m_camera.image2Local ( w1,
                                p1 );
                
         C3DVector p2 = m_rotation * p1 + m_translation;
                
         m_camera.local2Image ( p2,
                                w2 );
                
         S2D<float> f1 ( w1.x() *  scaleX_f,
                         w1.y() *  scaleY_f );
         S2D<float> f2 ( w2.x() *  scaleX_f,
                         w2.y() *  scaleY_f );
         S2D<float> f3 ( w3.x() *  scaleX_f,
                         w3.y() *  scaleY_f );
                
         //list_p -> setLineColor ( SRgb(0,255,0) );
         //list_p -> addLine( f1, f2 );
         C3DVector diff = w2-w3;

         if ( !m_inclDispInColorEnc_b ) diff.at(2) = 0;
                
         float error_f = diff.sumOfSquares();
                
         /// Encode range.
         SRgb  lcolor;
         SRgba fcolor;
         m_colorEnc.colorFromValue ( error_f,
                                     lcolor );
         fcolor = lcolor;
         fcolor.a = 100;
                
         if ( error_f > m_finalSigmaTolerance_f && !m_showOutliers_b )
            continue;

         list_p -> setLineColor ( lcolor );
         list_p -> setFillColor ( fcolor );
                    
         list_p -> addLine( f1, f3 );
         list_p -> addLine( f2, f3 );
         if ( error_f > m_finalSigmaTolerance_f )
            list_p -> addFilledCircle( f2, 3+fabs( w2.z() - w3.z() )/20 );
         else
            list_p -> addFilledSquare ( f2, 3+fabs( w2.z() - w3.z() )/20 );
      }
   }

   return COperator::show();
}

/// Init event.
bool
CStereoEgoMotionOp::initialize()
{
   /// Get input from parent.
   m_translation.clear();
   m_rotation.loadIdentity( );

   memset(m_covVar_p, 0, sizeof(double)*36);
   for (int i = 0; i < 6; ++i)
   {
      m_covVar_p[i*6+i] += 1.e20;
      m_state_p[i] = 0;
   }

   m_intRotation.loadIdentity();
   m_intTranslation.clear();
    
   m_trackHistoryACTUAL_v.clear();
   m_trackHistoryACTUAL_v.resize ( 2 );
   m_trackHistory_v.clear();
   m_trackHistory_v.resize       ( 2 );

   return COperator::initialize();
}

/// Reset event.
bool
CStereoEgoMotionOp::reset()
{
   return COperator::reset();
}

bool
CStereoEgoMotionOp::exit()
{
   return COperator::exit();
}

void 
CStereoEgoMotionOp::keyPressed ( CKeyEvent * f_event_p )
{
   return COperator::keyPressed ( f_event_p );    
}

double CStereoEgoMotionOp::operator () ( const double *f_params_p, int  ) const
{
   const CFeatureVector &curr = m_selectedFeaturesCurr;

   double residuum_f = 0;
   C3DMatrix m;
   m.loadIdentity();

   C3DVector t ( f_params_p[3],
                 f_params_p[4],
                 f_params_p[5] );
    
   C3DVector axis (  f_params_p[0], 
                     f_params_p[1], 
                     f_params_p[2] );

   double angle_d = axis.magnitude();

   if ( angle_d )
   {
      axis /= angle_d;
      m.rotateAxis ( axis, angle_d );
   }

   double count_d = 0.;

   for (int i = 0 ; i < (int)curr.size(); ++i)
   {
      //C3DVector w1 ( prev[i].u, prev[i].v, prev[i].d );
      C3DVector w2;
      C3DVector w3 ( curr[i].u, curr[i].v, curr[i].d );

      C3DVector p2 = m * m_selectedPrev3D_v[i] + t;
        
      if ( m_camera.local2Image ( p2,
                                  w2 ) )
      {
         C3DVector diff = w2-w3;
           
         double sqDist_f = diff.sumOfSquares();
         count_d    += m_weight[i];
         residuum_f += sqDist_f * m_weight[i];
      }
   }

   return residuum_f / count_d;
}


void
CStereoEgoMotionOp::mouseMoved ( CMouseEvent * f_event_p )
{
   return COperator::mouseMoved ( f_event_p );
}


