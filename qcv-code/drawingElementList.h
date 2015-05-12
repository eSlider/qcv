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

#ifndef __DRAWINGELEMENT_H
#define __DRAWINGELEMENT_H

/**
 *******************************************************************************
 *
 * @file drawingElementList.h
 *
 * \class CDrawingElementList
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

/* CONSTANTS */

namespace QCV
{
    class CDrawingElementList
    {
    public:
        
        CDrawingElementList() {}
        virtual ~CDrawingElementList() {}
        
        /******************************************************
         * methods
         ******************************************************/
        
        // Clear all elements.
        virtual bool clear() = 0;
        
        // Inform if this drawing list is blendable.
        virtual bool isBlendable() = 0;
        
        // Draw all elements.
        virtual bool show() const = 0;
        
        // Number of elements.
        virtual int  getSize() const = 0;
        
        // Write elements in a SVG file.
        virtual bool write ( FILE*                f_file_p,
                             const float          f_offsetX_f = 0.0,
                             const float          f_offsetY_f = 0.0,
                             const std::string    f_prefix_str = "") const;

        // Number of elements.
        virtual std::string  getGroupName() const { return "DrawingElement"; };

    private:
        
    };

    inline
    bool CDrawingElementList::write ( FILE*                /* f_file_p */,
                                      const float          /* f_offsetX_f = 0.0 */,
                                      const float          /* f_offsetY_f = 0.0 */,
                                      const std::string    /* f_prefix_str = "" */ ) const
    {
        //printf("write(...) not implemented for this class.\n");
        return false;
    }
    
    

} // Namespace QCV


#endif // __DRAWINGELEMENT_H
