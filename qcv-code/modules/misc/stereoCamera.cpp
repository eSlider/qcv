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

/*@@@**************************************************************************
* \file  stereoCamera
* \author Hernan Badino
* \date  Thu Jun 11 17:29:31 EDT 2009
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2010 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <stdio.h>

#include "doubleParam.h"
#include "stereoCamera.h"

using namespace QCV;


CStereoCamera::CStereoCamera ( ) 
        : CCamera ( ),
          m_baseline_d (         1. ),
          m_fuB_d (              0. )
{
}

CStereoCamera::CStereoCamera ( const CParamIOHandling & fr_paraReader )
        : CCamera ( ),
          m_baseline_d (         1. ),
          m_fuB_d (              0. )
{
    load ( fr_paraReader );
}

CStereoCamera::~CStereoCamera()
{

}


inline bool
CStereoCamera::load ( const CParamIOHandling & fr_paraReader )
{    
    std::string value_str;
    CDoubleParameter paraDouble;
    
    if ( fr_paraReader.get ( "Baseline", value_str ) && 
         paraDouble.setValueFromString ( value_str ))
        m_baseline_d = paraDouble.getValue();

    bool ret = CCamera::load( fr_paraReader );

    m_fuB_d = m_fu_d * m_baseline_d;

    return ret;
}

inline 
bool
CStereoCamera::setBaseline( double f_baseline_d )
{
    m_baseline_d = f_baseline_d;
    m_fuB_d = m_fu_d * m_baseline_d;
    return true;
}

inline double
CStereoCamera::getBaseline() const
{
    return m_baseline_d;
}

inline bool
CStereoCamera::world2Image ( double  f_x_d, 
                             double  f_y_d,
                             double  f_z_d,
                             double &fr_u_d,
                             double &fr_v_d ) const
{
    return CCamera::world2Image ( f_x_d, f_y_d, f_z_d, fr_u_d, fr_v_d );
}


inline bool
CStereoCamera::world2Image ( C3DVector  f_point,
                             C3DVector &fr_projection ) const
{
    return world2Image ( f_point, 
                         fr_projection.at(0),
                         fr_projection.at(1),
                         fr_projection.at(2) );
}


inline bool
CStereoCamera::world2Image ( C3DVector  f_point,
                             double    &fr_u_d,
                             double    &fr_v_d,
                             double    &fr_d_d ) const
{
    if (m_useTransform_b)
    {
        f_point = m_rotation.multiplyTransposed(f_point - m_translation);
    }
    
    return local2Image ( f_point,
                         fr_u_d, 
                         fr_v_d,
                         fr_d_d );
}

inline bool
CStereoCamera::world2Image ( double  f_x_d, 
                             double  f_y_d,
                             double  f_z_d,
                             double &fr_u_d,
                             double &fr_v_d,
                             double &fr_d_d ) const
{
    return world2Image ( C3DVector(f_x_d, f_y_d, f_z_d),
                         fr_u_d,
                         fr_v_d,
                         fr_d_d );

}

inline bool
CStereoCamera::local2Image ( C3DVector  f_point,
                             C3DVector &fr_projection ) const
{
    return local2Image ( f_point, 
                         fr_projection.at(0),
                         fr_projection.at(1),
                         fr_projection.at(2) );
}


inline bool
CStereoCamera::local2Image ( C3DVector  f_point,
                             double    &fr_u_d,
                             double    &fr_v_d,
                             double    &fr_d_d ) const
{
    bool res_b;    
    
    res_b = CCamera::local2Image ( f_point,
                                   fr_u_d, 
                                   fr_v_d );

    fr_d_d = m_fuB_d / f_point.z();

    return res_b && f_point.z() > 0;
}

inline bool
CStereoCamera::local2Image ( double  f_x_d, 
                             double  f_y_d,
                             double  f_z_d,
                             double &fr_u_d,
                             double &fr_v_d,
                             double &fr_d_d ) const
{
    return local2Image ( C3DVector(f_x_d, f_y_d, f_z_d),
                         fr_u_d,
                         fr_v_d,
                         fr_d_d );

}

inline bool
CStereoCamera::local2Image ( double  f_x_d, 
                             double  f_y_d,
                             double  f_z_d,
                             C3DVector &fr_projection ) const
{
    return local2Image ( C3DVector(f_x_d, f_y_d, f_z_d),
                         fr_projection.at(0),
                         fr_projection.at(1),
                         fr_projection.at(2) );
}

/// Image position to world position.
inline bool
CStereoCamera::image2World ( double  f_u_d, 
                             double  f_v_d,
                             double  f_d_d,
                             double &fr_x_d,
                             double &fr_y_d,
                             double &fr_z_d ) const
{
    C3DVector vec;
    
    bool success_b =  image2World ( f_u_d,
                         f_v_d,
                         f_d_d,
                         vec );
    fr_x_d = vec.x();
    fr_y_d = vec.y();
    fr_z_d = vec.z();

    return success_b;
}

/// Image position to world position.
inline bool
CStereoCamera::image2World ( double     f_u_d, 
                             double     f_v_d,
                             double     f_d_d,
                             C3DVector &f_point ) const
{
    bool success_b = image2Local ( f_u_d,
                                   f_v_d,
                                   f_d_d,
                                   f_point );
    
    if (m_useTransform_b)
    {
        f_point = m_rotation * f_point + m_translation;
    }

    return success_b;    
}

/// Image position to world position.
inline bool
CStereoCamera::image2World ( C3DVector &f_imgPoint,
                             C3DVector &f_wPoint ) const
{
    return image2World ( f_imgPoint.x(),
                         f_imgPoint.y(),
                         f_imgPoint.z(),
                         f_wPoint );
}

/// Image position to local position.
inline bool
CStereoCamera::image2Local ( double  f_u_d, 
                             double  f_v_d,
                             double  f_d_d,
                             double &fr_x_d,
                             double &fr_y_d,
                             double &fr_z_d ) const
{
    C3DVector vec;
    
    bool success_b =  image2Local ( f_u_d,
                                    f_v_d,
                                    f_d_d,
                                    vec );
    fr_x_d = vec.x();
    fr_y_d = vec.y();
    fr_z_d = vec.z();

    return success_b;
}

/// Image position to local position.
inline bool
CStereoCamera::image2Local ( double     f_u_d, 
                             double     f_v_d,
                             double     f_d_d,
                             C3DVector &f_point ) const
{
    if ( f_d_d <= 0 ) return false;
    
    f_point.at(2) = m_fuB_d / f_d_d;

    return CCamera::imageAndDistance2Local ( f_u_d,
                                             f_v_d,
                                             f_point.z(),
                                             f_point.at(0),
                                             f_point.at(1));
}

/// Image position to local position.
inline bool
CStereoCamera::image2Local ( C3DVector  f_imgPoint,
                             C3DVector &f_wPoint ) const
{
    return image2Local ( f_imgPoint.x(),
                         f_imgPoint.y(),
                         f_imgPoint.z(),
                         f_wPoint );
}

/// In local coordinate system
inline double
CStereoCamera::getDisparityFromDistance ( double f_dist_d ) const
{
    return m_fuB_d / f_dist_d;
}

/// In local coordinate system
inline double
CStereoCamera::getDistanceFromDisparity ( double f_disp_d ) const
{
    return m_fuB_d / f_disp_d;
}

/// Image position to world position.
void
CStereoCamera::scale ( double f_scale_d )
{
    CCamera::scale ( f_scale_d );
    m_fuB_d = m_fu_d * m_baseline_d;
}

/// Focal length U.
bool
CStereoCamera::setFocalLength ( double f_f_d )
{
    bool ret_b = CCamera::setFocalLength( f_f_d );

    m_fuB_d = m_fu_d * m_baseline_d;    

    return ret_b;    
}

/// Pixel width
bool
CStereoCamera::setSu ( double f_su_d )
{
    bool ret_b = CCamera::setSu( f_su_d );

    m_fuB_d = m_fu_d * m_baseline_d;

    return ret_b;    
}

/// Print camera parameters.
void
CStereoCamera::print ( ) const
{
    printf("Stereo camera parameters are:\n"
           "Focal length: %f\n"
           "Pixel width: %f\n"
           "Pixel height: %f\n"
           "Fu: %f\n"
           "Fv: %f\n"
           "U0: %f\n"
           "V0: %f\n"
           "Baseline: %f\n",
           m_focalLength_d,
           m_focalLength_d/m_fu_d,
           m_focalLength_d/m_fv_d,
           m_fu_d,
           m_fv_d,
           m_u0_d,
           m_v0_d,
           m_baseline_d );
}

/// Print camera parameters.
CStereoCamera & 
CStereoCamera::operator = ( const CCamera other )
{
    *((CCamera *) this) = other;
    return *this;
}

