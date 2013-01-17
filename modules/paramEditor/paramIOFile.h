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


#ifndef __PARAMIOFILE_H
#define __PARAMIOFILE_H

/**
 *******************************************************************************
 *
 * @file paramIOFile.h.
 *
 * \class CParamIOFile
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Concrete class for handling input/output of parameters from
 * files.
 *
 ******************************************************************************/

/* INCLUDES */
#include"paramIOHandling.h"

#include <QtCore/QXmlStreamReader>
#include <map>

/* CONSTANTS */

namespace QCV
{
    class CParamIOFile : public CParamIOHandling
    {
    /// Constructors / Destructor.
    public:
        CParamIOFile( std::string f_filePath_str = "" );
        virtual ~CParamIOFile( );

    public:
        /// Load file.
        bool        load ( std::string f_filePath_str );

        /// Save file.
        bool        save ( std::string f_filePath_str = "" );        

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
        
        struct ParamState_t
        {
            std::string value_str;
            std::string comment_str;
        };

        typedef std::map<std::string, ParamState_t> ParamMap_t;
        typedef std::map<std::string, ParamMap_t>   CategoryMap_t;

    /// Help methods.
    private:
        /// Read xml file.
        bool readVicParXml( QXmlStreamReader & fr_qxsReader );

        /// Read category.
        bool readCategory( QXmlStreamReader & fr_qxsReader,
                           ParamMap_t       & fr_paramMap );

        /// Read parameter.
        bool readParameter( QXmlStreamReader & fr_qxsReader,
                            ParamState_t     & fr_state );
        

    protected:

        /// Path to file.
        std::string              m_filePath_str;
        
        /// Map of parameters.
        CategoryMap_t            m_map;
        
    };
}


#endif // __PARAMIOFILE_H
