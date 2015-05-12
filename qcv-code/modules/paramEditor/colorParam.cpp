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
 * \file  colorParam
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "colorParam.h"
#include "stringOp.h"
#include "colorParamEditor.h"

using namespace QCV;

CColorParameter::CColorParameter ( std::string               f_name_str, 
                                   std::string               f_comment_str,
                                   SRgba                     f_color,
                                   bool                      f_useAlpha_b,
                                   CParameterBaseConnector * f_connector_p )
        : CParameter ( f_name_str, f_comment_str, f_connector_p ),
          m_color (                                     f_color ),
          m_useAlpha_b (                           f_useAlpha_b )
{
    /// update string value.
    CParameter::updateInitialValue();
    update();
}

CColorParameter::~CColorParameter()
{}

std::string
CColorParameter::getStringFromValue ( ) const
{
    char str[32];
    sprintf(str,"%i,%i,%i,%i", m_color.r, m_color.g, m_color.b, m_color.a );
    return std::string(str);
}


// Return true if the first word after elimintaing spaces is 
// is true or false or if the string contains a valid number.
// In any other case return false and no value is assigned.

bool
CColorParameter::setValueFromString ( std::string f_val_str )
{
    int fields_i;
    unsigned int r,g,b,a = 255;
    
    if  (m_useAlpha_b )
    {
        fields_i = sscanf(f_val_str.c_str(), "%u,%u,%u,%u", 
                          &r, &g, &b, &a);
        if (fields_i != 4)
            return false;
    }
    else
    {
        fields_i = sscanf(f_val_str.c_str(), "%u,%u,%u", 
                          &r, &g, &b);
        if (fields_i != 3)
            return false;
    }

    m_color.r = r;
    m_color.g = g;
    m_color.b = b;
    m_color.a = a;

    updateInitialValue();
    return update();
}

SRgba CColorParameter::getValue ( )
{
    return m_color;
}

bool
CColorParameter::setValue ( SRgba f_color,
                            bool  f_shouldUpdate_b )
{
    if ( f_color == m_color )
        return true;

    m_color = f_color;
 
    if ( f_shouldUpdate_b )
        return update();
    return true;
}

::QWidget *
CColorParameter::createEditor ( )
{
    return  (m_qtEditor_p = new CColorParameterEditor ( this ));
}
