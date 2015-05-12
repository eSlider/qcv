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

#ifndef __3DMATRIX_H
#define __3DMATRIX_H

/*@@@**************************************************************************
 ** 
 * \file   3DMatrix.h
 * \class  C3DMatrix
 * \author Hernan Badino
 *
 * Implements a simple 3D matrix for handling rotations of 3D vectors
 * and points.
 * 
*******************************************************************************
*****             (C) Hernan Badino 2010 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <stdlib.h>
//#include <algorithm>

#include "3DRowVector.h"
#include "3DMatrix.h"

/* CONSTANTS */

namespace QCV
{
///// Template generator for macros with datatype parameter.
#define DATA_TYPES_OPERATOR_GENERATOR(Macro_)           \
    Macro_##_CONCRETE(char)                             \
    Macro_##_CONCRETE(unsigned char)                    \
    Macro_##_CONCRETE(short int)                        \
    Macro_##_CONCRETE(unsigned short  int)              \
    Macro_##_CONCRETE(int)                              \
    Macro_##_CONCRETE(unsigned int)                     \
    Macro_##_CONCRETE(long)                             \
    Macro_##_CONCRETE(unsigned long int)                \
    Macro_##_CONCRETE(float)                            \
    Macro_##_CONCRETE(double)                           \
    Macro_##_CONCRETE(long double)

///// BINARY OPERATOR MACROS FOR STANDARD C++ NUMERIC TYPES
#define INLINE_BODY_DEF_SCALAR_BIN_OPERATOR(Op_, Dt_)   \
    C3DMatrix operator Op_ ( Dt_ f_value_f  ) const     \
    {                                                   \
        C3DMatrix result;                               \
        double *dest_p      = result.m_elems_p;         \
        const double *src_p = m_elems_p;                \
                                                        \
        for ( int i = 0; i < 9; ++i )                   \
        {                                               \
            dest_p[i] = src_p[i] Op_ f_value_f;         \
        }                                               \
        return result;                                  \
    }

#define INLINE_SCALAR_BIN_OPERATORS_CONCRETE(Dt_)       \
    INLINE_BODY_DEF_SCALAR_BIN_OPERATOR(*, Dt_)         \
    INLINE_BODY_DEF_SCALAR_BIN_OPERATOR(/, Dt_)

#define INLINE_SCALAR_BIN_OP_GENERATOR                  \
    DATA_TYPES_OPERATOR_GENERATOR(INLINE_SCALAR_BIN_OPERATORS)


///// UNARY OPERATOR MACROS FOR STANDARD C++ NUMERIC TYPES
#define INLINE_BODY_DEF_SCALAR_UN_OPERATOR(Op_, Dt_)    \
    C3DMatrix &operator Op_ ( Dt_ f_value_f  )          \
    {                                                   \
        double *dest_p = m_elems_p;                     \
                                                        \
        for ( int i = 0; i < 9; ++i )                   \
        {                                               \
            dest_p[i] Op_ f_value_f;                    \
        }                                               \
        return *this;                                   \
    }

#define INLINE_SCALAR_UN_OPERATORS_CONCRETE(Dt_)        \
    INLINE_BODY_DEF_SCALAR_UN_OPERATOR(*=, Dt_)         \
    INLINE_BODY_DEF_SCALAR_UN_OPERATOR(/=, Dt_)

#define INLINE_SCALAR_UN_OP_GENERATOR                   \
    DATA_TYPES_OPERATOR_GENERATOR(INLINE_SCALAR_UN_OPERATORS)

    class C3DMatrix
    {
        friend class C3DRowVector;

    /// Constructors, Destructors
    public:
        C3DMatrix ( ) {}
        C3DMatrix ( const double * const f_vals_p ) { memcpy ( m_elems_p, f_vals_p, 9 * sizeof(double)); }
        ~C3DMatrix ( ){}
        
    public:

        /// Access to elements.
        double &       at ( int f_elem_i );
        double         at ( int f_elem_i ) const;

        double &       at ( int f_row_i, int f_col_i );
        double         at ( int f_row_i, int f_col_i ) const;
        
        /// Set values.
        void           set ( const double * const f_vals_p );
        
        /// Load identity matrix.
        void           clear ();

        /// Load identity matrix.
        void           loadIdentity ();

        /// Load diagonal matrix.
        void           diagonalize ( double f_val_f );

        /// Load diagonal matrix.
        void           diagonalize ( double f_val1_d,
                                     double f_val2_d,
                                     double f_val3_d );

        /// Load diagonal matrix.
        void           diagonalize ( C3DVector f_vals );

        /// Get the transposed matrix.
        C3DMatrix      getTranspose ( ) const;
        
        /// Get the inverted matrix.
        C3DMatrix      getInverse ( ) const;
        
        /// Invert this matrix.
        void           invert ( );

        /// Tranpose this matrix.
        void           transpose ( );

    public:
        /// Some useful operators 
        /// Matrix multiplication.
        C3DMatrix      operator * ( const C3DMatrix & f_other ) const;

        /// Sum with another anmatrix.
        C3DMatrix      operator + ( const C3DMatrix & f_other ) const;
 
        /// Subtraction with another matrix.
        C3DMatrix      operator - ( const C3DMatrix & f_other ) const;

        /// Unary multiplication.
        C3DMatrix&     operator *= ( const C3DMatrix & f_other );

        /// Unary sum.
        C3DMatrix&     operator += ( const C3DMatrix & f_other );

        /// Unary substraction.
        C3DMatrix&     operator -= ( const C3DMatrix & f_other );

        /// Comparison operator.
        bool           operator == ( const C3DMatrix & f_other ) const;

        /// Comparison operator.
        bool           operator != ( const C3DMatrix & f_other ) const;

        /// Add binary operators of the form:
        ///   C3DMatrix operator Op_ ( DataType_ )
        /// for all standard C++ data types and being Op_
        /// / and *.

        INLINE_SCALAR_BIN_OP_GENERATOR
        /// Add unary operators of the form:
        ///   C3DMatrix operator Op_ ( DataType_ )
        /// for all standard C++ data types and being Op_
        /// /= and *=.
        INLINE_SCALAR_UN_OP_GENERATOR
        
    /// Cast operator.
    public:
        /// Cast operator.
        operator bool( ) const;

    /// Rotations
    public:
        void          rotateX ( double f_angle_d );
        void          rotateY ( double f_angle_d );
        void          rotateZ ( double f_angle_d );
        
        void          rotateAxis ( C3DRowVector axis, 
                                   double       f_angle_d );
        
        void          rotateAxis ( C3DRowVector axis );
        
        void          rotate ( C3DRowVector axis, 
                               double       f_angle_d );
        
        void          rotate ( C3DRowVector axis );

        void          loadFromQuaternion ( const double *f_quat_p );

        void          convert2Quaternion ( double *f_quat_p ) const;

        C3DMatrix     getInterpolation( const C3DMatrix &f_m2, 
                                        double alpha_d ) const;

        C3DMatrix     interpolate( const C3DMatrix &f_m2, 
                                   double alpha_d );

        void          getRotationAngles( double &fr_pitch_d, 
                                         double &fr_yaw_d, 
                                         double &fr_roll_d ); 

        void          getRotationAxis( C3DVector &fr_rotAxis ) const;

        void          getRotationAxis( C3DVector &fr_normRotAxis,
                                       double    &fr_angle_d ) const;

    public:
        /// Matrix Vector multiplication.
        C3DRowVector  multiply   ( const C3DRowVector & f_other ) const;
        C3DRowVector  operator * ( const C3DRowVector & f_other ) const;
        
        /// Multiply tranposed matrix with vector.
        C3DRowVector  multiplyTransposed ( const C3DRowVector & f_other ) const;

    /// Other operations
    public:
        /// Trace.
        double        trace ( ) const;

        /// Determinant.
        double        determinant ( ) const;


     public:
       /// print values.
        void          print () const;

    private:
        double      m_elems_p[9];        
    };
    
    
#undef DATA_TYPES_OPERATOR_GENERATOR
#undef INLINE_BODY_DEF_SCALAR_BIN_OPERATOR
#undef INLINE_SCALAR_BIN_OPERATORS_CONCRETE
#undef INLINE_BODY_DEF_SCALAR_UN_OPERATOR
#undef INLINE_SCALAR_UN_OPERATORS_CONCRETE
#undef INLINE_SCALAR_UN_OP_GENERATOR  

#include "3DMatrix_inline.h"
};

#endif // __3DMATRIX_H

/* ////////////  Version History ///////////////
 *  $Log: 3DMatrix.h,v $
 *  Revision 1.3  2010/01/18 17:12:29  badino
 *  General updates and added new classes.
 *
 *  Revision 1.2  2009/11/18 15:51:01  badino
 *  badino: documentation added. Some other global changes.
 *
 *//////////////////////////////////////////////
