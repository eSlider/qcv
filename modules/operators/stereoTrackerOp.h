/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 2.1. See the GNU LGPL version 2.1 for details.
 * QCV is distributed "AS IS" without ANY WARRANTY, without even the
 * implied warranty of merchantability or fitness for a particular
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

#ifndef __STEREOTRACKER_H
#define __STEREOTRACKER_H

/**
*******************************************************************************
*
* @file stereotracker.h
*
* \class CStereoTracker
* \author Hernan Badino (hernan.badino@gmail.com)
* \brief Class for resizing images.
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

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
    class CGfttFreakOp;
   
    class CStereoTrackerOp: public COperator
    {
    /// Constructor, Desctructors
    public:    

    /// Parameter access
    public:
        ADD_PARAM_ACCESS         (bool,        m_compute_b,          Compute );
        ADD_PARAM_ACCESS         (S2D<float>,  m_centralPointOffset, CentralPointOffset );
    /// Constructor, Desctructors
    public:    
        
        /// Constructors.
        CStereoTrackerOp ( COperator * const f_parent_p = NULL,
                   const std::string f_name_str = "Stereo Tracker" );
        
        /// Virtual destructor.
        virtual ~CStereoTrackerOp ();

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

        /// Good feature to track operator
        CGfttFreakOp *              m_gftt_p;
       
        /// Compute?
        bool                        m_compute_b;

        /// Stereo Camera
        CStereoCamera               m_origCamera;

        /// Stereo Camera
        CStereoCamera               m_camera;

        /// Central point offset
        S2D<float>                  m_centralPointOffset;
       
    };
}
#endif // __STEREOTRACKER_H
 
