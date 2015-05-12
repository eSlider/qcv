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


CClockOpNode::CClockOpNode ( CNode *                   f_op_p,
                             CClockTreeNodeAbstract *  f_parent_p )
        : CClockTreeNodeAbstract (      f_parent_p  ),
          m_container_p (                    f_op_p )
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
     if (getChildCount())
     {
         if (f_prefix_str != "")
             printf("%s * %s\n", f_prefix_str.c_str(), getName().c_str() );
         else
             printf("* %s\n", getName().c_str() );
         
         if ( getClockCount() )
         {
             for (unsigned int i = 0;  i < getClockCount(); ++i)
                 getClockChild ( i ) -> printClock ( f_prefix_str + "  " );
         }
         
         if ( getOpCount() )
         {
             for (unsigned int i = 0;  i < getOpCount(); ++i)
                 getOpChild ( i ) -> printClock( f_prefix_str + "  " );
         }
     }
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
    printf("%s  ", f_prefix_str.c_str());
    m_clock_p -> print();
    
    for (unsigned int i = 0;  i < getChildCount(); ++i)
        getChild ( i ) -> printClock( );
}

