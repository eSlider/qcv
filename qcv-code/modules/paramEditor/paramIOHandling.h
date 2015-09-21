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


#ifndef __PARAMIOHANDLING_H
#define __PARAMIOHANDLING_H

/**
 *******************************************************************************
 *
 * @file paramIOHandling.h
 *
 * \class CParamIOHandling
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Abstract class for handling input/output of parameters.
 *
 * Concrete child classes must implement the get and set functions.
 *
 ******************************************************************************/

/* INCLUDES */
#include <string>
#include <locale>

/* CONSTANTS */

namespace QCV
{
    class CParamIOHandling
    {
    /// Destructor.
    public:
        CParamIOHandling () { std::locale::global(std::locale("C")); }
        virtual ~CParamIOHandling () {}
        
    /// Read and save
    public:
        
        /// Load file.
        virtual bool        load ( std::string f_filePath_str ) = 0;

        /// Save file.
        virtual bool        save ( std::string f_filePath_str = "" ) const = 0;

        /// Read parameter.
        virtual bool get ( std::string  f_paramName_str,
                           std::string  f_category_str,
                           std::string &fr_value_str,
                           std::string &fr_comment_str ) const = 0;

        /// Save parameter
        virtual bool set ( std::string  f_paramName_str,
                           std::string  f_category_str,
                           std::string  fr_value_str,
                           std::string  fr_comment_str ) = 0;
        
        /// Read parameter in current category.
        virtual bool get ( std::string  f_paramName_str,
                           std::string &fr_value_str ) const = 0;

        /// Read parameter in current category.
        virtual bool get ( std::string  f_paramName_str,
                           std::string &fr_value_str,
                           std::string &fr_comment_str ) const = 0;
        
        /// Save parameter in current category.
        virtual bool set ( std::string  f_paramName_str,
                           std::string  fr_value_str,
                           std::string  fr_comment_str = std::string("") ) = 0;

        /// Set the current category.
        virtual bool setCurrentCategory ( std::string  f_category_str );

        /// Save the current category.
        virtual std::string getCurrentCategory (  ) const;

    /// Private members.
    private:
        /// Current Category.
        std::string        m_currentCategory_str;
    };
    
    /// Set the current category.
    inline bool 
    CParamIOHandling::setCurrentCategory ( std::string  f_category_str )
    {
        m_currentCategory_str = f_category_str;
        return true;
    }

    /// Save the current category.
    inline std::string
    CParamIOHandling::getCurrentCategory ( ) const
    {
        return m_currentCategory_str;
    }
}

    /// Constructor/Destructor

#endif // __PARAMIOHANDLING_H
