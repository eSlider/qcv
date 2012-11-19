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

#ifndef __NODE_H
#define __NODE_H

/**
 *******************************************************************************
 *
 * @file node.h
 *
 * \class CNode
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Defines the base class for the nodes of the framework.
 *
 *
 *******************************************************************************/

//#include <iostream>
#include <string>
#include <vector>

/* CONSTANTS */

/* PROTOTYPES */

/* CLASS DEFINITION */

namespace QCV
{
/* PROTOTYPES */        
    class CNode
    {
    /// Constructor, Desctructors
    public:    

        /// Constructors.
        CNode ( CNode * const f_parent_p     = NULL, 
                const std::string f_name_str = "Unnamed Node" );

        /// Virtual destructor.
        virtual ~CNode ();

        /// Get name of node.
        virtual std::string           getName() const;

        /// Get name of node.
        virtual bool                  setName( const std::string f_name );

        /// Get the input of this node.
        virtual CNode*                getParent ( ) const { return m_parent_p; }


    /// Support functions for internal use.
    public:
        /// Add a child to this node
        virtual bool   addChild ( CNode * const f_child_p, 
                                  const int f_priority_i = 1 );

        /// Get a child with given id
        virtual CNode * 
                       getChild ( unsigned int f_id_ui );

        /// Remove all children
        virtual void   deleteChildren (  );

    /// Private data types
    protected:
        struct SChildren
        {
            /// Pointer to node.
            CNode *   ptr_p;
            
            /// Priority
            int       priority_i;
            
            /// Node < for sorting operations.
            int operator < ( SChildren &other ) const
            {
                return priority_i < other.priority_i;
            }            
        };
        
    /// Private members
    protected:    

        /// Node's parent.
        CNode *                        m_parent_p;

        /// Node's children.
        std::vector<SChildren>         m_children_v;

        /// Node's name.
        std::string                    m_name_str;
    };


} // Namespace QCV

#endif // __NODE_H
