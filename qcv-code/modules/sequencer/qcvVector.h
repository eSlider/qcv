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

#ifndef __QCVVECTOR_H
#define __QCVVECTOR_H

/**
*******************************************************************************
*
* @file qcv_vector.cpp
*
* \class CVector
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief Defines a simple vector with overload of the template type
* operator so that it can be used as a container of a single
* element. Example:
*
*    CVector<int> vec ( 1 ); // it will add 1 as element
*    int value = vec; /// will return the first element in the vector.
*
*******************************************************************************/

/* INCLUDES */

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
    template <class _Type>
    class CVector: public std::vector<_Type>
    {
    public:
        CVector(): std::vector<_Type> ( ) { }
        CVector(int f_n_i): std::vector<_Type> ( f_n_i ) { }
        CVector(int f_n_i, _Type f_defval): std::vector<_Type> ( f_n_i, f_defval ) { }
        CVector(_Type f_elem): std::vector<_Type> ( ) { std::vector<_Type>::push_back(f_elem); }
        CVector(const CVector<_Type> &f_vec): std::vector<_Type> ( ) { if (f_vec.size()) std::vector<_Type>::insert( std::vector<_Type>::begin(), f_vec.begin(), f_vec.end()); }

        operator _Type ()
        {
            static _Type defval;
            
            if ( std::vector<_Type>::size() )
            {
                _Type val = std::vector<_Type>::operator [] ( 0 );
                return val;
            }
            return defval;
        }
        
        operator _Type () const
        {
            static _Type defval;

            if ( std::vector<_Type>::size() )
            {
                _Type val = std::vector<_Type>::operator [] ( 0 );
                return val;
            }
            return defval;
        }

    protected:

        /// It must have a constructor without params
        static _Type m_default;        
    };
}

#endif  //__QCVVECTOR_H
