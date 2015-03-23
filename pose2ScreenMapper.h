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

#ifndef __GTPOSE2SCREENMAPPER_H
#define __GTPOSE2SCREENMAPPER_H

#include "standardTypes.h"

namespace QCV
{
   struct SPose2ScreenMapper
   {
      SPose2ScreenMapper(): 
         visRangeX(  0., 0. ),
         visRangeY(  0., 0. ),
         visScale_d (     0 ),
         offset   (  0., 0. )
      {
         
      }
      
      // world 2 screen
      S2D<double> world2Screen(S2D<double> pos)
      {
         S2D<double> res;
         
         res.x = visScale_d * (pos.x - visRangeX.min)+offset.x;
         res.y = visScale_d * (visRangeY.max - pos.y)+offset.y;
         //res.y = visScale_d * (-visRangeY.min + pos.y)+offset.y;
         
         return res;
      }
      
      // screen 2 world
      S2D<double> screen2World(S2D<double> pos)
      {
         S2D<double> res;
         
         res.x = (pos.x-offset.x)/visScale_d + visRangeX.min;
         res.y = (pos.y-offset.y)/visScale_d + visRangeY.min;
         
         return res;
      }
      
      
      S2D<double> visRangeX;
      S2D<double> visRangeY;
      double      visScale_d;
      S2D<double> offset;
   };
}

#endif
