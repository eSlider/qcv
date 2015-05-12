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


#ifndef __UINTEDITOR_H
#define __UINTEDITOR_H

/**
 *******************************************************************************
 *
 * @file uintEditor.h
 *
 * \class CUIntEditor
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements an editor for an object of type CUIntParameter.
 *
 * The class implements a simple Qt editor with one input box where
 * the user enters a unsigned integer value.
 *
 *******************************************************************************/

/* INCLUDES */
#include <QtGui/QWidget>

/* PROTOTYPES */
class QLineEdit;

namespace QCV
{
    /* PROTOTYPES */

    class CUIntEditor : public QWidget
    {
        Q_OBJECT
        public:
        
        /// Constructors/Destructor
        CUIntEditor ( unsigned int   f_value_ui,
                      QWidget *      f_parent_p = NULL );
        
        virtual ~CUIntEditor ();

        unsigned int getValue() const { return m_value_ui; }
        
        bool   setValue( unsigned int f_value_ui );
        
        virtual QSize sizeHint () const;

    protected slots:
        void lineEditChanged();

    signals:
        void valueChanged( unsigned int );
        
        /// Protected members
    protected:

        /// Stored value.
        QLineEdit *          m_qle_p;

        /// Unsigned int value
        unsigned int         m_value_ui;
    };
}

#endif // __UINTEDITOR_H

