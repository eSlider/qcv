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


/*@@@**************************************************************************
 * \file  enumParam
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include <QtGui/QWidget>
#include "enumParam.h"
#include "stringOp.h"
#include "enumParamEditor.h"

#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <limits>

using namespace QCV;

template < class _EnumType >
CEnumParameter<_EnumType>::CEnumParameter ( std::string               f_name_str, 
                                            std::string               f_comment_str,
                                            _EnumType                 f_value_e,
                                            std::string               f_descr_str,
                                            CParameterBaseConnector * f_connector_p )
        : CEnumParameterBase ( f_name_str, f_comment_str, f_connector_p ),
          m_value_e (                                 f_value_e )
{
    m_map[(int)f_value_e] = f_descr_str;
    
    /// update.
    CParameter::updateInitialValue();

    update();
}

template < class _EnumType >
CEnumParameter<_EnumType>::~CEnumParameter()
{
}

template < class _EnumType >
std::string
CEnumParameter<_EnumType>::getStringFromValue ( ) const
{
    return CStringOp::numToString<int> ( ((int)m_value_e) );
}

template < class _EnumType >
bool
CEnumParameter<_EnumType>::setValueFromString ( std::string f_val_str )
{
    if (f_val_str.length() == 0)
        return false;
    
    CStringOp::trimLeft ( f_val_str );

    const char* begin_p = f_val_str.c_str();
    char* end_p   = NULL;
    errno = 0;
    double val_d = strtod ( begin_p, &end_p );
    
    if ( end_p != begin_p && 
         std::isfinite(val_d) &&
         !errno )
    {
        m_value_e = (_EnumType)val_d;
        CParameter::updateInitialValue();
        return update();
    }
    
    /// Else do not change nothing and return false.
    return false;
}

template < class _EnumType >
_EnumType CEnumParameter<_EnumType>::getValue ( )
{
    return m_value_e;
}

template < class _EnumType >
bool CEnumParameter<_EnumType>::setValue ( _EnumType f_value_e,
                                           bool      f_shouldUpdate_b )
{
    if ( f_value_e == m_value_e )
        return true;

    m_value_e = f_value_e;

    if ( getEditor() )
        ((::QWidget*)getEditor()) -> update();

    if (f_shouldUpdate_b )
        return update();
    return true;
}

template < class _EnumType >
void CEnumParameter<_EnumType>::addDescription ( _EnumType   f_value_e,
                                                 std::string f_descr_str )
{
    m_map[(int)f_value_e] = f_descr_str;
}

template < class _EnumType >
void CEnumParameter<_EnumType>::removeDescription ( _EnumType   f_value_e  )
{
    CMapper_t::iterator it=m_map.find((int)f_value_e);

    if (it != m_map.end())
    {    
        m_map.erase(it);
    }
}

template < class _EnumType >
::QWidget *
CEnumParameter<_EnumType>::createEditor ( )
{
    return  (m_qtEditor_p = new CEnumParameterEditor ( this ));
}


/// Get number of descriptions.
template < class _EnumType >
int
CEnumParameter<_EnumType>::getCount ( ) const
{
    return m_map.size();
}


/// Get description/value pair.
template < class _EnumType >
bool
CEnumParameter<_EnumType>::getPair ( unsigned int  f_index_ui,
                                     int         & fr_value_i,
                                     std::string & fr_descr_str )
{
    if ( f_index_ui >= m_map.size() )
        return false;
    
    std::map< int, std::string >::const_iterator it = m_map.begin();
    
    //it=it+f_index_ui;
    
    for (unsigned int i = 0; i < f_index_ui; ++i) ++it;
    
    fr_descr_str = it->second;
    fr_value_i   = (int)(it->first);

    return true;
}


/// Get and set int value
template < class _EnumType >
int
CEnumParameter<_EnumType>::getIntValue ( ) const
{
    return (int)(m_value_e);
}
        
template < class _EnumType >
bool
CEnumParameter<_EnumType>::setValueFromInt ( unsigned int f_newVal_ui )
{
    m_value_e = (_EnumType) ( f_newVal_ui );
    return update();
}

template < class _EnumType >
bool
CEnumParameter<_EnumType>::setValueFromIdx ( unsigned int f_index_ui )
{
    if ( f_index_ui >= m_map.size() )
        return false;

    std::map< int, std::string >::const_iterator it = m_map.begin();
    
    for (unsigned int i = 0; i < f_index_ui; ++i) ++it;
    
    m_value_e   = (_EnumType)(it->first);

    return update();
}

template < class _EnumType >
void
CEnumParameter<_EnumType>::updateWidgetContent()
{
    ((CEnumParameterEditor*) m_qtEditor_p)->updateContent();    
}


template < class _EnumType >
std::string
CEnumParameter<_EnumType>::getDescription( _EnumType f_value_e )
{
    CMapper_t::iterator it=m_map.find((int)f_value_e);

    if (it != m_map.end())
    {    
        return it->second;
    }
    return "";
}
