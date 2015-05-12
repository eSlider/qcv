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
 * \file  3DVectorParam.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****     (C) COPYRIGHT Hernan Badino                                     *****
 ******************************************************************************/

/* INCLUDES */
#include <limits>
#include <stdio.h>
#include "3DVectorParam.h"
#include "3DVectorParamEditor.h"
#include "stringOp.h"

using namespace QCV;

C3DVectorParameter::C3DVectorParameter ( std::string               f_name_str, 
                                         std::string               f_comment_str,
                                         cv::Vec3d                 f_value,
                                         std::string               f_name1_str,
                                         std::string               f_name2_str,
                                         std::string               f_name3_str,
                                         CParameterBaseConnector * f_connector_p )
        : CParameter ( f_name_str, f_comment_str, f_connector_p ),
          m_value (                       f_value )
{
    m_names_p[0] = f_name1_str;
    m_names_p[1] = f_name2_str;
    m_names_p[2] = f_name3_str;

    /// update string value.
    CParameter::updateInitialValue();
    update();
}

C3DVectorParameter::~C3DVectorParameter()
{}

std::string
C3DVectorParameter::getStringFromValue ( ) const
{
    std::string str1 = CStringOp::numToString<double> ( m_value[0],  0, std::numeric_limits<float>::digits );
    std::string str2 = CStringOp::numToString<double> ( m_value[1],  0, std::numeric_limits<float>::digits );
    std::string str3 = CStringOp::numToString<double> ( m_value[2],  0, std::numeric_limits<float>::digits );

    str1 += ",";
    str1 += str2;
    str1 += ",";
    str1 += str3;

    return str1;
}

bool
C3DVectorParameter::setValueFromString ( std::string f_val_str )
{
    int fields_i;
    double x_d, y_d, z_d;

    fields_i = sscanf(f_val_str.c_str(), "%lf,%lf,%lf",
                      &x_d, 
                      &y_d,
                      &z_d );

    if (fields_i != 3)
        return false;

    m_value = cv::Vec3d( x_d, y_d, z_d);

    updateInitialValue();
    return update();
}

cv::Vec3d
C3DVectorParameter::getValue ( ) const
{
    return m_value;
}

bool
C3DVectorParameter::setValue ( cv::Vec3d   f_value,
                               bool        f_shouldUpdate_b )
{
    if ( f_value[0] == m_value[0] &&
         f_value[1] == m_value[1] &&
         f_value[2] == m_value[2] )
        return true;

    m_value = f_value;

    if ( f_shouldUpdate_b )
        return update();
    return true;
}

::QWidget *
C3DVectorParameter::createEditor ( )
{
    return  (m_qtEditor_p = new C3DVectorParamEditor ( this ));
}
