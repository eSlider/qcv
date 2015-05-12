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

#ifndef __DISPLAYTREENODE_H
#define __DISPLAYTREENODE_H

/*@@@**************************************************************************
 ** \file  displayTreeNode
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "node.h"
#include "drawingList.h"

#include <vector>

/* CONSTANTS */


namespace QCV
{
    class CDisplayTreeNodeAbstract
    {
    /// Data types
    public:
        
        typedef enum
        {
            NT_CONTAINER,
            NT_DISPLAY
        } ENodeType_t;
        

    /// Consructor/Destructor.
    public:

        /// Constructor.
        CDisplayTreeNodeAbstract ( CDisplayTreeNodeAbstract * f_parent_p = NULL ) 
                : m_parent_p ( f_parent_p  ) {};
        
        /// Destructor. 
       virtual ~CDisplayTreeNodeAbstract() {}

        /// Get node type
        virtual ENodeType_t                 getNodeType () const = 0;
        
        /// Get children count.
        virtual unsigned int                getChildCount () const 
        { 
            return 0; 
        }

        /// Get name.
        virtual std::string                 getName() const = 0;

        /// Get parent.
        virtual CDisplayTreeNodeAbstract *  getParent()
        {
            return m_parent_p;
            
        }

        /// Get child.
        virtual CDisplayTreeNodeAbstract *  getChild ( 
                unsigned int /* f_number_ui */ ) const
        { 
            return NULL;
        }

        int getIndexInParent ( ) const
        {
            if (m_parent_p)
            {
                for (unsigned int i = 0; 
                     i < m_parent_p -> getChildCount(); 
                     ++i)
                {
                    if ( getName() == 
                         m_parent_p -> getChild ( i ) -> getName( ) )
                        return i;
                }
            }
            
            return -1;    
        }

    protected:

        /// Parent node.
        CDisplayTreeNodeAbstract *  m_parent_p;       
    };
    
    class CDisplayNode: public CDisplayTreeNodeAbstract
    {
        friend class CDisplayOpNode;
        
    public:
        /// Constructor.
        CDisplayNode ( CDrawingList *              f_drawingList_p,
                       CDisplayTreeNodeAbstract *  f_parent_p = NULL )
                : CDisplayTreeNodeAbstract (      f_parent_p  ),
                  m_drawingList_p (           f_drawingList_p ) {};
        
        /// Destructor. 
        virtual ~CDisplayNode() 
        {
            if (m_drawingList_p) 
                delete m_drawingList_p;
        }
        
        /// Get node type
        virtual ENodeType_t getNodeType () const
        { 
            return CDisplayTreeNodeAbstract::NT_DISPLAY; 
        };

        std::string getName() const 
        {
            if ( m_drawingList_p )
                return m_drawingList_p -> getName();
            return "";
        }
        
        S2D<int> getPosition() const 
        {
            if ( m_drawingList_p )
                return m_drawingList_p -> getPosition();

            return S2D<int>();
        }

        bool isVisible() const 
        {
            if ( m_drawingList_p )
                return m_drawingList_p -> isVisible();

            return false;
        }

        int getElementsCount( ) const 
        {
            if ( m_drawingList_p )
                return m_drawingList_p -> getElementsCount();
            return -1;
        }

        void setVisibility( bool f_val_b ) const 
        {
            if ( m_drawingList_p )
                return m_drawingList_p -> setVisibility(f_val_b);
        }

        CDrawingList * getDrawingList() const 
        {
            return m_drawingList_p;
        }

    protected:        
        CDrawingList * m_drawingList_p;        
    };
    
    class CDisplayOpNode: public CDisplayTreeNodeAbstract
    {        
    /// Constructor/Destructor
    public:
        
        CDisplayOpNode ( CNode *                     f_op_p,
                         CDisplayTreeNodeAbstract *  f_parent_p = NULL );

        virtual ~CDisplayOpNode ( );

    /// Some methods.
    public:
        void              appendChild( CDisplayOpNode *   f_child_p );
        void              appendChild( CDisplayNode   *   f_child_p );

        /// Get node type
        virtual ENodeType_t getNodeType() const
        { 
            return CDisplayTreeNodeAbstract::NT_CONTAINER; 
        };

        unsigned int      getChildCount () const 
        { 
            return ( m_opChildren.size() + 
                     m_displayChildren.size() );    
        }

        /// Generic getChild.
        virtual CDisplayTreeNodeAbstract *  
                          getChild ( unsigned int f_number_ui ) const; 

        /// Specific getChild.
        CDisplayOpNode *  getOpChild      ( const CNode * const f_op_p) const;

        /// Specific getChild.
        CDisplayOpNode *  getOpChild      (  const unsigned int f_number_ui ) const;

        /// Specific number of displays.
        unsigned int      getOpCount ( ) const
        {
            return m_opChildren.size();
        }

        /// Specific getChild.
        CDisplayNode   *  getDisplayChild ( const std::string & f_name_str ) const;

        /// Specific getChild.
        CDisplayNode   *  getDisplayChild ( const unsigned int f_number_ui ) const;

        /// Specific number of displays.
        unsigned int      getDisplayCount ( ) const;

        /// Get index of this node in the parent.
        int               getIndexInParent ( ) const;

        std::string       getName() const { return ( m_container_p?m_container_p -> getName():"" ); }
        
        CNode *           getContainer() const { return  m_container_p; }
        
    protected:
        /// Children.
        std::vector<CDisplayOpNode *>     m_opChildren;

        /// Children.
        std::vector<CDisplayNode *>       m_displayChildren;
        
        /// Data.
        CNode *                           m_container_p;
    };
}


#endif // __DISPLAYTREENODE_H
