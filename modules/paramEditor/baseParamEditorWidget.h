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


#ifndef __BASEPARAMEDITORWIDGET_H
#define __BASEPARAMEDITORWIDGET_H

/*@@@**************************************************************************
 ** \file  baseParamEditorWidget
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****          (C) COPYRIGHT Hernan Badino - All Rights Reserved          *****
******************************************************************************/

/* INCLUDES */
#include <QtGui/QWidget>

/* CONSTANTS */
namespace QCV
{
    /* PROTOTYPES */

    class CBaseParamEditorWidget: public QWidget
    {
        Q_OBJECT

    public:
        
        /// Constructors/Destructor
        CBaseParamEditorWidget ( QWidget *  f_parent_p ) :
                QWidget (f_parent_p )
        {}
        
        virtual ~CBaseParamEditorWidget () {}
        

    public:
        virtual bool updateWidget() { return false; }
    };
}

#endif // __BASEPARAMEDITORWIDGET_H
