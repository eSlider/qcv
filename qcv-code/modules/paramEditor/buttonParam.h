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


#ifndef __BUTTONPARAM_H
#define __BUTTONPARAM_H

/**
 *******************************************************************************
 *
 * @file buttonParam.h
 *
 * \class CButtonParameter
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Parameter interface for a button.
 *
 * The class implements a parameter interface for a button.
 *
 *******************************************************************************/

/* INCLUDES */
#include "parameter.h"

namespace QCV
{
    /* PROTOTYPES */

    class CButtonParameter : public CParameter
    {
    public:
        
        /// Constructors/Destructor
        CButtonParameter (  std::string                f_name_str = "",
                            std::string                f_comment_str = "",
                            CParameterBaseConnector *  f_connector_p = NULL );

        virtual ~CButtonParameter ();

    public:
        /// Get and set values from strings.
        virtual std::string     getStringFromValue ( ) const;
        virtual bool            setValueFromString ( std::string f_val_str );

        /// Get editor.
        virtual QWidget *       createEditor ( );


        /// Should this parameter have an associated label in the GUI?
        virtual bool            showWithLabel() { return false; }

        /// Can/should this parameter be saved?
        virtual bool            shouldBeSaved() { return false; }

    public slots:
        virtual void clicked();

    /// Protected members
    protected:

        /// No stored value.
    };
}


#endif // __BUTTONPARAM_H
