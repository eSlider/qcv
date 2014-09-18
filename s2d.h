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

#ifndef __S2D_H
#define __S2D_H

#include <limits>
#include <opencv/cv.h>

namespace QCV
{
    template <class Type>
    class S2D
    {
    public:
        /// Constructor.
        S2D ( const Type f_x = std::numeric_limits<Type>::max(), 
              const Type f_y = std::numeric_limits<Type>::max() )
                : x ( f_x ),
                  y ( f_y ) {}

        S2D ( const cv::Size f_size ): 
            x ( f_size.width  ),
            y ( f_size.height ) { }
        
        S2D ( const cv::Point_<int> f_pos ): 
            x ( f_pos.x  ),
            y ( f_pos.y ) { }
        
        S2D ( const cv::Point_<float> f_pos ): 
            x ( f_pos.x  ),
            y ( f_pos.y ) { }
        
        S2D ( const cv::Point_<double> f_pos ): 
            x ( f_pos.x  ),
            y ( f_pos.y ) { }
        
        int operator < ( const S2D<Type> &f_other ) const
        { 
            return ( y < f_other.y || 
                     ( y == f_other.y && 
                       x < f_other.y ) );
        }

        bool operator == ( const S2D<Type> &f_other ) const
        { 
            return ( y == f_other.y && x == f_other.x );
        }

        bool operator != ( const S2D<Type> &f_other ) const
        { 
            return ! (operator == ( f_other ));
        }

        operator cv::Size( )
        {
            return cv::Size ( width, height );
        }

        operator cv::Point_<int> ( )
        {
            return cv::Point_<int>(x,y);
        }        

        bool isValid () const
        { 
            return ( x != std::numeric_limits<Type>::max() || 
                     y != std::numeric_limits<Type>::max() );
        }

        void invalidate () 
        { 
            x = std::numeric_limits<Type>::max();
            y = std::numeric_limits<Type>::max();
        }
 
        union
        {
            Type val[2];
            
            struct {             
                /// x
                Type x;
                
                /// y
                Type y;
            };
            
            struct {
                /// width
                Type width;
                
                /// height
                Type height;
            };
            
            struct {             
                /// min
                Type min;
                
                /// max
                Type max;
            };
        };
    };
}

#endif
