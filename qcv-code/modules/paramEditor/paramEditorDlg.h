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


#ifndef __PARAMEDITORDLG_H
#define __PARAMEDITORDLG_H

/**
 *******************************************************************************
 *
 * @file paramEditorDlg.h
 *
 * \class CParameterEditorDlg
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Class implementing the dialog where parameters are shown and can be
 * edited. 
 *
 * The dialog is composed of three parts. The top/left sections shows
 * a tree structure with the parameter sets. The top/right section show the actual
 * parameters for the active parameter set in the top/left section. The lower 
 * section has some buttons for storing and reading parameters from files.
 *
 ******************************************************************************/

/* INCLUDES */
#include <QtGui/QWidget>
#include <QModelIndex>
/* CONSTANTS */

/* PROTOTYPES */
class QGLWidget;
class QSplitter;
class QTreeView;
class QFrame;
class QSplitter;
class CParamIOPFile;

#include "paramIOHandling.h"

namespace QCV
{

/* PROTOTYPES */
    class CParameterSet;
    //class CDisplayOpNode;

/* CLASS DEFINITION */

    class CParameterEditorDlg: public QWidget
    {
        Q_OBJECT
        
        public:
        CParameterEditorDlg (  CParameterSet *    f_rootNode_p,
                               QWidget *          f_parent_p,
                               CParamIOHandling * f_parser_p = NULL );
        ~CParameterEditorDlg ();
        
    /// Protected slots
    protected slots:

        void reloadParameterPage ( const QModelIndex & f_index  );
        void save ( );
        void load ( );
        void toggleVisibility ( );

    /// Protected help methods.
    protected:
        void parameterPageExited();
        
        
    private:

        /// Root node.
        CParameterSet *                   m_rootNode_p;

        /// Root node.
        CParameterSet *                   m_currentPage_p;

        /// Splitter to separate categorie tree from parameters.
        QSplitter *                       m_splitter_p;
        
        /// Display tree view for categories.
        QTreeView *                       m_qtvCategory_p;

        /// Display tree view for categories.
        QFrame *                          m_qfParamPage_p;

        /// Parser
        CParamIOHandling *                m_parser_p;
    };
}


#endif // __PARAMEDITORDLG_H
