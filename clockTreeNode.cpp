/*@@@**************************************************************************
* \file  clockTreeNode
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "clockTreeNode.h"
#include "clock.h"
#include <stdio.h>

using namespace QCV;


CClockOpNode::CClockOpNode ( CNode *               f_op_p,
                             CClockTreeNodeAbstract *  f_parent_p )
        : CClockTreeNodeAbstract (      f_parent_p  ),
          m_container_p (                     f_op_p )
{
}

CClockOpNode::~CClockOpNode ( )
{
    for (unsigned int i = 0; i < m_opChildren.size(); ++i)
    {
        delete m_opChildren[i];
    }    

    m_opChildren.clear();

    for (unsigned int i = 0; i < m_clockChildren.size(); ++i)
    {
        delete m_clockChildren[i];
    }    

    m_clockChildren.clear();
}

void
CClockOpNode::appendChild(  CClockOpNode *  f_child_p )
{
    /// Ensure parentship.
    if ( f_child_p == NULL ) return;
    f_child_p -> m_parent_p = this;

    m_opChildren.push_back (  f_child_p );
}

void
CClockOpNode::appendChild(  CClockNode *  f_child_p )
{
    /// Ensure parentship.
    if ( f_child_p == NULL ) return;
    f_child_p -> m_parent_p = this;

    m_clockChildren.push_back (  f_child_p );
}


////////////////////////////

CClockTreeNodeAbstract *  
CClockOpNode::getChild ( unsigned int f_number_ui ) const
{
    if (f_number_ui < m_opChildren.size())
        return m_opChildren[f_number_ui];

    f_number_ui -= m_opChildren.size();
    
    if ( f_number_ui < m_clockChildren.size())
        return m_clockChildren[f_number_ui];

    return NULL;
}

CClockOpNode *
CClockOpNode::getOpChild (const CNode * const f_op_p ) const
{    
    for (unsigned int i = 0; i < m_opChildren.size(); ++i)
    {
        if ( m_opChildren[i] -> m_container_p == f_op_p )
            return m_opChildren[i];
    }

    return NULL;
}

CClockOpNode *
CClockOpNode::getOpChild ( const unsigned int f_number_ui ) const
{
    if (f_number_ui < m_opChildren.size())
        return m_opChildren[f_number_ui];

    return NULL;    
}

 void 
 CClockOpNode::printClock( std::string f_prefix_str )
 {
     for (unsigned int i = 0;  i < getChildCount(); ++i)
         getChild ( i ) -> printClock( f_prefix_str + " " );
 }


CClockNode *
CClockOpNode::getClockChild (const std::string & f_name_str ) const
{    
    for (unsigned int i = 0; i < m_clockChildren.size(); ++i)
    {
        if ( m_clockChildren[i] -> getName() == f_name_str )
            return m_clockChildren[i];
    }

    return NULL;
}

CClockNode *
CClockOpNode::getClockChild (const unsigned int f_number_ui ) const
{
    if (f_number_ui < m_clockChildren.size())
        return m_clockChildren[f_number_ui];

    return NULL;    
}

void
CClockNode::printClock(std::string f_prefix_str )
{
    printf("%s", f_prefix_str.c_str());
    m_clock_p -> print();
    
    for (unsigned int i = 0;  i < getChildCount(); ++i)
        getChild ( i ) -> printClock( );
}

