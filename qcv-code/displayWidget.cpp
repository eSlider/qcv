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


#include <Qt>
#include <QtGui>
#include <QGLWidget>
#include <QSettings>
#include <QInputDialog>

#include "displayWidget.h"
#include "helpWidget.h"
#include "display.h"
#include "drawingListHandler.h"
#include "displayTreeDlg.h"
#include "displayTreeNode.h"

using namespace QCV;

CDisplayWidget::CDisplayWidget( QWidget *              f_parent_p,
                                CDrawingListHandler *  f_handler_p,
                                int                    f_screenWidth_i,
                                int                    f_screenHeight_i )
        : QWidget (                      f_parent_p ),
          m_drawingListHandler_p (      f_handler_p ),
          m_treeDlg_p (                        NULL ),
          m_glDisplay_p (                      NULL ),
          m_grabbing_i (                          0 ),

          m_qfTopControls_p (                  NULL ),
          m_qfNumScreens_p (                   NULL ),
          m_qlabel1_p (                        NULL ),
          m_qsbNumHorScreens_p (               NULL ),
          m_qsbNumVertScreens_p (              NULL ),
    m_qsbSkipDisplay_p (                 NULL ),
          m_qfStatusBar_p (                    NULL ),
          m_qStatusBarSplitter_p (             NULL ),
          m_qlUserMessage_p (                  NULL ),
          m_qlSystemMessage_p (                NULL ),
          m_qpbDrawingList_p (                 NULL ),
// 0 is on update only. 1 is on every paintgl event
          m_qtwHelp_p (                        NULL ),
          m_imgBuffer_v (                         0 ),
          m_grabTimerId_i (                       0 ),
    m_saveImgsTimerId_i (                   0 ),
    m_counter_ui (                          0 )
{
    setWindowTitle(tr("Main Display"));
    setObjectName(windowTitle());

    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/symbols/display.png")), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);

    if (m_drawingListHandler_p)
    {
        createForm();   
        m_glDisplay_p->setScreenSize(S2D<unsigned int> ( 
                                             f_screenWidth_i,
                                             f_screenHeight_i ) );                      

        QSettings qSettings;
        
        m_qsbNumHorScreens_p  -> setValue ( qSettings.value(objectName() + QString("/num_x_screens"), 3 ).toInt() );
        m_qsbNumVertScreens_p -> setValue ( qSettings.value(objectName() + QString("/num_y_screens"), 3 ).toInt() );
        m_qsbSkipDisplay_p    -> setValue ( qSettings.value(objectName() + QString("/skip_display"),  0 ).toInt() );

        m_treeDlg_p = new CDisplayTreeDlg ( NULL,
                                            m_glDisplay_p,
                                            m_drawingListHandler_p -> getRootNode() );

        m_drawingListHandler_p -> setScreenSize(m_glDisplay_p->getScreenSize());
        

        /// Connections:
        connect( m_glDisplay_p, SIGNAL(fullScreenSwitched ( )), 
                 this,          SLOT(  switchFullScreen ( )));

        connect( m_glDisplay_p, SIGNAL(exitFullScreen ( )), 
                 this,          SLOT(  exitFullScreen ( )));

        connect( m_glDisplay_p, SIGNAL(keyPressed ( CKeyEvent *)), 
                 this,          SLOT(  keyPressed ( CKeyEvent *)));

        connect( m_glDisplay_p, SIGNAL(mouseMoved ( CMouseEvent *)),
                 this,          SLOT(  mouseMoved ( CMouseEvent *)));

        connect( m_qsbNumHorScreens_p,  SIGNAL( valueChanged ( int )),
                 this,                  SLOT(   updateScreenCount ( ) ) );

        connect( m_qsbNumVertScreens_p, SIGNAL( valueChanged ( int )),
                 this,                  SLOT(   updateScreenCount ( ) ) );

        connect( m_qsbSkipDisplay_p,    SIGNAL( valueChanged ( int )),
                 this,                  SLOT(   skipDisplay ( ) ) );
        connect( m_qpbDrawingList_p, SIGNAL(  clicked ( )), 
                 this,               SLOT(    showHideTreeDlg() ));
        
        updateScreenCount ( );
   }

    m_qtwHelp_p = new CHelpWidget(NULL);

    connect ( m_glDisplay_p, SIGNAL( glPainted() ), 
              this,          SLOT(   glJustPainted() ) );

    QSettings settings;
    QString name = QString("CDisplayWidget/geometry/") + (f_parent_p?f_parent_p->objectName():QString("default"));
    restoreGeometry(settings.value(name).toByteArray());

    /// Setup image format for grabbing.
    m_items << tr("jpg") << tr("png") << tr("ppm");
    m_imgFormat_str = m_items[0].toStdString();
    
}

CDisplayWidget::~CDisplayWidget()
{
    if ( m_treeDlg_p )
        delete m_treeDlg_p;    
}

void 
CDisplayWidget::closeEvent(QCloseEvent *f_event_p)
{
    QSettings settings;
    QString name = QString("CDisplayWidget/geometry/") + (parent()?parent()->objectName():QString("default"));
    settings.setValue(name, saveGeometry());

    m_treeDlg_p -> close();
    
    QWidget::closeEvent(f_event_p);
}

void CDisplayWidget::createForm()
{
    /// Main layout
    QGridLayout *gridLayout_p = new QGridLayout(this);
    gridLayout_p->setContentsMargins ( 0, 0, 0, 0);

    m_qfTopControls_p = new QFrame(this);

  
   QSizePolicy sizePolicyFixed(QSizePolicy::Fixed, QSizePolicy::Fixed);

    /// Size policy for frame containing spin boxes.
    sizePolicyFixed.setHorizontalStretch(0);
    sizePolicyFixed.setVerticalStretch(0);
    sizePolicyFixed.setHeightForWidth(true);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    /// Size policy top control.
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(m_qfTopControls_p->sizePolicy().hasHeightForWidth());
 
    m_qfTopControls_p->setSizePolicy(sizePolicyFixed);
    m_qfTopControls_p->setFrameShape(QFrame::StyledPanel);
    m_qfTopControls_p->setFrameShadow(QFrame::Raised);
    m_qfTopControls_p->setMinimumSize(QSize(225, 31));
    m_qfTopControls_p->setMaximumSize(QSize(225, 31));

    /// Grid layout for top controls.
    QGridLayout *layoutTopCtrls_p = new QGridLayout(m_qfTopControls_p);
    //m_qpbShowAllScreens_p = new QPushButton(m_qfTopControls_p);

    /// Button Show all Screens.

    //layoutTopCtrls_p->addWidget(m_qpbShowAllScreens_p, 0, 0, 1, 1);

 
    QFrame *qfButton_p = new QFrame(m_qfTopControls_p);

    qfButton_p->setSizePolicy(sizePolicyFixed);
    qfButton_p->setMinimumSize(QSize(21, 21));
    qfButton_p->setMaximumSize(QSize(21, 21));
    qfButton_p->setFrameShape(QFrame::NoFrame);
    qfButton_p->setFrameShadow(QFrame::Raised);

    // Button action.
    m_qpbDrawingList_p = new QPushButton(qfButton_p);

    m_qpbDrawingList_p->setSizePolicy(sizePolicyFixed);
    m_qpbDrawingList_p->setMinimumSize(QSize(21, 21));
    m_qpbDrawingList_p->setMaximumSize(QSize(21, 21));
    m_qpbDrawingList_p->setGeometry(QRect(0, 0, 21, 21));

    layoutTopCtrls_p->addWidget(qfButton_p, 0, 0, 1, 1);
    layoutTopCtrls_p->setContentsMargins ( 5, 0, 0, 5);
    /// Line Edits for selecting number of screens.
    m_qfNumScreens_p = new QFrame(m_qfTopControls_p);

    m_qfNumScreens_p->setSizePolicy(sizePolicyFixed);
    m_qfNumScreens_p->setMinimumSize(QSize(225, 21));
    m_qfNumScreens_p->setMaximumSize(QSize(225, 21));
    m_qfNumScreens_p->setFrameShape(QFrame::NoFrame);
    m_qfNumScreens_p->setFrameShadow(QFrame::Raised);
    m_qfNumScreens_p->setGeometry(QRect(100,0,225,21));

    /// "X" label
    m_qlabel1_p = new QLabel(m_qfNumScreens_p);
    m_qlabel1_p->setGeometry(QRect(45, 0, 16, 19));
    m_qlabel1_p->setSizePolicy(sizePolicyFixed);

    /// Spin boxes.
    m_qsbNumHorScreens_p = new QSpinBox(m_qfNumScreens_p);
    m_qsbNumHorScreens_p->setGeometry(QRect(0, 0, 41, 21));
    m_qsbNumHorScreens_p->setMinimum(1);
    m_qsbNumHorScreens_p->setMaximum(9);
    m_qsbNumHorScreens_p->setValue(3);
    

    m_qsbNumVertScreens_p = new QSpinBox(m_qfNumScreens_p);
    m_qsbNumVertScreens_p->setGeometry(QRect(60, 0, 41, 21));
    m_qsbNumVertScreens_p->setMinimum(1);
    m_qsbNumVertScreens_p->setMaximum(9);
    m_qsbNumVertScreens_p->setValue(3);
    {
        QLabel *label_p = new QLabel(m_qfNumScreens_p);
        label_p->setText("Skip");
        label_p->setGeometry(QRect(110, 0, 30, 21));
    }

    m_qsbSkipDisplay_p = new QSpinBox(m_qfNumScreens_p);
    m_qsbSkipDisplay_p->setGeometry(QRect(140, 0, 54, 21));
    m_qsbSkipDisplay_p->setMinimum(0);
    m_qsbSkipDisplay_p->setMaximum(1000);
    m_qsbSkipDisplay_p->setValue(0);

    /// Add to the layout of the top controls.
    layoutTopCtrls_p->addWidget(m_qfNumScreens_p, 0, 1, 1, 1);
    
    m_qfTopControls_p->setSizePolicy(sizePolicyFixed);    
    m_qfTopControls_p->setGeometry(0,0,130,21);
    m_qfTopControls_p->setFrameShape(QFrame::NoFrame);
    m_qfTopControls_p->setFrameShadow(QFrame::Raised);
    
    gridLayout_p->addWidget(m_qfTopControls_p, 1, 0, 1, 1);

    /// Main Display
    m_dispFrame_p = new QFrame(this);
    m_dispFrame_p->setFrameShape (QFrame::StyledPanel);//(QFrame::NoFrame);
    m_dispFrame_p->setFrameShadow(QFrame::Raised);     //(/QFrame::Plain);
    
    m_glDisplay_p = new CDisplay ( m_drawingListHandler_p -> getRootNode(), m_dispFrame_p );
    QGridLayout *dispLayout_p = new QGridLayout ( m_dispFrame_p );
    dispLayout_p->addWidget ( m_glDisplay_p );
    dispLayout_p->setContentsMargins ( 0, 0, 0, 0);

    QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(m_glDisplay_p->sizePolicy().hasHeightForWidth());
    m_glDisplay_p->setSizePolicy(sizePolicy3);
    m_dispFrame_p->setSizePolicy(sizePolicy3);

    gridLayout_p->addWidget(m_dispFrame_p, 0, 0, 1, 2);

    /// Status bar
    m_qfStatusBar_p = new QFrame(this);

    QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(m_qfStatusBar_p->sizePolicy().hasHeightForWidth());

    m_qfStatusBar_p->setSizePolicy(sizePolicy4);
    m_qfStatusBar_p->setFrameShape (QFrame::StyledPanel);
    m_qfStatusBar_p->setFrameShadow(QFrame::Raised);     

    m_qfStatusBar_p->setMaximumSize(QSize(16777215, 33));

    QHBoxLayout * horLayout_p = new QHBoxLayout(m_qfStatusBar_p);

    m_qStatusBarSplitter_p = new QSplitter(m_qfStatusBar_p);
    m_qStatusBarSplitter_p->setOrientation(Qt::Horizontal);

    m_qlUserMessage_p = new QLabel(m_qStatusBarSplitter_p);

    //sizePolicy4.setHeightForWidth(m_qlUserMessage_p->sizePolicy().hasHeightForWidth());

    m_qlUserMessage_p->setSizePolicy(sizePolicy4);
    m_qlUserMessage_p->setMaximumSize(QSize(16777215, 16777215));

    m_qStatusBarSplitter_p->addWidget(m_qlUserMessage_p);

    m_qlSystemMessage_p = new QLabel(m_qStatusBarSplitter_p);

    //sizePolicy4.setHeightForWidth(m_qlSystemMessage_p->sizePolicy().hasHeightForWidth());
    m_qlSystemMessage_p->setSizePolicy(sizePolicy4);
    m_qlSystemMessage_p->setMaximumSize(QSize(16777215, 16777215));

    m_qStatusBarSplitter_p->addWidget(m_qlSystemMessage_p);

    horLayout_p->addWidget(m_qStatusBarSplitter_p);

    gridLayout_p->addWidget(m_qfStatusBar_p, 1, 1, 1, 1);

    //m_qpbShowAllScreens_p->setText(QString());
    m_qpbDrawingList_p->setText(QString());
    //m_qpbAction3_p->setText(QString());
    m_qlabel1_p->setText("X");

}


QWidget * CDisplayWidget::getDialog() const
{
    return m_treeDlg_p;
}

CDisplay * CDisplayWidget::getDisplay() const
{
    return m_glDisplay_p;
}

void CDisplayWidget::update ( int f_forceUpdate_b )
{
    if ( f_forceUpdate_b ||
         m_drawingListHandler_p -> mustUpdateDisplay ( ) )
    {
        update();
    }
}

void CDisplayWidget::update()
{    
    ++m_counter_ui;
    if ( m_qsbSkipDisplay_p && (m_qsbSkipDisplay_p->value()==0 || !(m_counter_ui % (unsigned int)m_qsbSkipDisplay_p->value()) ) )
    m_glDisplay_p -> updateGL();
    
    QWidget::update();

    m_drawingListHandler_p -> setDisplayUpdateFlag ( false );

    if ( m_grabbing_i == 1 )
        grabAndSaveFrame();
}

void CDisplayWidget::glJustPainted()
{
    if ( m_grabbing_i == 2 )
        grabAndSaveFrame();
}

void CDisplayWidget::grabAndSaveFrame()
{
    if (m_grabbing_i)
    {
        static int imgNr_i = 0;
        QImage saveImage = m_glDisplay_p -> renderGL();
        
        char fileName_p[256];
        
        sprintf(fileName_p, "grabbedDisplayWidgetImg_%05i.%s", imgNr_i, m_imgFormat_str.c_str() );
        
        saveImage.save( fileName_p, m_imgFormat_str.c_str(), 100 );
        
        printf("file \"%s\" saved\n", fileName_p);

        ++imgNr_i;
    }
}

void 
CDisplayWidget::stopGrabbingAndSaveImgs()
{
    m_grabbing_i = 0;
    m_saveImgsTimerId_i = startTimer ( 1 );
    /// We might want to check here if timer has failed,
    /// in order to save all imgs in a single loop.
}

void CDisplayWidget::grabFrameAndStoreInBuffer()
{
    if (m_grabbing_i)
    {
        QImage * saveImage_p = new QImage();

        if (!saveImage_p)
        {
            /// Buffer full?  
            stopGrabbingAndSaveImgs();
            return;
        }

        *saveImage_p = m_glDisplay_p -> renderGL();
        
        m_imgBuffer_v.push_back ( saveImage_p );

        if ( (m_imgBuffer_v.size() % 100) == 0 )
            printf("Image buffer size is %zi\n", m_imgBuffer_v.size());
    }
}


void CDisplayWidget::switchFullScreen ()
{
    static Qt::WindowStates prevState;
    if ( not isInFSMode() )
    {
        prevState = m_glDisplay_p -> windowState ();

        static_cast<QGridLayout *>(m_dispFrame_p -> layout()) -> removeWidget(m_glDisplay_p);

        m_glDisplay_p -> setParent ( NULL );

        QDesktopWidget *desktop_p = QApplication::desktop();

        int myDesktop_i = desktop_p->screenNumber(this);
        QRect rect = desktop_p->screenGeometry(myDesktop_i);

        m_glDisplay_p -> setWindowState ( Qt::WindowStates((unsigned int)Qt::WindowFullScreen | (unsigned int)Qt::WindowStaysOnTopHint | (unsigned int)Qt::FramelessWindowHint) );
        m_glDisplay_p -> setGeometry(rect);
        m_glDisplay_p -> show();
    }
    else
    {
        m_glDisplay_p -> setWindowState (prevState);

        if (m_dispFrame_p->layout()) delete m_dispFrame_p->layout();
        
        QGridLayout *dispLayout_p = new QGridLayout ( m_dispFrame_p );
        dispLayout_p->addWidget ( m_glDisplay_p );
        dispLayout_p->setContentsMargins ( 0, 0, 0, 0);
        m_glDisplay_p -> setParent ( m_dispFrame_p );
        m_glDisplay_p -> show();
        m_glDisplay_p -> setFocus();
    }
}

void
CDisplayWidget::exitFullScreen ()
{
    if ( isInFSMode() )
        switchFullScreen();
}

void
CDisplayWidget::keyPressed ( CKeyEvent * f_keyEvent_p )
{
    if (f_keyEvent_p -> qtKeyEvent_p -> key() == Qt::Key_G && m_saveImgsTimerId_i == 0)
    {
        if (m_grabbing_i) 
        {
            if (m_grabbing_i == 3)
            {
                stopGrabbingAndSaveImgs();
            }
            else
            {
                m_grabbing_i = 0;
            }

            killTimer ( m_grabTimerId_i );
        }
        else
        {
            bool ok_b;
            
            m_imgFormat_str = QInputDialog::getItem(this, 
                                                    "Display Grabbing",
                                                    "Image Format", 
                                                    m_items, 
                                                    0, 
                                                    false, 
                                                    &ok_b).toStdString();
            if ( ok_b )
            {
                if ( !(f_keyEvent_p->qtKeyEvent_p->modifiers() & Qt::AltModifier) && 
                     f_keyEvent_p->qtKeyEvent_p->modifiers() & Qt::ControlModifier )
                    m_grabbing_i = 2;
                if ( f_keyEvent_p->qtKeyEvent_p->modifiers() & Qt::AltModifier && 
                     f_keyEvent_p->qtKeyEvent_p->modifiers() & Qt::ControlModifier )
                {
                    int ms_i = QInputDialog::getInt ( this, 
                                                      "Display Grabbing", 
                                                      "Enter grabbing time interval", 
                                                      100, 
                                                      0, 
                                                      2147483647, 
                                                      1, 
                                                      &ok_b );
                    if (ok_b)
                    {
                        printf("Starting timer with %i ms\n", ms_i);
                        m_grabTimerId_i = startTimer ( ms_i );
                        m_grabbing_i = 3;
                    }                
                }
                else
                    m_grabbing_i = 1;
            }        
        }
    }

    if (f_keyEvent_p -> qtKeyEvent_p -> key() == Qt::Key_Escape && m_saveImgsTimerId_i != 0)
    {
        killTimer(m_saveImgsTimerId_i);
        m_saveImgsTimerId_i = 0;
        
        for (unsigned int i = 0; i < m_imgBuffer_v.size(); ++i)
        {
            delete m_imgBuffer_v[i];
        }
        m_imgBuffer_v.clear();
    }
    
    else if (f_keyEvent_p -> qtKeyEvent_p -> key() == Qt::Key_H)
    {
        m_qtwHelp_p -> show();
        m_qtwHelp_p -> raise();
    }    
}

void CDisplayWidget::timerEvent ( QTimerEvent * f_event_p )
{
    printf("Timer Event with id = %i bufferId = %i\n", f_event_p->timerId(), m_grabTimerId_i);
    
    if ( f_event_p->timerId() == m_grabTimerId_i )
    {    
        if ( m_grabbing_i == 3 )
            grabFrameAndStoreInBuffer();
        else
        {
            m_grabbing_i = 0;
            /// Timer should be active otherwise
            killTimer(m_grabTimerId_i);
            m_grabTimerId_i = 0;
        }
    }
    else if ( f_event_p->timerId() == m_saveImgsTimerId_i )
    {
        m_grabbing_i = 0;

        if ( m_imgBuffer_v.empty() )
        {
            killTimer ( m_saveImgsTimerId_i );
            m_saveImgsTimerId_i = 0;
        }
        else
        {
            char fileName_p[256];
            
            sprintf(fileName_p, "grabbedDisplayWidgetImg_%05i.%s", (int)(m_imgBuffer_v.size())-1, m_imgFormat_str.c_str() );
            
            m_imgBuffer_v.back()->save( fileName_p, m_imgFormat_str.c_str(), 100 );
            
            printf("file \"%s\" saved\n", fileName_p);
            
            delete m_imgBuffer_v.back();
            m_imgBuffer_v.pop_back();
        }
    }   
    printf("finished\n");
}


void
CDisplayWidget::mouseMoved ( CMouseEvent *  f_event_p )
{
    char str[4096];

    snprintf(str, 4096, "(Display Size: %ix%i) Screen [%i,%i] - Abs Pos [%7.2f,%7.2f] - Scr Pos [%7.2f,%7.2f]\n",
             m_glDisplay_p->width(),
             m_glDisplay_p->height(),
             f_event_p->displayScreen.x,
             f_event_p->displayScreen.y,
             f_event_p->posInDisplay.x,
             f_event_p->posInDisplay.y,
             f_event_p->posInScreen.x,
             f_event_p->posInScreen.y );

    m_qlSystemMessage_p -> setText( str );    
}

void
CDisplayWidget::setScreenCount ( const S2D<unsigned int> f_size )
{
    m_qsbNumHorScreens_p->setValue(f_size.x);
    m_qsbNumVertScreens_p->setValue(f_size.y);    
}

S2D<unsigned int>
CDisplayWidget::getScreenCount ( ) const
{
    return S2D<unsigned int> ( m_qsbNumHorScreens_p->value(), 
                               m_qsbNumVertScreens_p->value());
}

void
CDisplayWidget::updateScreenCount ( )
{
    m_glDisplay_p->setScreenCount ( S2D<unsigned int> (m_qsbNumHorScreens_p->value(),
                                                       m_qsbNumVertScreens_p->value()));

    QSettings qSettings;
        
    qSettings.setValue( objectName() + QString("/num_x_screens"), m_qsbNumHorScreens_p->value() );
    qSettings.setValue( objectName() + QString("/num_y_screens"), m_qsbNumVertScreens_p->value() );
    update();
}

void
CDisplayWidget::setSkipDisplay ( int f_n_i )
{
    m_qsbSkipDisplay_p->setValue(f_n_i);
}

int
CDisplayWidget::getSkipDisplay ( ) const
{
    return m_qsbSkipDisplay_p->value();
}

void
CDisplayWidget::updateSkipDisplay ( )
{
    //m_glDisplay_p->setSkipDisplay ( m_qsbSkipDisplay_p->value() );

    QSettings qSettings;
        
    qSettings.setValue( objectName() + QString("/skip_display"), m_qsbSkipDisplay_p->value() );
}

bool
CDisplayWidget::setScreenSize ( const S2D<unsigned int> f_size )
{
    m_glDisplay_p->setScreenSize ( f_size );
    m_drawingListHandler_p -> setScreenSize ( m_glDisplay_p->getScreenSize() );
    return true;
}

void
CDisplayWidget::showHideTreeDlg()
{
    if ( m_treeDlg_p->isHidden() )
        m_treeDlg_p->show();
    else
        m_treeDlg_p->hide();
}
