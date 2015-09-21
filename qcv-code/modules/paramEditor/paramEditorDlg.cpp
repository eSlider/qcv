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
 * \file  paramEditorDlg.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include <QGridLayout>
#include <QSplitter>
#include <QTreeView>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QSettings>
#include <QGroupBox>
#include <QFileDialog>

#include "paramIOPFile.h"
#include "paramIOXmlFile.h"
#include "paramEditorDlg.h"
#include "paramTreeItemModel.h"
#include "parameterSet.h"
#include "parameter.h"

#include "paramLineSeparator.h"
#include "paramGroup.h"
#include "paramGroupEnd.h"

//#include "displayTreeNode.h"

using namespace QCV;

CParameterEditorDlg::CParameterEditorDlg  ( CParameterSet *    f_rootNode_p,
                                            QWidget *          f_parent_p,
                                            CParamIOHandling * f_parser_p )
        : QWidget (                      f_parent_p ),
          m_rootNode_p (               f_rootNode_p ),
          m_currentPage_p (                    NULL ),
          m_splitter_p (                       NULL ),
          m_qtvCategory_p (                    NULL ),
          m_qfParamPage_p (                    NULL ),
          m_parser_p (                   f_parser_p )
{
    std::locale::global(std::locale("C"));
    setWindowTitle ( tr("Parameter Editor") );
    setObjectName  ( windowTitle() );

    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/symbols/param.png")), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);
    
    /// Load button.
    QPushButton *pushButtonLoad_p = new QPushButton(this);
    pushButtonLoad_p->setObjectName(QString::fromUtf8("Load"));
    pushButtonLoad_p->setText(tr("Load"));
    connect(pushButtonLoad_p, SIGNAL(clicked(bool)), this, SLOT(load()));

    /// Save button.
    QPushButton *pushButtonSave_p = new QPushButton(this);
    pushButtonSave_p->setObjectName(QString::fromUtf8("Save"));
    pushButtonSave_p->setText(tr("Save"));
    connect(pushButtonSave_p, SIGNAL(clicked(bool)), this, SLOT(save()));   
    
    /// Splitter
    m_splitter_p = new QSplitter( this );
    //m_splitter_p -> move (100,100);
    m_splitter_p -> setOrientation ( Qt::Horizontal );

    /// Layout
    QGridLayout * gridLayout_p = new QGridLayout(this);

    gridLayout_p->setObjectName(QString::fromUtf8("GridLayout"));
    gridLayout_p->addWidget(m_splitter_p,     0, 0, 1, -1);
    gridLayout_p->addWidget(pushButtonLoad_p, 1, 0, 1, 1);
    gridLayout_p->addWidget(pushButtonSave_p, 1, 1, 1, 1);

    /// Tree View
    m_qtvCategory_p = new QTreeView( m_splitter_p );
    m_qtvCategory_p->setObjectName(QString::fromUtf8("Display Tree View Categories"));

    /// Add a scroll area.
    QScrollArea *scrollArea_p = new QScrollArea ( m_splitter_p );
    scrollArea_p -> setWidgetResizable ( true );

    m_qfParamPage_p = new QFrame ( scrollArea_p );
    m_qfParamPage_p -> setObjectName(QString::fromUtf8("Parameter Page"));
    scrollArea_p    -> setWidget( m_qfParamPage_p );

    //m_qfParamPage_p -> setSizePolicy ( QSizePolicy(QSizePolicy::MinimumExpanding, 
    //                                               QSizePolicy::MinimumExpanding ) );
    
    m_qfParamPage_p -> setFrameShape (  QFrame::Panel );
    m_qfParamPage_p -> setFrameShadow ( QFrame::Sunken );
    scrollArea_p -> resize ( 400, 800);
    m_splitter_p -> addWidget ( m_qtvCategory_p );
    m_splitter_p -> addWidget ( scrollArea_p );

    //QSettings settings;
    //m_splitter_p -> restoreState( settings.value("ParameterSplitter/position").toByteArray() );

    /// Tree View Model
    if ( f_rootNode_p )
    {
        CParamTreeItemModel *model_p = new CParamTreeItemModel ( f_rootNode_p );
        m_qtvCategory_p -> setModel ( model_p );
    }

    /// Grid layout in the frame.
    //new QGridLayout(m_qfParamPage_p);

    QGridLayout * paramPageLayout_p = new QGridLayout(m_qfParamPage_p);
    paramPageLayout_p -> setVerticalSpacing (25);

    connect ( m_qtvCategory_p, SIGNAL( clicked             ( const QModelIndex & )),
              this,            SLOT  ( reloadParameterPage ( const QModelIndex & )) );

    QSettings settings;
    restoreGeometry ( settings.value(objectName() + QString("/geometry")).toByteArray() );
    m_splitter_p -> restoreState( settings.value("ParameterSplitter/position").toByteArray() );
 
 }

CParameterEditorDlg::~CParameterEditorDlg ()
{
   if (isVisible())
   {
      QSettings settings;
      settings.setValue( objectName() + QString("/geometry"), 
                         saveGeometry() );
      
      settings.setValue("ParameterSplitter/position", m_splitter_p->saveState());
   }

   if ( m_qtvCategory_p -> selectionModel() )
      delete m_qtvCategory_p -> selectionModel();
}

void
CParameterEditorDlg::toggleVisibility (  )
{
   if (isVisible())
   {
      QSettings settings;
      settings.setValue( objectName() + QString("/geometry"), 
                         saveGeometry() );
      
      settings.setValue("ParameterSplitter/position", m_splitter_p->saveState());
   }
   
   setVisible(!isVisible());
}  

void
CParameterEditorDlg::reloadParameterPage ( const QModelIndex & f_index  )
{
    parameterPageExited ();
    
    m_currentPage_p = static_cast<CParameterSet *>(f_index.internalPointer());

    if (!m_currentPage_p) 
    {
        printf("There is no CParameterSet object for this category\n");
        return;
    }
    m_qfParamPage_p -> hide();
    
    QGridLayout * grid_p = static_cast<QGridLayout *>(m_qfParamPage_p->layout());
    QLayoutItem * layoutItem_p;
    
    while ( (layoutItem_p = grid_p -> itemAt( 0 ) ) )
    {
        grid_p -> removeItem ( layoutItem_p );

        // this means is a Qlabel that we want to delete.
        if ( layoutItem_p -> widget() )
            delete layoutItem_p -> widget();

        delete layoutItem_p;
    }
    
    /// Let set the size policy for the labels.
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    //sizePolicy.setHeightForWidth(false);
    
    CParameterGroup * group_p;
    const int maxGroupLevel_i = 16;
    /// 16 max levels.
    QGridLayout * groupGrids_p[maxGroupLevel_i];
    int currLevel_i = 0;
   
    groupGrids_p[0] = grid_p;
    
    for (unsigned int i = 0; i < m_currentPage_p -> getParameterCount(); ++i)
    {
        CParameter *  param_p    = m_currentPage_p -> getParameter ( i );
        //if ( param_p ) 
        //    printf("param_p->getName() = %s\n", param_p->getName().c_str());
        //else
        //    printf("param_p is NULL\n");
        
        if ( dynamic_cast<CParamLineSeparator *>(param_p) )
        {            
            QFrame *line_p = new QFrame( this );
            line_p -> setFrameShape ( QFrame::HLine );
            line_p -> setFrameShadow ( QFrame::Sunken );
            line_p -> setSizePolicy ( sizePolicy );
                
            grid_p       -> setRowMinimumHeight ( i, 1 );
            grid_p       -> addWidget ( line_p, i, 0, 1, -1);
        }
        else if (  (group_p = dynamic_cast<CParameterGroup *>(param_p) ) )
        {
            if ( currLevel_i < maxGroupLevel_i )
            {
                ++currLevel_i;
                QGroupBox *frame_p = new QGroupBox( group_p->getName().c_str(), this );
                //frame_p -> setFrameShape (  QGroupBox::Panel );
                //frame_p -> setFrameShadow ( QGroupBox::Sunken );
                frame_p -> setSizePolicy ( QSizePolicy(QSizePolicy::MinimumExpanding, 
                                                       QSizePolicy::MinimumExpanding) );
                
                /// Apply Color
                if (group_p -> applyColor ())
                {
                    SRgb color = group_p -> getColor();
                    
                    QPalette palette;
                    palette.setColor(frame_p->backgroundRole(), QColor(color.r, color.g, color.b, 255));
                    frame_p->setPalette(palette);
                    frame_p->setAutoFillBackground(true);
                    
                }
                
                grid_p       -> setRowMinimumHeight ( i, 1 );
                grid_p       -> addWidget ( frame_p, i, 0, 1, -1);
                
                grid_p = groupGrids_p[currLevel_i] = new QGridLayout ( frame_p );
            }        
            else
            {
                printf("The maximal amount of levels has been achieved. "
                       "No more parameter subgroupping possible.\n");
            }
        }
        else if (dynamic_cast<CParameterGroupEnd *>(param_p) )
        {
            if ( currLevel_i > 0 )
            {
                --currLevel_i;
                grid_p -> addItem ( new QSpacerItem ( 0, 0, 
                                                      QSizePolicy::Minimum, 
                                                      QSizePolicy::Expanding),
                                m_currentPage_p -> getParameterCount(), 0 );
                
                grid_p -> addItem ( new QSpacerItem ( 0, 0, 
                                                      QSizePolicy::Minimum, 
                                                      QSizePolicy::Expanding),
                                    m_currentPage_p -> getParameterCount(), 1, 1 );
                grid_p = groupGrids_p[currLevel_i];
            }        
            else
            {
                printf("Too many objects of CParameterGroupEnd for this page.\n");
            }
        }
        else
        {
            QWidget * paramEditor_p;
        
            param_p -> updateFromContainer();

            paramEditor_p = param_p -> createEditor();
            //printf("Editor %p created\n", paramEditor_p);

            if ( paramEditor_p )
            {
                if ( param_p -> showWithLabel() )
                {
                    QLabel *  newLabel_p = new QLabel ( this );
                    
                    newLabel_p -> setSizePolicy (QSizePolicy(QSizePolicy::Minimum, 
                                                             QSizePolicy::Fixed ) );
                    
                    newLabel_p -> setText       ( QString(param_p -> getName().c_str()) );
                    newLabel_p -> setToolTip    ( QString(param_p -> getComment().c_str()) );
                    
                    grid_p     -> addWidget ( newLabel_p, i, 0 );
                    
                    grid_p -> addItem ( new QSpacerItem ( 10, 0, 
                                                          QSizePolicy::Fixed, 
                                                          QSizePolicy::Fixed ), i, 1 );
                    
                    grid_p     -> addWidget ( paramEditor_p, i, 2);
                    grid_p     -> setRowMinimumHeight ( i, 2 );
                }
                else
                {
                    grid_p     -> addWidget ( paramEditor_p, i, 0);
                    grid_p     -> setRowMinimumHeight ( i, 0 );
                }
            }
        }

    } 

    // The spacers restrict the pairs label/parameter to lie on the top of the 
    // parameter page.
    // Otherwise, the parameter are centered in the middle of the page (unless
    // there are enogh pairs to cover all the page).
    //for (currLevel_i; currLevel_i >= 0;  --currLevel_i)
    {
        grid_p = groupGrids_p[currLevel_i];
        grid_p -> addItem ( new QSpacerItem ( 0, 0, 
                                              QSizePolicy::Minimum, 
                                              QSizePolicy::Expanding),
                        m_currentPage_p -> getParameterCount(), 0 );
        
        //grid_p -> addItem ( new QSpacerItem ( 0, 0, 
        //                                      QSizePolicy::Minimum, 
        //                                      QSizePolicy::Expanding),
        //                    m_currentPage_p -> getParameterCount(), 1, 1 );
    }
    
    m_qfParamPage_p -> show();    
}

void CParameterEditorDlg::save ( )
{
    QString qfileName_str;
    
    qfileName_str = QFileDialog::getSaveFileName( this,
                                                  tr("Open File"), 
                                                  QString(),
                                                  "XML files (*.xml);; P files (*.p);; Other (*)" );
    if (qfileName_str.isEmpty()) return;
    
    std::string fileName_str = qfileName_str.toStdString();

    size_t pos_i = fileName_str.find_last_of ( '.' );

    if (pos_i == std::string::npos )
    {
       printf("%s:%i Unrecognized file extension for file %s\n", __FILE__, __LINE__, fileName_str.c_str() );    
       return;  
    }

    std::string fileExtension_str = fileName_str.substr(pos_i);
    
    if (m_parser_p)
    {
       m_rootNode_p -> save ( *m_parser_p );
       m_parser_p->save ( fileName_str );
    }
    else
    {
       if (fileExtension_str == ".xml")
       {
          CParamIOXmlFile paramIO;
          m_rootNode_p -> save ( paramIO );
          paramIO.save ( fileName_str );
       }
       else if (fileExtension_str == ".p")
       {
          CParamIOPFile paramIO;
          m_rootNode_p -> save ( paramIO );
          paramIO.save ( fileName_str );
       }
       else
          printf("%s:%i Unrecognized file extension \"%s\" for file %s\n", __FILE__, __LINE__, fileExtension_str.c_str(), fileName_str.c_str() );
    }
}

void CParameterEditorDlg::load ( )
{
    QString qfileName_str;
    
    qfileName_str = QFileDialog::getOpenFileName( this,
                                                  tr("Open File"), 
                                                  QString(),
                                                  "XML files (*.xml);; P files (*.p);; Other (*)" );
    if (qfileName_str.isEmpty()) return;

    std::string fileName_str = qfileName_str.toStdString();
    
    size_t pos_i = fileName_str.find_last_of ( '.' );

    if (pos_i == std::string::npos )
    {
       printf("%s:%i Unrecognized file extension for file %s\n", __FILE__, __LINE__, fileName_str.c_str() );    
       return;  
    }

    std::string fileExtension_str = fileName_str.substr(pos_i);
    
    if ( m_parser_p )
    {
       m_parser_p -> load ( fileName_str );
       m_rootNode_p -> load ( *m_parser_p );
    }
    else
    {
       if (fileExtension_str == ".xml")
       {
          CParamIOXmlFile paramIO;
          paramIO.load ( fileName_str );
          m_rootNode_p -> load ( paramIO );
       }
       else if (fileExtension_str == ".p")       
       {
          CParamIOPFile paramIO;
          paramIO.load ( fileName_str );
          m_rootNode_p -> load ( paramIO );
       }
       else
          printf("%s:%i Unrecognized file extension \"%s\" for file %s\n", __FILE__, __LINE__, fileExtension_str.c_str(), fileName_str.c_str() );    
    }
}


void CParameterEditorDlg::parameterPageExited ()
{
    if ( m_currentPage_p )
    {
        for (unsigned int i = 0; i < m_currentPage_p -> getParameterCount(); ++i)
        {
            CParameter *  param_p    = m_currentPage_p -> getParameter ( i );
            //printf("Notifying of delection to %p\n", param_p -> getEditor());
            param_p -> notifyEditorsDeletion ();
        }
    }
}
