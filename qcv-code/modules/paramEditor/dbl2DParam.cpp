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
 * \file  dbl2DParam
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "dbl2DParam.h"
#include "dbl2DParamEditor.h"
#include "stringOp.h"
#include <stdio.h>

using namespace QCV;

CDbl2DParameter::CDbl2DParameter ( std::string               f_name_str, 
                                   std::string               f_comment_str,
                                   S2D<double>               f_value,
                                   std::string               f_name1_str,
                                   std::string               f_name2_str,
                                   CParameterBaseConnector * f_connector_p )
        : CParameter ( f_name_str, f_comment_str, f_connector_p ),
          m_value (                       f_value )
{
    m_names_p[0] = f_name1_str;
    m_names_p[1] = f_name2_str;

    /// update string value.
    CParameter::updateInitialValue();
    update();
}

CDbl2DParameter::~CDbl2DParameter()
{}

std::string
CDbl2DParameter::getStringFromValue ( ) const
{
    std::string str  = CStringOp::numToString<double> ( m_value.x,  0, std::numeric_limits<float>::digits );
    std::string str2 = CStringOp::numToString<double> ( m_value.y,  0, std::numeric_limits<float>::digits );

    str += ",";
    str += str2;

    return str;
}

bool
CDbl2DParameter::setValueFromString ( std::string f_val_str )
{
    int fields_i;
    double x_d, y_d;

    fields_i = sscanf(f_val_str.c_str(), "%lf,%lf",
                      &x_d, 
                      &y_d );

    if (fields_i != 2)
        return false;

    m_value.x = x_d;
    m_value.y = y_d;

    updateInitialValue();
    return update();
}

S2D<double>
CDbl2DParameter::getValue ( ) const
{
    return m_value;
}

bool
CDbl2DParameter::setValue ( S2D<double> f_value,
                            bool        f_shouldUpdate_b )
{
    if ( f_value == m_value )
        return true;

    m_value = f_value;

    if ( f_shouldUpdate_b )
        return update();
    return true;
}

::QWidget *
CDbl2DParameter::createEditor ( )
{
    return  (m_qtEditor_p = new CDbl2DParameterEditor ( this ));
}
