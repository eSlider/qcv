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


#ifndef __DOUBLEPARAM_H
#define __DOUBLEPARAM_H

/**
 *******************************************************************************
 *
 * @file doubleParam.h
 *
 * \class CDoubleParameter
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Parameter interface for a double variable.
 *
 * The class implements a parameter interface for a double variable.
 *
 *******************************************************************************/


/* INCLUDES */
#include "parameter.h"

namespace QCV
{

    class CDoubleParameter : public CParameter
    {
        /// Constructors/Destructor
    public:
        
        /// Constructors/Destructor
        CDoubleParameter (   std::string               f_name_str = "", 
                             std::string               f_comment_str = "",
                             double                    f_value_d = 0.0,
                             CParameterBaseConnector * f_connector_p = NULL );

        virtual ~CDoubleParameter ();

    public:
        /// Get and set values from strings.
        virtual std::string     getStringFromValue ( ) const;
        virtual bool            setValueFromString ( std::string f_val_str );
 
        /// Get and set bool value
        virtual double          getValue ( ) const;
        virtual bool            setValue ( double f_value_b,
                                           bool   f_shouldUpdate_b = true );


        /// Get editor.
        virtual QWidget *       createEditor ( );

        /// Protected members
    protected:

        /// Stored value.
        double                  m_value_d;
    };
}


#endif // __BOOLPARAM_H
