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

#ifndef __CAMERA_H
#define __CAMERA_H

/**
 *******************************************************************************
 *
 * @file camera.h
 *
 * \class CCamera
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements a simple Pinhole stereo camera model.
 *
 * The class implements the required methods to transform 3D
 * measurements into image measurements. 
 *
*******************************************************************************
*****             (C) Hernan Badino 2010 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "paramIOHandling.h"

#include "3DRowVector.h"
#include "3DMatrix.h"

/* CONSTANTS */


namespace QCV
{
    
    class CCamera
    {
    /// Constructors/Destructor
    public:
        
        CCamera ( );
        CCamera ( const CParamIOHandling & fr_paraReader );
        virtual ~CCamera();
        
    /// Get/Set methods.
    public:
        virtual bool load ( const CParamIOHandling & fr_paraReader );
        virtual void print ( ) const;
        
    /// Get/Set methods.
    public:
        /// Focal length U.
        virtual double  getFu() const;

        /// Focal length V.
        virtual double  getFv() const;        

        /// Principal Point U
        virtual bool    setU0( double f_u0_d );
        virtual double  getU0() const;

        /// Principal Point V
        virtual bool    setV0( double f_v0_d );
        virtual double  getV0() const;

        /// Get pixel width.
        virtual bool    setSu( double f_su_d );
        virtual double  getSu() const;

        /// Get pixel height.
        virtual bool    setSv( double f_sv_d );
        virtual double  getSv() const;

        /// Get pixel height.
        virtual bool    setFocalLength ( double f_f_d );
        virtual double  getFocalLength() const;

        /// Principal Point
        virtual void   setPrincipalPoint( double f_u0_d, 
                                          double f_v0_d );
        /// Set rotation.
        virtual bool      setRotation ( C3DMatrix f_matrix );
        virtual C3DMatrix getRotation (  ) const;

        /// Set translation.
        virtual bool      setTranslation ( C3DVector f_matrix );
        virtual C3DVector getTranslation (  ) const;
        
        
    /// Image/World transformations.
    public:
        /// World 3D point to image point.
        virtual bool   world2Image ( double  f_x_d, 
                                     double  f_y_d,
                                     double  f_z_d,
                                     double &fr_u_d,
                                     double &fr_v_d ) const;

        /// World 3D point to image point.
        virtual bool   world2Image ( C3DVector  f_point,
                                     double    &fr_u_d,
                                     double    &fr_v_d ) const;

        /// Local 3D point to image point.
        virtual bool   local2Image ( double  f_x_d, 
                                     double  f_y_d,
                                     double  f_z_d,
                                     double &fr_u_d,
                                     double &fr_v_d ) const;

        /// Local 3D point to image point.
        virtual bool   local2Image ( C3DVector  f_point,
                                     double    &fr_u_d,
                                     double    &fr_v_d ) const;

        /// Image position to local position.
        virtual bool   imageAndDistance2Local ( double  f_u_d, 
                                                double  f_v_d,
                                                double  f_z_d,
                                                double &fr_x_d,
                                                double &fr_y_d ) const;

    /// Operations on the camera
    public:       
        virtual void   scale ( double f_scale_d );
        

    /// Operators;
    public:
        bool operator == ( const CCamera & f_other ) const;
        bool operator != ( const CCamera & f_other ) const;

   /// Protected members:
    protected:

        /// Focal length.
        double             m_focalLength_d;

        /// Focal length in U.
        double             m_fu_d;

        /// Focal length in V
        double             m_fv_d;

        /// Focal length in U.
        double             m_su_d;

        /// Focal length in V
        double             m_sv_d;

        /// Principal point U component.
        double             m_u0_d;

        /// Principal point V component.
        double             m_v0_d;

        /// Use camera <-> world trasnformation?
        bool               m_useTransform_b;
        
        /// Rotation w.r.t. to the world
        C3DMatrix          m_rotation;

        /// Translation w.r.t. to the world
        C3DVector          m_translation;
        
    };
}
    
#endif // __CAMERA_H
