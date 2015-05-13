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

#ifndef __FEATUREKFOP_H
#define __FEATUREKFOP_H

/**
*******************************************************************************
*
* @file featKFOp.cpp
*
* \class CFeatureKFOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief Framework operator class for tracking features with Kalman Filter
* 
* 
*******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <limits>

#include "operator.h"
#include "feature.h"

#include "kf3DStereoPointCommon.h"
#include "kf3DStereoPointVector.h"
#include "kf3DStereoPoint.h"

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
   class CFeatureKFOp: public QCV::COperator
   {
   /// Constructor, Desctructors
   public:    
        
      /// Constructors.
      CFeatureKFOp ( COperator * const f_parent_p = NULL );
        
      /// Virtual destructor.
      virtual ~CFeatureKFOp ();

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

   /// User Operation Events
   public:
      /// Key pressed in display.
      virtual void keyPressed (     CKeyEvent * f_event_p );

   /// Gets and sets
   public:

   /// Gets and sets (Parameters)
   public:

      ADD_PARAM_ACCESS (bool,        m_compute_b,           Compute );
      ADD_PARAM_ACCESS (std::string, m_trackInpId_str,      TrackInputIdStr );
      ADD_PARAM_ACCESS (std::string, m_egoMInpId_str,       EgoMInputIdStr );

      ADD_PARAM_ACCESS (int,         m_maxNoMeasCount_i,    MaxNoMeasCount );
        
      ADD_PARAM_ACCESS (C3DVector,   m_measVariance,        MeasVariance );
      ADD_PARAM_ACCESS (C3DVector,   m_initialVelVariance,  InitialVelVariance );

      ADD_PARAM_ACCESS (S2D<double>, m_sysVariance,         SystemVariance );

      ADD_PARAM_ACCESS (bool,        m_useFixCycleTime_b,   UseFixCycleTime );
      ADD_PARAM_ACCESS( double,      m_cycleTime_d,         CycleTime );

   protected:

      void registerDrawingLists();
      void registerParameters();
      C3DVector getClosestSpeed(const SFeature       &f_feat ,
                                const CFeatureVector &f_featVector_v );
        
   private:
        
      /// Compute?
      bool                           m_compute_b;

      /// Input String
      std::string                    m_trackInpId_str;

      /// Input String
      std::string                    m_egoMInpId_str;

      /// Initialized?
      bool                           m_initialized_b;

      /// Common data structures for the kalman filter.
      CKF3DStereoPointCommon         m_kfCommon;

      /// Common data structures for the kalman filter.
      CKF3DStereoPointVector         m_kfVector;        

      /// Maximum no measurement count.
      int                            m_maxNoMeasCount_i;

      /// Measurement variance
      C3DVector                      m_measVariance;

      /// Initial velocity variances.
      C3DVector                      m_initialVelVariance;

      /// System Variance
      S2D<double>                    m_sysVariance;

      /// Use cycle Time or get it as input?
      bool                           m_useFixCycleTime_b;
        
      /// Assumed Cycle Time
      double                         m_cycleTime_d;

   };
}
#endif // __FEATUREKFOP_H
