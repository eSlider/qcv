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

#ifndef __ROADPLANEDETECTIONOP_H
#define __ROADPLANEDETECTIONOP_H

/**
*******************************************************************************
*
* @file   CRoadPlaneDetectionOp.h
*
* \class  CRoadPlaneDetectionOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief  QCV Operator class for computing the road orientation and
* position with respect to the camera.
*
*******************************************************************************
*****             (C) Hernan Badino 2011 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <limits>

#include "operator.h"
#include "stereoCamera.h"
#include "3DPointVector.h"

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
    struct SRoadPlaneOutput
    {
        SRoadPlaneOutput()
        {
            gpRotation.loadIdentity();
            gpTranslation.clear();
            result.clear();
            imgResult.clear();
        }
        
        /// Rotation of the ground plane.
        C3DMatrix            gpRotation;

        /// Translation of the ground plane.
        C3DVector            gpTranslation;

        /// Optimal result found
        C3DVector            result;

        /// Optimal image result found
        C3DVector            imgResult;
    };

    class CRoadPlaneDetectionOp: public QCV::COperator
    {
        /// Constructor, Desctructors
    public:    
        
        /// Constructors.
        CRoadPlaneDetectionOp ( COperator* const f_parent_p = NULL );
        
        /// Virtual destructor.
        virtual ~CRoadPlaneDetectionOp ();

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

    /// I/O registration.
    public:
        // /// Set the input of this operator
        // bool setInput  ( const cv::Mat & f_dispImg );

        // /// Gets the output of this operator
        // bool getOutput ( SRoadPlaneOutput & f_planeParams ) const;

    /// I/O registration.
    public:
        /// Set the input of this operator
        bool setStereoCamera  ( const CStereoCamera & f_camera );

        /// Set the input of this operator
        CStereoCamera getStereoCamera  ( ) const 
        {
            return m_camera;
        }
        

    /// Gets and sets.
    public:

        ADD_PARAM_ACCESS(std::string, m_dispImgInputId_str, DispImgInputId );
        ADD_PARAM_ACCESS(std::string, m_camInputId_str,     CameraInputId );

        ADD_PARAM_ACCESS(bool,        m_compute_b,          Compute );

        ADD_PARAM_ACCESS(float,       m_iniResiduum_f,      InitialResiduum );
        ADD_PARAM_ACCESS(float,       m_maxResiduum_f,      MaxResiduum );
        ADD_PARAM_ACCESS(float,       m_acceptedResiduum_f, AcceptedResiduum );
        ADD_PARAM_ACCESS(float,       m_iniH_f,             InitialHeight );
        ADD_PARAM_ACCESS(float,       m_iniTilt_f,          InitialTilt );
        ADD_PARAM_ACCESS(float,       m_iniRoll_f,          InitialRoll );
        ADD_PARAM_ACCESS(float,       m_maxDistance_f,      MaxDistance );
        ADD_PARAM_ACCESS(int,         m_maxIters_i,         MaxIterations );
        ADD_PARAM_ACCESS(int,         m_rowStart_i,         RowStart );

        ADD_PARAM_ACCESS(bool,        m_showOutliers_b,     ShowOutliers );
        ADD_PARAM_ACCESS(bool,        m_showInliers_b,      ShowInliers );

        ADD_PARAM_ACCESS(float,       m_gridZBack_f,        GridZBack );
        ADD_PARAM_ACCESS(int,         m_gridZ_i,            NumberGridZ );
        ADD_PARAM_ACCESS(float,       m_gridZStep_f,        StepGridZ );

        ADD_PARAM_ACCESS(int,         m_gridX_i,            NumberGridX ); 
        ADD_PARAM_ACCESS(float,       m_gridXStep_f,        StepGridX );

        ADD_PARAM_ACCESS(bool,        m_apply2Camera_b,     Apply2Camera );

        ADD_PARAM_ACCESS(C3DVector,   m_origin,             Origin );
        ADD_PARAM_ACCESS(float,       m_yaw_f,              Yaw );


        ADD_PARAM_ACCESS(float,       m_residuumTol_f,      ResiduumTolerance );
        ADD_PARAM_ACCESS(float,       m_sigmaTol_f,         SigmaTolerance );
        
        ADD_PARAM_ACCESS(bool,        m_computeResidualImage_b,  ComputeResidualImage );

        ADD_PARAM_ACCESS(bool,        m_iniPoseFromCamera_b,     InitialPoseFromCamera );
       /// Protected methods.
    protected:

        void registerDrawingLists();
        void registerParameters();

        void setInitialPose();

        bool calculateRoadPlane( const cv::Mat       & f_dispImg,
                                 const CStereoCamera & f_camera,
                                 float               & fr_residuum_f );

        bool calculateMatrices( const cv::Mat       & f_dispImg,
                                const C3DVector     & f_prediction,
                                const float           f_residuum_f,
                                C3DMatrix           & fr_AtA,
                                C3DVector           & fr_Atb);
        /// Private members
    private:
        
        /// Input id
        std::string          m_dispImgInputId_str;
        
        /// Input id
        std::string          m_camInputId_str;
        
        /// Compute?
        bool                 m_compute_b;        

        /// Disp img input id.
        SRoadPlaneOutput     m_planeParams;

        /// Input disparity image
        cv::Mat              m_dispImg;
        
        /// Input disparity image
        CStereoCamera        m_camera;
        
        /// Apply detection to camera?
        bool                 m_apply2Camera_b;

        /// Optimal result found
        C3DVector            m_result;

        /// Optimal image result found
        C3DVector            m_imgResult;

        /// Optimal result found
        C3DVector            m_iniResult;

        /// Optimal image result found
        C3DVector            m_iniImgResult;

        /// Vector for grid display.
        C3DPointDataVector   m_displayGrid;
        
        /// Vector for grid display.
        float                m_residuum_f;

        /// Min disparity to consider.
        float                m_minDisp_f;

        /// Initial residuum of distance from prediction
        float                m_iniResiduum_f;

        /// Maximum residuum to detect if estimation was successful.
        float                m_maxResiduum_f;

        /// Accepted residuum to break iterations.
        float                m_acceptedResiduum_f;

        /// Initial height.
        float                m_iniH_f;

        /// Initial tilt.
        float                m_iniTilt_f;

        /// Initial roll.
        float                m_iniRoll_f;

        /// Max distance.
        float                m_maxDistance_f;

        /// Max iterations
        int                  m_maxIters_i;

        /// Start in given row
        int                  m_rowStart_i;

        /// Show outliers
        bool                 m_showInliers_b;

        /// Show outliers
        bool                 m_showOutliers_b;

        /// Show grid backward
        float                m_gridZBack_f;
        
        /// Show number of depth grids.
        int                  m_gridZ_i;
        
        /// Step of each grid
        float                m_gridZStep_f;
        
        /// Show grid horizontal
        int                  m_gridX_i;

        /// Step of each grid horizontal.
        float                m_gridXStep_f;
        
        /// Rotation of the ground plane.
        C3DMatrix            m_gpRotation;

        /// Translation of the ground plane.
        C3DVector            m_gpTranslation;

        /// Translation origin from the computed plane.
        C3DVector            m_origin;

        /// Yaw
        float                m_yaw_f;

        /// Residuum tolerance
        float                m_residuumTol_f;
        
        /// Residuum tolerance
        float                m_sigmaTol_f;

        /// Compute residual image
        bool                 m_computeResidualImage_b;

        /// Residual image
        cv::Mat              m_residualImg;

        /// Initial pose from camera?
        bool                 m_iniPoseFromCamera_b;
        
    };
}
#endif // __ROADPLANEDETECTIONOP_H
