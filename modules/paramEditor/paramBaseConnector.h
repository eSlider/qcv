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


#ifndef __PARAMBASECONNECTOR_H
#define __PARAMBASECONNECTOR_H

/**
 *******************************************************************************
 * @file paramBaseConnector.h
 *
 * \class CParameterBaseConnector
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Abstract base class for connecting parameters with variables.
 *
 * Abstract base class for connecting parameters with variables. The children
 * concrete classes must implement the methods updateContainer and 
 * updateParameterObj.
 *
 *******************************************************************************/

/* INCLUDES */
#include "parameter.h"

/* CONSTANTS */
namespace QCV
{
    class CParameterBaseConnector
    {
    /// Constructors / Destructor
    public:
        
        CParameterBaseConnector (): m_parameter_p (0) {}
        virtual ~CParameterBaseConnector () {}

    /// Virtual public functions
    public:
        virtual bool updateContainer() = 0;
        
        /// Update the parameter object calling the setValue method
        /// of the parameter.
        virtual bool updateParameterObj() = 0;

        virtual bool setParameterObject ( CParameter * f_param_p )
        {
            m_parameter_p = f_param_p; return true;
        }
   /// Protected members
    protected:
        /// Pointer to the parameter type class.
        CParameter *          m_parameter_p;
    };
    
/**
 *******************************************************************************
 *
 * \class CParameterConnector
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Template concrete class for connecting a parameter with a variable in a 
 * container class.
 *
 * The template types to specified are the container class of the parameter, the 
 * parameter type and its corresponding parameter class type (derived from 
 * CParameter). When instantiating an object a pointer to the container object
 * and pointers to the set and get methods of the class must be specified.
 *
 *******************************************************************************/

    template < class _ParamContainerClass, 
               class _ParamType,
               class _ParamTypeClass >
    class CParameterConnector: public CParameterBaseConnector
    {
    /// Public data types
    public:
        typedef bool       (_ParamContainerClass::*SetMethod_t) ( _ParamType );
        typedef _ParamType (_ParamContainerClass::*GetMethod_t) ( ) const;

        CParameterConnector (_ParamContainerClass *  f_container_p,
                             GetMethod_t             f_getMethod_p,
                             SetMethod_t             f_setMethod_p ) 
                : CParameterBaseConnector (             ),
                  m_paramContainerObj_p(  f_container_p ),
                  m_getMethod_p (         f_getMethod_p ),
                  m_setMethod_p (         f_setMethod_p )
        { 
        }

        virtual ~CParameterConnector () {}
        
        
    /// Virtual public functions
    public:
        /// Update calling the set method of the container.
        virtual bool updateContainer();

        /// Update the parameter object calling the setValue method
        /// of the parameter.
        virtual bool updateParameterObj();

    /// Protected members
    protected:

        /// Pointer to container object.
        _ParamContainerClass *     m_paramContainerObj_p;
 
        /// Get method in param container class.
        GetMethod_t                m_getMethod_p;

        /// Set method in param container class.
        SetMethod_t                m_setMethod_p;        
    };
    
    template < class _ParamContainerClass, 
               class _ParamType,
               class _ParamTypeClass >
    inline
    bool CParameterConnector< _ParamContainerClass, 
                              _ParamType,
                              _ParamTypeClass >::updateContainer() 
    {
        _ParamTypeClass * ptr = static_cast<_ParamTypeClass *>(m_parameter_p);

        if (!ptr) return false;
        
        const _ParamType param1 = ptr -> getValue();
        const _ParamType param2 = (m_paramContainerObj_p->*m_getMethod_p)( );

        if ( !(param1 == param2) )
        {
            if ( not (m_paramContainerObj_p->*m_setMethod_p)( param1 ) )
                return false;
        }
        return true;
    }

    template < class _ParamContainerClass, 
               class _ParamType,
               class _ParamTypeClass >
    inline
    bool CParameterConnector< _ParamContainerClass,
                              _ParamType,
                              _ParamTypeClass >::updateParameterObj()
    {
        _ParamTypeClass * ptr = static_cast<_ParamTypeClass *>(m_parameter_p);

        if (!ptr)
            return false;

        const _ParamType param1 = ptr -> getValue();
        const _ParamType param2 = (m_paramContainerObj_p->*m_getMethod_p)( );

        if ( !(param1 == param2) )
        {
            //return ptr->updateFromContainer();
            
            if ( not (ptr -> setValue ( param2, false ) ) )
                return false;
        }
        return true;
    }    


/**
 *******************************************************************************
 *
 * \class CParamButtonConnector
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Template concrete class for connecting a parameter with a method in a 
 * container class.
 *
 * The template types to specified are the container class of the method, and 
 * its corresponding parameter class type CButtonParameter (derived from 
 * CParameter). When instantiating an object a pointer to the container object
 * and pointers to the method to be called must be specified.
 *
 *******************************************************************************/
    class CButtonParameter;
    

    template < class _ParamContainerClass  >
    class CParamButtonConnector: public CParameterBaseConnector
    {
    /// Public data types
    public:
        typedef void       (_ParamContainerClass::*CallMethod_t) ( );

        CParamButtonConnector (_ParamContainerClass *  f_container_p,
                               CallMethod_t            f_callMethod_p ) 
                : CParameterBaseConnector (             ),
                  m_paramContainerObj_p(  f_container_p ),
                  m_callMethod_p (        f_callMethod_p )
        { 
        }

        virtual ~CParamButtonConnector () {}

    /// Virtual public functions
    public:
        /// Update calling the set method of the container.
        virtual bool updateContainer();        

        /// Update the parameter object calling the setValue method
        /// of the parameter.
        virtual bool updateParameterObj() { return true; }            

    /// Protected members
    protected:

        /// Pointer to container object.
        _ParamContainerClass *     m_paramContainerObj_p;

        /// Get method in param container class.
        CallMethod_t               m_callMethod_p;
    };

    template < class _ParamContainerClass > 
    inline
    bool CParamButtonConnector< _ParamContainerClass >::updateContainer() 
    {
        CButtonParameter * ptr = static_cast<CButtonParameter *>(m_parameter_p);

        if (!ptr) return false;

        (m_paramContainerObj_p->*m_callMethod_p)();

        return true;
    }
}

#endif // __PARAMBASECONNECTOR_H
