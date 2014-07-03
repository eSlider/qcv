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


#ifndef __STRINGOP_H
#define __STRINGOP_H

/**
 *******************************************************************************
 *
 * @file stringOp.h
 *
 * \class CStringOp
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements a series of static methods for handling strings.
 *
 * Implements a series of static methods for handling strings, such as
 * triming and number to string conversion.
 *
 *
 *******************************************************************************/

/* INCLUDES */
#include <iostream>
#include <ios>
#include <string>
#include <sstream>

namespace QCV
{
    class CStringOp
    {
        /// Constructors / Destructor.
    public:
        CStringOp ( );
        virtual ~CStringOp ( );

    public:
        static bool trimLeft        ( std::string &str );
        static bool trimRight       ( std::string &str );
        static bool trim            ( std::string &str );
        static bool isAlphaNumeric  ( const std::string &str );
        static bool isValidName     ( const std::string &str );

        template <class Type>
        static std::string numToString( const Type&             f_value, 
                                        int                     f_width_i = 0, 
                                        int                     f_precision_i = 0, 
                                        std::ios_base::fmtflags f_flags = (std::ios_base::fmtflags)0, 
                                        char                    f_fillchar_c = 0 );
    };

    typedef CStringOp strOp;

    template <class Type> std::string 
    CStringOp::numToString( const Type&             f_value, 
                            int                     f_width_i, 
                            int                     f_precision_i, 
                            std::ios_base::fmtflags f_flags, 
                            char                    f_fillchar_c )
    {
        std::ostringstream ss;
        if (f_width_i > 0) ss.width( f_width_i );
        if (f_precision_i > 0) ss.precision( f_precision_i );
        if (f_flags != 0) ss.flags( f_flags );
        if (f_fillchar_c != 0) ss.fill( f_fillchar_c );

        ss << f_value;

        return ss.str();
    }
}

#endif // __STRINGOPS_H
