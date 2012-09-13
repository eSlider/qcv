/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 3. See the GNU LGPL version 3 for details.
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

#ifndef __CIOBJ_H
#define __CIOBJ_H

/**
 *******************************************************************************
 *
 * @file ioObj.h
 *
 * \class CIOObj
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief I/O base class for operator's I/O registration.
 *
 *******************************************************************************/

/* INCLUDES */

/* CONSTANTS */
#include <string>

//// SOME USEFUL MACROS
#define showval(a) { std::cout << #a " = " << a << "\n"; }

/* CLASS DEFINITION */

namespace QCV
{
    class CIOObj
    {
    /// Constructor, Desctructors
    public:
        /// Constructors.
        CIOObj() { }

        /// Virtual destructor.
        virtual ~CIOObj() { };

        virtual const CIOObj& copyFrom ( const CIOObj &f_other )
        {
            *this = f_other; 
            return *this;
        }

    private:
    };

/**
 *******************************************************************************
 *
 * \class CIONumeric
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Numeric I/O classes for operator's I/O registration.
 *
 *******************************************************************************/

    template< class _NumType >
    class CIONumeric: public CIOObj
    {

    public:
        CIONumeric (_NumType iniVal = 0 ) { m_val = iniVal; }
        virtual ~CIONumeric () { }

#define CIONUMERIC_ADD_OPERATORS(_Type)                                 \
                                                                        \
        CIONumeric<_NumType>& operator = ( _Type val )                  \
        {                                                               \
            m_val = (_NumType) val;                                     \
            return *this;                                               \
        };                                                              \
                                                                        \
        CIONumeric<_NumType>& operator = (                              \
                                      const CIONumeric<_Type>& other )  \
        {                                                               \
            m_val = (_NumType) other;                                   \
            return *this;                                               \
        };                                                              \
                                                                        \
        operator _Type () const                                         \
        {                                                               \
            return (_Type) (m_val);                                     \
        };

        CIONUMERIC_ADD_OPERATORS ( bool )
        CIONUMERIC_ADD_OPERATORS ( char )
        CIONUMERIC_ADD_OPERATORS ( unsigned char )
        CIONUMERIC_ADD_OPERATORS ( short int )
        CIONUMERIC_ADD_OPERATORS ( unsigned short int )
        CIONUMERIC_ADD_OPERATORS ( int )
        CIONUMERIC_ADD_OPERATORS ( unsigned int )
        CIONUMERIC_ADD_OPERATORS ( long int )
        CIONUMERIC_ADD_OPERATORS ( float )
        CIONUMERIC_ADD_OPERATORS ( double )
        CIONUMERIC_ADD_OPERATORS ( long double )

#undef CIONUMERIC_ADD_OPERATORS

        /// Private members
        private:

        /// _NumType member.
        _NumType m_val;
    };

    typedef CIONumeric<bool>                CIO_bool;
    typedef CIONumeric<int>                 CIO_int;
    typedef CIONumeric<unsigned int>        CIO_uint;
    typedef CIONumeric<int>                 CIO_i32;
    typedef CIONumeric<unsigned int>        CIO_ui32;
    typedef CIONumeric<short int>           CIO_i16;
    typedef CIONumeric<unsigned short int>  CIO_ui16;
    typedef CIONumeric<char>                CIO_i8;
    typedef CIONumeric<char>                CIO_char;
    typedef CIONumeric<unsigned char>       CIO_ui8;
    typedef CIONumeric<unsigned char>       CIO_uchar;
    typedef CIONumeric<float>               CIO_f32;
    typedef CIONumeric<float>               CIO_float;
    typedef CIONumeric<double>              CIO_f64;
    typedef CIONumeric<double>              CIO_double;
    typedef CIONumeric<long double>         CIO_f80;

    class CIOString: public CIOObj
    {

    public:
        CIOString ( std::string f_iniVal_str = "" ) { m_str = f_iniVal_str; }
        virtual ~CIOString () { }

        CIOString& operator = ( std::string f_str )
        {
            m_str = f_str;
            return *this;
        };

        CIOString& operator = ( const CIOString& other )
        {
            m_str = other.m_str;
            return *this;
        };

        operator std::string () const
        {
            return m_str;
        }


    private:
        std::string m_str;
    };
    
} // namespace

#endif // __CIOBJ_H

