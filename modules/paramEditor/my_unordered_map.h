//
// Copyright 2012
//
// National Robotics Engineering Center, Carnegie Mellon University
// 10 40th Street, Pittsburgh, PA 15201
// www.nrec.ri.cmu.edu
//
// NREC Confidential and Proprietary
// Do not distribute without prior written permission
//
// This software is licensed under an Internal Use Only license as per
// section 9.1 in the Master Agreement dated January 1, 2008 between
// Carnegie Mellon University and Deere & Company, except for items that
// are under separate licensing agreements between Deere and CMU.
//
// This notice must appear in all copies of this file and its derivatives
//

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
