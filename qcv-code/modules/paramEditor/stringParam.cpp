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
* \file  stringParam
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "stringParam.h"
#include "stringOp.h"
#include "stringParamEditor.h"

using namespace QCV;

CStringParameter::CStringParameter ( std::string               f_name_str, 
                                     std::string               f_comment_str,
                                     std::string               f_value_str,
                                     CParameterBaseConnector * f_connector_p )
        : CParameter ( f_name_str, f_comment_str, f_connector_p ),
          m_str (                                   f_value_str )
{
    /// update string value.
    updateInitialValue();    
    update();
}

CStringParameter::~CStringParameter()
{}

std::string
CStringParameter::getStringFromValue ( ) const
{
    return m_str;
}


// Return true if the first word after elimintaing spaces is 
// is true or false or if the string contains a valid number.
// In any other case return false and no value is assigned.

bool
CStringParameter::setValueFromString ( std::string f_val_str )
{
    m_str = f_val_str;
    
    /// Else do not change nothing and return false.
    
    updateInitialValue();    
    return update();
}

std::string CStringParameter::getValue ( ) const
{
    return m_str;
}

bool
CStringParameter::setValue ( std::string f_value_str,
                             bool        f_shouldUpdate_b )
{
    if ( f_value_str == m_str )
        return true;

    m_str = f_value_str;
    
    if ( f_shouldUpdate_b )
        return update();

    return true;
}

::QWidget *
CStringParameter::createEditor ( )
{
    return  (m_qtEditor_p = new CStringParameterEditor ( this ));
}
