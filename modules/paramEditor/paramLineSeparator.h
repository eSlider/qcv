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


#ifndef __PARAMLINESEPARATOR_H
#define __PARAMLINESEPARATOR_H

/**
 *******************************************************************************
 *
 * @file paramLineSeparator.h
 *
 * \class CParamLineSeparator
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Virtual parameter for defining an horizontal line in the parameter 
 * display.
 *
 ******************************************************************************/

/* INCLUDES */
#include "parameter.h"

/* CONSTANTS */

/* PROTOTYPES */

namespace QCV
{
    class CParamLineSeparator: public CParameter
    {
    /// Constructors/Destructor
    public:
        CParamLineSeparator ( );
        virtual ~CParamLineSeparator ( );
        
    public:
        virtual std::string     getStringFromValue ( ) const;
        virtual bool            setValueFromString ( std::string f_val_str );

        /// Can/should this parameter be saved?
        virtual bool            shouldBeSaved() { return false; }

    /// Protected help methods.
    protected:
   };
}


#endif // __PARAMLINESEPARATOR_H
