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
* @file featKFOp.cpp
*
* \class CFeatureKFOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief Framework operator class for for tracking features with Kalman Filter
*
*******************************************************************************/

/* INCLUDES */

#include "featKFOp.h"
#include "drawingList.h"
#include "stereoCamera.h"
#include "rigidMotion.h"
#include "clock.h"

#include "feature.h"

#include "featVisKFOp.h"

//#include <sys/time.h>

using namespace QCV;


/// Constructors.
CFeatureKFOp::CFeatureKFOp ( COperator * const f_parent_p )
   : COperator (            f_parent_p, "3D Feature KF" ),
     m_compute_b (                                false ),
     m_trackInpId_str (                "Feature Vector" ),
     m_egoMInpId_str (          "Ego-Motion Estimation" ),
     m_initialized_b (                            false ),
     m_kfCommon (                                       ),
     m_kfVector (                                       ),
          
     m_maxNoMeasCount_i (                             2 ),

     m_measVariance (                  0.25, 0.25, 0.25 ),
     m_initialVelVariance (            100., 100., 100. ),
     m_sysVariance (                         0.01, 0.01 ),
     m_useFixCycleTime_b  (                       false ),
     m_cycleTime_d (                             1./30. )
{
   addChild ( new  CFeatureKFDisplayOp        ( this ) );

   /// Some default values.
   registerDrawingLists();
   registerParameters();
}

void
CFeatureKFOp::registerDrawingLists()
{
   registerDrawingList ("Vectors",
                        S2D<int> (0, 0),
                        false);
}

void
CFeatureKFOp::registerParameters()
{
   ADD_STR_PARAMETER( "Feat. Track Input String Id", 
                      "String identification of the input. Input "
                      "must be a CKltFeatureVector object type.",
                      m_trackInpId_str,
                      this,
                      TrackInputIdStr, 
                      CFeatureKFOp );

   ADD_STR_PARAMETER( "Ego-Motion Input String Id", 
                      "String identification of the ego-motion. Input "
                      "must be a SRigidMotion object type.",
                      m_egoMInpId_str,
                      this,
                      EgoMInputIdStr, 
                      CFeatureKFOp );

   ADD_LINE_SEPARATOR;

   ADD_BOOL_PARAMETER( "Compute", 
                       "Compute Kalman filter vectors?",
                       m_compute_b,
                       this,
                       Compute, 
                       CFeatureKFOp );
    
   ADD_INT_PARAMETER( "Max No Measurement Count",
                      "Maximum no measurement count.",
                      m_maxNoMeasCount_i,
                      this,
                      MaxNoMeasCount,
                      CFeatureKFOp );

   /// Maximum no measurement count.
   ADD_BOOL_PARAMETER( "Check N Sigma",
                       "Check if measurement is within the N sigma of state variance.",
                       m_kfCommon.getCheckNSigmaTest(),
                       &m_kfCommon,
                       CheckNSigmaTest,
                       CKF3DStereoPointCommon );

   /// Min Age 4 sigma test.
   ADD_INT_PARAMETER( "Min Age 4 N Sigma Test",
                      "Min age that a feature must have "
                      "to apply the N Sigma Test.",
                      m_kfCommon.getMinAge4NSigmaTest(),
                      &m_kfCommon,
                      MinAge4NSigmaTest,
                      CKF3DStereoPointCommon );

   /// Max sq sigma dist.
   ADD_DOUBLE_PARAMETER( "Max Sq. N Sigma",
                         "Value of maximum square N sigma for N Sigma Test.",
                         m_kfCommon.getMaxSqNSigma(),
                         &m_kfCommon,
                         MaxSqNSigma,
                         CKF3DStereoPointCommon );

   /// Max sq sigma dist.
   ADD_DOUBLE_PARAMETER( "NIS Fading Factor",
                         "Intensity of the low pass filter for NIS values.",
                         m_kfCommon.getNISIntegrationFactor(),
                         &m_kfCommon,
                         NISIntegrationFactor,
                         CKF3DStereoPointCommon );

   /// Measurement variances
   ADD_3DVEC_PARAMETER( "Measurement Variance",
                        "Variances for each u,v,d measurement component [px^2].",
                        m_measVariance,
                        "u","v","d",
                        this,
                        MeasVariance,
                        CFeatureKFOp );

   /// Measurement variances
   ADD_3DVEC_PARAMETER( "Initial Vel Variance",
                        "Initial variances for each vx,vy,vz state component [m^2].",
                        m_initialVelVariance,
                        "vx","vy","vz",
                        this,
                        InitialVelVariance,
                        CFeatureKFOp );

   ADD_DBL2D_PARAMETER( "System Variance",
                        "System variances for position and velocity components [m^2].",
                        m_sysVariance,
                        "pos", "vel",
                        this,
                        SystemVariance,
                        CFeatureKFOp );

   ADD_BOOL_PARAMETER( "Fix Cycle Time",
                       "Use a fix given cycle time?",
                       m_useFixCycleTime_b,
                       this,
                       UseFixCycleTime,
                       CFeatureKFOp );

   ADD_DOUBLE_PARAMETER( "Cycle Time",
                         "Assumed cycle time [s].",
                         m_cycleTime_d,
                         this,
                         CycleTime,
                         CFeatureKFOp );
}

/// Virtual destructor.
CFeatureKFOp::~CFeatureKFOp ()
{
}

                           
/// Cycle event.
C3DVector CFeatureKFOp::getClosestSpeed( const SFeature       &f_feat,
                                         const CFeatureVector &f_featVector_v  )
{
   const float maxFeatDist_f = 20*20;
   const int   ageTh_i = 3; 
   float minDist_f = maxFeatDist_f;

   C3DVector p0 ( f_feat.u, f_feat.v, f_feat.d );
   C3DVector vel ( 0, 0, 0);
   
   for (size_t j = 0; j < f_featVector_v.size(); ++j)
   {
      int age_i = m_kfVector[j].getAge();
      
      if (age_i > ageTh_i)
      {
         C3DVector p1 (f_featVector_v[j].u,f_featVector_v[j].v,f_featVector_v[j].d);
         float dist_f = (p1 - p0).sumOfSquares();
         if ( dist_f  < maxFeatDist_f &&
              dist_f < minDist_f )
         {
            minDist_f = dist_f;
            vel = m_kfVector[j].getVelocity();
         }
      }
   }

   return vel;
}

/// Cycle event.
bool CFeatureKFOp::cycle()
{
   /// Request input data.
   CFeatureVector *   featVector_p = getInput<CFeatureVector> ( m_trackInpId_str );

   CStereoCamera *    camera_p     = getInput<CStereoCamera>("Rectified Camera");

   SRigidMotion *     egoMotion_p  = getInput<SRigidMotion>( m_egoMInpId_str );

   double cycleTime_d;
    
   if ( m_useFixCycleTime_b )
      cycleTime_d = m_cycleTime_d;
   else
      cycleTime_d = getInput<double> ("Cycle Time", m_cycleTime_d );

   if ( featVector_p && camera_p && egoMotion_p )
   {
      if ( m_kfVector.size() != featVector_p->size() )
      {
         m_kfVector.resize ( featVector_p->size() );
         m_kfVector[0].setCommonData( &m_kfCommon );            
      }

      C3DMatrix covVar;
      covVar.diagonalize( m_measVariance );

      m_kfCommon.setInitialVelocityVariance ( m_initialVelVariance );
      m_kfCommon.setSystemVariance ( m_sysVariance.x, m_sysVariance.y );
      m_kfCommon.setMeasurementCovMatrix ( covVar );        
      m_kfCommon.setCamera ( *camera_p );

      if (m_compute_b)
      {
         C3DMatrix rotation    = egoMotion_p->rotation;
         C3DVector translation = egoMotion_p->translation;       
            
         m_kfCommon.setEnvironmentMotion ( rotation, translation, cycleTime_d );

         for (unsigned int i = 0 ; i < featVector_p->size(); ++i )
         {
            if ( m_kfVector[i].getStatus() != CKF3DStereoPoint::KFS_PREDICTED )
               m_kfVector[i].predict();
         }

         for (unsigned int i = 0 ; i < featVector_p->size(); ++i )
         {
            const SFeature &feat = (*featVector_p)[i];

            if ( feat.state != SFeature::FS_NEW  && 
                 feat.state != SFeature::FS_TRACKED  )
            {
               m_kfVector[i].reset();
            }
            else
            {
               if ( m_kfVector[i].getAge() <= 0 )
               {
                  if ( feat.d > 1.e-3 )
                  {
                     C3DVector vel = getClosestSpeed(feat, *featVector_p);
                     m_kfVector[i].initialize ( feat.u, feat.v, feat.d );
                  }
               }
               else 
               {
                  if ( feat.state == SFeature::FS_NEW && feat.d > 1.e-3 )
                  {
                     m_kfVector[i].initialize ( feat.u, feat.v, feat.d );
                  }                    
                  else
                  {
                     bool success_b = false;
                            
                     if ( feat.d > 1.e-3 )
                        success_b = m_kfVector[i].update ( feat.u, feat.v, feat.d );
                     else
                     {
                        if ( m_kfVector[i].getNoMeasurementCount() <= m_maxNoMeasCount_i )
                           success_b = m_kfVector[i].update ( feat.u, feat.v );
                     }

                     if ( !success_b )
                     {
                        m_kfVector[i].reset();   
                     }
                  }
               }
            }
         }
      }
   }
    
   /// Set output.
   registerOutput ( "KF 3D Stereo Point Vector",       &m_kfVector );
   registerOutput ( "KF 3D Stereo Point Common Data",  &m_kfCommon );

   return COperator::cycle();
}


/// Show event.
bool CFeatureKFOp::show()
{
   return COperator::show();
}

/// Init event.
bool CFeatureKFOp::initialize()
{
   /// Set output.
   registerOutput ( "KF 3D Stereo Point Vector",       &m_kfVector );
   registerOutput ( "KF 3D Stereo Point Common Data",  &m_kfCommon );

   return COperator::initialize();
}

/// Reset event.
bool CFeatureKFOp::reset()
{
   return COperator::reset();
}

bool CFeatureKFOp::exit()
{
   return COperator::exit();
}

void 
CFeatureKFOp::keyPressed ( CKeyEvent * f_event_p )
{
   return COperator::keyPressed ( f_event_p );    
}
