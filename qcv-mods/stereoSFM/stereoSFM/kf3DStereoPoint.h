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

#ifndef __KF3DSTEREOPOINT_H
#define __KF3DSTEREOPOINT_H

/*@@@**************************************************************************
** \file  kf3DStereoPoint
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
#include "kf3DStereoPointCommon.h"

/* CONSTANTS */

namespace QCV
{
    
   class CKF3DStereoPoint
   {
   /// Constructors/Destructor
   public:
      typedef enum
      {
         KFS_UNINITIALIZED = 0,
         KFS_INITIALIED,
         KFS_UPDATED,
         KFS_PREDICTED
      } EKFStatus;
        
   /// Constructors/Destructor
   public:
      CKF3DStereoPoint ( );
      virtual ~CKF3DStereoPoint ( );

   public:
      /// Initialize with new measurement.
      bool initialize  ( const C3DVector &measurement );

      /// Initialize with new measurement.
      bool initialize  ( float f_u_f, 
                         float f_v_f,
                         float f_d_f );

      /// Initialize with new measurement and covariance matrix.
      bool initialize  ( const C3DVector &measurement,
                         const C3DMatrix &f_covMatrix );

      /// Initialize with new measurement and covariance matrix.
      bool initialize  ( float f_u_f, 
                         float f_v_f,
                         float f_d_f,
                         const C3DMatrix &f_covMatrix );

      /// Update with measurement.
      bool update      ( const C3DVector &measurement );

      /// Update without disparity measurement
      bool update      ( float f_u_f, 
                         float f_v_f );

      /// Update with measurement.
      bool update      ( float f_u_f, 
                         float f_v_f,
                         float f_d_f );

      /// Update with measurement and covariance matrix.
      bool update      ( const C3DVector &measurement,
                         const C3DMatrix &f_covMatrix );

      /// Update with measurement and covariance matrix.
      bool update      ( float f_u_f, 
                         float f_v_f,
                         float f_d_f,
                         const C3DMatrix &f_covMatrix );

      /// Predict.
      bool predict     ( );

      /// Reset state.
      bool reset       ( );

   /// Gets and Sets
   public:
      /// Get current state
      void getStateVector       ( double * f_state_p ) const;

      /// Get current covariance
      void getCovarianceMatrix  ( double *f_covariance_p ) const;
    
      /// Get current state
      void getCurrentState      ( C3DVector &fr_pos,
                                  C3DVector &fr_vel ) const;
        
      /// Get current state
      C3DVector getPosition     ( ) const;

      /// Get current state
      C3DVector getVelocity     ( ) const;

      /// Get predicted measurement.
      C3DVector getPredictedMeasurement ( ) const;

      /// Get position covariance matrix.
      void getPositionCovMatrix ( C3DMatrix &fr_cov ) const;

      /// Get velocity covariance matrix.
      void getVelocityCovMatrix ( C3DMatrix &fr_cov ) const;

      /// Set common object.
      void setCommonData        ( CKF3DStereoPointCommon * f_ptr_p );

      /// Get Age
      int getAge () const { return m_age_i; }
            
      /// Get Age
      int getNoMeasurementCount () const { return m_noMeasCount_i; }

      /// Get Age
      double getNIS () const { return m_nis_d; }
            
      /// Get Age
      EKFStatus getStatus () const { return (EKFStatus) m_status; }

   /// Protected methods
   protected:
      bool updateFilter     ( const C3DVector &f_meas,
                              const C3DMatrix &f_R );
        
      bool initializeFilter ( const C3DVector &f_meas,
                              const C3DMatrix &f_R,
                              const C3DVector  f_speed = C3DVector(0,0,0) );
        
   /// Protected members.
   protected:
      /// Current State
      double                            m_state_p[6];

      /// Covariance Matrix
      double                            m_covMatrix_p[36];

      /// Age
      int                               m_age_i;

      /// No measurement count.
      int                               m_noMeasCount_i;

      /// Static common data
      static CKF3DStereoPointCommon *   m_commonData_p;

      /// No measurement count.
      double                            m_nis_d;
        
      /// unsigned char status (defined as ubyte to spare memory).
      unsigned char                     m_status;
   };

}

#endif // __KF3DSTEREOPOINT_H
