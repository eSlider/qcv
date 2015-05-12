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
* \file  parameterSet
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "parameterSet.h"
#include "parameter.h"

using namespace QCV;

/// Default Constructor.
CParameterSet::CParameterSet ( CParameterSet * const                   f_parent_p, 
                               const std::string                       f_name_str  )
   : m_parent_p (              NULL ),
     m_subset (                     ),
     m_parameter (                  ),
     m_name_str (        f_name_str )

{
    if ( f_parent_p )
       f_parent_p -> addSubset ( this );
}

/// Virtual destructor.
CParameterSet::~CParameterSet ( )
{
   // We assume that subsets are deleted by their corresponding creator.
    // for (unsigned int i = 0; i < m_subset.size(); ++i)
    // {
    //     delete m_subset[i];
    // }

    // m_subset.clear();
    
    for (unsigned int i = 0; i < m_parameter.size(); ++i)
    {
        delete m_parameter[i];
    }

    m_parameter.clear();
}

/// Load from IO.
bool
CParameterSet::load ( CParamIOHandling  &fr_io,
                      const std::string &f_prefix_str )
{
    std::string catId_str ( "" );

    if (f_prefix_str != "" )
    {
        catId_str = f_prefix_str;
        catId_str += std::string (".");
    }

    catId_str += m_name_str;

    fr_io.setCurrentCategory ( catId_str );
    
    bool res_b = true;

    /// Load first parameters of this subset.
    for (unsigned int i = 0; i < m_parameter.size(); ++i)
    {
        std::string value_str;
        if (not fr_io.get ( m_parameter[i] -> getName(),
                            value_str ) )
            res_b = false;
        else 
        {
            if ( not m_parameter[i] -> setValueFromString ( value_str ) )
                res_b = false;
        }
    }    

    /// Load now all subsets.
    for (unsigned int i = 0; i < m_subset.size(); ++i)
    {
        if ( not m_subset[i] -> load ( fr_io, catId_str ) )
            res_b = false;
    }

    return res_b;
}

/// Save to IO.
bool
CParameterSet::save ( CParamIOHandling  &fr_io,
                      const std::string &f_prefix_str ) const
{
    std::string catId_str ( "" );

    if (f_prefix_str != "" )
    {
        catId_str = f_prefix_str;
        catId_str += std::string (".");
    }

    catId_str += m_name_str;

    fr_io.setCurrentCategory ( catId_str );

    bool res_b = true;

    //printf("saving parameter set %s\n", m_name_str.c_str());
    
    /// Save first parameters of this subset.
    for (unsigned int i = 0; i < m_parameter.size(); ++i)
    {
        if (m_parameter[i] -> shouldBeSaved ( ) )
        {
            m_parameter[i] -> updateFromContainer();
            
            if (! fr_io.set ( m_parameter[i] -> getName(),
                              m_parameter[i] -> getStringFromValue (),
                              m_parameter[i] -> getComment() ) )
                res_b = false;
        }
    }

    /// Save now all subsets.
    for (unsigned int i = 0; i < m_subset.size(); ++i)
    {
        //printf("recursing in parameter set %s\n", m_subset[i] -> m_name_str.c_str());
       if (! m_subset[i] -> save ( fr_io, catId_str ) )
            res_b = false;
    }

    return res_b;
}


/// Get category name.
std::string
CParameterSet::getName ( ) const
{
    return m_name_str;
}


/// Set category name.
bool
CParameterSet::setName ( const std::string f_name_str )
{
    m_name_str = f_name_str;
    return true;
}


/// Get subset count
unsigned int 
CParameterSet::getSubsetCount (  ) const
{
    return m_subset.size();
}

/// Get subset.
CParameterSet *
CParameterSet::getSubset ( std::string f_name_str ) const
{
    for (unsigned int i = 0; i < m_subset.size(); ++i )
    {
        if ( m_subset[i] -> getName () == f_name_str )
            return m_subset[i];
    }
    
    return NULL;
}

/// Get subset.
CParameterSet *
CParameterSet::getSubset ( unsigned int f_index_ui ) const
{
    if ( f_index_ui < m_subset.size() )
        return m_subset[f_index_ui];
    
    return NULL;
}

/// Add subset.
bool
CParameterSet::addSubset ( CParameterSet * f_newSet_p )
{
    if ( f_newSet_p == NULL )
        return false;
        
    m_subset.push_back ( f_newSet_p );
    f_newSet_p -> m_parent_p = this;

    return true;
}

/// Get parameter count
unsigned int 
CParameterSet::getParameterCount (  ) const
{
    return m_parameter.size();
}

/// Get parameter.
CParameter *
CParameterSet::getParameter ( std::string f_name_str ) const
{
    for (unsigned int i = 0; i < m_parameter.size(); ++i )
    {
        if ( m_parameter[i] -> getName () == f_name_str )
            return m_parameter[i];
    }
    
    return NULL;
}

/// Get parameter.
CParameter *
CParameterSet::getParameter ( unsigned int f_index_ui ) const
{
    if (f_index_ui < m_parameter.size())
        return m_parameter[f_index_ui];

    return NULL;
}

/// Add parameter.
CParameter *
CParameterSet::addParameter ( CParameter * f_newParameter_p )
{
    if ( f_newParameter_p != NULL )
        m_parameter.push_back ( f_newParameter_p );

    return f_newParameter_p;
}

/// Get parameter.
CParameterSet *
CParameterSet::getParent ( ) const
{
    return m_parent_p;
}

void
CParameterSet::orphanSubsets ( )
{
    m_subset.clear();
}
