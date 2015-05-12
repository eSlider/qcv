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

#ifndef __STEREOCAMERA_H
#define __STEREOCAMERA_H

/**
 *******************************************************************************
 *
 * @file stereoCamera.h
 *
 * \class CStereoCamera
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements a stereo camera model.
 *
 * The class implements the required methods to transform stereo measurements
 * (i.e. 2D image position plus disparity) into 3D Euclidean points. 
 * This class is derived from CCamera incorporating the baseline component.
 *
*******************************************************************************
*****             (C) Hernan Badino 2010 - All Rights Reserved            *****
******************************************************************************/


/* INCLUDES */
#include "camera.h"

/* CONSTANTS */


namespace QCV
{
    
    class CStereoCamera: public CCamera
    {
    /// Constructors/Destructor
    public:
        
        CStereoCamera ( );
        CStereoCamera ( const CParamIOHandling & fr_paraReader );
        virtual ~CStereoCamera();
        
    /// Load from param file.
    public:
        virtual bool load ( const CParamIOHandling & fr_paraReader );
        virtual void print ( ) const;
        
    /// Get/Set methods.
    public:
        /// Focal length U.
        virtual bool    setBaseline( double f_baseline_d );
        virtual double  getBaseline() const;

   /// Get/Set methods specialization from parent.
    public:
        void scale          ( double f_scale_d );
        bool setFocalLength ( double f_f_d );
        bool setSu          ( double f_su_d );

        /// Some useful functions.
    public:
        /// World 3D point to image point.
        virtual double  getDisparityFromDistance ( double f_dist_d ) const;
        virtual double  getDistanceFromDisparity ( double f_disp_d ) const;

        
        /// World 3D point to image point.
        virtual bool   world2Image ( double  f_x_d, 
                                     double  f_y_d,
                                     double  f_z_d,
                                     double &fr_u_d,
                                     double &fr_v_d ) const;
        

        /// World 3D point to image point.
        virtual bool   world2Image ( C3DVector  f_point,
                                     double    &fr_u_d,
                                     double    &fr_v_d,
                                     double    &fr_d_d ) const;

        /// World 3D point to image projection point.
        virtual bool   world2Image ( C3DVector  f_point,
                                     C3DVector &fr_projection ) const;

        /// World 3D point to image point.
        virtual bool   world2Image ( double  f_x_d, 
                                     double  f_y_d,
                                     double  f_z_d,
                                     double &fr_u_d,
                                     double &fr_v_d,
                                     double &fr_d_d ) const;

        /// Local 3D point to image point.
        virtual bool   local2Image ( C3DVector  f_point,
                                     double    &fr_u_d,
                                     double    &fr_v_d,
                                     double    &fr_d_d ) const;

        /// Local 3D point to image projection point.
        virtual bool   local2Image ( C3DVector  f_point,
                                     C3DVector &fr_projection ) const;

        /// Local 3D point to image point.
        virtual bool   local2Image ( double  f_x_d, 
                                     double  f_y_d,
                                     double  f_z_d,
                                     double &fr_u_d,
                                     double &fr_v_d,
                                     double &fr_d_d ) const;

        /// Local 3D point to image point.
        virtual bool   local2Image ( double  f_x_d, 
                                     double  f_y_d,
                                     double  f_z_d,
                                     C3DVector &fr_projection ) const;

        /// Image position to world position.
        virtual bool   image2World ( double  f_u_d, 
                                     double  f_v_d,
                                     double  f_d_d,
                                     double &fr_x_d,
                                     double &fr_y_d,
                                     double &fr_z_d ) const;

        /// Image position to world position.
        virtual bool   image2World ( double      f_u_d, 
                                     double      f_v_d,
                                     double      f_d_d,
                                     C3DVector  &f_point ) const;

        /// Image position to world position.
        virtual bool   image2World ( C3DVector  &f_imgPoint,
                                     C3DVector  &f_point ) const;

        /// Image position to local position.
        virtual bool   image2Local ( double  f_u_d, 
                                     double  f_v_d,
                                     double  f_d_d,
                                     double &fr_x_d,
                                     double &fr_y_d,
                                     double &fr_z_d ) const;

        /// Image position to local position.
        virtual bool   image2Local ( double      f_u_d, 
                                     double      f_v_d,
                                     double      f_d_d,
                                     C3DVector  &f_point ) const;

        /// Image position to local position.
        virtual bool   image2Local ( C3DVector   f_imgPoint,
                                     C3DVector  &f_point ) const;

    /// Copy operator.
    public:       
        CStereoCamera & operator = ( const CCamera other );
        
    /// Operations on the camera
    public:       

    /// Protected members:
    protected:

        /// Focal length.
        double            m_baseline_d;

        /// Focal length.
        double            m_fuB_d;
    };
}
    
#endif // __STEREOCAMERA_H

/* ////////////  Version History ///////////////
 *  $Log: stereoCamera.h,v $
 *  Revision 1.3  2009/11/18 15:51:01  badino
 *  badino: documentation added. Some other global changes.
 *
 *  Revision 1.2  2009/10/13 19:52:26  badino
 *  badino: added documentation.
 *
 *//////////////////////////////////////////////
