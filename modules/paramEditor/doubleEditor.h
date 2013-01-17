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


#ifndef __DOUBLEEDITOR_H
#define __DOUBLEEDITOR_H

/**
 *******************************************************************************
 *
 * @file doubleEditor.h
 *
 * \class CDoubleEditor
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements an editor for an object of type CDoubleParameter.
 *
 * The class implements a simple Qt editor with one input box where
 * the user enters a double value.
 *
 ******************************************************************************/

/* INCLUDES */
#include <QtGui/QWidget>

/* PROTOTYPES */
class QLineEdit;

namespace QCV
{
    /* PROTOTYPES */

    class CDoubleEditor : public QWidget
    {
        Q_OBJECT
        public:
        
        /// Constructors/Destructor
        CDoubleEditor ( double     f_value_d,
                        QWidget *  f_parent_p = NULL );

        virtual ~CDoubleEditor ();

        double getValue() const 
        { return m_value_d; }

        bool   setValue( double f_value_d );
                         
        unsigned int getDecimals() const 
        { return m_maxDecimals_ui; }

        void         setDecimals( unsigned int f_maxDec_ui ) 
        { m_maxDecimals_ui = f_maxDec_ui; }
        
        virtual QSize sizeHint () const;

    protected slots:
        void lineEditChanged();

    signals:
        void valueChanged( double );
        
        /// Protected members
    protected:

        /// Stored value.
        QLineEdit *          m_qle_p;

        /// Double value
        double               m_value_d;

        /// Double value
        unsigned int         m_maxDecimals_ui;
    };
}

#endif // __DOUBLEEDITOR_H
