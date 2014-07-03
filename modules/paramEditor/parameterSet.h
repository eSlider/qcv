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


#ifndef __PARAMETERSET_H
#define __PARAMETERSET_H

/**
 *******************************************************************************
 *
 * @file parameterSet.h
 *
 * \class CParameterSet
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Class for handling parameter sets.
 *
 * This class has the behavious of a node in a tree. A parameter set might have 
 * a parnent, multiple parameters and multiple parameter set as child. Parameters
 * might be read/written by providing a CParamIOHandling with the load and save
 * methods.
 *
 ******************************************************************************/

/* INCLUDES */

#include "paramIOHandling.h"

#include <string>
#include <vector>

/* CONSTANTS */

namespace QCV
{
    /* PROTOTYPES */
    class CParameter;    

    class CParameterSet
    {
    /// Constructor/Destructor
    public:
        /// Default Constructor.
        CParameterSet ( CParameterSet * const                  f_parent_p, 
                        const std::string                      f_name_str = "Unnamed" );

        /// Virtual destructor.
        virtual ~CParameterSet ( );

    public:
        /// Load from IO.
        bool               load ( CParamIOHandling &fr_io,
                                  const std::string &f_prefix_str = std::string("") );

        /// Save to IO.
        bool               save ( CParamIOHandling &fr_io,
                                  const std::string &f_prefix_str = std::string("") ) const;

        /// Get category name.
        std::string        getName ( ) const;

        /// Set category name.
        bool               setName ( const std::string f_name_str );

        /// Get subset count.
        unsigned int       getSubsetCount (  ) const;

        /// Get subset.
        CParameterSet *    getSubset ( std::string f_name_str ) const;

        /// Get subset by index number.
        CParameterSet *    getSubset ( unsigned int f_index_ui ) const;

        /// Add subset.
        bool               addSubset ( CParameterSet * f_newSet_p );

        /// Get parameter.
        CParameterSet *    getParent ( ) const;

        /// Get parameter count.
        unsigned int       getParameterCount (  ) const;

        /// Get parameter.
        CParameter *       getParameter ( std::string f_name_str ) const;

        /// Get parameter by index number
        CParameter *       getParameter ( unsigned int f_index_ui ) const;

        /// Add parameter.
        CParameter *       addParameter ( CParameter * f_newParameter_p );

        /// Orphan childs.
        void               orphanSubsets ( );

    protected:

        /// Parent of this set.
        CParameterSet *                   m_parent_p;
        
        /// Sub sets of parameters.
        std::vector<CParameterSet *>      m_subset;

        /// Vector of parameters.
        std::vector<CParameter *>         m_parameter;

        /// Name of this set (category).
        std::string                       m_name_str;
    };


}

#endif // __PARAMETERSET_H
