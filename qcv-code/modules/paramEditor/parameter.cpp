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
* \file  parameter
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <QtGui/QWidget>
#include "parameter.h"
#include "paramBaseConnector.h"

using namespace QCV;

CParameter::CParameter ( std::string f_name_str, 
                         std::string f_comment_str,
                         CParameterBaseConnector * f_connector_p )
  : m_name_str  (             f_name_str ),
    m_comment_str  (       f_comment_str ),
    m_qtEditor_p (                  NULL ),
    m_connector_p (        f_connector_p )
{
    if (m_connector_p)
        m_connector_p -> setParameterObject ( this );
}

CParameter::~CParameter()
{
    if ( m_connector_p )
        delete m_connector_p;
}


CParameterBaseConnector * 
CParameter::setConnector ( CParameterBaseConnector * f_connector_p )
{
    m_connector_p = f_connector_p;    

    if (m_connector_p)
        m_connector_p -> setParameterObject ( this );

    return m_connector_p;    
}

CParameterBaseConnector * 
CParameter::getConnector (  ) const
{
    return m_connector_p;
}

bool
CParameter::update()
{
    //printf("qtEditor_p = %p\n", m_qtEditor_p);
    
    if ( m_qtEditor_p ) 
        m_qtEditor_p -> updateWidget();

    //printf("updated()\n");

    bool res_b;
    
    if ( m_connector_p )
    {
        res_b = m_connector_p -> updateContainer();

        if (res_b == false)
        {
            res_b = updateFromContainer();
            if ( m_qtEditor_p ) 
                m_qtEditor_p -> updateWidget();
        }
    }
    else
        res_b = true;
    
    return res_b;
}

bool
CParameter::updateFromContainer()
{
    if ( m_connector_p )
    {
        bool res_b = m_connector_p -> updateParameterObj();

        //if ( res_b && m_qtEditor_p )
        //    m_qtEditor_p -> updateWidget();

        return res_b;        
    }
    else
        return true;
}
