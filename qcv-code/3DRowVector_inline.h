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
* \file  3DMatrix_inline.h
* \author Hernan Badino
* \date  Thu Jun 18 09:52:19 EDT 2009
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2010 - All Rights Reserved            *****
******************************************************************************/

#include <cstdio>
#include <math.h>
#include <string.h> /// For memcpy

//#include "3DMatrix.h"

namespace QCV
{

inline void
C3DRowVector::clear ()
{
    memset(m_elems_p, 0, 3 * sizeof(double));

        //for (int i = 0; i < 3; ++i)
        //m_elems_p[i] = 0.;
}

inline void
C3DRowVector::print () const
{
    printf("\n%.15f %.15f %.15f\n", 
           m_x_d, m_y_d, m_z_d );
}

/// Sum with another vector.
inline C3DRowVector  
C3DRowVector::operator + ( const C3DRowVector & f_other ) const
{
    C3DRowVector result;

    result.m_x_d = m_x_d + f_other.m_x_d;
    result.m_y_d = m_y_d + f_other.m_y_d;
    result.m_z_d = m_z_d + f_other.m_z_d;

    return result;    
}


/// Subtraction with another vector.
inline C3DRowVector
C3DRowVector::operator - ( const C3DRowVector & f_other ) const
{
    C3DRowVector result;

    result.m_x_d = m_x_d - f_other.m_x_d;
    result.m_y_d = m_y_d - f_other.m_y_d;
    result.m_z_d = m_z_d - f_other.m_z_d;

    return result;
}


/// Unary sum.
inline C3DRowVector &
C3DRowVector::operator += ( const C3DRowVector & f_other )
{
    m_x_d += f_other.m_x_d;
    m_y_d += f_other.m_y_d;
    m_z_d += f_other.m_z_d;

    return *this;
}


/// Unary subtraction.
inline C3DRowVector &
C3DRowVector::operator -= ( const C3DRowVector & f_other )
{
    m_x_d -= f_other.m_x_d;
    m_y_d -= f_other.m_y_d;
    m_z_d -= f_other.m_z_d;

    return *this;
}

/// Get the magnitude of vector.
inline double
C3DRowVector::magnitude() const
{
    return sqrt(sumOfSquares());
}

/// Get the magnitude of vector.
inline double
C3DRowVector::sumOfSquares() const
{
    return m_x_d * m_x_d + m_y_d * m_y_d + m_z_d * m_z_d;
}


/// Get the magnitude of vector.
inline void
C3DRowVector::normalize()
{
    double mag_d = magnitude();
    
    m_x_d /= mag_d;
    m_y_d /= mag_d;
    m_z_d /= mag_d;
}

/// Return the cross product of two vectors.
inline C3DRowVector 
C3DRowVector::crossProduct ( const C3DRowVector & f_other ) const
{
    C3DRowVector res;

    res.m_x_d = m_y_d * f_other.m_z_d - m_z_d * f_other.m_y_d;
    res.m_y_d = m_z_d * f_other.m_x_d - m_x_d * f_other.m_z_d;
    res.m_z_d = m_x_d * f_other.m_y_d - m_y_d * f_other.m_x_d;

    return res;
}

/// Return the cross product of two vectors.
inline double
C3DRowVector::dotProduct ( const C3DRowVector & f_other ) const
{
    double val_d =  ( m_x_d * f_other.m_x_d +
                      m_y_d * f_other.m_y_d +
                      m_z_d * f_other.m_z_d );
    
    return val_d;
}

/// Return the cross product of two vectors.
inline double 
C3DRowVector::operator * ( const C3DRowVector & f_other ) const
{
    double val_d =  ( m_x_d * f_other.m_x_d +
                      m_y_d * f_other.m_y_d +
                      m_z_d * f_other.m_z_d );
    
    return val_d;
}

/// Comparison operator.
inline bool
C3DRowVector::operator == ( const C3DRowVector & f_other ) const
{
    return ( m_x_d == f_other.m_x_d && 
             m_y_d == f_other.m_y_d && 
             m_z_d == f_other.m_z_d );
}


/// Comparison operator.
inline bool
C3DRowVector::operator != ( const C3DRowVector & f_other ) const
{
    return ( m_x_d != f_other.m_x_d ||
             m_y_d != f_other.m_y_d || 
             m_z_d != f_other.m_z_d );
}

inline
C3DRowVector::operator bool (  ) const
{
    return ( m_x_d || m_y_d || m_z_d );
}

inline
C3DRowVector::operator cv::Vec3d (  ) const
{
    return cv::Vec3d ( m_x_d, m_y_d, m_z_d );
}

inline bool 
operator ! ( const C3DRowVector & f_vector )
{
    return (!f_vector.x() && !f_vector.y() && !f_vector.z());
}

inline C3DRowVector 
operator - ( const C3DRowVector & f_vector )
{
    return C3DRowVector(-f_vector.x(), -f_vector.y(), -f_vector.z() );
}

}
/* ////////////  Version History ///////////////
 *  $Log: 3DRowVector_inline.h,v $
 *  Revision 1.3  2010/01/18 17:12:29  badino
 *  General updates and added new classes.
 *
 *  Revision 1.2  2009/11/18 15:51:01  badino
 *  badino: documentation added. Some other global changes.
 *
 *//////////////////////////////////////////////
