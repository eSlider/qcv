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

/*@@@**************************************************************************
 * \file  kf3DStereoPointCommon
 * \author Hernan Badino
 * \notes 
 ******************************************************************************/

/* INCLUDES */
#include "kf3DStereoPointCommon.h"

using namespace QCV;

/// Constructors/Destructor
CKF3DStereoPointCommon::CKF3DStereoPointCommon ( )
   : m_rotation (                ),
     m_translation (             ),
     m_cycleTime_d (          0. ),
     m_camera (                  ),
     m_fu_d (                 0. ),
     m_fu2_d (                0. ),
     m_fv_d (                 0. ),
     m_B_d (                  0. ),
     m_B2_d (                 0. ),
     m_Bfu_d (                0. ),
     m_Bsvu_d (               0. ),
     m_iniVarVelX_d (         0. ),
     m_iniVarVelY_d (         0. ),
     m_iniVarVelZ_d (         0. ),
     m_R (                       ),
     m_sysPosVar_d (        0.01 ),
     m_sysVelVar_d (        0.04 ),

     m_minAge4NSigmaTest_i (   3 ),
     m_maxSqNSigma_d (        9. ),
     m_checkNSigmaTest_b ( false ),

     m_nisAlpha_d (          0.8 )
{
   memset( m_A_p, 0, sizeof(double) * 36);
   memset( m_B_p, 0, sizeof(double) * 6);
   memset( m_Q_p, 0, sizeof(double) * 36);

   m_rotation.clear();
   m_translation.clear();
   m_translation.clear();
   m_R.clear();
}

CKF3DStereoPointCommon::~CKF3DStereoPointCommon ( )
{
}


/// Set sensor motion.
bool
CKF3DStereoPointCommon::setEnvironmentMotion ( const C3DMatrix & f_rotation, 
                                               const C3DVector & f_translation, 
                                               double            f_time_d )
{
   /// Lets update first the A matrix and B vector.
   m_rotation    = f_rotation;
   m_translation = f_translation;

   for (int i = 0; i < 3; ++i)
   {
      for (int j = 0; j < 3; ++j)
      {
         m_A_p[(i  )*6+j  ] = f_rotation.at(i,j);
         m_A_p[(i  )*6+j+3] = f_time_d * f_rotation.at(i,j);
         m_A_p[(i+3)*6+j+3] = f_rotation.at(i,j);
         m_A_p[(i+3)*6+j  ] = 0.;
      }

      m_B_p[i]   = f_translation.at(i);
      m_B_p[i+3] = 0;
   }

   /// Lets compute now the corresponding Q matrix.
   double t2_d   = f_time_d * f_time_d;
   double t3_d   = t2_d * f_time_d;
   double val1_d = m_sysPosVar_d * f_time_d + m_sysVelVar_d * t3_d/3.;
   double val2_d = m_sysVelVar_d * t2_d/2.;
   double val3_d = m_sysVelVar_d * f_time_d;

   memset(m_Q_p, 0, sizeof(double) * 36);

   for (int i = 0; i < 3; ++i)
   {
      m_Q_p[(i  )*6+i  ] = val1_d;
      m_Q_p[(i+3)*6+i  ] = m_Q_p[i*6+i+3] = val2_d;
      m_Q_p[(i+3)*6+i+3] = val3_d;
   }

   return true;
}

/// Set stereo camera.
bool
CKF3DStereoPointCommon::setCamera ( const CStereoCamera & f_camera )
{
   m_camera = f_camera;
   m_fu_d   = m_camera.getFu();
   m_fu2_d  = m_fu_d * m_fu_d;
   m_fv_d   = m_camera.getFv();
   m_B_d    = m_camera.getBaseline();
   m_B2_d   = m_B_d * m_B_d;\
   m_Bfu_d  = m_B_d * m_fu_d;
   m_Bsvu_d = m_B_d * m_fu_d / m_fv_d;

   return true;
}


/// Set stereo camera.
bool
CKF3DStereoPointCommon::setMeasurementCovMatrix ( const C3DMatrix & f_matrix )
{
   m_R = f_matrix;
   return true;
}


/// Set initial variances.
bool
CKF3DStereoPointCommon::setInitialVelocityVariance ( double f_vx_d, 
                                                     double f_vy_d, 
                                                     double f_vz_d )
{
   m_iniVarVelX_d = f_vx_d;
   m_iniVarVelY_d = f_vy_d;
   m_iniVarVelZ_d = f_vz_d;
    
   return true;
}


/// Set initial variances.
bool
CKF3DStereoPointCommon::setInitialVelocityVariance ( const C3DVector &f_variance )
{

   return setInitialVelocityVariance ( f_variance.x(),
                                       f_variance.y(),
                                       f_variance.z() );
}


/// Set initial variances.
bool
CKF3DStereoPointCommon::setSystemVariance ( double f_posVar_d, 
                                            double f_velVar_d )
{
   m_sysPosVar_d = f_posVar_d;
   m_sysVelVar_d = f_velVar_d;
   return true;
}


bool
CKF3DStereoPointCommon::setInitialXVelocityVariance ( double f_v_d )
{
   m_iniVarVelX_d = f_v_d;
   return true;
}

double
CKF3DStereoPointCommon::getInitialXVelocityVariance ( ) const
{
   return m_iniVarVelX_d;
}

bool
CKF3DStereoPointCommon::setInitialYVelocityVariance ( double f_v_d )
{
   m_iniVarVelY_d = f_v_d;
   return true;
}

double
CKF3DStereoPointCommon::getInitialYVelocityVariance ( ) const
{
   return m_iniVarVelY_d;
}

bool
CKF3DStereoPointCommon::setInitialZVelocityVariance ( double f_v_d )
{
   m_iniVarVelZ_d = f_v_d;
   return true;
}

double
CKF3DStereoPointCommon::getInitialZVelocityVariance ( ) const
{
   return m_iniVarVelZ_d;
}

/// Set system velocity variance.
bool
CKF3DStereoPointCommon::setSystemPositionVariance ( double f_var_d )
{
   m_sysPosVar_d = f_var_d;
   return true;
}

double
CKF3DStereoPointCommon::getSystemPositionVariance ( ) const
{
   return m_sysPosVar_d;    
}

/// Set system position variance.
bool
CKF3DStereoPointCommon::setSystemVelocityVariance ( double f_var_d )
{
   m_sysVelVar_d = f_var_d;
   return true;
}
double
CKF3DStereoPointCommon::getSystemVelocityVariance ( ) const
{
   return m_sysVelVar_d;    
}

bool
CKF3DStereoPointCommon::setCheckNSigmaTest ( bool f_newVal_b )
{
   m_checkNSigmaTest_b =  f_newVal_b;
   return true;
}

bool
CKF3DStereoPointCommon::getCheckNSigmaTest ( ) const
{
   return m_checkNSigmaTest_b;
}

bool
CKF3DStereoPointCommon::setMinAge4NSigmaTest ( int f_newAge_i )
{
   m_minAge4NSigmaTest_i = f_newAge_i;
   return true;
}

int
CKF3DStereoPointCommon::getMinAge4NSigmaTest ( ) const
{
   return m_minAge4NSigmaTest_i;
}

bool
CKF3DStereoPointCommon::setMaxSqNSigma ( double f_newDist_d )
{
   m_maxSqNSigma_d = f_newDist_d;
   return true;
}

double
CKF3DStereoPointCommon::getMaxSqNSigma () const
{
   return m_maxSqNSigma_d;
}

/// Set/Get normalized innovation squares integration factor.
bool
CKF3DStereoPointCommon::setNISIntegrationFactor ( double f_newFactor_d )
{
   m_nisAlpha_d = f_newFactor_d;
   return true;
}

double
CKF3DStereoPointCommon::getNISIntegrationFactor ( ) const
{
   return m_nisAlpha_d;
}

