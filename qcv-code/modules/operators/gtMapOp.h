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

#ifndef __GTMAPOP_H
#define __GTMAPOP_H

/*@@@**************************************************************************
 ** \file  gtMapOp.h
 * \date   April 3, 2012
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****            (C) Hernan Badino 2010 - All Rights Reserved             *****
******************************************************************************/

/* INCLUDES */
#include <math.h>

#include "operator.h"
#include "3DPointVector.h"
#include "rigidMotion.h"
#include "pose2ScreenMapper.h"

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
    /* PROTOTYPES */
    //class  CRigidMotionWidget;

   
    class CGTMapOp: public COperator
    {
        /// Constructor, Desctructors
    public:    
        
        /// Constructors.
      CGTMapOp ( COperator * const f_parent_p = NULL,
		 const std::string f_name_str = "Ground Truth Pose Visualizer");
        
        /// Virtual destructor.
        virtual ~CGTMapOp ();

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

    /// Dialogs.
    public:

    /// Parameters.
    public:
        ADD_PARAM_ACCESS_NOTIFIER (std::string,  m_filePath_str,             FilePath, loadFromFile );
        ADD_PARAM_ACCESS (bool,                  m_absolutePose_b,           AbsolutePose );
        ADD_PARAM_ACCESS (float,                 m_maxDistance_f,            MaxDistance );
        ADD_PARAM_ACCESS (int,                   m_cencSections_i,           ColorEncodingSections );
        ADD_PARAM_ACCESS (std::string,           m_egoMInpId_str,            EgoMInputIdStr );
        ADD_PARAM_ACCESS (unsigned int,          m_maxPoses_i,               MaxPoses );

    /// Protected Help Methods.
    protected:
        
        bool loadFromFile (  );

    /// Private Data Types
    private:
       
       typedef std::vector<float> Data;
       
        /// GT Poses
        std::vector<Data>         m_poses_v;

        /// Visual odometry poses
        std::vector<Data>         m_voPoses_v;

        /// Output String
        std::string               m_filePath_str;

        /// Motion string id
        std::string               m_egoMInpId_str;
        /// Absolute or relative pose?
        bool                      m_absolutePose_b;

        /// Max distance for display
        float                     m_maxDistance_f;

        // Number of sections for color encoding
        int                       m_cencSections_i;

        /// Pose2Screen mapper
        SPose2ScreenMapper         m_mapper;

        /// Rotation
        C3DMatrix                  m_rotation;

        /// Translation
        C3DVector                  m_translation;

        /// Output
        SRigidMotion               m_totalMotion;

        /// Vector of X translation
        std::vector<float>         m_x_v; 

        /// Vector of X translation
        std::vector<float>         m_y_v;

        /// Vector of X translation
        std::vector<float>         m_z_v;

        /// Vector of rotation axis X
        std::vector<float>         m_rx_v; 

        /// Vector of rotation axis Y
        std::vector<float>         m_ry_v;

        /// Vector of rotation axis Z
        std::vector<float>         m_rz_v;

        /// Vector of Pitch
        std::vector<float>         m_pitch_v;

        /// Vector of Yaw
        std::vector<float>         m_yaw_v;

        /// Vector of Roll
        std::vector<float>         m_roll_v;
      
        /// Counter
        size_t                     m_idx_i;
      
        /// Max poses
        unsigned int               m_maxPoses_i;
   };
}


#endif // __GTMAPOP_H
