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


#ifndef __PARAMMACROS_H
#define __PARAMMACROS_H

/**
 *******************************************************************************
 *
 * @file paramMacros.h
 *
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Macros used for adding parameters to operators.
 *
 ******************************************************************************/

/* INCLUDES */
#include "parameterSet.h"
#include "paramBaseConnector.h"
#include "boolParam.h"
#include "doubleParam.h"
#include "intParam.h"
#include "uintParam.h"
#include "floatParam.h"
#include "stringParam.h"
#include "filePathParam.h"
#include "dirPathParam.h"
#include "colorParam.h"
#include "enumParam.h"
#include "paramLineSeparator.h"
#include "paramGroup.h"
#include "paramGroupEnd.h"
#include "displayStateParam.h"
#include "int2DParam.h"
#include "dbl2DParam.h"
#include "flt2DParam.h"
#include "uint2DParam.h"
#include "3DVectorParam.h"
#include "buttonParam.h"

/* CONSTANTS */


/* TO BE USED MAINLY IN CLASSES DEFINITIONS */

#define ADD_SETTER(ptype, param, suffix) \
bool set##suffix( ptype newvalue )       \
{                                        \
    param = newvalue;                    \
    return true;                         \
}

#define ADD_SETTER_NOTIFIER(ptype, param, suffix, notifier) \
bool set##suffix( ptype newvalue )                          \
{                                                           \
    param = newvalue;                                       \
    notifier();                                             \
    return true;                                            \
}

#define ADD_SETTER_BOUNDED(ptype, param, suffix, min, max ) \
bool set##suffix( ptype newvalue )                          \
{                                                           \
    if ( newvalue < min || newvalue > max ) return false;         \
    param = newvalue;                                       \
    return true;                                            \
}

#define ADD_SETTER_BOUNDED_NOTIFIER(ptype, param, suffix, min, max, notifier) \
bool set##suffix( ptype newvalue )                          \
{                                                           \
    if ( newvalue < min || newvalue > max ) return false;         \
    param = newvalue;                                       \
    notifier();                                             \
    return true;                                            \
}

#define ADD_GETTER(ptype, param, suffix) \
ptype get##suffix( ) const               \
{                                        \
    return param;                        \
}

#define ADD_PARAM_ACCESS(ptype, param, suffix) \
ADD_SETTER(ptype, param, suffix)               \
ADD_GETTER(ptype, param, suffix)

#define ADD_PARAM_ACCESS_BOUNDED(ptype, param, suffix, min, max)    \
ADD_SETTER_BOUNDED(ptype, param, suffix, min, max)                  \
ADD_GETTER(ptype, param, suffix)

#define ADD_PARAM_ACCESS_BOUNDED_NOTIFIER(ptype, param, suffix, min, max, notifier ) \
    ADD_SETTER_BOUNDED_NOTIFIER(ptype, param, suffix, min, max, notifier )           \
ADD_GETTER(ptype, param, suffix)

#define ADD_PARAM_ACCESS_NOTIFIER(ptype, param, suffix, notifier) \
ADD_SETTER_NOTIFIER(ptype, param, suffix, notifier)               \
ADD_GETTER(ptype, param, suffix)

/*****************************************************/
/*****************************************************/
/*****************************************************/

/* TO BE USED MAINLY IN CONSTRUCTORS OF CLASSES DERIVED FROM COperator */

#define ADD_BOOL_PARAMETER(name, comment, defaultVal, obj, suffix, classT)                  \
    m_paramSet_p -> addParameter (                                                         \
            new CBoolParameter ( name, comment, defaultVal,                            \
                                 new CParameterConnector< classT, bool, CBoolParameter>    \
                                 ( obj,                                                   \
                                   &classT::get##suffix,                                   \
                                   &classT::set##suffix ) ) )

#define ADD_DOUBLE_PARAMETER(name, comment, defaultVal, obj, suffix, classT)                   \
    m_paramSet_p -> addParameter (                                                            \
            new CDoubleParameter ( name, comment, defaultVal,                             \
                                   new CParameterConnector< classT, double, CDoubleParameter> \
                                   ( obj,                                                    \
                                     &classT::get##suffix,                                    \
                                     &classT::set##suffix ) ) )

#define ADD_INT_PARAMETER(name, comment, defaultVal, obj, suffix, classT)             \
    m_paramSet_p -> addParameter (                                                   \
            new CIntParameter ( name, comment, defaultVal,                       \
                                new CParameterConnector< classT, int, CIntParameter> \
                                ( obj,                                              \
                                  &classT::get##suffix,                              \
                                  &classT::set##suffix ) ) )

#define ADD_UINT_PARAMETER(name, comment, defaultVal, obj, suffix, classT)             \
    m_paramSet_p -> addParameter (                                      \
            new CUIntParameter ( name, comment, defaultVal,             \
                                 new CParameterConnector< classT, unsigned int, CUIntParameter> \
                                 ( obj,                                 \
                                   &classT::get##suffix,                \
                                   &classT::set##suffix ) ) )

#define ADD_FLOAT_PARAMETER(name, comment, defaultVal, obj, suffix, classT)                   \
    m_paramSet_p -> addParameter (                                                           \
            new CFloatParameter ( name, comment, defaultVal,                             \
                                   new CParameterConnector< classT, float, CFloatParameter>  \
                                   ( obj,                                                   \
                                     &classT::get##suffix,                                   \
                                     &classT::set##suffix ) ) )

#define ADD_STR_PARAMETER(name, comment, defaultVal, obj, suffix, classT)                           \
    m_paramSet_p -> addParameter (                                                                 \
            new CStringParameter ( name, comment, defaultVal,                                  \
                                   new CParameterConnector< classT, std::string, CStringParameter> \
                                   ( obj,                                                         \
                                     &classT::get##suffix,                                         \
                                     &classT::set##suffix ) ) )

#define ADD_FPATH_PARAMETER(name, comment, defaultVal, obj, suffix, classT)                           \
    m_paramSet_p -> addParameter (                                      \
            new CFilePathParameter ( name, comment, defaultVal,     \
                                     new CParameterConnector< classT, std::string, CFilePathParameter> \
                                     ( obj,                            \
                                       &classT::get##suffix,            \
                                       &classT::set##suffix ) ) )

#define ADD_DPATH_PARAMETER(name, comment, defaultVal, obj, suffix, classT)                           \
    m_paramSet_p -> addParameter (                                      \
            new CDirPathParameter ( name, comment, defaultVal,     \
                                    new CParameterConnector< classT, std::string, CDirPathParameter> \
                                    ( obj,                             \
                                      &classT::get##suffix,             \
                                      &classT::set##suffix ) ) )

#define ADD_RGB_PARAMETER(name, comment, defaultVal, obj, suffix, classT)                           \
    m_paramSet_p -> addParameter (                                                         \
            new CColorParameter ( name, comment, defaultVal, false,                    \
                                  new CParameterConnector< classT, SRgb, CColorParameter>  \
                                  ( obj,                                                  \
                                    &classT::get##suffix,                                  \
                                    &classT::set##suffix ) ) )

#define ADD_RGBA_PARAMETER(name, comment, defaultVal, obj, suffix, classT)                  \
    m_paramSet_p -> addParameter (                                                         \
            new CColorParameter ( name, comment, defaultVal, true,                     \
                                  new CParameterConnector< classT, SRgba, CColorParameter> \
                                  ( obj,                                                  \
                                    &classT::get##suffix,                                  \
                                    &classT::set##suffix ) ) )

#define ADD_ENUM_PARAMETER(name, comment, EnumType_, defaultVal, obj, suffix, classT) \
    m_paramSet_p -> addParameter ( \
            new CEnumParameter<EnumType_> ( name, comment, defaultVal, "None", \
                                            new CParameterConnector< classT, EnumType_, CEnumParameter<EnumType_> > \
                                            ( obj,  \
                                              &classT::get##suffix, \
                                              &classT::set##suffix ) ) )

#define ADD_DISPSTATE_PARAMETER(name, comment, defaultVal, obj, suffix, classT) \
    m_paramSet_p -> addParameter ( \
            new CDisplayStateParameter ( name, comment, defaultVal, \
                                         new CParameterConnector< classT, SDisplayState, CDisplayStateParameter > \
                                         ( obj, \
                                           &classT::get##suffix, \
                                           &classT::set##suffix ) ) )

#define ADD_INT2D_PARAMETER(name, comment, defaultVal, name1, name2, obj, suffix, classT) \
    m_paramSet_p -> addParameter (                                      \
            new CInt2DParameter ( name, comment, defaultVal, name1, name2, \
                                  new CParameterConnector< classT, S2D<int>, CInt2DParameter> \
                                  ( obj,                                \
                                    &classT::get##suffix,               \
                                    &classT::set##suffix ) ) )

#define ADD_DBL2D_PARAMETER(name, comment, defaultVal, name1, name2, obj, suffix, classT) \
    m_paramSet_p -> addParameter (                                      \
            new CDbl2DParameter ( name, comment, defaultVal, name1, name2, \
                                  new CParameterConnector< classT, S2D<double>, CDbl2DParameter> \
                                  ( obj,                                \
                                    &classT::get##suffix,               \
                                    &classT::set##suffix ) ) )

#define ADD_FLT2D_PARAMETER(name, comment, defaultVal, name1, name2, obj, suffix, classT) \
    m_paramSet_p -> addParameter (                                      \
            new CFlt2DParameter ( name, comment, defaultVal, name1, name2, \
                                  new CParameterConnector< classT, S2D<float>, CFlt2DParameter> \
                                  ( obj,                                \
                                    &classT::get##suffix,               \
                                    &classT::set##suffix ) ) )

#define ADD_UINT2D_PARAMETER(name, comment, defaultVal, name1, name2, obj, suffix, classT) \
    m_paramSet_p -> addParameter (                                      \
            new CUInt2DParameter ( name, comment, defaultVal, name1, name2, \
                                  new CParameterConnector< classT, S2D<unsigned int>, CUInt2DParameter> \
                                  ( obj,                                \
                                    &classT::get##suffix,               \
                                    &classT::set##suffix ) ) )

#define ADD_3DVEC_PARAMETER(name, comment, defaultVal, name1, name2, name3, obj, suffix, classT) \
    m_paramSet_p -> addParameter (                                      \
            new C3DVectorParameter ( name, comment, defaultVal, name1, name2, name3, \
                                  new CParameterConnector< classT, C3DVector, C3DVectorParameter> \
                                  ( obj,                                \
                                    &classT::get##suffix,               \
                                    &classT::set##suffix ) ) )

#define ADD_BUTTON_PARAMETER( name, comment, obj, method, classT )                         \
    m_paramSet_p -> addParameter (                                                         \
            new CButtonParameter ( name, comment,                                          \
                                   new CParamButtonConnector < classT >                    \
                                 ( obj,                                                    \
                                   method ) ) )

/*****************************************************/
/*****************************************************/
/*****************************************************/

#define ADD_LINE_SEPARATOR                                       \
    m_paramSet_p -> addParameter ( new CParamLineSeparator () )

#define BEGIN_PARAMETER_GROUP(name, usecolor, color)                               \
    m_paramSet_p -> addParameter ( new CParameterGroup ( name, usecolor, color ) )

#define END_PARAMETER_GROUP                                      \
    m_paramSet_p -> addParameter ( new CParameterGroupEnd ( ) )

#endif // __PARAMMACROS_H

