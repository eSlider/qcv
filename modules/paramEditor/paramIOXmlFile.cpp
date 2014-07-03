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
* \author Hernan Badino
*******************************************************************************

/* INCLUDES */
#include "paramIOXmlFile.h"

#include <QtCore/QXmlStreamReader>
#include <QtCore/QFile>

using namespace QCV;

CParamIOXmlFile::CParamIOXmlFile( std::string f_filePath_str )
        : m_filePath_str (          f_filePath_str )
{
    if ( not f_filePath_str.empty() )
        load ( m_filePath_str );
}

CParamIOXmlFile::~CParamIOXmlFile( )
{}


/// Load file.
bool
CParamIOXmlFile::load ( std::string f_filePath_str )
{
    QFile qFile (f_filePath_str.c_str());

    if (!qFile.open(QFile::ReadOnly | QFile::Text))
    {
        printf("Error opening file %s\n", f_filePath_str.c_str());
        return false;
    }
    
    if (not qFile.exists())
        return false;

    QXmlStreamReader qxsReader (&qFile);

    while (not qxsReader.atEnd()) 
    {
        qxsReader.readNext();

        if (qxsReader.isStartElement()) 
        {
            if (qxsReader.name() == "vicpar")
            {
                return readVicParXml(qxsReader);
            }
            else
            {
                printf("The file is not an VICPAR version 1.0 file.\n");
                qxsReader.raiseError(QObject::tr("The file is not an ParamHandlingPAR version 1.0 file."));
                return false;    
            }
        }
        else
        {
        }
        
    }
    
    return not qxsReader.error();    
}

bool 
CParamIOXmlFile::readVicParXml( QXmlStreamReader & fr_qxsReader )
{
    while (not fr_qxsReader.atEnd()) 
    {
        fr_qxsReader.readNext();

        if ( fr_qxsReader.isEndElement() && 
             fr_qxsReader.name() == "vicpar" ) 
            break;

        if ( fr_qxsReader.isStartElement() )
        {
            if ( fr_qxsReader.name() == "category" )
            {
                std::string catName_str = 
                    fr_qxsReader.attributes().value("name").toString().toStdString();

                readCategory ( fr_qxsReader, m_map[catName_str] );
            }
        }
    }

    return true;    
}

bool 
CParamIOXmlFile::readCategory( QXmlStreamReader & fr_qxsReader,
                            ParamMap_t       & fr_paramMap )
{
    while (not fr_qxsReader.atEnd()) 
    {
        fr_qxsReader.readNext();
        
        if ( fr_qxsReader.isEndElement() && 
             fr_qxsReader.name() == "category" )
            break;

        if (fr_qxsReader.isStartElement())
        {
            if ( fr_qxsReader.name() == "parameter" )
            {
                std::string paramName_str = 
                    fr_qxsReader.attributes().value("name").toString().toStdString();

                readParameter ( fr_qxsReader, fr_paramMap[paramName_str] );
            }
        }
    }
    return true;
}

bool 
CParamIOXmlFile::readParameter( QXmlStreamReader & fr_qxsReader,
                             ParamState_t     & fr_state )

{
    while (not fr_qxsReader.atEnd()) 
    {
        fr_qxsReader.readNext();
 
        if ( fr_qxsReader.isEndElement() && 
             fr_qxsReader.name() == "parameter" )
            break;
       
        if (fr_qxsReader.isStartElement())
        {
            if ( fr_qxsReader.name() == "value" )
                fr_state.value_str = fr_qxsReader.readElementText().toStdString();

            if ( fr_qxsReader.name() == "comment" )
                fr_state.comment_str = fr_qxsReader.readElementText().toStdString();
        }
    }
    return true;
}

/// Save file.
bool
CParamIOXmlFile::save ( std::string f_filePath_str ) const
{
#if 1
    QFile qfile ( f_filePath_str.c_str() );
    qfile.open(QIODevice::WriteOnly);

    QXmlStreamWriter writer(&qfile);
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent ( 2 );
    
    writer.writeStartDocument( "1.0", true );
    writer.writeDTD("<!DOCTYPE vicpar>");
    writer.writeStartElement("vicpar");

    CategoryMap_t::const_iterator itCat = m_map.begin();

    for (; itCat != m_map.end() ; ++itCat )
    {
        writer.writeStartElement("category");
        writer.writeAttribute("name", itCat->first.c_str() );

        ParamMap_t::const_iterator itParam = (itCat->second).begin();

        for (; itParam != (itCat->second).end() ; ++itParam )
        {
            const ParamState_t &param = itParam->second;
 
            writer.writeStartElement("parameter");
            writer.writeAttribute("name", itParam->first.c_str());
            {
                writer.writeStartElement("value");
                writer.writeCharacters ( param.value_str.c_str() );
                writer.writeEndElement (); // value

                writer.writeStartElement("comment");
                writer.writeCharacters ( param.comment_str.c_str() );
                writer.writeEndElement (); // comment
            }            
            writer.writeEndElement (); // parameter    
       }

        writer.writeEndElement (); // category    
        
    }
    writer.writeEndElement (); // vicpar
    writer.writeEndDocument();
    qfile.close();

#endif
        return false;
    
}

/// Read parameter.
bool 
CParamIOXmlFile::get ( std::string  f_paramName_str,
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
CParamIOXmlFile::set ( std::string  f_paramName_str,
                    std::string  f_category_str,
                    std::string  fr_value_str,
                    std::string  fr_comment_str )
{
    ParamState_t & param = m_map[f_category_str][f_paramName_str];
    
    param.value_str   = fr_value_str;
    param.comment_str = fr_comment_str;
    return true;
}

/// Read parameter with in current category.
bool
CParamIOXmlFile::get ( std::string  f_paramName_str,
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
CParamIOXmlFile::get ( std::string  f_paramName_str,
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
CParamIOXmlFile::set ( std::string  f_paramName_str,
                    std::string  fr_value_str,
                    std::string  fr_comment_str )
{
    ParamState_t & param = m_map[getCurrentCategory()][f_paramName_str];
    
    param.value_str   = fr_value_str;
    param.comment_str = fr_comment_str;
    return true;
}


/// Set the current category.
inline bool 
CParamIOXmlFile::setCurrentCategory ( std::string  f_category_str )
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
