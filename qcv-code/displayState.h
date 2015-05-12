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

#ifndef __DISPLAYSTATE_H
#define __DISPLAYSTATE_H

/**
 * @file displaystate.h
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

#include "s2d.h"

namespace QCV
{
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

#endif // __DISPLAYSTATE_H
