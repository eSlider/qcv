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

#include <stdio.h>
#include <math.h>
#include "3DRowVector.h"

/// Set to 0 all elements.
inline void
C3DMatrix::clear ()
{
    memset(m_elems_p, 0, 9 * sizeof(double));
    
    //for (int i = 0; i < 9; ++i)
    //    m_elems_p[i] = 0.;
}

/// Set values from vector.
inline void
C3DMatrix::set ( const double * const f_vals_p )
{ 
    memcpy ( m_elems_p, f_vals_p, 9 * sizeof(double) ); 
}

inline double &
C3DMatrix::at ( int f_row_i, int f_col_i )
{ 
    return m_elems_p[f_row_i*3+f_col_i]; 
}

inline double
C3DMatrix::at ( int f_row_i, int f_col_i ) const
{ 
    return m_elems_p[f_row_i*3+f_col_i]; 
}

inline double &
C3DMatrix::at ( int f_elem_i )
{ 
    return m_elems_p[f_elem_i];
}

inline double
C3DMatrix::at ( int f_elem_i ) const
{ 
    return m_elems_p[f_elem_i]; 
}

inline void
C3DMatrix::loadIdentity ()
{
    for (int i = 0; i < 9; ++i)
        m_elems_p[i] = ((i%4)==0);
}

inline void
C3DMatrix::diagonalize ( double f_val_d )
{
    memset(m_elems_p, 0, 9 * sizeof(double));
    m_elems_p[0] = m_elems_p[4] = m_elems_p[8] = f_val_d;

    //for (int i = 0; i < 9; ++i)
    //    m_elems_p[i] = ((i%4)==0)?f_val_d:0;
}

inline void
C3DMatrix::diagonalize ( double f_val1_d,
                         double f_val2_d,
                         double f_val3_d )
{
    memset(m_elems_p, 0, 9 * sizeof(double));
    m_elems_p[0] = f_val1_d;
    m_elems_p[4] = f_val2_d;
    m_elems_p[8] = f_val3_d;
}

inline void
C3DMatrix::diagonalize (  C3DVector f_val )
{
    memset(m_elems_p, 0, 9 * sizeof(double));
    m_elems_p[0] = f_val.x();
    m_elems_p[4] = f_val.y();
    m_elems_p[8] = f_val.z();
}

inline void
C3DMatrix::print () const
{
    printf("\n%.15f %.15f %.15f\n%.15f %.15f %.15f\n%.15f %.15f %.15f\n", 
           m_elems_p[0], m_elems_p[1], m_elems_p[2],
           m_elems_p[3], m_elems_p[4], m_elems_p[5],
           m_elems_p[6], m_elems_p[7], m_elems_p[8] );
}

inline C3DMatrix 
C3DMatrix::operator * ( const C3DMatrix & f_other ) const
{
    C3DMatrix result;

    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
        {
            result.m_elems_p[i*3+j] = m_elems_p[i*3] * f_other.m_elems_p[j];
            for(int k = 1; k < 3; ++k)
                result.m_elems_p[i*3+j] += m_elems_p[i*3+k] * f_other.m_elems_p[k*3+j];
        }
    
    return result;
}

inline C3DMatrix 
C3DMatrix::operator + ( const C3DMatrix & f_other ) const
{
    C3DMatrix result;

    for (int i = 0; i < 9; ++i)
        result.m_elems_p[i] = m_elems_p[i] + f_other.m_elems_p[i];

    return result;
}

inline C3DMatrix 
C3DMatrix::operator - ( const C3DMatrix & f_other ) const
{
    C3DMatrix result;

    for (int i = 0; i < 9; ++i)
        result.m_elems_p[i] = m_elems_p[i] - f_other.m_elems_p[i];

    return result;
}

inline C3DMatrix &
C3DMatrix::operator *= ( const C3DMatrix & f_other )
{
    C3DMatrix auxiliar;

    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
        {
            auxiliar.m_elems_p[i*3+j] = m_elems_p[i*3] * f_other.m_elems_p[j];
            for(int k = 1; k < 3; ++k)
                auxiliar.m_elems_p[i*3+j] += m_elems_p[i*3+k] * f_other.m_elems_p[k*3+j];
        }
    
    *this = auxiliar;
    return *this;
}

inline C3DMatrix &
C3DMatrix::operator += ( const C3DMatrix & f_other )
{
    for(int i = 0; i < 9; ++i)
    {
        m_elems_p[i] += f_other.m_elems_p[i];
    }
    
    return *this;
}

inline C3DMatrix &
C3DMatrix::operator -= ( const C3DMatrix & f_other )
{
    for(int i = 0; i < 9; ++i)
    {
        m_elems_p[i] -= f_other.m_elems_p[i];
    }
    
    return *this;
}

inline C3DMatrix
C3DMatrix::getInverse ( ) const
{
    // from row 1 to row 4
    C3DMatrix aux = *this;
    C3DMatrix res;
    res.loadIdentity();

    for (int i = 0; i < 3; i++)
    {
        // normalize the diagonal elements
        double e = aux.m_elems_p[i*3+i];
        for (int j = 0; j < 3; j++)
        {
            aux.m_elems_p[i*3+j] /= e;
            res.m_elems_p[i*3+j] /= e;
        }
        
        // make other elements to be 0
        for (int k = 0; k < 3; k++)
        {
            if (k != i)
            {
                e = -aux.m_elems_p[k*3+i];
                for (int j = 0; j < 3; j++)
                {
                    aux.m_elems_p[k*3+j] += e * aux.m_elems_p[i*3+j];
                    res.m_elems_p[k*3+j] += e * res.m_elems_p[i*3+j];
                }
            }
        }
    }
    
    return res;
}

inline C3DMatrix
C3DMatrix::getTranspose ( ) const
{
    C3DMatrix result;
    result.m_elems_p[0] = m_elems_p[0];
    result.m_elems_p[1] = m_elems_p[3];
    result.m_elems_p[2] = m_elems_p[6];
    result.m_elems_p[3] = m_elems_p[1];
    result.m_elems_p[4] = m_elems_p[4];
    result.m_elems_p[5] = m_elems_p[7];
    result.m_elems_p[6] = m_elems_p[2];
    result.m_elems_p[7] = m_elems_p[5];
    result.m_elems_p[8] = m_elems_p[8];
    
    return result;
}

inline void
C3DMatrix::invert ( )
{
#if 0 // algorithm
    C3DMatrix res;
    res.loadIdentity();

    printf("C3DMatrix res;\nres.loadIdentity();\n");
    
    for (int i = 0; i < 3; i++)
    {
        // normalize the diagonal elements
        double e = m_elems_p[i*3+i];
        printf("e = m_elems_p[%i];\n", i*3+i );
        
        for (int j = 0; j < 3; j++)
        {
            printf("m_elems_p[%i] /= e;\n", i*3+j);            
            m_elems_p[i*3+j] /= e;
            printf("res.m_elems_p[%i] /= e;\n", i*3+j);
            res.m_elems_p[i*3+j] /= e;
        }
        
        // make other elements to be 0
        for (int k = 0; k < 3; k++)
        {
            if (k != i)
            {
                e = -m_elems_p[k*3+i];
                printf("e = -m_elems_p[%i];\n", k*3+i);            
                for (int j = 0; j < 3; j++)
                {
                    printf ("m_elems_p[%i] += e * m_elems_p[%i];\n", k*3+j , i*3+j);
                    m_elems_p[k*3+j] += e * m_elems_p[i*3+j];
                    printf("res.m_elems_p[%i] += e * res.m_elems_p[%i];\n", k*3+j, i*3+j);
                    res.m_elems_p[k*3+j] += e * res.m_elems_p[i*3+j];
                }
            }
        }
    }
    
    printf("*this = res;\n");
    *this = res;
#elif 1
    C3DMatrix res;

    res.loadIdentity();

    res.m_elems_p[0] /= m_elems_p[0];
    m_elems_p[1]     /= m_elems_p[0];
    res.m_elems_p[1] /= m_elems_p[0];
    m_elems_p[2]     /= m_elems_p[0];
    res.m_elems_p[2] /= m_elems_p[0];

    res.m_elems_p[3] -= m_elems_p[3] * res.m_elems_p[0];
    m_elems_p[4]     -= m_elems_p[3] * m_elems_p[1];
    res.m_elems_p[4] -= m_elems_p[3] * res.m_elems_p[1];
    m_elems_p[5]     -= m_elems_p[3] * m_elems_p[2];
    res.m_elems_p[5] -= m_elems_p[3] * res.m_elems_p[2];

    res.m_elems_p[6] -= m_elems_p[6] * res.m_elems_p[0];
    m_elems_p[7]     -= m_elems_p[6] * m_elems_p[1];
    res.m_elems_p[7] -= m_elems_p[6] * res.m_elems_p[1];
    m_elems_p[8]     -= m_elems_p[6] * m_elems_p[2];
    res.m_elems_p[8] -= m_elems_p[6] * res.m_elems_p[2];

    ////////////////////////////

    res.m_elems_p[3] /= m_elems_p[4];
    res.m_elems_p[4] /= m_elems_p[4];
    m_elems_p[5]     /= m_elems_p[4];
    res.m_elems_p[5] /= m_elems_p[4];

    res.m_elems_p[0] -= m_elems_p[1] * res.m_elems_p[3];
    res.m_elems_p[1] -= m_elems_p[1] * res.m_elems_p[4];
    m_elems_p[2]     -= m_elems_p[1] * m_elems_p[5];
    res.m_elems_p[2] -= m_elems_p[1] * res.m_elems_p[5];

    res.m_elems_p[6] -= m_elems_p[7] * res.m_elems_p[3];
    res.m_elems_p[7] -= m_elems_p[7] * res.m_elems_p[4];
    m_elems_p[8]     -= m_elems_p[7] * m_elems_p[5];
    res.m_elems_p[8] -= m_elems_p[7] * res.m_elems_p[5];

    /////////////////////////////

    res.m_elems_p[6] /= m_elems_p[8];
    res.m_elems_p[7] /= m_elems_p[8];
    res.m_elems_p[8] /= m_elems_p[8];

    res.m_elems_p[0] -= m_elems_p[2] * res.m_elems_p[6];
    res.m_elems_p[1] -= m_elems_p[2] * res.m_elems_p[7];
    res.m_elems_p[2] -= m_elems_p[2] * res.m_elems_p[8];

    res.m_elems_p[3] -= m_elems_p[5] * res.m_elems_p[6];
    res.m_elems_p[4] -= m_elems_p[5] * res.m_elems_p[7];
    res.m_elems_p[5] -= m_elems_p[5] * res.m_elems_p[8];
    
    //res.print();

    *this = res;

#else
    C3DMatrix res = *this;
    double m48 = res.m_elems_p[4]*res.m_elems_p[8];
    double m72 = res.m_elems_p[7]*res.m_elems_p[2];
    double m15 = res.m_elems_p[1]*res.m_elems_p[5];
    double m07 = res.m_elems_p[0]*res.m_elems_p[7];
    double m31 = res.m_elems_p[3]*res.m_elems_p[1];
    double m64 = res.m_elems_p[6]*res.m_elems_p[4];
    
    double det = ( res.m_elems_p[0]*m48 + 
                   res.m_elems_p[3]*m72 + 
                   res.m_elems_p[6]*m15 - 
                   res.m_elems_p[5]*m07 - 
                   m31*res.m_elems_p[8] - 
                   m64*res.m_elems_p[2] );

    printf("det = %f determinant = %f\n", det, determinant());
    
    res.m_elems_p[0] = (m48 - res.m_elems_p[7]*res.m_elems_p[5])/det;
    res.m_elems_p[1] = (res.m_elems_p[6]*res.m_elems_p[5] - res.m_elems_p[3]*res.m_elems_p[8])/det;
    res.m_elems_p[2] = (res.m_elems_p[3]*res.m_elems_p[7] - m64)/det;
    res.m_elems_p[3] = (m72 - res.m_elems_p[1]*res.m_elems_p[8])/det;
    res.m_elems_p[4] = (res.m_elems_p[0]*res.m_elems_p[8] - res.m_elems_p[6]*res.m_elems_p[2])/det;
    res.m_elems_p[5] = (res.m_elems_p[6]*res.m_elems_p[1] - m07)/det;
    res.m_elems_p[6] = (m15 - res.m_elems_p[4]*res.m_elems_p[2])/det;
    res.m_elems_p[7] = (res.m_elems_p[3]*res.m_elems_p[2] - res.m_elems_p[0]*res.m_elems_p[5])/det;
    res.m_elems_p[8] = (res.m_elems_p[0]*res.m_elems_p[4] - m31)/det;

//     double det = (res.m_elems_p[0]*m48 + 
//                   res.m_elems_p[3]*m72 + 
//                   res.m_elems_p[6]*m15 - 
//                   m07*res.m_elems_p[5] - 
//                   m31*res.m_elems_p[8] - 
//                   m64*res.m_elems_p[2] );

//     m_elems_p[0] = (m48 - res.m_elems_p[7]*res.m_elems_p[5])/det;
//     m_elems_p[1] = (res.m_elems_p[6]*res.m_elems_p[5] - res.m_elems_p[3]*res.m_elems_p[8])/det;
//     m_elems_p[2] = (res.m_elems_p[3]*res.m_elems_p[7] - m64)/det;
//     m_elems_p[3] = (m72 - res.m_elems_p[1]*res.m_elems_p[8])/det;
//     m_elems_p[4] = (res.m_elems_p[0]*res.m_elems_p[8] - res.m_elems_p[6]*res.m_elems_p[2])/det;
//     m_elems_p[5] = (res.m_elems_p[6]*res.m_elems_p[1] - m07)/det;
//     m_elems_p[6] = (m15 - res.m_elems_p[4]*res.m_elems_p[2])/det;
//     m_elems_p[7] = (res.m_elems_p[3]*res.m_elems_p[2] - res.m_elems_p[0]*res.m_elems_p[7])/det;
//     m_elems_p[8] = (res.m_elems_p[0]*res.m_elems_p[4] - m31)/det;
#endif
}

inline void
C3DMatrix::transpose ( )
{
    std::swap(m_elems_p[1], m_elems_p[3]);
    std::swap(m_elems_p[2], m_elems_p[6]);
    std::swap(m_elems_p[5], m_elems_p[7]);    
}


inline C3DRowVector 
C3DMatrix::multiply ( const C3DRowVector & f_vector ) const
{
    return operator * ( f_vector );    
}

inline C3DRowVector 
C3DMatrix::operator * ( const C3DRowVector & f_vector ) const
{
    C3DRowVector result;

    for (int i = 0; i < 3; i++)
    {
        result.m_elems_p[i]  = m_elems_p[i*3+0] * f_vector.m_elems_p[0];
        result.m_elems_p[i] += m_elems_p[i*3+1] * f_vector.m_elems_p[1];    
        result.m_elems_p[i] += m_elems_p[i*3+2] * f_vector.m_elems_p[2];    
    }
    
    return result;
}

/// Multiply transposed matrix with vector.
inline C3DRowVector 
C3DMatrix::multiplyTransposed ( const C3DRowVector & f_vector ) const
{
    C3DRowVector result;

    for (int i = 0; i < 3; i++)
    {
        result.m_elems_p[i]  = m_elems_p[0*3+i] * f_vector.m_elems_p[0];
        result.m_elems_p[i] += m_elems_p[1*3+i] * f_vector.m_elems_p[1];    
        result.m_elems_p[i] += m_elems_p[2*3+i] * f_vector.m_elems_p[2];    
    }
    
    return result;
}

inline void
C3DMatrix::rotateX ( double f_angle_d )
{
    C3DMatrix rotation;
    
    double sin_d = sin(f_angle_d);
    double cos_d = cos(f_angle_d);
    
    rotation.m_elems_p[0] = 1.;
    rotation.m_elems_p[1] = 0.;
    rotation.m_elems_p[2] = 0.;
    rotation.m_elems_p[3] = 0.;
    rotation.m_elems_p[4] = cos_d;
    rotation.m_elems_p[5] = -sin_d;
    rotation.m_elems_p[6] = 0.;
    rotation.m_elems_p[7] = sin_d;
    rotation.m_elems_p[8] = cos_d;
    
    *this *= rotation;
}

inline void
C3DMatrix::rotateY ( double f_angle_d )
{
    C3DMatrix rotation;
    
    double sin_d = sin(f_angle_d);
    double cos_d = cos(f_angle_d);
    
    rotation.m_elems_p[0] = cos_d;
    rotation.m_elems_p[1] = 0.;
    rotation.m_elems_p[2] = sin_d;
    rotation.m_elems_p[3] = 0.;
    rotation.m_elems_p[4] = 1.;
    rotation.m_elems_p[5] = 0;
    rotation.m_elems_p[6] = -sin_d;
    rotation.m_elems_p[7] = 0.;
    rotation.m_elems_p[8] = cos_d;
    
    *this *= rotation;
}

inline void
C3DMatrix::rotateZ ( double f_angle_d )
{
    C3DMatrix rotation;
    
    double sin_d = sin(f_angle_d);
    double cos_d = cos(f_angle_d);
    
    rotation.m_elems_p[0] = cos_d;
    rotation.m_elems_p[1] = -sin_d;
    rotation.m_elems_p[2] = 0.;
    rotation.m_elems_p[3] = sin_d;
    rotation.m_elems_p[4] = cos_d;
    rotation.m_elems_p[5] = 0.;
    rotation.m_elems_p[6] = 0.;
    rotation.m_elems_p[7] = 0.;
    rotation.m_elems_p[8] = 1.;
    
    *this *= rotation;
}

inline void 
C3DMatrix::rotate ( C3DRowVector f_axis, 
                    double       f_angle_d )
{
    rotateAxis(f_axis,f_angle_d);    
}

inline void 
C3DMatrix::rotateAxis ( C3DRowVector f_axis, 
                        double       f_angle_d )
{
#if 0
    C3DMatrix rotation;
    const double cos_d   = cos( f_angle_d );
    const double sin_d   = sin( f_angle_d );
    const double icos_d  = 1.0 - cos_d;

    const double xy_d    = f_axis.x()*f_axis.y();
    const double xz_d    = f_axis.x()*f_axis.z();
    const double yz_d    = f_axis.y()*f_axis.z();
    const double x_sin_d = f_axis.x()*sin_d;
    const double y_sin_d = f_axis.y()*sin_d;
    const double z_sin_d = f_axis.z()*sin_d;

    rotation.m_elems_p[ 0 ] = cos_d + f_axis.x()*f_axis.x()*icos_d;
    rotation.m_elems_p[ 1 ] = xy_d*icos_d - z_sin_d;
    rotation.m_elems_p[ 2 ] = xz_d*icos_d + y_sin_d;

    rotation.m_elems_p[ 3 ] = xy_d*icos_d + z_sin_d;
    rotation.m_elems_p[ 4 ] = cos_d + f_axis.y()*f_axis.y()*icos_d;
    rotation.m_elems_p[ 5 ] = yz_d*icos_d - x_sin_d;

    rotation.m_elems_p[ 6 ] = xz_d*icos_d - y_sin_d;
    rotation.m_elems_p[ 7 ] = yz_d*icos_d + x_sin_d;
    rotation.m_elems_p[ 8 ] = cos_d + f_axis.z()*f_axis.z()*icos_d;

    *this *= rotation;

#else
    C3DMatrix rotation;

    double sin_d = sin(f_angle_d);
    double cos_d = cos(f_angle_d);
    C3DRowVector xyzS  = f_axis * sin_d;
    C3DRowVector xyzC  = f_axis * (1 - cos_d);

    C3DRowVector xyz2C ( f_axis.m_x_d * xyzC.m_y_d,
                         f_axis.m_y_d * xyzC.m_z_d,
                         f_axis.m_z_d * xyzC.m_x_d );

    
    rotation.m_elems_p[0] = f_axis.m_x_d * xyzC.m_x_d + cos_d;
    rotation.m_elems_p[1] = xyz2C.m_x_d - xyzS.m_z_d;
    rotation.m_elems_p[2] = xyz2C.m_z_d + xyzS.m_y_d;
    rotation.m_elems_p[3] = xyz2C.m_x_d + xyzS.m_z_d;
    rotation.m_elems_p[4] = f_axis.m_y_d * xyzC.m_y_d + cos_d;
    rotation.m_elems_p[5] = xyz2C.m_y_d - xyzS.m_x_d;
    rotation.m_elems_p[6] = xyz2C.m_z_d - xyzS.m_y_d;
    rotation.m_elems_p[7] = xyz2C.m_y_d + xyzS.m_x_d;
    rotation.m_elems_p[8] = f_axis.m_z_d * xyzC.m_z_d + cos_d;
  
    *this *= rotation;
#endif
}

inline void 
C3DMatrix::rotate ( C3DRowVector f_axis )
{
    rotateAxis(f_axis);
}

inline void 
C3DMatrix::rotateAxis ( C3DRowVector f_axis )
{
    double angle_d = f_axis.magnitude();

    if ( fabs(angle_d) > 1.e-9) 
       f_axis /= angle_d;
    
    return rotateAxis ( f_axis, angle_d );
}

/// Comparison operator.
inline bool
C3DMatrix::operator == ( const C3DMatrix & f_other ) const
{
    return ( (m_elems_p[0] == f_other.m_elems_p[0]) &&
             ( (m_elems_p[1] == f_other.m_elems_p[1]) && 
               ( (m_elems_p[2] == f_other.m_elems_p[2]) && 
                 ( (m_elems_p[3] == f_other.m_elems_p[3]) && 
                   ( (m_elems_p[4] == f_other.m_elems_p[4]) && 
                     ( (m_elems_p[5] == f_other.m_elems_p[5]) && 
                       ( (m_elems_p[6] == f_other.m_elems_p[6]) && 
                         ( (m_elems_p[7] == f_other.m_elems_p[7]) && 
                           ( (m_elems_p[8] == f_other.m_elems_p[8]) ) ) ) ) ) ) ) ) );
}

/// Comparison operator.
inline bool
C3DMatrix::operator != ( const C3DMatrix & f_other ) const
{
    return !operator == (f_other);
}

/// Compute trace.
inline double
C3DMatrix::trace ( ) const
{
    return m_elems_p[0] + m_elems_p[4] + m_elems_p[8];
}

/// Compute trace.
inline double
C3DMatrix::determinant ( ) const
{
    double sum1_d = ( m_elems_p[0] * 
                      ( m_elems_p[4] * m_elems_p[8] -
                        m_elems_p[5] * m_elems_p[7] ) );
    
    double sum2_d = ( m_elems_p[1] * 
                      ( m_elems_p[5] * m_elems_p[6] -
                        m_elems_p[3] * m_elems_p[8] ) );
    
    double sum3_d = ( m_elems_p[2] * 
                      ( m_elems_p[3] * m_elems_p[7] -
                        m_elems_p[4] * m_elems_p[6] ) );
    
    return sum1_d + sum2_d + sum3_d;
}


/// Return the outer product of two vectors.
//C3DMatrix    outerProduct ( const C3DRowVector & f_other ) const;

inline C3DMatrix
C3DRowVector::outerProduct ( const C3DRowVector & f_other ) const
{
    C3DMatrix m;
    m.m_elems_p[0*3+0] = m_x_d * f_other.m_x_d;
    m.m_elems_p[0*3+1] = m_x_d * f_other.m_y_d;
    m.m_elems_p[0*3+2] = m_x_d * f_other.m_z_d;
    m.m_elems_p[1*3+0] = m_y_d * f_other.m_x_d;
    m.m_elems_p[1*3+1] = m_y_d * f_other.m_y_d;
    m.m_elems_p[1*3+2] = m_y_d * f_other.m_z_d;
    m.m_elems_p[2*3+0] = m_z_d * f_other.m_x_d;
    m.m_elems_p[2*3+1] = m_z_d * f_other.m_y_d;
    m.m_elems_p[2*3+2] = m_z_d * f_other.m_z_d;
    
    return m;
}

inline
C3DMatrix::operator bool (  ) const
{
    return ( m_elems_p[0*3+0] ||
             m_elems_p[0*3+1] ||
             m_elems_p[0*3+2] ||
             m_elems_p[1*3+0] ||
             m_elems_p[1*3+1] ||
             m_elems_p[1*3+2] ||
             m_elems_p[2*3+0] ||
             m_elems_p[2*3+1] ||
             m_elems_p[2*3+2] );
}

/// The alpha_d value is for the *this matrix.
inline C3DMatrix
C3DMatrix::interpolate( const C3DMatrix &f_m2, 
                        double alpha_d )
{
    *this = getInterpolation( f_m2, alpha_d );
    return *this;
}

/// The alpha_d value is for the *this matrix.
inline C3DMatrix
C3DMatrix::getInterpolation( const C3DMatrix &f_m2, 
                             double alpha_d ) const
{
  C3DMatrix retm;
  double quat1_p[4], quat2_p[4], intquat_p[4];

  convert2Quaternion(quat1_p);
  f_m2.convert2Quaternion(quat2_p);

  // Calculate the cosine of the angle between the two
  double scale0_d;
  double scale1_d;
  double cos_d = quat1_p[0] * quat2_p[0] + quat1_p[1] * quat2_p[1] + quat1_p[2] * quat2_p[2] + quat1_p[3] * quat2_p[3];
  
  // If the angle is significant, use the spherical interpolation
  if( (1 - fabs(cos_d)) >  1.e-7 )
  {
    double temp_d  = acos( fabs(cos_d) );
    double sinus_d = sin ( temp_d );
    scale0_d = sin( (1 - alpha_d) * temp_d) / sinus_d;
    scale1_d = sin(      alpha_d  * temp_d) / sinus_d;
  }
  else
  {  //  Else use the cheaper linear interpolation
    scale0_d = 1 - alpha_d;
    scale1_d = alpha_d;
  }
  
  if( cos_d < 0 ) scale1_d = -scale1_d;
  
  intquat_p[0] = quat1_p[0] * scale0_d + quat2_p[0] * scale1_d;
  intquat_p[1] = quat1_p[1] * scale0_d + quat2_p[1] * scale1_d;
  intquat_p[2] = quat1_p[2] * scale0_d + quat2_p[2] * scale1_d;
  intquat_p[3] = quat1_p[3] * scale0_d + quat2_p[3] * scale1_d;

  // Convert again to matrix and return result.
  retm.loadFromQuaternion( intquat_p );

  return retm;
}


inline void 
C3DMatrix::loadFromQuaternion(const double *f_quat_p)
{
  double sqw =  f_quat_p[0] * f_quat_p[0]; //w
  double sqx =  f_quat_p[1] * f_quat_p[1]; //x
  double sqy =  f_quat_p[2] * f_quat_p[2]; //y
  double sqz =  f_quat_p[3] * f_quat_p[3]; //z
  
  m_elems_p[0*3+0] = sqw + sqx - sqy - sqz ;
  m_elems_p[0*3+1] = 2*( f_quat_p[1]*f_quat_p[2] + f_quat_p[0]*f_quat_p[3] );
  m_elems_p[0*3+2] = 2*( f_quat_p[1]*f_quat_p[3] - f_quat_p[0]*f_quat_p[2] );
  m_elems_p[1*3+0] = 2*( f_quat_p[1]*f_quat_p[2] - f_quat_p[0]*f_quat_p[3] );
  m_elems_p[1*3+1] = sqw + sqy - sqx - sqz ;
  m_elems_p[1*3+2] = 2*( f_quat_p[2]*f_quat_p[3] + f_quat_p[0]*f_quat_p[1] );
  m_elems_p[2*3+0] = 2*( f_quat_p[1]*f_quat_p[3] + f_quat_p[0]*f_quat_p[2] );
  m_elems_p[2*3+1] = 2*( f_quat_p[2]*f_quat_p[3] - f_quat_p[0]*f_quat_p[1] );
  m_elems_p[2*3+2] = sqw + sqz - sqx - sqy ;
}

inline void
C3DMatrix::convert2Quaternion( double *f_quat_p ) const
{
  // Calculate the trace of the matrix T from the equation:
  double T = 1 + m_elems_p[0*3+0] + m_elems_p[1*3+1] + m_elems_p[2*3+2];

  //  If the trace of the matrix is greater than zero, then
  //  perform an "instant" calculation in order  to avoid large distortions!
  double S;
  if (T > 1.e-7)
  {
    S = 0.5 / sqrt(T);
    f_quat_p[0] = 0.25 / S;
    f_quat_p[1] = (m_elems_p[1*3+2] - m_elems_p[2*3+1]) * S;
    f_quat_p[2] = (m_elems_p[2*3+0] - m_elems_p[0*3+2]) * S;
    f_quat_p[3] = (m_elems_p[0*3+1] - m_elems_p[1*3+0]) * S;
    return;
  }
  
  // If the trace of the matrix is equal to zero then identify
  // which major diagonal m_elems_p has the greatest value.
  // Depending on this, calculate the following:

  if ( m_elems_p[0*3+0] > m_elems_p[1*3+1] && m_elems_p[0*3+0] > m_elems_p[2*3+2] )  
  {	// Column 0: 
    S  = sqrt( 1.0 + m_elems_p[0*3+0] - m_elems_p[1*3+1] - m_elems_p[2*3+2] ) * 2;
    f_quat_p[1] = 0.25 * S;
    f_quat_p[2] = (m_elems_p[0*3+1] + m_elems_p[1*3+0] ) / S;
    f_quat_p[3] = (m_elems_p[0*3+2] + m_elems_p[2*3+0] ) / S;
    f_quat_p[0] = (m_elems_p[2*3+1] - m_elems_p[1*3+2] ) / S;
    return;
  } 

  if ( m_elems_p[1*3+1] > m_elems_p[2*3+2] ) 
  {			// Column 1: 
    S  = sqrt( 1.0 + m_elems_p[1*3+1] - m_elems_p[0*3+0] - m_elems_p[2*3+2] ) * 2;
    f_quat_p[1] = (m_elems_p[1*3+0] + m_elems_p[0*3+1] ) / S;
    f_quat_p[2] = 0.25 * S;
    f_quat_p[3] = (m_elems_p[2*3+1] + m_elems_p[1*3+2] ) / S;
    f_quat_p[0] = (m_elems_p[2*3+0] - m_elems_p[0*3+2] ) / S;
    return;
  } 

  S  = sqrt( 1.0 + m_elems_p[2*3+2] - m_elems_p[0*3+0] - m_elems_p[1*3+1] ) * 2;
  f_quat_p[1] = (m_elems_p[0*3+2] + m_elems_p[2*3+0] ) / S;
  f_quat_p[2] = (m_elems_p[2*3+1] + m_elems_p[1*3+2] ) / S;
  f_quat_p[3] = 0.25 * S;
  f_quat_p[0] = (m_elems_p[1*3+0] - m_elems_p[0*3+1] ) / S;

}


inline bool 
operator ! ( const C3DMatrix & f_matrix )
{
    return ! ((bool)f_matrix);
}

inline C3DMatrix
operator - ( const C3DMatrix & f_matrix )
{
    return f_matrix * -1;
}

inline void 
C3DMatrix::getRotationAngles( double &fr_pitch_d, 
                              double &fr_yaw_d, 
                              double &fr_roll_d )
{
    double dSinPitch_d, dCosPitch_d;
    
    fr_pitch_d = atan2(m_elems_p[1*3+2], -m_elems_p[2*3+2]);
    dSinPitch_d = sin( fr_pitch_d );
    dCosPitch_d = cos( fr_pitch_d );
    fr_yaw_d  = atan2( m_elems_p[0*3+2], dCosPitch_d*m_elems_p[2*3+2] - dSinPitch_d*m_elems_p[1*3+2]);
    fr_roll_d = atan2( dCosPitch_d*m_elems_p[1*3+0] + dSinPitch_d*m_elems_p[2*3+0],
                       dCosPitch_d*m_elems_p[1*3+1] + dSinPitch_d*m_elems_p[2*3+1]);
    
    //  printf("fr_yaw_d:%g\n", fr_yaw_d);
    if (fabs(fr_pitch_d) > M_PI/2.0)
    {
        fr_pitch_d = fr_pitch_d > M_PI/2.0 ? fr_pitch_d - M_PI : fr_pitch_d + M_PI;
        fr_yaw_d = M_PI - fr_yaw_d;
        if (fr_yaw_d > M_PI) fr_yaw_d -= 2.0*M_PI;
        fr_roll_d = fr_roll_d - M_PI;
        if (fr_roll_d < -M_PI) fr_roll_d += 2.0*M_PI;
    }

  // badino: commented out
#if 0
    const double epsilon = 1 - 1.e-6; // = 0.999999

  if (m_elems_p[2*3+0] > epsilon) { // singularity at north pole
    // printf("Gimbal Lock - North Pole Exception\n");
    fr_pitch_d = atan2(m_elems_p[0*3+1],m_elems_p[1*3+1]);
    fr_yaw_d = M_PI/2;
    fr_roll_d = 0.0;
  }
  if (m_elems_p[2*3+0] < -epsilon) { // singularity at south pole
    // printf("Gimbal Lock - South Pole Exception\n");
    fr_pitch_d = atan2(m_elems_p[0*3+1],m_elems_p[1*3+1]);
    fr_pitch_d = -M_PI/2;
    fr_roll_d = 0.0;
  }
#endif

}

inline void 
C3DMatrix::getRotationAxis( C3DVector &fr_rotAxis ) const
{
    double angle_d;
    
    getRotationAxis(fr_rotAxis, angle_d );
    
    fr_rotAxis *= angle_d;
}

inline void 
C3DMatrix::getRotationAxis( C3DVector &fr_normRotAxis, 
                            double    &fr_angle_d ) const
{    
    fr_angle_d = acos((trace() - 1.) / 2. );

    if (fabs(fr_angle_d) > 1.e-5)
    {
        double norm_d = 2. * sin(fr_angle_d);
        
        fr_normRotAxis = C3DVector ( m_elems_p[2*3+1] - m_elems_p[1*3+2], 
                                     m_elems_p[0*3+2] - m_elems_p[2*3+0], 
                                     m_elems_p[1*3+0] - m_elems_p[0*3+1] ) / norm_d;
    }
    else
        fr_normRotAxis.clear();
}
