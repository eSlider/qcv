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


#ifndef __ENUMPARAM_H
#define __ENUMPARAM_H

/**
 *******************************************************************************
 *
 * @file enumParam.h
 *
 * \class CEnumParameter
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Templated class for a parameter interface for a enum variable.
 *
 * The template type must be that of the desired enum type.
 *
 *******************************************************************************/

/* INCLUDES */
#include "enumParamBase.h"

namespace QCV
{
    template < class _EnumType >
    class CEnumParameter : public CEnumParameterBase
    {
    /// Constructors/Destructor
    public:
        
        /// Constructors/Destructor
        CEnumParameter (  std::string               f_name_str    = "",
                          std::string               f_comment_str = "",
                          _EnumType                 f_value_e     = (_EnumType) 0,
                          std::string               f_descr_srt   = "No description",
                          CParameterBaseConnector * f_connector_p = NULL );

        virtual ~CEnumParameter ();

    /// Derived from parent
    public:
        /// Get and set values from strings.
        virtual std::string     getStringFromValue ( ) const;
        virtual bool            setValueFromString ( std::string f_val_str ); 

        /// Get editor.
        virtual QWidget *       createEditor ( );

    public:
        /// Get and set bool value
        virtual _EnumType       getValue ( );
        virtual bool            setValue ( _EnumType f_newVal_i,
                                           bool      f_shouldUpdate_b = true );

        /// Add description of a value.
        virtual void            addDescription ( _EnumType   f_value_e,
                                                 std::string f_descr_srt );

        /// Remove an item.
        virtual void            removeDescription ( _EnumType   f_value_e  );
       
        /// Update widget contents
        virtual void            updateWidgetContent();
        
        /// Update widget contents
        virtual std::string     getDescription( _EnumType f_value_e );
        
    /// Protected members derived from parent.
    protected:
        
        /// Get number of descriptions.
        virtual int             getCount ( ) const;

        /// Get description/value pair.
        virtual bool            getPair ( unsigned int  f_index_i,
                                          int         & fr_value_i,
                                          std::string & fr_descr_str );

        /// Get and set int value
        virtual int             getIntValue ( ) const;

        virtual bool            setValueFromInt ( unsigned int f_newVal_i );

        /// Set value from order in the map list.
        virtual bool            setValueFromIdx ( unsigned int f_idx_ui );

    /// Protected data types.
    protected:

        typedef std::map<int, std::string> CMapper_t;

    /// Protected members
    protected:
        /// Value.
        _EnumType                      m_value_e;

        /// Stored value.
        CMapper_t                      m_map;
    };
}

#include "enumParam_inline.h"

#endif // __ENUMPARAM_H
