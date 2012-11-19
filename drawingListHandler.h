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

#ifndef __DRAWINGLISTHANDLER_H
#define __DRAWINGLISTHANDLER_H

/**
 *******************************************************************************
 *
 * @file drawingListHandler.h
 *
 * \class CDrawingListHandler
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief This class implements a handler for drawing list.
 *
 * It handles drawing lists, searching existing drawing list or creating them if
 * the requested drawing list does not exist.
 *
 *******************************************************************************/

/* INCLUDES */
#include <string>
#include <map>

#include "standardTypes.h"

/* CONSTANTS */

namespace QCV
{
 /* CONSTANTS */


/* DATA TYPES */


/* PROTOTYPES */

    class CNode;
    class CDrawingList;
    class CDisplayOpNode;
    
    class CDrawingListHandler
    {
    /// Public data types.
    public:


    /// Construction, Destruction
    public:        
        CDrawingListHandler( CNode * f_root_p = NULL);
        virtual ~CDrawingListHandler();

    /// Get and sets.
    public:
        /// Get pointer to drawing list.
        CDrawingList *    getDrawingList ( std::string f_name_str, 
                                           CNode *     f_op_p );

        /// Get root node.
        CDisplayOpNode  * getRootNode ( ) const;

        /// Set drawing-list-changed flag.
        void              setDisplayUpdateFlag ( bool f_val_b );

        /// Set the default screen size.
        bool              setScreenSize ( const S2D<unsigned int> f_size );

        /// Set the default screen size.
        S2D<unsigned int> getScreenSize ( ) const { return m_screenSize; }
        

        /// Get drawing-list-changed flag.
        bool              mustUpdateDisplay ( ) const;

    private:

        /// Root node.
        CDisplayOpNode *          m_root_p;

        /// Flag for indicating a change in some drawing list.
        bool                      m_drawingListChanged_b;
        
        /// Default screen size
        S2D<unsigned int>         m_screenSize;
    };    


    inline CDisplayOpNode  * 
    CDrawingListHandler::getRootNode ( ) const 
    { 
        return m_root_p; 
    }

    inline void 
    CDrawingListHandler::setDisplayUpdateFlag ( bool f_val_b )
    {
        m_drawingListChanged_b = f_val_b;
    }

    inline bool
    CDrawingListHandler::mustUpdateDisplay ( ) const
    {
        return m_drawingListChanged_b;
    }
}

#endif // __DRAWINGLISTHANDLER_H
