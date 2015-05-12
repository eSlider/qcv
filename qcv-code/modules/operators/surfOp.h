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

#ifndef __SURFOP_H
#define __SURFOP_H

/**
*******************************************************************************
*
* @file surfOp.h
*
* \class CSurfOp
* \author Hernan Badino (hernan.badino@gmail.com)
* \brief Class for resizing images.
*
*******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <limits>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#if CV_MAJOR_VERSION > 2 || ( CV_MAJOR_VERSION == 2 && CV_MINOR_VERSION >= 4 )
#include <opencv2/nonfree/features2d.hpp>
#endif

#include "operator.h"
#include "colorEncoding.h"

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
    class CSurfOp: public COperator
    {
    public:
            
        class CCvSurf: public cv::SURF
        {
        public:
            ADD_PARAM_ACCESS (bool,        extended,            Extended );
#if CV_MAJOR_VERSION > 2 || ( CV_MAJOR_VERSION == 2 && CV_MINOR_VERSION >= 2 )
            ADD_PARAM_ACCESS (bool,        upright,             Upright );
#endif
            ADD_PARAM_ACCESS (double,      hessianThreshold,    HessianThreshold );
            ADD_PARAM_ACCESS (int,         nOctaves,            Octaves );
            ADD_PARAM_ACCESS (int,         nOctaveLayers,       OctaveLayers );
        };
        
        
        /// Constructor, Desctructors
    public:    
        
        /// Constructors.
        CSurfOp ( COperator * const f_parent_p = NULL,
                  const std::string f_name_str = "Surf Operator" );
        
        /// Virtual destructor.
        virtual ~CSurfOp ();

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

        /// Mouse moved.
        virtual void mouseMoved (     CMouseEvent * f_event_p );

    /// Parameters
    public:

        ADD_PARAM_ACCESS (bool, m_compute_b,               Compute );
        ADD_PARAM_ACCESS (bool, m_useProvidedKeypoints_b,  UseProvidedKeypoints );
        ADD_PARAM_ACCESS (bool, m_computeDescriptors_b,    ComputeDescriptors );

    /// Help methods
    protected:

        void registerDrawingLists(  );

        void registerParameters(  );

    private:
        
        /// Input image Id.
        std::string                m_inpImageId_str;
        
        /// Compute?
        bool                        m_compute_b;
        
        /// Use provided keypoints?
        bool                        m_useProvidedKeypoints_b;

        /// Surf operator
        CCvSurf                     m_surfOp;

        /// Input image
        cv::Mat                     m_img;

        /// Auxiliar image
        cv::Mat                     m_imgAux;

        /// Detected/Input Keypoints 
        std::vector<cv::KeyPoint>   m_keyPoints;

        /// Detected/Input Keypoints 
        std::vector<float>          m_descriptors;

        /// Response color encoding
        CColorEncoding              m_respCE;

        /// Copute descriptors?
        bool                        m_computeDescriptors_b;

    };
}
#endif // __SURFOP_H
 
