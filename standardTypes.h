/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 3. See the GNU LGPL version 3 for details.
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

#ifndef __STANDARDTYPES_H
#define __STANDARDTYPES_H

/**
 *******************************************************************************
 *
 * @file standardTypes.h
 *
 * \class S2D
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements a template class for handling 2 dimensional values.
 *
 * The class defines a template for handling 2 dimensional values. The 2 
 * public values can be accessed in multiple ways (by defining an Union):
 *   - x, y
 *   - width, height
 *   - min, max
 *
 *
 *******************************************************************************/

/* INCLUDES */
#include <vector>
#include "ioObj.h"

#include "s2d.h"

/* CONSTANTS */

namespace QCV
{
    class CFloatVector: public std::vector<float>, public CIOObj
    {
    public:
        CFloatVector() {};
        CFloatVector(size_t n): std::vector<float>(n) {};
        CFloatVector( size_t n, 
                      float val ): std::vector<float>(n, val) {};
        
    };
    
    class CDoubleVector: public std::vector<double>, public CIOObj
    {
    public:
        CDoubleVector() {};
        CDoubleVector(size_t n): std::vector<double>(n) {};
        CDoubleVector( size_t n, 
                      double val ): std::vector<double>(n, val) {};
        
    };

    class CIntVector: public std::vector<int>, public CIOObj
    {
    public:
        CIntVector() {};
        CIntVector(size_t n): std::vector<int>(n) {};
        CIntVector( size_t n, 
                      int val ): std::vector<int>(n, val) {};
        
    };
    
    
/**
 * @file standardTypes.h
 *
 * \class SDisplayState
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements a container of the state of a display screen.
 *
 * Implements a container with a S2D<int> for storing position and 
 * a boolean variable for storing visibility of a screen.
 *
 */
    struct SDisplayState
    {
        /// Constructor
        SDisplayState (S2D<int> pos, bool f_visible_b)
                : position (          pos ),
                  visible_b(  f_visible_b ) {}
        
        /// Operator ==
        bool operator == (const SDisplayState & other ) const
        {
            return other.position == position && other.visible_b == visible_b;
        }    
        
        /// Line width.
        S2D<int>                   position;
        
        /// Line width.
        bool                       visible_b;
    };
}

#endif // __STANDARDTYPES_H
