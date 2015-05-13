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

#ifndef __KF3DPREDICTION_H
#define __KF3DPREDICTION_H

/*@@@**************************************************************************
** \file  kf3DPrediction
* \author Hernan Badino
* \notes  
*******************************************************************************
*****          (C) COPYRIGHT Hernan Badino - All Rights Reserved          *****
******************************************************************************/

/* INCLUDES */

#include <string.h>
#include <iostream>
#include <algorithm>
#include "3DRowVector.h"
#include "3DMatrix.h"
#include "stereoCamera.h"

/* CONSTANTS */

namespace QCV
{
   class CKF3DStereoPointCommon
   {
   /// Friend Class
      friend class CKF3DStereoPoint;

   /// Constructors/Destructor
   public:
      CKF3DStereoPointCommon ( );
      virtual ~CKF3DStereoPointCommon ( );
        
   public:
      /// Set sensor motion.
      bool setEnvironmentMotion ( const C3DMatrix & f_rotation, 
                                  const C3DVector & f_translation, 
                                  double            f_time_d );

      /// Set stereo camera.
      bool setCamera        ( const CStereoCamera & f_camera );

      /// Set stereo camera.
      bool setMeasurementCovMatrix ( const C3DMatrix & f_matrix );

      /// Set initial variances.
      bool setInitialVelocityVariance ( double f_vx_d, 
                                        double f_vy_d, 
                                        double f_vz_d );

      /// Set initial variances.
      bool setInitialVelocityVariance ( const C3DVector &f_variance );

      /// Set/Get initial variance per component
      bool   setInitialXVelocityVariance ( double f_v_d );
      double getInitialXVelocityVariance ( ) const;

      bool   setInitialYVelocityVariance ( double f_v_d );
      double getInitialYVelocityVariance ( ) const;

      bool   setInitialZVelocityVariance ( double f_v_d );
      double getInitialZVelocityVariance ( ) const;


      /// Set system variances.
      bool setSystemVariance ( double f_posVar_d, 
                               double f_velVar_d );

      /// Set/Get system velocity variance.
      bool   setSystemPositionVariance ( double f_var_d );
      double getSystemPositionVariance ( ) const;
        
      /// Set/Get system position variance.
      bool   setSystemVelocityVariance ( double f_var_d );
      double getSystemVelocityVariance ( ) const;

      /// Set/Get innovation integration factor.
      bool   setNISIntegrationFactor ( double f_var_d );
      double getNISIntegrationFactor ( ) const;

   /// Access to some Parameters.
   public:
      bool   setCheckNSigmaTest ( bool f_newVal_b );
      bool   getCheckNSigmaTest ( ) const;

      bool   setMinAge4NSigmaTest ( int f_newAge_i );
      int    getMinAge4NSigmaTest ( ) const;

      bool   setMaxSqNSigma ( double f_newDist_d );
      double getMaxSqNSigma () const;

   /// Protected members
   protected:
      /// Rotation matrix.
      C3DMatrix        m_rotation;

      /// Translation vector.
      C3DVector        m_translation;

      /// Translation vector.
      double           m_cycleTime_d;

      /// Transition matrix.
      double           m_A_p[36];

      /// Input vector
      double           m_B_p[6];

      /// System covariance.
      double           m_Q_p[36];

      /// Rotation matrix.
      CStereoCamera    m_camera;

      /// Focal length (from stereo camera).
      double           m_fu_d;

      /// Focal length to the square (from stereo camera).
      double           m_fu2_d;

      /// Focal length (from stereo camera).
      double           m_fv_d;

      /// Baseline (from stereo camera).
      double           m_B_d;

      /// Baseline to the square (from stereo camera).
      double           m_B2_d;

      /// Baseline (from stereo camera).
      double           m_Bfu_d;

      /// Baseline times aspect ratio of focal lengths (from stereo camera).
      double           m_Bsvu_d;

      /// Initial variance for X velocity component.
      double           m_iniVarVelX_d;

      /// Initial variance for Y velocity component.
      double           m_iniVarVelY_d;

      /// Initial variance for Z velocity component.
      double           m_iniVarVelZ_d;

      /// Measurement covariance matrix.
      C3DMatrix        m_R;

      /// Initial variance for Z velocity component.
      double           m_sysPosVar_d;

      /// Initial variance for Z velocity component.
      double           m_sysVelVar_d;

      /// Minimum filter age for applying the N sigma test.
      int              m_minAge4NSigmaTest_i;

      /// Filter age for applying the N sigma test.
      double           m_maxSqNSigma_d;
        
      /// Apply N sigma test?
      bool             m_checkNSigmaTest_b;

      /// Apply N sigma test?
      double           m_nisAlpha_d;
   };
}

#endif // __KF3DPREDICTION_H
