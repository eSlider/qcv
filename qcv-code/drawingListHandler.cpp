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
* \file  drawingListHandler
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "node.h"
#include "drawingListHandler.h"
#include "displayTreeNode.h"
#include <stdio.h>

#define MAX_CONTAINER_LEVELS 256

using namespace QCV;

CDrawingListHandler::CDrawingListHandler (  CNode * f_opRoot_p )
        : m_root_p (                NULL ),
          m_drawingListChanged_b ( false )
{
    if ( f_opRoot_p )
    {
        m_root_p = new CDisplayOpNode( f_opRoot_p );
    }
}

CDrawingListHandler::~CDrawingListHandler()
{
    delete m_root_p;    
}

CDrawingList * 
CDrawingListHandler::getDrawingList ( std::string  f_name_str, 
                                      CNode *      f_op_p )
{
    if (not f_op_p) return NULL;

    CNode *  ops_p[MAX_CONTAINER_LEVELS];
    int level_i;

    /// Build list of parent containers.
    ops_p[0] = f_op_p;

    for (level_i = 1;
         level_i < MAX_CONTAINER_LEVELS; 
         ++level_i)
    {
        ops_p[level_i] = ops_p[level_i-1] -> getParent();
        if ( not ops_p[level_i] )
            break;
    }

    if ( level_i == MAX_CONTAINER_LEVELS )
    {
        printf( "The maximal amount of container levels of %i has been achieved.",
                 MAX_CONTAINER_LEVELS  );
    }

    --level_i;

    //printf("ops_p->name = %s level_i = %i\n", ops_p[level_i]->getName().c_str(), level_i);
    
    if ( not m_root_p )
        m_root_p = new CDisplayOpNode( ops_p[level_i] );
    //else
    //consistency check can be made here.

    CDisplayOpNode * node_p   = m_root_p;
    CDisplayOpNode * parent_p = node_p;

    while ( level_i > 0 )
    {
        /// Consistency check can be made here between node_p->m_data.container_p and ops_p[level_i]
        --level_i;
        parent_p = node_p;
        node_p   = node_p -> getOpChild ( ops_p[level_i] );
        if ( !node_p ) break;
    }

    if ( !node_p )
    {
        node_p = parent_p;
        while ( level_i >= 0 )
        {
            CDisplayOpNode *  newChild_p = new CDisplayOpNode ( ops_p[level_i--] );
            node_p -> appendChild ( newChild_p );
            node_p = newChild_p;    
        }
    }
    
    /// Search now for display.
    CDisplayNode * child_p = node_p -> getDisplayChild ( f_name_str );

    if (!child_p)
    {
        CDrawingList * drawList_p = new CDrawingList ( f_name_str );
        child_p    = new CDisplayNode ( drawList_p );
        node_p -> appendChild ( child_p );
        return drawList_p;
    }

    child_p->getDrawingList()->setScreenSize(m_screenSize);
    return child_p -> getDrawingList();
}

bool
CDrawingListHandler::setScreenSize ( const S2D<unsigned int> f_size )
{
    m_screenSize = f_size;
    return true;
}
