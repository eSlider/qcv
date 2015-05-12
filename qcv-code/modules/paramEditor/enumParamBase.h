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


#ifndef __ENUMPARAMBASE_H
#define __ENUMPARAMBASE_H

/**
 *******************************************************************************
 *
 * @file enumParamBase.h
 *
 * \class CEnumParameterBase
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Base class for a parameter interface for a enum variable.
 *
 * The class implements a parameter interface for an enum variable.
 *
 *******************************************************************************/

/* INCLUDES */

/* INCLUDES */
#include "parameter.h"
#include <map>

namespace QCV
{
    class CEnumParameterBase : public CParameter
    {
        friend class CEnumParameterEditor;
    /// Constructors/Destructor
    public:
        
        /// Constructors/Destructor
        CEnumParameterBase (  std::string               f_name_str    = "",
                              std::string               f_comment_str = "",
                              CParameterBaseConnector * f_connector_p = NULL );
                  

        virtual ~CEnumParameterBase ();

    public:
        /// Get and set values from strings.
        virtual std::string     getStringFromValue ( ) const;
                
        virtual bool            setValueFromString ( std::string f_val_str );        
 
        /// Get editor.
        virtual QWidget *       createEditor ( );


    /// Protected members.
    protected:
        
        /// Get number of descriptions.
        virtual int             getCount ( ) const = 0;

        /// Get description/value pair.
        virtual bool            getPair ( unsigned int  f_index_i,
                                          int         & fr_value_i,
                                          std::string & fr_descr_str ) = 0;

        /// Get and set int value
        /// Getting undefined reference if defining this funciton as virtual pure so
        /// an implementation was added.
        virtual int             getIntValue ( ) const {return 0;}; 
        

        virtual bool            setValueFromInt ( unsigned int f_newVal_i ) = 0;

        /// Set value from order in the map list.
        virtual bool            setValueFromIdx ( unsigned int f_idx_ui ) = 0;

    /// Protected members.
    protected:

    /// Protected members
    protected:
    };
}

#endif // __ENUMPARAMBASE_H
