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

#include <QApplication>

#include "cinterface.h"

using namespace QCV;

bool callBackKeyboard    ( CKeyEvent *f_kevent_p );
bool callBackTimer       ( CTimerEvent *f_cevent_p );

void loadFrame (  );
void registerDrawingLists ( );

typedef std::vector <std::string>   CStringVector;
typedef std::vector <CStringVector> CStringVectorVector;

/// Filenames for all cameras
CStringVectorVector      g_files_v;
/// Current frame
int                      g_counter_i;
/// Max number of frames between all cameras
int                      g_maxImages_i;

int main(int f_argc_i, char *f_argv_p[])
{
    if ( f_argc_i < 3 || std::string(f_argv_p[1]) != "--cam" )
    {
        printf("Usage %s --cam img01 img02 ... img0N --cam img11 img12 ... img1N\n", f_argv_p[0]);
        printf("Example: %s --cam imgs/*c0*.png --cam imgs/*c1*.jpg\n", f_argv_p[0]);
        exit(1);        
    }
    
    QApplication app (f_argc_i, f_argv_p);
    
    /// Initialize application
    initialize("Top Level Drawing Lists");

    /// Load image filename data in vectors
    g_counter_i = 0;
    g_files_v.push_back( CStringVector() );
    
    for (int i = 2; i < f_argc_i; ++i)
    {
        if (std::string(f_argv_p[i]) == std::string("--cam"))
        {
            g_maxImages_i = std::max((int)g_files_v.back().size(), g_maxImages_i);

            g_files_v.push_back( CStringVector() );

            continue;
        }

        /// Store filename
        g_files_v.back().push_back ( f_argv_p[i] );
    }    
    
    g_maxImages_i = std::max((int)g_files_v.back().size(), g_maxImages_i);

    /// Drawing list
    registerDrawingLists();
  
    /// Load first image
    loadFrame ( );
    
    /// Set callback functions
    setKeyPressedEventCBF ( &callBackKeyboard );

    int retval_i = app.exec();

    terminate();

    return retval_i;
}

bool
callBackTimer( CTimerEvent * /*f_cevent_p*/ )
{
    ++g_counter_i;
    g_counter_i %= g_maxImages_i;
    loadFrame ( );
    return true;
}


bool
callBackKeyboard( CKeyEvent *f_kevent_p )
{
    const int timeout_i = 1;
    static int ms_i = 0;
    
    switch (f_kevent_p -> qtKeyEvent_p -> key() )
    {
        /// Move one frame backward
    case  Qt::Key_Left:
        --g_counter_i;
        break;

        /// Move one frame forward
    case  Qt::Key_Right:
        ++g_counter_i;
        break;

        /// Move five frames forward
    case  Qt::Key_Up:
        g_counter_i+=5;
        break;

        /// Move five frames backward
    case  Qt::Key_Down:
        g_counter_i-=5;
        break;

        /// Play/Pause
    case  Qt::Key_P:
    case  Qt::Key_Space:
        ms_i = timeout_i - ms_i;
        setTimerEventCBF ( &callBackTimer, ms_i );
        break;
        
        /// Stop. Go to firt frame.
    case  Qt::Key_S:
        g_counter_i=0;
        break;
        /// Stop. Go to firt frame.

        /// Advance 20 frames.
    case  Qt::Key_PageUp:
        g_counter_i=std::min((int)g_maxImages_i-1,g_counter_i+20);
        break;

        /// Back 20 frames.
    case  Qt::Key_PageDown:
        g_counter_i=std::max(0,g_counter_i-20);
        break;
    }

    g_counter_i = ( g_counter_i + 100*g_maxImages_i)%g_maxImages_i;
    
    /// Compute and show now.
    loadFrame ( );

    return true;
}

void
loadFrame (  )
{
    static cv::Mat src;

    cv::Size maxSize(0,0);
    
    for (unsigned int c = 0; c < g_files_v.size(); ++c)
    {
        char str[256];
        sprintf(str, "Camera %i Image", c );            

        getDrawingList ( str ) -> clear();

        if ( (int)g_files_v[c].size() > g_counter_i)
        {
            src = cv::imread (  g_files_v[c][g_counter_i].c_str() );

            if ( src.size().width > 0 )
            {
                displayImage ( src, str, false );

                if (src.size().width > maxSize.width)
                    maxSize.width = src.size().width;

                if (src.size().height > maxSize.height)
                    maxSize.height = src.size().height;                
            }
        }
    }
    
    setScreenSize ( maxSize );

    updateDisplay();
    
}

void
registerDrawingLists()
{
    /// Position drawing list in default location.
    /// User might want to change the order of display by
    /// drag-and-drop later.

    float aux_f    = sqrtf((float)g_files_v.size());
    int   countX_i = ceilf(aux_f);
    int   countY_i = countX_i - ((countX_i * countX_i) - g_files_v.size())/countX_i;

    /// Set screen count.
    setScreenCount ( cv::Size(countX_i,countY_i));
    
    for (unsigned int i = 0; i < g_files_v.size(); ++i)
    {    
        char str[256];
        sprintf(str, "Camera %i Image", i );
        
        registerDrawingList ( str, i%countX_i, i/countX_i );
    }
}
