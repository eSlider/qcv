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

/* INCLUDES */
#include "kf3DStereoPoint.h"

using namespace QCV;

CKF3DStereoPointCommon* CKF3DStereoPoint::m_commonData_p = NULL;


CKF3DStereoPoint::CKF3DStereoPoint ( )
   : m_age_i (                 -1 ),
     m_noMeasCount_i (          0 ),
     m_nis_d (                  0 ),
     m_status (  KFS_UNINITIALIZED )
{
   memset ( m_state_p,     0, sizeof(double) * 6 );
   memset ( m_covMatrix_p, 0, sizeof(double) * 6 * 6 );
}

CKF3DStereoPoint::~CKF3DStereoPoint ( )
{
    
}

/// Initialize with new measurement
bool
CKF3DStereoPoint::initialize  (  float f_u_f, 
                                 float f_v_f,
                                 float f_d_f )
{
   return initializeFilter( C3DVector(f_u_f, f_v_f, f_d_f),
                            m_commonData_p->m_R );
}

/// Initialize with new measurement
bool
CKF3DStereoPoint::initialize  ( const C3DVector &f_meas )
{
   return initializeFilter ( f_meas,
                             m_commonData_p->m_R );
}

/// Initialize with new measurement
bool
CKF3DStereoPoint::initialize  ( float f_u_f, 
                                float f_v_f,
                                float f_d_f,
                                const C3DMatrix &f_covMatrix )
{
   return initializeFilter( C3DVector(f_u_f, f_v_f, f_d_f),
                            f_covMatrix );
}

/// Initialize with new measurement
bool
CKF3DStereoPoint::initialize  ( const C3DVector &f_meas,
                                const C3DMatrix &f_covMatrix )
{
   return initializeFilter( f_meas,
                            f_covMatrix );
}

/// Initialize with measurement and covariance matrix.
bool
CKF3DStereoPoint::initializeFilter  ( const C3DVector &f_meas,
                                      const C3DMatrix &f_R,
                                      const C3DVector f_speed )
{
   if ( !m_commonData_p ) return false;

   C3DVector p1;
    
   /// Initialize state: position first.
   m_commonData_p -> m_camera.image2Local ( f_meas.x(),
                                            f_meas.y(),
                                            f_meas.z(),
                                            m_state_p[0],
                                            m_state_p[1],
                                            m_state_p[2] );


   /// velocity now.
   m_state_p[3] = f_speed.x();
   m_state_p[4] = f_speed.y();
   m_state_p[5] = f_speed.z();

   //double z2_d = p1.z()*p1.z();
   //double a_d = m_commonData_p->m_fu2_d / (z2_d * p1.z());
   //double *p = m_covMatrix_p;

   memset( m_covMatrix_p, 0, sizeof(double) * 36 );
    
   /// Initialize covariance: position first
   /// Equation is G R G^T with G partial derivatives of triangulation equation and R
   /// covariance matrix of measurement.
   double d2_d = f_meas.z() * f_meas.z();
    
   C3DMatrix G;

   G.at(0,0) = m_commonData_p->m_B_d / f_meas.z();
   G.at(0,1) = 0;
   G.at(0,2) = -f_meas.x() * m_commonData_p->m_B_d / d2_d;
   G.at(1,0) = 0;
   G.at(1,1) = -m_commonData_p->m_Bsvu_d / f_meas.z();
   G.at(1,2) = +f_meas.y() * m_commonData_p->m_Bsvu_d / d2_d;
   G.at(2,0) = 0;
   G.at(2,1) = 0;
   G.at(2,2) = -m_commonData_p->m_Bfu_d / d2_d;

   C3DMatrix Gt = G.getTranspose();
    
   C3DMatrix covMatPos = G * f_R * Gt;

   m_covMatrix_p[0*6+0] = covMatPos.at(0,0);
   m_covMatrix_p[0*6+1] = covMatPos.at(0,1);
   m_covMatrix_p[0*6+2] = covMatPos.at(0,2);
   m_covMatrix_p[1*6+0] = covMatPos.at(1,0);
   m_covMatrix_p[1*6+1] = covMatPos.at(1,1);
   m_covMatrix_p[1*6+2] = covMatPos.at(1,2);
   m_covMatrix_p[2*6+0] = covMatPos.at(2,0);
   m_covMatrix_p[2*6+1] = covMatPos.at(2,1);
   m_covMatrix_p[2*6+2] = covMatPos.at(2,2);

   /// velocity now.
   m_covMatrix_p[3*6+3] = m_commonData_p -> m_iniVarVelX_d;
   m_covMatrix_p[4*6+4] = m_commonData_p -> m_iniVarVelY_d;
   m_covMatrix_p[5*6+5] = m_commonData_p -> m_iniVarVelZ_d;

   m_age_i = 1;
   m_noMeasCount_i = 0;
    
   m_status = KFS_INITIALIED;
    
   return true;
}

/// Update with measurement
bool
CKF3DStereoPoint::update ( float f_u_f, 
                           float f_v_f )
{
   if (m_age_i <= 0 || !m_commonData_p)
      return false;

   float d_f = m_commonData_p->m_Bfu_d / m_state_p[2];
   ++m_noMeasCount_i;
   ++m_age_i;
    
   return updateFilter( C3DVector ( f_u_f, f_v_f, d_f ),
                        m_commonData_p -> m_R );
}

/// Update with measurement
bool
CKF3DStereoPoint::update ( const C3DVector &measurement )
{
   if (m_age_i <= 0 || !m_commonData_p)
      return false;

   m_noMeasCount_i = 0;
   ++m_age_i;
    
   return updateFilter( measurement,                         
                        m_commonData_p -> m_R );
}

/// Update with measurement
bool
CKF3DStereoPoint::update ( float f_u_f, 
                           float f_v_f,
                           float f_d_f )
{
   if (m_age_i <= 0 || !m_commonData_p)
      return false;

   return update( C3DVector ( f_u_f, f_v_f, f_d_f ) );
}

/// Update with measurement and covariance matrix
bool
CKF3DStereoPoint::update ( const C3DVector &measurement,
                           const C3DMatrix &f_covMatrix  )
{
   if (m_age_i <= 0 || !m_commonData_p)
      return false;

   m_noMeasCount_i = 0;
   ++m_age_i;
    
   return updateFilter( measurement,                         
                        f_covMatrix );
}

/// Update with measurement and covariance matrix
bool
CKF3DStereoPoint::update ( float f_u_f, 
                           float f_v_f,
                           float f_d_f,
                           const C3DMatrix &f_covMatrix  )
{
   if (m_age_i <= 0 || !m_commonData_p)
      return false;

   return update( C3DVector ( f_u_f, f_v_f, f_d_f ),
                  f_covMatrix );
}

/// Update with measurement
bool
CKF3DStereoPoint::updateFilter ( const C3DVector &measurement,
                                 const C3DMatrix &f_R )
{
   /// We are using conventional KF for now.

   // Lets compute first error vector v
   C3DVector hx;
   m_commonData_p -> m_camera.local2Image (  m_state_p[0], m_state_p[1], m_state_p[2],
                                             hx.at(0), hx.at(1), hx.at(2) );
   C3DVector v = measurement - hx;

   // Compute F and K matrices
   /// F = H P H^T + R    
   /// Let's construct first the F matrix.
   C3DMatrix H;
   double Z2_d = m_state_p[2] * m_state_p[2];

   H.at(0,0) = m_commonData_p->m_fu_d / m_state_p[2];
   H.at(0,1) = 0;
   H.at(0,2) = -m_commonData_p->m_fu_d * m_state_p[0] / Z2_d;
   H.at(1,0) = 0;
   H.at(1,1) = -m_commonData_p->m_fv_d / m_state_p[2];
   H.at(1,2) = +m_commonData_p->m_fv_d * m_state_p[1] / Z2_d;
   H.at(2,0) = 0;
   H.at(2,1) = 0;
   H.at(2,2) = -m_commonData_p->m_Bfu_d / Z2_d;

   C3DMatrix Ht = H.getTranspose();
    
   /// Extract upper left 3x3 submatrix.
   C3DMatrix P;
   P.at(0,0) = m_covMatrix_p[0*6+0]; 
   P.at(0,1) = P.at(1,0) = m_covMatrix_p[0*6+1]; 
   P.at(0,2) = P.at(2,0) = m_covMatrix_p[0*6+2];
   P.at(1,1) = m_covMatrix_p[1*6+1];
   P.at(1,2) = P.at(2,1) = m_covMatrix_p[1*6+2];
   P.at(2,2) = m_covMatrix_p[2*6+2];

   C3DMatrix F = (H * P * Ht) + f_R;
   C3DMatrix FInv = F.getInverse();

   /// Check the N sigma test as (v^T F^-1 v)
   double mahalanobis_d = v.dotProduct( FInv * v );

   if ( m_commonData_p -> m_checkNSigmaTest_b && 
        m_age_i >= m_commonData_p -> m_minAge4NSigmaTest_i )
   {
      //printf("Mahalanobis is %f\n", mahalanobis_d );

      if ( mahalanobis_d >  m_commonData_p -> m_maxSqNSigma_d )
      {
         /// 3 sigma test failed.
         return false;
      }        
   }

   if ( m_age_i == 2 ) // age two means first update
      m_nis_d = mahalanobis_d;
   else if ( m_age_i < m_commonData_p -> m_minAge4NSigmaTest_i )
      /// Average of values is performed at the beginning.
      m_nis_d = (m_nis_d * (m_age_i-1) + mahalanobis_d)/m_age_i;
   else
      /// Now, low pass filtering.
      m_nis_d = ( ( m_nis_d       * m_commonData_p -> m_nisAlpha_d) + 
                  ( mahalanobis_d * (1.-m_commonData_p -> m_nisAlpha_d) ) );

   /// Compute now H^T F^-1
   /// Note:
   /// Observe that the real HtFInv matrix is a 6x3 matrix but since
   /// the left 3x3 submatrix of H is a null matrix, the corresponding
   /// lower 3x3 submatrix of the resulting matrix (H^T F^-1) is also 
   /// a null matrix.
   C3DMatrix HtFInv = Ht * FInv;
    
   /// Define K matrix (6x3)
   double K_p[32];

   /// Now construct final K matrix as the multiplication of current predicted
   // covariance matrix and HtFInv, i.e. K = P (H^T F^-1)
   for (int i = 0; i < 6; ++i)
   {
      for (int j = 0; j < 3; ++j)
      {
         K_p[i*3+j] = ( m_covMatrix_p[i*6+0] * HtFInv.at(0,j) + 
                        m_covMatrix_p[i*6+1] * HtFInv.at(1,j) + 
                        m_covMatrix_p[i*6+2] * HtFInv.at(2,j) );
         /// The rest of the elements of the real 6x3 HtFInv matrix are 0 so
         /// we optimize computation by avoiding additional multiplications.
      }
   }

   /// Update now system estimate as x_k = x'_k + K (z_k - h(x'_k))
   for (int i = 0; i < 6; ++i)
   {
      m_state_p[i] += ( K_p[i*3+0] * v.x() + 
                        K_p[i*3+1] * v.y() + 
                        K_p[i*3+2] * v.z() );
   }

   /// Update now covariance as P_k = (I - K H) P

   /// Let's build first (-K H)
   double KH_p[6*6]; memset(KH_p, 0, sizeof(double)*36);

   /// Here we take advantage of the fact that the (3x3) matrix is upper 
   /// triangular and has zero on the entry (1,2). so:    
   for (int i = 0; i < 6; ++i)
   {
      KH_p[i*6+0] = -K_p[i*3+0] * H.at(0,0);
      KH_p[i*6+1] = -K_p[i*3+1] * H.at(1,1);
      KH_p[i*6+2] = -( K_p[i*3+0] * H.at(0,2) + 
                       K_p[i*3+1] * H.at(1,2) + 
                       K_p[i*3+2] * H.at(2,2) );
   }

   // Now compute (I - K H)
   for (int i = 0; i < 6; ++i)
      KH_p[i*6+i] = 1+KH_p[i*6+i];

   /// Finally compute (I - K H) P

   /// Note:
   /// The matrix (I - K H) has 0 for the entries j>3 && i!=j
   /// and 1 for entries i==j, j>3 so we take advantaje and
   /// optimize for it:

   double newCovMat_p[36];

   int j = 0;
   newCovMat_p[0*6+j] = ( KH_p[0*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[0*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[0*6+2] * m_covMatrix_p[2*6+j] );
    
   newCovMat_p[1*6+j] = ( KH_p[1*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[1*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[1*6+2] * m_covMatrix_p[2*6+j] );

   newCovMat_p[2*6+j] = ( KH_p[2*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[2*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[2*6+2] * m_covMatrix_p[2*6+j] );

   newCovMat_p[3*6+j] = ( KH_p[3*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[3*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[3*6+2] * m_covMatrix_p[2*6+j] +
                          m_covMatrix_p[3*6+j] );

   newCovMat_p[4*6+j] = ( KH_p[4*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[4*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[4*6+2] * m_covMatrix_p[2*6+j] + 
                          m_covMatrix_p[4*6+j] );

   newCovMat_p[5*6+j] = ( KH_p[5*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[5*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[5*6+2] * m_covMatrix_p[2*6+j] +
                          m_covMatrix_p[5*6+j] );

   j = 1;    
   newCovMat_p[1*6+j] = ( KH_p[1*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[1*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[1*6+2] * m_covMatrix_p[2*6+j] );

   newCovMat_p[2*6+j] = ( KH_p[2*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[2*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[2*6+2] * m_covMatrix_p[2*6+j] );

   newCovMat_p[3*6+j] = ( KH_p[3*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[3*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[3*6+2] * m_covMatrix_p[2*6+j] +
                          m_covMatrix_p[3*6+j] );

   newCovMat_p[4*6+j] = ( KH_p[4*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[4*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[4*6+2] * m_covMatrix_p[2*6+j] + 
                          m_covMatrix_p[4*6+j] );

   newCovMat_p[5*6+j] = ( KH_p[5*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[5*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[5*6+2] * m_covMatrix_p[2*6+j] +
                          m_covMatrix_p[5*6+j] );

   j = 2;
   newCovMat_p[2*6+j] = ( KH_p[2*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[2*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[2*6+2] * m_covMatrix_p[2*6+j] );

   newCovMat_p[3*6+j] = ( KH_p[3*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[3*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[3*6+2] * m_covMatrix_p[2*6+j] +
                          m_covMatrix_p[3*6+j] );

   newCovMat_p[4*6+j] = ( KH_p[4*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[4*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[4*6+2] * m_covMatrix_p[2*6+j] + 
                          m_covMatrix_p[4*6+j] );

   newCovMat_p[5*6+j] = ( KH_p[5*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[5*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[5*6+2] * m_covMatrix_p[2*6+j] +
                          m_covMatrix_p[5*6+j] );

   j = 3;
   newCovMat_p[3*6+j] = ( KH_p[3*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[3*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[3*6+2] * m_covMatrix_p[2*6+j] +
                          m_covMatrix_p[3*6+j] );

   newCovMat_p[4*6+j] = ( KH_p[4*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[4*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[4*6+2] * m_covMatrix_p[2*6+j] + 
                          m_covMatrix_p[4*6+j] );

   newCovMat_p[5*6+j] = ( KH_p[5*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[5*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[5*6+2] * m_covMatrix_p[2*6+j] +
                          m_covMatrix_p[5*6+j] );

   j = 4;
   newCovMat_p[4*6+j] = ( KH_p[4*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[4*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[4*6+2] * m_covMatrix_p[2*6+j] + 
                          m_covMatrix_p[4*6+j] );

   newCovMat_p[5*6+j] = ( KH_p[5*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[5*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[5*6+2] * m_covMatrix_p[2*6+j] +
                          m_covMatrix_p[5*6+j] );

   j = 5;
   newCovMat_p[5*6+j] = ( KH_p[5*6+0] * m_covMatrix_p[0*6+j] + 
                          KH_p[5*6+1] * m_covMatrix_p[1*6+j] + 
                          KH_p[5*6+2] * m_covMatrix_p[2*6+j] +
                          m_covMatrix_p[5*6+j] );

   newCovMat_p[0*6+1] = newCovMat_p[1*6+0];
   newCovMat_p[0*6+2] = newCovMat_p[2*6+0];
   newCovMat_p[0*6+3] = newCovMat_p[3*6+0];
   newCovMat_p[0*6+4] = newCovMat_p[4*6+0];
   newCovMat_p[0*6+5] = newCovMat_p[5*6+0];

   newCovMat_p[1*6+2] = newCovMat_p[2*6+1];
   newCovMat_p[1*6+3] = newCovMat_p[3*6+1];
   newCovMat_p[1*6+4] = newCovMat_p[4*6+1];
   newCovMat_p[1*6+5] = newCovMat_p[5*6+1];

   newCovMat_p[2*6+3] = newCovMat_p[3*6+2];
   newCovMat_p[2*6+4] = newCovMat_p[4*6+2];
   newCovMat_p[2*6+5] = newCovMat_p[5*6+2];

   newCovMat_p[3*6+4] = newCovMat_p[4*6+3];
   newCovMat_p[3*6+5] = newCovMat_p[5*6+3];

   newCovMat_p[4*6+5] = newCovMat_p[5*6+4];
    

   memcpy(m_covMatrix_p, newCovMat_p, 36*sizeof(double));
   /*
     printf("Covariance matrix is:\n");
     for (int i = 0; i < 6; ++i)
     {
     printf("| ");
     for (int j = 0; j < 6; ++j)
     printf("%13g ", m_covMatrix_p[i*6+j]);
     printf("|\n");
     }

     printf("Covariance matrix symmetry:\n");
     for (int i = 0; i < 6; ++i)
     {
     printf("| ");
     for (int j = 0; j < 6; ++j)
     printf("%13g ", m_covMatrix_p[i*6+j] - m_covMatrix_p[j*6+i]);
     printf("|\n");
     }
   */

   m_status = KFS_UPDATED;

   return true;
}

/// Predict
bool
CKF3DStereoPoint::predict ( )
{
   if (m_age_i <= 0 || !m_commonData_p )
      return false;

   /// State prediction is x_k+1 = A x_k + B
   /// Note: the transition matrix,  input vector and system variance
   /// contained in the  common object must correspond to the current 
   /// time. So before calling predict() the setEnvironmentMotion(...) 
   /// method of class CKF3DStereoPointCommon must be called.
   double newState_p[6];
    
   for (int i = 0; i < 6; ++i)
   {
      newState_p[i] = ( ( m_commonData_p->m_A_p[i*6+0] * m_state_p[0] + 
                          m_commonData_p->m_A_p[i*6+1] * m_state_p[1] + 
                          m_commonData_p->m_A_p[i*6+2] * m_state_p[2] + 
                          m_commonData_p->m_A_p[i*6+3] * m_state_p[3] + 
                          m_commonData_p->m_A_p[i*6+4] * m_state_p[4] + 
                          m_commonData_p->m_A_p[i*6+5] * m_state_p[5] ) + 
                        m_commonData_p->m_B_p[i] );
   }

   memcpy(m_state_p, newState_p, 6 * sizeof(double));

   /// Covariance Prediction is P_k+1 = A_k+1 P_k A_k+1^T + Q_k+1
   /// Lets compute first A_k+1 P_k
   double AP_p[36];    
    
   /// Note: 
   /// Here we use the knowledge that the left-lower 3x3 submatrix 
   /// of A_k+1 is a null matrix.
   for (int j = 0; j < 6; ++j)
   {
      for (int i = 0; i < 6; ++i)
      {
         AP_p[i*6+j] = ( m_commonData_p->m_A_p[i*6+0] * m_covMatrix_p[0*6+j] + 
                         m_commonData_p->m_A_p[i*6+1] * m_covMatrix_p[1*6+j] + 
                         m_commonData_p->m_A_p[i*6+2] * m_covMatrix_p[2*6+j] + 
                         m_commonData_p->m_A_p[i*6+3] * m_covMatrix_p[3*6+j] + 
                         m_commonData_p->m_A_p[i*6+4] * m_covMatrix_p[4*6+j] + 
                         m_commonData_p->m_A_p[i*6+5] * m_covMatrix_p[5*6+j] );
      }

      for (int i = 3; 0 && i < 6; ++i)
      {
         AP_p[i*6+j] = ( m_commonData_p->m_A_p[i*6+3] * m_covMatrix_p[3*6+j] + 
                         m_commonData_p->m_A_p[i*6+4] * m_covMatrix_p[4*6+j] + 
                         m_commonData_p->m_A_p[i*6+5] * m_covMatrix_p[5*6+j] );
      }
   }

   /// Lets compute now A_k+1 P_k A_k+1^T

   double APAt_p[36];
   /// Note: 
   /// Here we use the knowledge that the right-upper 3x3 submatrix 
   /// of A_k+1^T is a null matrix.
   for (int i = 0; i < 6; ++i)
   {
      for (int j = 0; j < 6; ++j)
      {
         APAt_p[i*6+j] = ( AP_p[i*6+0] * m_commonData_p->m_A_p[j*6+0] + 
                           AP_p[i*6+1] * m_commonData_p->m_A_p[j*6+1] + 
                           AP_p[i*6+2] * m_commonData_p->m_A_p[j*6+2] +
                           AP_p[i*6+3] * m_commonData_p->m_A_p[j*6+3] +
                           AP_p[i*6+4] * m_commonData_p->m_A_p[j*6+4] +
                           AP_p[i*6+5] * m_commonData_p->m_A_p[j*6+5] );
      }

      for (int j = 3; 0 && j < 6; ++j)
      {
         APAt_p[i*6+j] = ( AP_p[i*6+3] * m_commonData_p->m_A_p[j*6+3] +
                           AP_p[i*6+4] * m_commonData_p->m_A_p[j*6+4] +
                           AP_p[i*6+5] * m_commonData_p->m_A_p[j*6+5] );
      }
   }

   /// Finally, lets add system covariance, i.e. A P A^T + Q
   for (int i = 0; i < 36; ++i)
      m_covMatrix_p[i] = APAt_p[i] + m_commonData_p->m_Q_p[i];

   m_status = KFS_PREDICTED;

   return true;
}


/// Get current state
void
CKF3DStereoPoint::getStateVector ( double * f_state_p ) const
{
   memcpy( f_state_p, 
           m_state_p, 
           sizeof(double) * 6 );
}


/// Get current covariance
void
CKF3DStereoPoint::getCovarianceMatrix  ( double *f_covariance_p ) const
{
   memcpy( f_covariance_p, 
           m_covMatrix_p, 
           sizeof(double) * 36 );
}


/// Get current state
void
CKF3DStereoPoint::getCurrentState      ( C3DVector &fr_pos,
                                         C3DVector &fr_vel ) const
{
   fr_pos.set( m_state_p[0], 
               m_state_p[1], 
               m_state_p[2] );

   fr_vel.set( m_state_p[3], 
               m_state_p[4], 
               m_state_p[5] );
}

/// Get current state
C3DVector
CKF3DStereoPoint::getPosition () const
{
   return C3DVector ( m_state_p[0],
                      m_state_p[1], 
                      m_state_p[2] );
}

/// Get current velocity
C3DVector
CKF3DStereoPoint::getVelocity () const
{
   return C3DVector ( m_state_p[3],
                      m_state_p[4], 
                      m_state_p[5] );
}

/// Get predicted measurement.
C3DVector
CKF3DStereoPoint::getPredictedMeasurement ( ) const
{
   C3DVector vec;
   m_commonData_p->m_camera.local2Image ( m_state_p[0],
                                          m_state_p[1],
                                          m_state_p[2],
                                          vec.at(0),
                                          vec.at(1),
                                          vec.at(2) );

   return vec;
}



/// Get position covariance matrix.
void
CKF3DStereoPoint::getPositionCovMatrix ( C3DMatrix &fr_cov ) const
{
   fr_cov.at(0,0) = m_covMatrix_p[0*6+0];
   fr_cov.at(0,1) = fr_cov.at(1,0) = m_covMatrix_p[0*6+1];
   fr_cov.at(0,2) = fr_cov.at(2,0) = m_covMatrix_p[0*6+2];
   fr_cov.at(1,1) = m_covMatrix_p[1*6+1];
   fr_cov.at(1,2) = fr_cov.at(2,1) = m_covMatrix_p[1*6+2];
   fr_cov.at(2,2) = m_covMatrix_p[2*6+2];    
}


/// Get velocity covariance matrix.
void
CKF3DStereoPoint::getVelocityCovMatrix ( C3DMatrix &fr_cov ) const
{
   fr_cov.at(0,0) = m_covMatrix_p[3*6+3];
   fr_cov.at(0,1) = fr_cov.at(1,0) = m_covMatrix_p[3*6+4];
   fr_cov.at(0,2) = fr_cov.at(2,0) = m_covMatrix_p[3*6+5];
   fr_cov.at(1,1) = m_covMatrix_p[4*6+4];
   fr_cov.at(1,2) = fr_cov.at(2,1) = m_covMatrix_p[4*6+5];
   fr_cov.at(2,2) = m_covMatrix_p[5*6+5];
}


/// Set common object.
void
CKF3DStereoPoint::setCommonData   ( CKF3DStereoPointCommon * f_ptr_p )
{
   m_commonData_p = f_ptr_p;
}

/// Get velocity covariance matrix.
bool
CKF3DStereoPoint::reset ( )
{
   memset(m_state_p, 0, 6 * sizeof(double));
   memset(m_covMatrix_p, 0, 36 * sizeof(double));
   m_age_i = -1;
   m_status = KFS_UNINITIALIZED;
   return true;
}

