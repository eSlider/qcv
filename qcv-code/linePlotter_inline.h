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

/*@@@**************************************************************************
 ** \file  linePlotter_inline.h
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****          (C) COPYRIGHT Hernan Badino - All Rights Reserved          *****
******************************************************************************/

#include "drawingList.h"

template <class _Type>
inline 
CLinePlotter<_Type>::CLinePlotter ( )
        : m_data (                ),
          m_data_p (      &m_data ),
          m_range (          0, 1 ),
          m_autoRange_b (    true )
{

}

template <class _Type>
inline 
CLinePlotter<_Type>::~CLinePlotter ( )
{

}

template <class _Type>
inline 
CLinePlotter<_Type>::CLinePlotter ( std::vector<_Type>   f_vector )
        : m_data (       f_vector ),
          m_data_p (      &m_data ),
          m_range (          0, 1 ),
          m_autoRange_b (    true )     
{

}
template <class _Type>
inline 
CLinePlotter<_Type>::CLinePlotter ( std::vector<_Type> *  f_vector_p )
        : m_data (                      ),
          m_data_p (         f_vector_p ),
          m_range (                0, 1 ),
          m_autoRange_b (          true )
{

}

/// Set auto range.
template <class _Type>
inline bool
CLinePlotter<_Type>::setYRangeAuto ( bool f_val_b )
{
    m_autoRange_b = f_val_b;
    return true;
}

/// Set auto range.
template <class _Type>
inline bool
CLinePlotter<_Type>::setYRange ( S2D<_Type> f_range )
{
    m_range       = f_range;
    m_autoRange_b = false;
    return true;
}

/// Set data
template <class _Type>
inline void 
CLinePlotter<_Type>::setData ( std::vector<_Type>   f_vector )
{
    m_data   = f_vector;
    m_data_p = &m_data;
}

/// Set data
template <class _Type>
inline void 
CLinePlotter<_Type>::setData ( std::vector<_Type> *  f_vector_p )
{
    m_data.clear();
    m_data_p =  f_vector_p;
}

/// Set data
template <class _Type>
inline  std::vector<_Type> *
CLinePlotter<_Type>::getData (  )
{
    return m_data_p;
}

template <class _Type>
inline void 
CLinePlotter<_Type>::plot ( CDrawingList *f_list_p,
                            int           f_w_i,
                            int           f_h_i )
{
   plot ( f_list_p,
          0,
          0,
          f_w_i,
          f_h_i );
}

template <class _Type>
inline void 
CLinePlotter<_Type>::plot ( CDrawingList *f_list_p,
                            int           f_x_i,
                            int           f_y_i,
                            int           f_w_i,
                            int           f_h_i )
{
    std::vector<_Type> & data = *m_data_p;
    
    if (data.size() == 0)  return;
    
    SRgba plcolor   = f_list_p->getLineColor();
    float plwidth_f = f_list_p->getLineWidth();
    
    f_list_p->setLineColor ( 255, 255, 255 );    
    f_list_p->setLineWidth ( 2 );
    f_list_p->addRectangle ( f_x_i, f_y_i, f_x_i+f_w_i, f_y_i+f_h_i);

    /// Search for maximum

    _Type maxVal;
    _Type minVal;

    if (m_autoRange_b == true)
    {
        maxVal = data[0];
        minVal = data[0];
        for (int i = 1; i < (int)data.size(); ++i)
        {
            if ( maxVal < data[i] )
                maxVal = data[i];
            if ( minVal > data[i] )
                minVal = data[i];
        }

        m_range.min = minVal;
        m_range.max = maxVal;
    }
    else
    {
        minVal = m_range.min;
        maxVal = m_range.max;    
    }
    
    _Type range = maxVal - minVal;
    
    /// Setup scale and offset variables.
    float w_f = f_w_i - 32;
    float h_f = f_h_i - 4;
    float ox_f = 32;
    float oy_f = f_h_i - 2;
    
    float dx_f =  w_f / data.size();
    float dy_f = -h_f / (float) range;

    float pSize_f = f_w_i/400.f;

    float pX_f = 0.f, pY_f = 0.f;
    float cX_f, cY_f;

    f_list_p->setLineWidth ( 1 );
    f_list_p->setLineColor ( plcolor );    

    for (int i = 0; i < (int)data.size(); ++i)
    {
        float val_f = std::min(std::max((float)data[i], (float)(minVal-range/1000.)), (float)(maxVal+range/1000.));

        //printf("data[%i] = %f, val = %f\n", i, (float)data[i], (float)val_f);

        cX_f =  f_x_i + ox_f + i * dx_f;
        cY_f =  f_y_i + oy_f + ((float)val_f-(float)minVal) * dy_f;

        if ( val_f >= minVal && val_f <= maxVal )
            f_list_p->addSquare ( cX_f, cY_f,
                                  pSize_f );
        
        if (i != 0)
        {
            f_list_p->addLine ( cX_f, cY_f, pX_f, pY_f );
        }
        
        pX_f = cX_f;
        pY_f = cY_f;
    }

    f_list_p->setLineColor ( 255, 255, 255 );    
    /// Show grid now.

    float pow_i = (int)(log(range)/2.30258509299);
    float gMax_f = powf(10, (pow_i-1));

    if ( range / gMax_f > 10 )
        gMax_f*=10;

    int   num_i = (int)((float)range / gMax_f + 0.5f);

    f_list_p->setLineColor ( 125, 125, 125 );    

    //SHOWVAL(num_i);
    //SHOWVAL(pow_i);
    //SHOWVAL((float)maxVal);
    //SHOWVAL(gMax_f);
    
    char str[256];
    
    for (int i = 0; i < num_i; ++i)
    {
        float y_f = oy_f + i * gMax_f * dy_f;

        if (i % 10)
            f_list_p->setLineWidth ( 1 );    
        else
            f_list_p->setLineWidth ( 2 );

        f_list_p->addLine ( f_x_i + ox_f,       f_y_i + y_f,
                            f_x_i + ox_f + w_f, f_y_i + y_f );


        sprintf(str, "%.2f", minVal + i * gMax_f );

        f_list_p-> addText ( str, f_x_i + 2, f_y_i + y_f - 4 , 8, false );

        f_list_p->addLine ( f_x_i + ox_f,       f_y_i + y_f,
                            f_x_i + ox_f + w_f, f_y_i + y_f );

    }
    f_list_p->setLineColor ( plcolor );    
    f_list_p->setLineWidth ( plwidth_f );
}

