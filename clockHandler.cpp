/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU General Public License, version 2.
 * See the GNU General Public License, version 2 for details.
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
* \file  clockHandler
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "node.h"
#include "clockHandler.h"
#include "clockTreeNode.h"

using namespace QCV;

CClockHandler::CClockHandler( CNode * f_root_p )
        : m_root_p (                NULL ),
          m_clockChanged_b (       false )
{
    if ( f_root_p )
    {
        m_root_p = new CClockOpNode ( f_root_p );
    }

}

CClockHandler::~CClockHandler()
{
    delete m_root_p;    
}

CClock * 
CClockHandler::getClock ( std::string  f_name_str, 
                          CNode *      f_op_p )
{
    if (not f_op_p) return NULL;

    CClockOpNode * node_p = m_root_p -> getOpChild ( f_op_p );

    if ( !node_p )
    {
        node_p = new CClockOpNode ( f_op_p );
        m_root_p -> appendChild ( node_p );
    }

    /// Search now for clock.
    CClockNode * child_p = node_p -> getClockChild ( f_name_str );

    // If not found, then create.
    if (!child_p)
    {
        CClock * clock_p = (CClock *) new CClock ( f_name_str );
        child_p    = new CClockNode ( clock_p );
        
        node_p -> appendChild ( child_p );
        return clock_p;
    }

    return child_p -> getClock();
}
