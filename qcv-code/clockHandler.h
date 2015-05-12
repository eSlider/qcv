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

#ifndef __CLOCKHANDLER_H
#define __CLOCKHANDLER_H

/**
 *******************************************************************************
 *
 * @file clockHandler.h
 *
 * \class CClockHandler
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief This class implements a handler for clock.
 *
 *******************************************************************************/

/* INCLUDES */
#include <string>
#include <map>

/* CONSTANTS */

namespace QCV
{
 /* CONSTANTS */


/* DATA TYPES */


/* PROTOTYPES */

    class CNode;
    class CClock;
    class CClockOpNode;
    
    class CClockHandler
    {
    /// Public data types.
    public:


    /// Construction, Destruction
    public:
        CClockHandler( CNode * f_root_p = NULL );
        virtual ~CClockHandler();

    /// Get and sets.
    public:
        /// Get pointer to clock.
        CClock *    getClock ( std::string f_name_str, 
                               CNode *     f_op_p );

        /// Get root node.
        CClockOpNode  * getRootNode ( ) const;

        /// Set clock-changed flag.
        void              setClockUpdateFlag ( bool f_val_b );

        /// Get clock-changed flag.
        bool              mustUpdateClock ( ) const;

    private:

        /// Root node.
        CClockOpNode *          m_root_p;

        /// Flag for indicating a change in some clock.
        bool                    m_clockChanged_b;
    };    


    inline CClockOpNode  * 
    CClockHandler::getRootNode ( ) const 
    { 
        return m_root_p; 
    }

    inline void 
    CClockHandler::setClockUpdateFlag ( bool f_val_b )
    {
        m_clockChanged_b = f_val_b;
    }

    inline bool
    CClockHandler::mustUpdateClock ( ) const
    {
        return m_clockChanged_b;
    }
}

#endif // __CLOCKHANDLER_H

