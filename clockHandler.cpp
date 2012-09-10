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

CClockHandler::CClockHandler()
        : m_root_p (                NULL ),
          m_clockChanged_b (       false )
{
    m_root_p = new CClockOpNode ( NULL, NULL );
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
