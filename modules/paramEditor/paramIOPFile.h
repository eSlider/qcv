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


#ifndef __PARAMIOPFILE_H
#define __PARAMIOPFILE_H

/**
*******************************************************************************
*
* @file paramIOXmlFile.h.
*
* \class CParamIOPFile
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief Concrete class for handling input/output of parameters from
* files.
*
******************************************************************************/

/* INCLUDES */
#include"paramIOHandling.h"
#include"my_unordered_map.h"

/* CONSTANTS */

namespace QCV
{
   class CParamIOPFile : public CParamIOHandling
   {
      /// Constructors / Destructor.
   public:
      CParamIOPFile( std::string f_filePath_str = "" );
      virtual ~CParamIOPFile( );

   public:
      /// Load file.
      bool        load ( std::string f_filePath_str );

      /// Save file.
      bool        save ( std::string f_filePath_str = "" ) const;

   public:
      /// Read parameter.
      virtual bool get ( std::string  f_paramName_str,
                         std::string  f_category_str,
                         std::string &fr_value_str,
                         std::string &fr_comment_str ) const;

      /// Save parameter
      virtual bool set ( std::string  f_paramName_str,
                         std::string  f_category_str,
                         std::string  fr_value_str,
                         std::string  fr_comment_str );
        
      /// Read parameter in current category.
      virtual bool get ( std::string  f_paramName_str,
                         std::string &fr_value_str ) const;

      /// Read parameter in current category.
      virtual bool get ( std::string  f_paramName_str,
                         std::string &fr_value_str,
                         std::string &fr_comment_str ) const;
        
      /// Save parameter in current category.
      virtual bool set ( std::string  f_paramName_str,
                         std::string  fr_value_str,
                         std::string  fr_comment_str = std::string("") );

   public:
      /// Set the current category.
      virtual bool setCurrentCategory ( std::string  f_category_str );

      /// Protected members.
   protected:
        
      enum EEntryType
      {
         ET_UNKNOWN,
         ET_INVALID,
         ET_CATEGORY,
         ET_PARAMETER
      };
      
         
      struct ParamState_t
      {
         std::string value_str;
         size_t      valueIndent_i;
         std::string comment_str;
         size_t      commentIndent_i;
      };

   public:
   private:

      /// This is required when using < C++98. If using > C++11, the
      /// standard unoredered_map can be used instead.
      typedef my_unordered_map<std::string, ParamState_t> ParamMap_t;
      typedef my_unordered_map<std::string, ParamMap_t>   CategoryMap_t;

      /// Help methods.
   private:
      /// Read entry.
      EEntryType readEntry (  std::string    f_line_str,
                              std::string   &fr_name_str,
                              std::string   &fr_value_str,
                              size_t        &fr_valueIndent_i,
                              std::string   &fr_comment_str,
                              size_t        &fr_commentIndent_i ) const;
        

   protected:

      /// Path to file.
      std::string              m_filePath_str;
        
      /// Map of parameters.
      CategoryMap_t            m_map;
        
   };
}


#endif // __PARAMIOPFILE_H
