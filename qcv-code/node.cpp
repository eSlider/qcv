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
* \file  node
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */

#include "node.h"

using namespace QCV;

CNode::CNode (  CNode * const f_parent_p /* = NULL */, 
                const std::string f_name_str /* = "Unnamed Node" */ )
        : m_parent_p (             f_parent_p ),
          m_name_str (             f_name_str )
{
}

CNode::~CNode ()
{
    deleteChildren ( );
}

void
CNode::deleteChildren ( )
{
    for (int i = m_children_v.size()-1; i >=0 ; --i)
    {
        delete m_children_v[i].ptr_p;
        m_children_v.pop_back();
    }
}

/// Add a child to this node.
CNode * 
CNode::getChild ( unsigned int f_id_ui )
{
    if (f_id_ui >= m_children_v.size())
        return NULL;

    return m_children_v[f_id_ui].ptr_p;
}

std::string 
CNode::getName() const
{
    return m_name_str;
}

bool
CNode::setName( std::string f_name_str )
{
    m_name_str = f_name_str;
    return true;
}


bool
CNode::addChild ( CNode * const f_child_p, 
                  const int f_priority_i /* = 1 */ )
{
    SChildren newChild;
    newChild.ptr_p      = f_child_p;
    newChild.priority_i = f_priority_i;

    // Add child.
    m_children_v.push_back( newChild );

    // If priority must be considered...
    if ( f_priority_i >= 0 )
    {
        unsigned int i;

        // Move child one position until corresponding position for new child is
        // found....

        for (i = m_children_v.size() - 1; 
             i > 0 && 
             m_children_v[i].priority_i < f_priority_i ; --i)
        {
            m_children_v[i] = m_children_v[i - 1];
        }

        m_children_v[i] = newChild;
    }

    return true;
}
