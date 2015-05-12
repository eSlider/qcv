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


#ifndef __PARAMETER_H
#define __PARAMETER_H

/**
 *******************************************************************************
 *
 * @file parameter.h
 *
 * \class CParameter
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Base abstract class for parameters.
 *
 * A parameter is identified with a name and it might have a comment. It also might 
 * have an editor and a connector to set and get functions in a class (through an 
 * object referenced by the type CParameterBaseConnector).
 * Concrete subclasses must implement getStringFromValue and setValueFromString 
 * methods.
 *
 ******************************************************************************/

/* INCLUDES */
#include "baseParamEditorWidget.h"
#include <string>

/* CONSTANTS */

/* PROTOTYPES */

namespace QCV
{
    /* PROTOTYPES */
    class CParameterBaseConnector;
    
    class CParameter
    {
    /// Constructors/Destructor
    public:
        CParameter ( std::string f_name_str    = "", 
                     std::string f_comment_str = "" ,
                     CParameterBaseConnector * f_connector_p = NULL );

        virtual ~CParameter ( );
        
    public:
        /// Get and set name.
        virtual std::string     getName () const;
        virtual bool            setName ( std::string f_name_str );
        
        /// Get and set name.
        virtual std::string     getComment () const;
        virtual bool            setComment ( std::string f_comment_str );

        /// Get and set values from strings.
        virtual std::string     getStringFromValue ( ) const = 0;
        virtual bool            setValueFromString ( std::string f_val_str ) = 0;
 
        CParameterBaseConnector *
                                setConnector ( CParameterBaseConnector * f_connector_p );

        CParameterBaseConnector *
                                getConnector ( ) const;
        
        /// Create editor.
        virtual QWidget *       createEditor ( ) { return ( m_qtEditor_p = 0 ); }

        /// Create editor.
        virtual void            notifyEditorsDeletion ( ) { m_qtEditor_p = 0; }
        
         /// Get editor.
        virtual QWidget *       getEditor ( ) const { return m_qtEditor_p; }
        
        /// Update initial value.
        void                    updateInitialValue();

        /// Update from container.
        virtual bool            updateFromContainer();

        /// Check if the parameter has changed from the initial value.
        virtual bool            hasChanged();

        /// Can/should this parameter be saved?
        virtual bool            shouldBeSaved() { return true; }

        /// Should this parameter have an associated label in the GUI?
        virtual bool            showWithLabel() { return true; }

    /// Protected help methods.
    protected:
        /// Update 
        virtual bool            update();


    /// Protected members
    protected:
        /// Name of the parameter.
        std::string                 m_name_str;

        /// Comment of the parameter.
        std::string                 m_comment_str;

        /// Name of the parameter.
        std::string                 m_initialValue_str;

        /// Corresponding editor.
        CBaseParamEditorWidget *    m_qtEditor_p;

        /// Connector with class get/set methods.
        CParameterBaseConnector *   m_connector_p;
   };
    
    inline std::string
    CParameter::getName () const
    {
        return m_name_str;
    }

    inline bool
    CParameter::setName ( std::string f_name_str )
    {
        m_name_str = f_name_str;
        return true;
    }

    inline std::string
    CParameter::getComment () const
    {
        return m_comment_str;
    }

    inline bool
    CParameter::setComment ( std::string f_comment_str )
    {
        m_comment_str = f_comment_str;
        return true;
    }

    inline void
    CParameter::updateInitialValue()
    {
        m_initialValue_str = getStringFromValue ();
    }
    
    inline bool
    CParameter::hasChanged()
    {
        return  m_initialValue_str != getStringFromValue ();
    }
   
}


#endif // __PARAMETER_H
