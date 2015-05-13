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
* @file voOp.cpp
*
* \class CStereoSFMOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "stereoSFMOp.h"
#include "paramMacros.h"
#include "drawingList.h"
#include "stereoTrackerOp.h"
#include "stereoEgoMotionOp.h"
#include "gtMapOp.h"
#include "featKFOp.h"

using namespace QCV;

/// Constructors.
CStereoSFMOp::CStereoSFMOp ( COperator * const f_parent_p,
			 const std::string f_name_str )
  : COperator (             f_parent_p, f_name_str ),
    m_compute_b (                             true )
{
   addChild ( new CStereoTrackerOp   ( this, "Stereo Tracker" ) );
   addChild ( new CStereoEgoMotionOp ( this ) );
   addChild ( new CGTMapOp           ( this ) );

   addChild ( new CFeatureKFOp       ( this ) );
 

   registerDrawingLists(  );
   registerParameters (  );
}

void
CStereoSFMOp::registerDrawingLists(  )
{
}

void
CStereoSFMOp::registerParameters(  )
{

   BEGIN_PARAMETER_GROUP("Computation", false, SRgb(220,0,0));
   ADD_BOOL_PARAMETER ( "Compute",
                        "Compute VO?.",
                        m_compute_b,
                        this,
                        Compute,
                        CStereoSFMOp );

   END_PARAMETER_GROUP;
}

/// Virtual destructor.
CStereoSFMOp::~CStereoSFMOp ()
{
}

/// Cycle event.
bool
CStereoSFMOp::cycle()
{
   bool success_b = true;
   
   if (m_compute_b)
   {
      registerOutput<SRigidMotion> ( "Predicted Motion", &m_lastMotion );
      
      success_b = COperator::cycle();
      
      SRigidMotion *  motion_p = getInput<SRigidMotion> ( "Integrated Ego-Motion Estimation" );
      
      if ( motion_p )
         m_lastMotion = *motion_p;
      else
      {
         m_lastMotion.clear();
      }
   }
   
   return success_b;
}
    
/// Show event.
bool CStereoSFMOp::show()
{
   cv::Mat img =  getInput<cv::Mat>("Image 0", cv::Mat() );
   
   /// Set the screen size if this is the parent operator.
   if ( img.size().width > 0 &&
        !getParentOp() )
   {
      setScreenSize ( img.size() );
   }

   return COperator::show();
}

/// Init event.
bool CStereoSFMOp::initialize()
{
   m_lastMotion.clear();
   return COperator::initialize();
}

/// Reset event.
bool CStereoSFMOp::reset()
{
   return COperator::reset();
}

bool CStereoSFMOp::exit()
{
   return COperator::exit();
}


