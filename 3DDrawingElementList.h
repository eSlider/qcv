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

#ifndef __3DDRAWINGELEMENT_H
#define __3DDRAWINGELEMENT_H

/**
 *******************************************************************************
 *
 * @file 3DDrawingElementList.h
 *
 * \class C3DDrawingElementList
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief This abstract class provides the base for primitives drawing lists.
 *
 * Concrete child classes must implement the methods clear(), isBlendable(), 
 * show() and getSize().
 *
 *******************************************************************************/

/* INCLUDES */

#include <string>
#include "3DRowVector.h"

/* CONSTANTS */

namespace QCV
{
    class C3DDrawingElementList
    {
    public:
        
        C3DDrawingElementList() {}
        virtual ~C3DDrawingElementList() {}
        
        /******************************************************
         * methods
         ******************************************************/
        
        // Clear all elements.
        virtual bool clear() = 0;
        
        // Draw all elements.
        virtual bool show() = 0;
        
        // Number of elements.
        virtual int  getSize() const = 0;
        
        // Write elements in a file.
        virtual bool write ( FILE*                f_file_p,
                             C3DVector            f_offset = C3DVector(0,0,0),
                             const std::string    f_prefix_str = "") const;

        // Number of elements.
        virtual std::string  getGroupName() const { return "3DDrawingElement"; };

    private:
        
    };

    inline
    bool C3DDrawingElementList::write ( FILE*                /* f_file_p */,
                                        C3DVector            /* f_offset=C3DVector(0,0,0) */,
                                        const std::string    /* f_prefix_str = "" */ ) const
    {
        //printf("write(...) not implemented for this class.\n");
        return false;
    }
    
    

} // Namespace QCV


#endif // __3DDRAWINGELEMENT_H
