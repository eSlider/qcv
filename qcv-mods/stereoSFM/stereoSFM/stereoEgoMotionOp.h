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

#ifndef __EGOMOTIONOP_H
#define __EGOMOTIONOP_H

/**
*******************************************************************************
*
* @file egoMotionOp.cpp
*
* \class CEgoMotionOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief Framework operator class for computing klt with Ipp.
*
* Framework operator class for computing visual odometry.
*  
*******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <limits>

#include "operator.h"
#include "feature.h"
#include "3DMatrix.h"
#include "3DRowVector.h"
#include "colorEncoding.h"
#include "rigidMotion.h"
#include "numericalSolver.h"
#include "stereoCamera.h"

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
   class CStereoEgoMotionOp: public QCV::COperator
   {
      /// Constructor, Desctructors
   public:

      /// Constructors.
      CStereoEgoMotionOp ( COperator * const f_parent_p = NULL, 
                           const std::string f_name_str = "Stereo Ego-Motion" );

      /// Virtual destructor.
      virtual ~CStereoEgoMotionOp ();

      /// Cycle event.
      virtual bool cycle();
    
      /// Show event.
      virtual bool show();
    
      /// Init event.
      virtual bool initialize();
    
      /// Reset event.
      virtual bool reset();
    
      /// Exit event.
      virtual bool exit();

      /// Mouse moved.
      virtual void mouseMoved (     CMouseEvent * f_event_p );

      /// User Operation Events
   public:
      /// Key pressed in display.
      virtual void keyPressed (     CKeyEvent * f_event_p );

      /// Operator to compute score
   public:
      double operator() ( const double *f_params_p, int  ) const;

      /// Gets and sets.
   public:

      ADD_PARAM_ACCESS (std::string,   m_cameraId_str,          CameraId );
      ADD_PARAM_ACCESS (std::string,   m_leftImgId_str,         LeftImageId );
      ADD_PARAM_ACCESS (std::string,   m_featPointVector_str,   FeaturePointVectorId );


      ADD_PARAM_ACCESS (bool,          m_compute_b,             Compute );
      ADD_PARAM_ACCESS (float,         m_tolerance_f,           Tolerance );

      ADD_PARAM_ACCESS (float,         m_transCompStep_f,       TranslationComputationStep );
      ADD_PARAM_ACCESS (float,         m_rotCompStep_f,         RotationComputationStep );
        
      ADD_PARAM_ACCESS (float,         m_expVar_f,              ExpectedVariance );
      ADD_PARAM_ACCESS (float,         m_minReprojRes_f,        MaxVariance4Weighting );
      ADD_PARAM_ACCESS (float,         m_varTolerance_f,        VarianceTolerance );        

      ADD_PARAM_ACCESS_BOUNDED (int,   m_maxOptIters_i,         MaxOptIterations, 0, 100 );
      ADD_PARAM_ACCESS_BOUNDED (int,   m_maxMinIters_i,         MaxMinIterations, 0, 100 );

      ADD_PARAM_ACCESS (bool,          m_showOutliers_b,        ShowOutliers );
      ADD_PARAM_ACCESS (bool,          m_forceReevaluation_b,   ForceReevaluation );

      ADD_PARAM_ACCESS(int,            m_minPoints_i,           MinPoints );

      ADD_PARAM_ACCESS (bool,          m_useMotionPrediction_b, UseMotionPrediction );

      ADD_PARAM_ACCESS (double,        m_sysRotVar_d,           SystemRotationVariance );
      ADD_PARAM_ACCESS (double,        m_sysTransVar_d,         SystemTranslationVariance );


      ADD_PARAM_ACCESS (bool,          m_integrateEgoMotion_b,  IntegrateEgoMotion );        
        
      ADD_PARAM_ACCESS (bool,          m_inclDispInColorEnc_b,  InclDispInColorEnc );

      ADD_PARAM_ACCESS (float,         m_initialSpeed_f,        InitialSpeed );

      ADD_PARAM_ACCESS (C3DVector,     m_residualStdDev,        ResidualStdDev );

      ADD_PARAM_ACCESS (float,         m_minDisparity_f,        MinDisparity );

      ADD_PARAM_ACCESS (float,         m_initialSigmaTol_f,     InitialSigmaTolerance );

      ADD_PARAM_ACCESS (bool,          m_printoutKitti_b,       PrintoutKITTIFormat );
      /// Protected Data Types
   protected:

   protected:

      void    registerDrawingLists();
      void    registerParameters();
      void    resizeHistoryVector();
      void    applySpectralClustering ( );        


      /// Private data type
   private:
          
      /// Private members
   private:
        
      /// Compute?
      std::string                       m_cameraId_str;

      /// Compute?
      std::string                       m_leftImgId_str;

      /// Feature Vector Input String
      std::string                       m_featPointVector_str;

      /// Compute?
      bool                              m_compute_b;

      /// Max optimization iterations.
      int                               m_maxOptIters_i;

      /// Max minimization iterations.
      int                               m_maxMinIters_i;

      /// Max expected variance.
      float                             m_expVar_f;

      /// Max variance for feature weighting
      float                             m_minReprojRes_f;

      /// Variance tolerance for outlier rejection.
      float                             m_varTolerance_f;

      /// Max tolerance for Powell iteration break.
      float                             m_tolerance_f;

      /// Force one reevaluation in the optimization step.
      bool                              m_forceReevaluation_b;

      /// Minimum amount of points for computation.
      int                               m_minPoints_i;

      /// Translation step for the computation of the hesse matrix.
      float                             m_transCompStep_f;

      /// Rotation step for the computation of the hesse matrix.
      float                             m_rotCompStep_f;
       
      /// Stereo Camera
      CStereoCamera                     m_camera;
       
      /// Previous Index.
      int                               m_pIdx_i;

      /// Current Index.
      int                               m_cIdx_i;

      /// Rotation axis.
      C3DVector                         m_predRotAxis;

      /// Translation vector
      C3DVector                         m_predTrans;

      /// Output of the operator
      SRigidMotion                      m_currMotion;

      /// Output of the operator
      SRigidMotion                      m_integratedMotion;

      /// previous feature vector.
      std::vector<CFeatureVector>       m_trackHistoryACTUAL_v;

      /// previous feature vector.
      std::vector<CFeatureVector>       m_trackHistory_v;

      /// List of initial valid features.
      CFeatureVector                    m_initialFeaturesCurr;

      /// List of initial valid  features.
      CFeatureVector                    m_initialFeaturesPrev;       

      /// List of initial 3D previous features.
      std::vector<C3DVector>            m_initialPrev3D_v;

      /// List of current features.
      CFeatureVector                    m_selectedFeaturesCurr;

      /// List of previous features.
      CFeatureVector                    m_selectedFeaturesPrev;

      /// List of selected 3D previous features.
      std::vector<C3DVector>            m_selectedPrev3D_v;

      /// Buffer containing the 3D of all previous 3D tracked features
      std::vector<C3DVector>            m_trackPrev3D_v;     

      /// Weights
      std::vector<double>               m_weightACTUAL;

      /// Weights
      std::vector<double>               m_weight;
        
      /// Rotation
      C3DMatrix                         m_rotation;

      /// Rotation
      C3DVector                         m_translation;        

      /// Color encoding type.
      CColorEncoding                    m_colorEnc;

      /// Color encoding type.
      bool                              m_inclDispInColorEnc_b;

      /// Show outliers?
      bool                              m_showOutliers_b;

      /// Integrate Ego-Motion
      bool                              m_enableMFI_b;
        
      /// Use motion prediction?
      bool                              m_useMotionPrediction_b;

      /// Integrate Ego-Motion
      bool                              m_integrateEgoMotion_b;
        
      /// Covariance matrix of the estimation.
      double                            m_covVar_p[36];

      /// Current integrated estimation.
      double                            m_state_p[6];

      /// System rotation variance (for integrated estimation).
      double                            m_sysRotVar_d;

      /// System translation variancew (for integrated estimation).
      double                            m_sysTransVar_d;

      /// Min known velocity of the camera in Z direction
      float                             m_initialSpeed_f;

      /// Expected residual standar deviation 
      C3DVector                         m_residualStdDev;

      /// Min disparity
      float                             m_minDisparity_f;

      /// Initial maximal square difference
      float                             m_initialSigmaTol_f;

      /// Final sigma tolerance
      float                             m_finalSigmaTolerance_f;
       
      /// Accumulated integrated rotation
      C3DMatrix                         m_intRotation;

      /// Accumulated integrated translation
      C3DVector                         m_intTranslation;

      /// Printout Kitti Format
      bool                              m_printoutKitti_b;
      
   };
}
#endif // __EGOMOTIONOP_H
