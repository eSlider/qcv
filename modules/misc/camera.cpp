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
 * \file  camera
 * \author Hernan Badino
 * \date  Wed Jun 10 18:56:01 EDT 2009
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2010 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include <stdio.h>
#include "doubleParam.h"
#include "camera.h"

using namespace QCV;

CCamera::CCamera ( ) 
        : m_focalLength_d (         1 ),
          m_fu_d (                  1 ),
          m_fv_d (                  1 ),
          m_su_d (                  1 ),
          m_sv_d (                  1 ),
          m_u0_d (                  0 ),
          m_v0_d (                  0 ),
          m_useTransform_b (    false )
{
    m_rotation.loadIdentity();    
    m_translation.clear();
}

CCamera::CCamera ( const CParamIOHandling & fr_paraReader )
        : m_focalLength_d (         1 ),
          m_fu_d (                  1 ),
          m_fv_d (                  1 ),
          m_su_d (                  1 ),
          m_sv_d (                  1 ),
          m_u0_d (                  0 ),
          m_v0_d (                  0 ),
          m_useTransform_b (    false )
{
    m_rotation.loadIdentity();
    m_translation.clear();
    load ( fr_paraReader );
}

CCamera::~CCamera()
{

}
        
bool
CCamera::load ( const CParamIOHandling & fr_paraReader )
{
    std::string value_str;
    CDoubleParameter paraDouble;

    if ( fr_paraReader.get ( "Focal Length", value_str ) && 
         paraDouble.setValueFromString ( value_str ))
        m_focalLength_d = paraDouble.getValue();
    
    if ( fr_paraReader.get ( "Pixel Width", value_str ) && 
         paraDouble.setValueFromString ( value_str ))
        m_su_d = paraDouble.getValue();

    if ( fr_paraReader.get ( "Pixel Height", value_str ) && 
         paraDouble.setValueFromString ( value_str ))
        m_sv_d = paraDouble.getValue();
    
    if ( fr_paraReader.get ( "Center X", value_str ) && 
         paraDouble.setValueFromString ( value_str ))
        m_u0_d = paraDouble.getValue();

    if ( fr_paraReader.get ( "Center Y", value_str ) && 
         paraDouble.setValueFromString ( value_str ))
        m_v0_d = paraDouble.getValue();

    m_fu_d = m_focalLength_d/m_su_d;
    m_fv_d = m_focalLength_d/m_sv_d;
    
    //float scale_f;
    if ( fr_paraReader.get ( "Scale", value_str ) && 
         paraDouble.setValueFromString ( value_str ))
    {
        scale(paraDouble.getValue());
    }

    double rot_d;
    m_rotation.loadIdentity();

    if ( fr_paraReader.get ( "Rotation X", value_str ) && 
         paraDouble.setValueFromString ( value_str ) )
    {
        m_useTransform_b = true;
        rot_d =  paraDouble.getValue();
        m_rotation.rotateX(rot_d);
    }
    
    if ( fr_paraReader.get ( "Rotation Y", value_str ) && 
         paraDouble.setValueFromString ( value_str ) )
    {
        m_useTransform_b = true;
        rot_d =  paraDouble.getValue();
        m_rotation.rotateY(rot_d);
    }

    if ( fr_paraReader.get ( "Rotation Z", value_str ) && 
         paraDouble.setValueFromString ( value_str ) )
    {
        m_useTransform_b = true;
        rot_d =  paraDouble.getValue();
        m_rotation.rotateZ(rot_d);
    }

    if ( fr_paraReader.get ( "Translation X", value_str ) && 
         paraDouble.setValueFromString ( value_str ) )
    {
        m_useTransform_b = true;
        m_translation.at(0) = paraDouble.getValue();
    }

    if ( fr_paraReader.get ( "Translation Y", value_str ) && 
         paraDouble.setValueFromString ( value_str ) )
    {
        m_useTransform_b = true;
        m_translation.at(1) = paraDouble.getValue();
    }

    if ( fr_paraReader.get ( "Translation Z", value_str ) && 
         paraDouble.setValueFromString ( value_str ) )
    {
        m_useTransform_b = true;
        m_translation.at(2) = paraDouble.getValue();
    }

    return true;
}
/*
/// Focal length U.
bool
CCamera::setFu( double f_fu_d )
{
    m_fu_d = f_fu_d;
    return true; 
}
*/
double
CCamera::getFu() const
{
    return m_fu_d;
}

/*
/// Focal length V.
bool
CCamera::setFv(double f_fv_d)
{
    m_fv_d = f_fv_d;
    return true;
}
*/

double
CCamera::getFv() const
{
    return m_fv_d;
}

/// Principal Point U
bool
CCamera::setU0( double f_u0_d )
{
    m_u0_d = f_u0_d;
    return true;
}

double
CCamera::getU0() const
{
    return m_u0_d;
}

/// Principal Point V
bool
CCamera::setV0( double f_v0_d )
{
    m_v0_d = f_v0_d;
    return true;
}

double
CCamera::getV0() const
{
    return m_v0_d;
}

/// Pixel width
bool
CCamera::setSu( double f_su_d )
{
    m_su_d = f_su_d;
    m_fu_d = m_focalLength_d/m_su_d;
    return true;
}

double
CCamera::getSu() const
{
    return m_su_d;
}

/// Pixel height
bool
CCamera::setSv( double f_sv_d )
{
    m_sv_d = f_sv_d;
    m_fv_d = m_focalLength_d/f_sv_d;
    return true;
}

double
CCamera::getSv() const
{
    return m_sv_d;
}

/// Get focal length
bool
CCamera::setFocalLength ( double f_f_d )
{
    m_focalLength_d = f_f_d;

    m_fv_d = m_focalLength_d/m_su_d;
    m_fu_d = m_focalLength_d/m_sv_d;
    
    return true;
}

double
CCamera::getFocalLength() const
{
    return m_focalLength_d;
}

/// Principal Point
void
CCamera::setPrincipalPoint( double f_u0_d, 
                            double f_v0_d )
{
    m_u0_d = f_u0_d;
    m_v0_d = f_v0_d;
}
        
/// World 3D point to image point.
bool
CCamera::world2Image ( C3DVector  f_point,
                       double    &fr_u_d,
                       double    &fr_v_d ) const
{
    if (m_useTransform_b )
    {
        f_point = m_rotation.multiplyTransposed(f_point - m_translation);
    }

    return local2Image ( f_point, fr_u_d, fr_v_d );
}

/// World 3D point to image point.
bool
CCamera::world2Image ( double  f_x_d, 
                       double  f_y_d,
                       double  f_z_d,
                       double &fr_u_d,
                       double &fr_v_d ) const
{
    return world2Image ( C3DVector (f_x_d, f_y_d, f_z_d),
                         fr_u_d,
                         fr_v_d );
}

/// Local 3D point to image point.
bool
CCamera::local2Image ( C3DVector  f_point,
                       double    &fr_u_d,
                       double    &fr_v_d ) const
{
    fr_u_d = f_point.x() / f_point.z() * m_fu_d + m_u0_d;
    fr_v_d = m_v0_d - f_point.y() / f_point.z() * m_fv_d;

    return true;
}


/// Local 3D point to image point.
bool
CCamera::local2Image ( double  f_x_d, 
                       double  f_y_d,
                       double  f_z_d,
                       double &fr_u_d,
                       double &fr_v_d ) const
{
    return local2Image ( C3DVector (f_x_d, f_y_d, f_z_d),
                         fr_u_d,
                         fr_v_d );
}

/// Image position to world position.
bool
CCamera::imageAndDistance2Local( double  f_u_d, 
                                 double  f_v_d,
                                 double  f_z_d,
                                 double &fr_x_d,
                                 double &fr_y_d ) const
{
    fr_x_d = (f_u_d - m_u0_d) / m_fu_d * f_z_d;
    fr_y_d = (m_v0_d - f_v_d) / m_fv_d * f_z_d;

    return true;
}

/// Image position to world position.
void
CCamera::scale ( double f_scale_d )
{
    //m_focalLength_d *= f_scale_d;
    setFocalLength(m_focalLength_d * f_scale_d );
    //m_fu_d          *= f_scale_d;
    //m_fv_d          *= f_scale_d;
    m_u0_d          *= f_scale_d;
    m_v0_d          *= f_scale_d;
}

/// Print camera parameters.
void
CCamera::print ( ) const
{
    printf("Monocular camera parameters are:\n"
           "Focal length: %f\n"
           "Pixel width: %f\n"
           "Pixel height: %f\n"
           "Fu: %f\n"
           "Fv: %f\n"
           "U0: %f\n"
           "V0: %f\n",
           m_focalLength_d,
           m_focalLength_d/m_fu_d,           m_focalLength_d/m_fv_d,
           m_fu_d,
           m_fv_d,
           m_u0_d,
           m_v0_d );

    printf("Rotation:"); m_rotation.print();
    printf("Translation:"); m_translation.print();
    
}

/// Set rotation.
bool
CCamera::setRotation ( C3DMatrix f_matrix )
{
    m_useTransform_b = true;
    m_rotation = f_matrix;
    return true;
}

C3DMatrix
CCamera::getRotation (  ) const
{
    return m_rotation;
}


/// Set translation.
bool
CCamera::setTranslation ( C3DVector f_vector )
{
    m_useTransform_b = true;
    m_translation = f_vector;
    return true;
}

C3DVector
CCamera::getTranslation (  ) const
{
    return  m_translation;
}

bool
CCamera::operator == ( const CCamera & f_other ) const
{
    double e = 1.e-9;
    
    return ( fabs(f_other.m_focalLength_d - m_focalLength_d) < e && 
             fabs(f_other.m_fu_d          - m_fu_d) < e && 
             fabs(f_other.m_fv_d          - m_fv_d) < e && 
             fabs(f_other.m_su_d          - m_su_d) < e && 
             fabs(f_other.m_sv_d          - m_sv_d) < e && 
             fabs(f_other.m_u0_d          - m_u0_d) < e && 
             fabs(f_other.m_v0_d          - m_v0_d) < e && 
             f_other.m_useTransform_b    == m_useTransform_b &&
             ( !f_other.m_useTransform_b || 
               ( f_other.m_useTransform_b &&  
                 f_other.m_rotation    == m_rotation && 
                 f_other.m_translation == m_translation ) ) );
}

bool
CCamera::operator != ( const CCamera & f_other ) const
{
    return not operator == (f_other);
}
