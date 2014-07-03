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


#ifndef __STRINGPARAM_H
#define __STRINGPARAM_H

/**
 *******************************************************************************
 *
 * @file stringParam.h
 *
 * \class CStringParameter
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Parameter interface for a std::string variable.
 *
 * The class implements a parameter interface for a std::string variable.
 *
 *******************************************************************************/

/* INCLUDES */
#include "parameter.h"

namespace QCV
{
    /* PROTOTYPES */

    class CStringParameter : public CParameter
    {
    public:
        
        /// Constructors/Destructor
        CStringParameter (  std::string               f_name_str = "",
                            std::string               f_comment_str = "",
                            std::string               f_value_str = "",
                            CParameterBaseConnector * f_connector_p = NULL );

        virtual ~CStringParameter ();

    public:
        /// Get and set values from strings.
        virtual std::string     getStringFromValue ( ) const;
        virtual bool            setValueFromString ( std::string f_val_str );
 
        /// Get and set string value
        virtual std::string     getValue ( ) const;
        virtual bool            setValue ( std::string f_value_str,
                                           bool        f_shouldUpdate_b = true );


        /// Get editor.
        virtual QWidget *       createEditor ( );

    /// Protected members
    protected:

        /// Stored value.
        std::string             m_str;
    };
}


#endif // __STRINGPARAM_H
