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
 * \file  paramIOXmlFile
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "paramIOPFile.h"
#include "stringOp.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace QCV;
   

CParamIOPFile::CParamIOPFile( std::string f_filePath_str )
   : m_filePath_str (          f_filePath_str )
{
   if ( not f_filePath_str.empty() )
      load ( m_filePath_str );
}

CParamIOPFile::~CParamIOPFile( )
{}

/// Load file.
bool
CParamIOPFile::load ( std::string f_filePath_str )
{ 
   FILE * file_p = fopen (f_filePath_str.c_str(), "r" );
   
   if (!file_p)
   {
      printf("%s:%i Error opening file %s\n", __FILE__, __LINE__, f_filePath_str.c_str());
      return false;
   }
   
   size_t  bufSize_i = 0;
   char *  line_p    = NULL;
   
   std::string currCat_str = "";
   
   while ( !feof ( file_p ) )
   {
      ssize_t chars_i = getline(&line_p, &bufSize_i, file_p );

      if (chars_i > 3 )
      {
         std::string name_str, value_str, comment_str;
         size_t commentIndent_i, valueIndent_i;
         
         EEntryType type_e = readEntry ( line_p, name_str, value_str, valueIndent_i, comment_str, commentIndent_i );

         if ( type_e == ET_CATEGORY )
         {
            currCat_str = name_str;
         }
         else if ( type_e == ET_PARAMETER )
         {
            ParamState_t &r_state = m_map[currCat_str][name_str];
            
            r_state.value_str       = value_str;
            r_state.comment_str     = comment_str;
            r_state.valueIndent_i   = valueIndent_i;
            r_state.commentIndent_i = commentIndent_i;
         }
      }
   }
   if (line_p)
      free ( line_p );
   
   fclose(file_p);
   
   return true;
}

CParamIOPFile::EEntryType
CParamIOPFile::readEntry ( std::string    f_line_str,
                           std::string   &fr_name_str,
                           std::string   &fr_value_str,
                           size_t        &fr_valueIndent_i,
                           std::string   &fr_comment_str,
                           size_t        &fr_commentIndent_i ) const
{
   CStringOp::trim( f_line_str );

   fr_name_str    = "";
   fr_value_str   = "";
   fr_comment_str = "";

   fr_valueIndent_i   = 0;
   fr_commentIndent_i = 0;
   

   if (f_line_str[0] == '[')
   {
      const size_t poss_i   = 1;
      size_t       length_i = std::string::npos;
      
      size_t pos_i = f_line_str.find_first_of ( ']', 1 );

      if ( pos_i == std::string::npos )
      {
         printf("%s:%i Category \"%s\" is missing closing bracket\n", __FILE__, __LINE__, f_line_str.substr(poss_i).c_str());
      }
      else
         length_i = pos_i - poss_i;

      fr_name_str = f_line_str.substr(poss_i, length_i);

      //if ( ! CStringOp::isValidName ( fr_name_str ) )
      //   printf("%s:%i Warning: Parameter name \"%s\" contains invalid characters\n", __FILE__, __LINE__, fr_name_str.c_str());

      return ET_CATEGORY;    
   }
   else
   {
      /// 1.- Find parameter name
      size_t pos_i = f_line_str.find_first_of ( '=', 1 );

      if ( pos_i == std::string::npos )
      {
         return ET_UNKNOWN;
      }

      fr_name_str = f_line_str.substr(0, pos_i);

      fr_valueIndent_i = fr_name_str.length() - 1 - std::min(fr_name_str.length()-1, fr_name_str.find_last_not_of ( ' ' ));

      CStringOp::trim ( fr_name_str );
      
      //if ( ! CStringOp::isValidName ( fr_name_str ) )
      //   printf("%s:%i Warning: Parameter name \"%s\" contains invalid characters\n", __FILE__, __LINE__, fr_name_str.c_str());


      /// 2.- Find parameter value
      
      f_line_str = f_line_str.substr( pos_i+1 );

      CStringOp::trim ( f_line_str );

      /// If first character is already '#', then no parameter has
      /// been assigned.
      if ( f_line_str[0] == '#' )
      {
         printf("%s:%i Missing parameter value for parameter \"%s\"\n", __FILE__, __LINE__, fr_name_str.c_str());
         return ET_INVALID;
      }
      
      // If first character is " then assume a string type parameter.
      if ( f_line_str[0] == '\"' )
      {
         pos_i = f_line_str.find_first_of ( '\"', 1 );

         if ( pos_i == std::string::npos )
         {
            printf("%s:%i Missing closing \" in value of parameter \"%s\"\n", __FILE__, __LINE__, fr_name_str.c_str());
            return ET_INVALID;
         }
         
         fr_value_str = f_line_str.substr( 0, pos_i );
         /// We wont't trim parameter value enclosed in ""

         fr_commentIndent_i = f_line_str.find_first_not_of ( ' ' );

         f_line_str = f_line_str.substr( pos_i+1 );         
         fr_commentIndent_i = f_line_str.find_first_not_of ( " \t" );
      }
      else
      {
         pos_i = f_line_str.find_first_of ( "#", 1 );

         if ( pos_i == std::string::npos )
         {
            fr_value_str = f_line_str;
         }
         else
         {
            fr_value_str = f_line_str.substr(0, pos_i);
         }
         
         CStringOp::trim(fr_value_str);

         f_line_str = f_line_str.substr( pos_i+1 );         
         fr_commentIndent_i = f_line_str.find_first_not_of ( " \t" );
      }
      
      CStringOp::trim ( f_line_str );

      /// 3.- Find parameter description
      if (!f_line_str.empty())
      {
         if ( f_line_str[0] != '#' )
         {
            return ET_PARAMETER;
         }
         
         fr_comment_str = f_line_str.substr(1);
         CStringOp::trim ( fr_comment_str );
      }  
   }

   return ET_PARAMETER;
}

bool isPeriod( const char c )
{
   return c == '.';
}


/// Save file.
bool
CParamIOPFile::save ( std::string f_filePath_str ) const
{
   FILE * file_p = fopen (f_filePath_str.c_str(), "w" );
   
   if (!file_p)
   {
      printf("Error opening file %s\n", f_filePath_str.c_str());
      return false;
   }
   
   CategoryMap_t::const_iterator itCat = m_map.begin();

   for (; itCat != m_map.end() ; ++itCat )
   {
      int indent_i = std::count_if(itCat->first.begin(), itCat->first.end(), isPeriod);
      
      std::string prefix_str (2*indent_i, ' ');

      fprintf(file_p, "\n%s[%s]\n", prefix_str.c_str(), itCat->first.c_str() );

      ParamMap_t::const_iterator itParam = (itCat->second).begin();

      for (; itParam != (itCat->second).end() ; ++itParam )
      {

         const ParamState_t &param = itParam->second;
 
         prefix_str = std::string (2*(indent_i+1), ' ');
         std::string prefixParam_str = std::string (std::max(param.valueIndent_i, (size_t)1), ' ');
         std::string prefixComment_str = std::string (param.commentIndent_i, ' ');

         if (param.comment_str.empty())
            fprintf(file_p, "%s%s%s= %s\n", 
                    prefix_str.c_str(),
                    itParam->first.c_str(),
                    prefixParam_str.c_str(), 
                    param.value_str.c_str() );
         else
            fprintf(file_p, "%s%s%s= %s%s # %s\n", 
                    prefix_str.c_str(),
                    itParam->first.c_str(),
                    prefixParam_str.c_str(), 
                    param.value_str.c_str(), 
                    prefixComment_str.c_str(), 
                    param.comment_str.c_str() );
      }
   }

   fclose(file_p);
   
   return true;
    
}

/// Read parameter.
bool 
CParamIOPFile::get ( std::string  f_paramName_str,
                     std::string  f_category_str,
                     std::string &fr_value_str,
                     std::string &fr_comment_str ) const
{
   CategoryMap_t::const_iterator itCat = m_map.find ( f_category_str );
    
   if ( itCat == m_map.end() )
   {
      return false;
   }
   else
   {
      const ParamMap_t &paramMap = itCat->second;        
      ParamMap_t::const_iterator itPar = paramMap.find ( f_paramName_str );
        
      if ( itPar == paramMap.end() )
      {
         return false;
      }
      else
      {
         fr_value_str   = itPar -> second.value_str;
         fr_comment_str = itPar -> second.comment_str;
      }
      return true;
   }
}


/// Save parameter
bool
CParamIOPFile::set ( std::string  f_paramName_str,
                     std::string  f_category_str,
                     std::string  fr_value_str,
                     std::string  fr_comment_str )
{
   ParamState_t & param = m_map[f_category_str][f_paramName_str];
    
   param.value_str   = fr_value_str;
   if ( param.comment_str == "" )
      param.comment_str = fr_comment_str;
   return true;
}

/// Read parameter with in current category.
bool
CParamIOPFile::get ( std::string  f_paramName_str,
                     std::string &fr_value_str,
                     std::string &fr_comment_str ) const
{
   CategoryMap_t::const_iterator itCat = m_map.find ( getCurrentCategory (  ) );
    
   if ( itCat == m_map.end() )
   {
      return false;
   }
   else
   {
      const ParamMap_t &paramMap = itCat->second;        
      ParamMap_t::const_iterator itPar = paramMap.find ( f_paramName_str );
        
      if ( itPar == paramMap.end() )
      {
         return false;
      }
      else
      {
         fr_value_str   = itPar -> second.value_str;
         fr_comment_str = itPar -> second.comment_str;
      }
      return true;
   }

}


bool
CParamIOPFile::get ( std::string  f_paramName_str,
                     std::string &fr_value_str ) const
{
   CategoryMap_t::const_iterator itCat = m_map.find ( getCurrentCategory (  ) );

   if ( itCat == m_map.end() )
   {
      return false;
   }
   else
   {
      const ParamMap_t &paramMap = itCat->second;        
      ParamMap_t::const_iterator itPar = paramMap.find ( f_paramName_str );
        
      if ( itPar == paramMap.end() )
      {
         return false;
      }
      else
      {
         fr_value_str = itPar -> second.value_str;
      }
      return true;
   }
}


/// Save parameter with in current category.
bool
CParamIOPFile::set ( std::string  f_paramName_str,
                     std::string  fr_value_str,
                     std::string  fr_comment_str )
{
   ParamState_t & param = m_map[getCurrentCategory()][f_paramName_str];

   param.value_str   = fr_value_str;

   if ( param.comment_str == "" )
      param.comment_str = fr_comment_str;

   return true;
}

/// Set the current category.
inline bool 
CParamIOPFile::setCurrentCategory ( std::string  f_category_str )
{
   CParamIOHandling::setCurrentCategory ( f_category_str );

   CategoryMap_t::const_iterator it = m_map.find ( f_category_str );
    
   if ( it == m_map.end() )
   {
      return false;
   }
   else
   {
      return true;
   }
}
