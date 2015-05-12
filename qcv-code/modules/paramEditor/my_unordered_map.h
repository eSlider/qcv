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

/**
*******************************************************************************
*
* @file my_unordered_map.h
*
* \class Implements a simple unorderd map.
* \author Hernan Badino (hernan.badino@gmail.com)
*
******************************************************************************/

#ifndef __MYUNORDEREDMAP_H
#define __MYUNORDEREDMAP_H


/* INCLUDES */
#include <vector>

/* CONSTANTS */

template  <typename T1, typename T2>
class my_unordered_map: public std::vector< std::pair<T1,T2> >
{
   typedef std::pair<T1,T2> pair_type;         
   typedef std::vector<pair_type> vector_type;
   
public:
   
   T2& operator [] ( const T1 &id )
   {
      typename vector_type::iterator it = find ( id );
      
      if (it != vector_type::end() ) return it->second;
      
      vector_type::push_back ( std::pair<T1,T2> ( id, T2() ) );
      
      return vector_type::back().second;            
   }         
   
   typename vector_type::const_iterator find ( const T1 &id ) const 
   {
      typename vector_type::const_iterator it   = vector_type::begin();
      typename vector_type::const_iterator last = vector_type::end();
      
      while (it!=last) {
         if (it->first == id) return it;
         ++it;
      }
      
      return vector_type::end();
   }
   
   typename vector_type::iterator find ( const T1 &id ) 
   {
      typename vector_type::iterator it   = vector_type::begin();
      typename vector_type::const_iterator last = vector_type::end();
      
      while (it!=last) {
         if (it->first == id) return it;
         ++it;
      }
      
      return vector_type::end();
   }   
};
      
#endif // __MYUNORDEREDMAP_H
