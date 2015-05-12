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

#ifndef __CLOCKTREENODE_H
#define __CLOCKTREENODE_H

/*@@@**************************************************************************
 ** \file  clockTreeNode
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "node.h"
#include "clock.h"

#include <vector>

/* CONSTANTS */


namespace QCV
{
    class CClockTreeNodeAbstract
    {
    /// Data types
    public:
        
        typedef enum
        {
            NT_CONTAINER,
            NT_CLOCK
        } ENodeType_t;
        

    /// Consructor/Destructor.
    public:

        /// Constructor.
        CClockTreeNodeAbstract ( CClockTreeNodeAbstract * f_parent_p = NULL ) 
                : m_parent_p ( f_parent_p  ) {};
        
        /// Destructor. 
       virtual ~CClockTreeNodeAbstract() {}

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
        virtual CClockTreeNodeAbstract *  getParent()
        {
            return m_parent_p;
            
        }

        /// Get child.
        virtual CClockTreeNodeAbstract *  getChild ( 
                unsigned int /* f_number_ui */ ) const
        { 
            return NULL;
        }

        virtual void resetClock() = 0;
        virtual void printClock(std::string f_prefix_str = "") = 0;

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
        CClockTreeNodeAbstract *  m_parent_p;       
    };
    
    class CClockNode: public CClockTreeNodeAbstract
    {
        friend class CClockOpNode;
        
    public:
        /// Constructor.
        CClockNode ( CClock *                  f_clock_p,
                     CClockTreeNodeAbstract *  f_parent_p = NULL )
                : CClockTreeNodeAbstract (      f_parent_p  ),
                  m_clock_p (                     f_clock_p ) {};
        
        /// Destructor. 
        virtual ~CClockNode() 
        {
            if (m_clock_p) 
                delete m_clock_p;
        }
        
        /// Get node type
        virtual ENodeType_t getNodeType () const
        { 
            return CClockTreeNodeAbstract::NT_CLOCK; 
        };

        std::string getName() const 
        {
            if ( m_clock_p )
                return m_clock_p -> getName();
            return "";
        }
        
        unsigned int getCount()const 
        {
            if ( m_clock_p )
                return m_clock_p -> getCount();

            return 0;
        }

        double getTotalTime() const 
        {
            if ( m_clock_p )
                return m_clock_p -> getTotalTime();

            return 0.0;
        }

        double getLoopTime() const 
        {
            if ( m_clock_p )
                return m_clock_p -> getLoopTime();

            return 0.0;
        }

        CClock * getClock() const 
        {
            return m_clock_p;
        }

        virtual void resetClock()
        {
            m_clock_p -> reset();

            for (unsigned int i = 0;  i < getChildCount(); ++i)
                getChild ( i ) -> resetClock( );
        }

        virtual void printClock(std::string f_prefix_str = ""); 

    protected:        
        CClock * m_clock_p;        
    };
    
    class CClockOpNode: public CClockTreeNodeAbstract
    {        
    /// Constructor/Destructor
    public:
        
        CClockOpNode ( CNode *                   f_op_p,
                       CClockTreeNodeAbstract *  f_parent_p = NULL );

        virtual ~CClockOpNode ( );

    /// Some methods.
    public:
        void              appendChild( CClockOpNode *   f_child_p );
        void              appendChild( CClockNode   *   f_child_p );

        /// Get node type
        virtual ENodeType_t getNodeType() const
        { 
            return CClockTreeNodeAbstract::NT_CONTAINER; 
        };

        unsigned int      getChildCount () const 
        { 
            return ( m_opChildren.size() + 
                     m_clockChildren.size() );    
        }

        /// Generic getChild.
        virtual CClockTreeNodeAbstract *  
                          getChild ( unsigned int f_number_ui ) const; 

        /// Specific getChild.
        CClockOpNode *    getOpChild      ( const CNode * const f_op_p) const;

        /// Specific getChild.
        CClockOpNode *    getOpChild      (  const unsigned int f_number_ui ) const;

        /// Specific number of clocks.
        unsigned int      getOpCount ( ) const
        {
            return m_opChildren.size();
        }

        /// Specific getChild.
        CClockNode   *    getClockChild ( const std::string & f_name_str ) const;

        /// Specific getChild.
        CClockNode   *    getClockChild ( const unsigned int f_number_ui ) const;

        /// Specific number of clocks.
        unsigned int      getClockCount ( ) const
        {
            return m_clockChildren.size();
        }

        /// Get index of this node in the parent.
        int               getIndexInParent ( ) const;

        std::string       getName() const { return ( m_container_p?m_container_p -> getName():"" ); }
        
        CNode *           getContainer() const { return  m_container_p; }
        
        virtual void      resetClock()
        {
            for (unsigned int i = 0;  i < getChildCount(); ++i)
                getChild ( i ) -> resetClock( );

            for (unsigned int i = 0;  i < getOpCount(); ++i)
                getOpChild ( i ) -> resetClock( );
        }

        virtual void      printClock( std::string f_prefix_str = "");
        
    protected:
        /// Children.
        std::vector<CClockOpNode *>     m_opChildren;

        /// Children.
        std::vector<CClockNode *>       m_clockChildren;
        
        /// Data.
        CNode *                         m_container_p;
    };
}


#endif // __CLOCKTREENODE_H
