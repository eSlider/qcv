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

#ifndef __STEREOVOOP_H
#define __STEREOVOOP_H

/**
*******************************************************************************
*
* @file stereoSFMOp.h
*
* \class  CStereoVisOdoOp
* \author Hernan Badino (hernan.badino@gmail.com)
* \brief  Class for computing stereo visual odometry
*
*******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <limits>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "imageFromFile.h"
#include "operator.h"
#include "colorEncoding.h"
#include "matVector.h"
#include "stereoCamera.h"
#include "rigidMotion.h"

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
   class CStereoSFMOp: public COperator
   {
   /// Constructor, Desctructors
   public:    

   /// Parameter access
   public:
      ADD_PARAM_ACCESS         (bool,        m_compute_b,          Compute );

   /// Constructor, Desctructors
   public:    
        
      /// Constructors.
      CStereoSFMOp ( COperator * const f_parent_p = NULL,
                        const std::string f_name_str = "Visual SFM Op" );
      
      /// Virtual destructor.
      virtual ~CStereoSFMOp ();

      /// Cycle event.
      virtual bool cycle( );
    
      /// Show event.
      virtual bool show();
    
      /// Init event.
      virtual bool initialize();
    
      /// Reset event.
      virtual bool reset();
    
      /// Exit event.
      virtual bool exit();

   protected:

      void registerDrawingLists( );

      void registerParameters( );

   private:

      /// Compute?
      bool                        m_compute_b;

      /// Last motion
      SRigidMotion                m_lastMotion;
   };
}
#endif // __STEREOVOOP_H
 
