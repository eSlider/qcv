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

#ifndef __3DROWVECTOR_H
#define __3DROWVECTOR_H

/*@@@**************************************************************************
 ** 
 * \file   3DRowVector.h
 * \class  3DVector
 * \author Hernan Badino
 *
 * Implements a simple 3D vector for 3D point operations.
 * 
*******************************************************************************
*****             (C) Hernan Badino 2010 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */

/* CONSTANTS */
#include <opencv/cv.h>

namespace QCV
{

    class C3DMatrix;
    
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
    C3DRowVector operator Op_ ( Dt_ f_value_f  ) const  \
    {                                                   \
        C3DRowVector result;                            \
        double *dest_p      = result.m_elems_p;         \
        const double *src_p = m_elems_p;                \
                                                        \
        for ( int i = 0; i < 3; ++i )                   \
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
    C3DRowVector &operator Op_ ( Dt_ f_value_f  )       \
    {                                                   \
        double *dest_p = m_elems_p;                     \
                                                        \
        for ( int i = 0; i < 3; ++i )                   \
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
    
    class C3DRowVector
    {
        friend class C3DMatrix;
        
    /// Constructors, Destructors
    public:
        C3DRowVector ( ) {}
        C3DRowVector ( double f_x_d, double f_y_d, double f_z_d) { m_x_d = f_x_d; m_y_d = f_y_d; m_z_d = f_z_d; }
        C3DRowVector ( const double * const f_vals_p ) { m_x_d = f_vals_p[0]; m_y_d = f_vals_p[1]; m_z_d = f_vals_p[2]; }
       ~C3DRowVector ( ){}
        
        C3DRowVector(cv::Vec3d f_vec )
        {
            set ( f_vec[0], f_vec[1], f_vec[2] );
        }

    public:
        
        /// Access to elements.,
        double  &at ( int f_row_i  )
        { return m_elems_p[f_row_i]; }

        double   at ( int f_row_i ) const
        { return m_elems_p[f_row_i]; }

        void     set (double f_x_d, double f_y_d, double f_z_d) { m_x_d = f_x_d; m_y_d = f_y_d; m_z_d = f_z_d; }

        bool    setX( double f_val_d ) { m_x_d = f_val_d; return true; }
        bool    setY( double f_val_d ) { m_y_d = f_val_d; return true; }
        bool    setZ( double f_val_d ) { m_z_d = f_val_d; return true; }

        double  getX( ) const { return m_x_d; }
        double  getY( ) const { return m_y_d; }
        double  getZ( ) const { return m_z_d; }
 
        double   x() const { return m_x_d; }
        double   y() const { return m_y_d; }
        double   z() const { return m_z_d; }        
            
        /// Clear vector.
        void     clear ();

        /// Normalize vector so that magnitude = 1
        void     normalize();

    /// Operations on vector.
    public:
        /// Get the magnitude of vector.
        double   magnitude() const;

        /// Get the magnitude of vector.
        double   sumOfSquares() const;

    public:

        /// Add binary operators of the form:
        ///   C3DRowVector operator Op_ ( DataType_ )
        /// for all standard C++ data types and being Op_
        /// / and *.
        INLINE_SCALAR_BIN_OP_GENERATOR

        /// Add unary operators of the form:
        ///   C3DRowVector operator Op_ ( DataType_ )
        /// for all standard C++ data types and being Op_
        /// /= and *=.
        INLINE_SCALAR_UN_OP_GENERATOR
        

    /// Operators with vectors and matrices.
    public:
        /// Sum with another vector.
        C3DRowVector  operator + ( const C3DRowVector & f_other ) const;

        /// Subtraction with another vector.
        C3DRowVector  operator - ( const C3DRowVector & f_other ) const;

        /// Dot product.
        double        operator * ( const C3DRowVector & f_other ) const;

        /// Unary sum.
        C3DRowVector& operator += ( const C3DRowVector & f_other );

        /// Unary subtraction.
        C3DRowVector& operator -= ( const C3DRowVector & f_other );

        /// Comparison operator.
        bool          operator == ( const C3DRowVector & f_other ) const;

        /// Comparison operator.
        bool          operator != ( const C3DRowVector & f_other ) const;

    /// Cast operator.
    public:
        /// Cast operator.
        operator bool( ) const;

        /// Cast operator.
        operator cv::Vec3d( ) const;

    /// Cross product
    public:

        C3DRowVector crossProduct ( const C3DRowVector & f_other ) const;

        double       dotProduct ( const C3DRowVector & f_other ) const;

        C3DMatrix    outerProduct ( const C3DRowVector & f_other ) const;

    public:
       /// print values.
        void     print () const;

    protected:
        union
        {
            double      m_elems_p[3];
            struct {    double m_x_d, m_y_d, m_z_d; };
        };
    };

    /// Not operator.
    bool operator ! ( const C3DRowVector & f_vector );


#undef DATA_TYPES_OPERATOR_GENERATOR
#undef INLINE_BODY_DEF_SCALAR_BIN_OPERATOR
#undef INLINE_SCALAR_BIN_OPERATORS_CONCRETE
#undef INLINE_BODY_DEF_SCALAR_UN_OPERATOR
#undef INLINE_SCALAR_UN_OPERATORS_CONCRETE
#undef INLINE_SCALAR_UN_OP_GENERATOR  

 
    /// Make Row vector be the default vector type.
    typedef C3DRowVector C3DVector;
    typedef std::vector<C3DRowVector> C3DPointDataVector;
 };


#include "3DRowVector_inline.h"

#endif // __3DROWVECTOR_H

/* ////////////  Version History ///////////////
 *  $Log: 3DRowVector.h,v $
 *  Revision 1.3  2010/01/18 17:12:29  badino
 *  General updates and added new classes.
 *
 *  Revision 1.2  2009/11/18 15:51:01  badino
 *  badino: documentation added. Some other global changes.
 *
 *//////////////////////////////////////////////
