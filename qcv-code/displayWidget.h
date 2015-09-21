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

#ifndef __DISPLAYWIDGET_H
#define __DISPLAYWIDGET_H

//#include <QtGui/QWidget>
#include <QtGui/QMainWindow>
#include <QApplication>

#include "display.h"

class QSpinBox;
class QSplitter;
class QFrame;
class QPushButton;
class QLabel;

namespace QCV
{
/* NAMESPACE PROTOTYPES */
    class CDrawingListHandler;
    class CDisplayTreeDlg;
    class CDisplayOpNode;
    
/* CLASS DEFINITION */
    class CDisplayWidget : public QWidget
    {
        Q_OBJECT
        
    public:
        CDisplayWidget( QWidget *              f_parent_p,
                        CDrawingListHandler *  f_handler_p,
                        int                    f_screenWidth_i,
                        int                    f_screenHeight_i );
                      
        virtual ~CDisplayWidget();

        QWidget  * getDialog()  const;
        CDisplay * getDisplay() const;

    public slots:
        /// Force update of display and status bars.
        virtual void update ( );

        /// Conditional update of display and status bars.
        virtual void update ( int f_forceUpdate_b );

        /// Switch full screen mode.
        virtual void switchFullScreen ( );
 
        /// Exit full screen mode.
        virtual void exitFullScreen ( );

        /// Force update of display and status bars.
        virtual void keyPressed ( CKeyEvent * f_keyEvent_p );        

        /// Mouse move event.
        virtual void mouseMoved ( CMouseEvent *  f_event_p );

        /// Update the amount of screens.
        virtual void updateScreenCount ( );

        /// Update the skip display number.
        virtual void updateSkipDisplay ( );
        /// Set the size of each scren
        virtual bool setScreenSize ( const S2D<unsigned int> f_size );

        /// Set how many  screens are displayed
        virtual void setScreenCount( const S2D<unsigned int> f_size );

        /// Set how many  screens are displayed
        virtual S2D<unsigned int> getScreenCount (  ) const;
        /// Set skip display
        void setSkipDisplay ( int f_n_i );
        
        /// Get skip display
        int  getSkipDisplay ( ) const;

        /// Show and hide tree dialog
        virtual void showHideTreeDlg();
        
    protected slots:
        virtual void glJustPainted();

        virtual void timerEvent ( QTimerEvent * f_event_p );
        
    protected:
        void closeEvent(QCloseEvent *f_event_p);
        
    signals:
            
    private slots:

    private:
        bool isInFSMode() const;

        void createForm(); 

        void grabAndSaveFrame();

        void grabFrameAndStoreInBuffer();

        void stopGrabbingAndSaveImgs();

        QString keyboardString();

        void help();
        
    private:

        /// Drawing list handler.
        CDrawingListHandler *  m_drawingListHandler_p;

        /// Display Tree Dialog.
        CDisplayTreeDlg *      m_treeDlg_p;

        /// GL Display
        CDisplay *             m_glDisplay_p;

        /// Is grabbing?
        int                    m_grabbing_i;

        /// Top controls frame
        QFrame *               m_dispFrame_p;
        
        /// Top controls frame
        QFrame *               m_qfTopControls_p;
        
        /// Frame containing qspin boxes.
        QFrame *               m_qfNumScreens_p;

        /// Label "X"
        QLabel *               m_qlabel1_p;

        /// Spin box for horizontal screens.
        QSpinBox *             m_qsbNumHorScreens_p;

        /// Spin box for vertical screens.
        QSpinBox *             m_qsbNumVertScreens_p;
        /// Spin box for vertical screens.
        QSpinBox *             m_qsbSkipDisplay_p;

        /// QFrame for the status bar.
        QFrame *               m_qfStatusBar_p;

        /// Splitter of user and systems messages.
        QSplitter *            m_qStatusBarSplitter_p;

        /// User message label.
        QLabel *               m_qlUserMessage_p;

        /// System message label.
        QLabel *               m_qlSystemMessage_p;

        /// Show drawing list tree
        QPushButton *          m_qpbDrawingList_p;

        /// Help
	QTabWidget *           m_qtwHelp_p;

        /// Buffer for grabbing
        std::vector<QImage *>  m_imgBuffer_v;
        
        /// Grab timer id
        int                    m_grabTimerId_i;
        
        /// Save grabbed images timer id
        int                    m_saveImgsTimerId_i;
      
        /// Save image format
        std::string            m_imgFormat_str;

        /// Formats
        QStringList            m_items;                
        /// Counter
        unsigned int           m_counter_ui;
   };

    inline bool 
    CDisplayWidget::isInFSMode() const
    {
        return !m_glDisplay_p -> parent();
    }
}

#endif // __DISPLAYWIDGET_H
