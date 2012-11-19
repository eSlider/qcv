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
* \file  displayTreeNode
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "displayTreeNode.h"
#include "drawingList.h"

using namespace QCV;


CDisplayOpNode::CDisplayOpNode ( CNode *                     f_op_p,
                                 CDisplayTreeNodeAbstract *  f_parent_p )
        : CDisplayTreeNodeAbstract (      f_parent_p  ),
          m_container_p (                      f_op_p )
{
}

CDisplayOpNode::~CDisplayOpNode ( )
{
    for (unsigned int i = 0; i < m_opChildren.size(); ++i)
    {
        delete m_opChildren[i];
    }    

    m_opChildren.clear();

    for (unsigned int i = 0; i < m_displayChildren.size(); ++i)
    {
        delete m_displayChildren[i];
    }    

    m_displayChildren.clear();
}

void
CDisplayOpNode::appendChild(  CDisplayOpNode *  f_child_p )
{
    /// Ensure parentship.
    if ( f_child_p == NULL ) return;
    f_child_p -> m_parent_p = this;

    m_opChildren.push_back (  f_child_p );
}

void
CDisplayOpNode::appendChild(  CDisplayNode *  f_child_p )
{
    /// Ensure parentship.
    if ( f_child_p == NULL ) return;
    f_child_p -> m_parent_p = this;

    m_displayChildren.push_back (  f_child_p );
}


////////////////////////////

CDisplayTreeNodeAbstract *  
CDisplayOpNode::getChild ( unsigned int f_number_ui ) const
{
    if (f_number_ui < m_opChildren.size())
        return m_opChildren[f_number_ui];

    f_number_ui -= m_opChildren.size();
    
    if ( f_number_ui < m_displayChildren.size())
        return m_displayChildren[f_number_ui];

    return NULL;
}

CDisplayOpNode *
CDisplayOpNode::getOpChild (const CNode * const f_op_p ) const
{    
    for (unsigned int i = 0; i < m_opChildren.size(); ++i)
    {
        if ( m_opChildren[i] -> m_container_p == f_op_p )
            return m_opChildren[i];
    }

    return NULL;
}

CDisplayOpNode *
CDisplayOpNode::getOpChild ( const unsigned int f_number_ui ) const
{
    if (f_number_ui < m_opChildren.size())
        return m_opChildren[f_number_ui];

    return NULL;    
}


CDisplayNode *
CDisplayOpNode::getDisplayChild (const std::string & f_name_str ) const
{    
    for (unsigned int i = 0; i < m_displayChildren.size(); ++i)
    {
        if ( m_displayChildren[i] -> getName() == f_name_str )
            return m_displayChildren[i];
    }

    return NULL;
}

CDisplayNode *
CDisplayOpNode::getDisplayChild (const unsigned int f_number_ui ) const
{
    if (f_number_ui < m_displayChildren.size())
        return m_displayChildren[f_number_ui];

    return NULL;    
}

unsigned int
CDisplayOpNode::getDisplayCount ( ) const
{
    return m_displayChildren.size();
}

