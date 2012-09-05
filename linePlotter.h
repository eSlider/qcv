/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU General Public License, version 2.
 * See the GNU General Public License, version 2 for details.
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

#ifndef __LINEPLOTTER_H
#define __LINEPLOTTER_H

/*@@@**************************************************************************
 ** \file  linePlotter
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****          (C) COPYRIGHT Hernan Badino - All Rights Reserved          *****
******************************************************************************/

/* INCLUDES */
#include "ioObj.h"

/* CONSTANTS */

namespace QCV
{
    /* PROTOTYPES */
    class CDrawingList;
    
    template < class _Type >
    class CLinePlotter: public CIOObj
    {

    /// Constructors/Destructor
    public:
        CLinePlotter ( );
        CLinePlotter ( std::vector<_Type>   f_vector );
        CLinePlotter ( std::vector<_Type> * f_vector_p );
        virtual ~CLinePlotter ( );

    public:
        
        /// Set data
        void setData ( std::vector<_Type>   f_vector );

        /// Set external data
        void setData ( std::vector<_Type> * f_vector_p );

        /// Set data
        std::vector<_Type> * getData ( );
        
    public:
        
        /// Set auto scale 
        bool setYRange ( S2D<_Type> f_range );

        /// Set auto scale 
        bool setYRangeAuto ( bool f_val_b );
        

    public:

        void plot ( CDrawingList *f_list_p,
                    int           f_w_i,
                    int           f_h_i );

    private:
        /// Data 
        std::vector<_Type>        m_data;

        /// External data 
        std::vector<_Type> *      m_data_p;
        
        /// Y Range
        S2D<_Type>                m_range;

        /// Y Range auto?
        bool                      m_autoRange_b;
    };
}

#include "linePlotter_inline.h"

#endif // __LINEPLOTTER_H
