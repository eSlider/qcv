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
 * \file  displayStateParam
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "displayStateParam.h"
#include "stringOp.h"
#include "displayStateParamEditor.h"
#include <stdio.h>

using namespace QCV;

CDisplayStateParameter::CDisplayStateParameter ( std::string               f_name_str, 
                                                 std::string               f_comment_str,
                                                 SDisplayState             f_displayState,
                                                 CParameterBaseConnector * f_connector_p )
        : CParameter ( f_name_str, f_comment_str, f_connector_p ),
          m_displayState (                       f_displayState )
{
    /// update string value.
    CParameter::updateInitialValue();
    update();
}

CDisplayStateParameter::~CDisplayStateParameter()
{}

std::string
CDisplayStateParameter::getStringFromValue ( ) const
{
    char str[32];
    sprintf(str,
            "%i,%i,%i", 
            m_displayState.position.x, 
            m_displayState.position.y, 
            m_displayState.visible_b );

    return std::string(str);
}

bool
CDisplayStateParameter::setValueFromString ( std::string f_val_str )
{
    int fields_i;
    int x_i, y_i, visible_i;

    fields_i = sscanf(f_val_str.c_str(), "%i,%i,%i",
                      &x_i, 
                      &y_i, 
                      &visible_i );

    if (fields_i != 3)
        return false;

    m_displayState.position.x = x_i;    
    m_displayState.position.y = y_i;
    m_displayState.visible_b = visible_i;

    updateInitialValue();
    return update();
}

SDisplayState
CDisplayStateParameter::getValue ( ) const
{
    return m_displayState;
}

bool
CDisplayStateParameter::setValue ( SDisplayState f_displayState,
                                   bool          f_shouldUpdate_b )
{
    if ( f_displayState == m_displayState )
        return true;

    m_displayState = f_displayState;
    
    if ( f_shouldUpdate_b )
        return update();
    return true;
    
}

::QWidget *
CDisplayStateParameter::createEditor ( )
{
    return  (m_qtEditor_p = new CDisplayStateParameterEditor ( this ));
}
