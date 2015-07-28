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
#include "clock.h"

using namespace QCV;

bool callBackKeyboard    ( CKeyEvent *f_kevent_p );
bool callBackTimer       ( CTimerEvent *f_cevent_p );

void loadAndShowFrame (  );
void registerDrawingLists ( );

typedef std::vector <std::string>   CStringVector;
typedef std::vector <CStringVector> CStringVectorVector;

/// Filenames for all cameras
CStringVectorVector      g_files_v(2);
/// Current frame
int                      g_counter_i;
/// Max number of frames between all cameras
int                      g_maxImages_i;

int main(int f_argc_i, char *f_argv_p[])
{
    if ( f_argc_i < 3 || ( std::string(f_argv_p[1]) != "--left" && std::string(f_argv_p[1]) != "--right" ) )
    {
        printf("Usage %s --left imgl1 imgl2 ... imglN --right imgr1 imgr2 ... imgrN\n", f_argv_p[0]);
        printf("Example: %s --left imgs/*c0*.png --right imgs/*c1*.jpg\n", f_argv_p[0]);
        exit(1);        
    }
    
    QApplication app (f_argc_i, f_argv_p);
    
    /// Initialize application
    initialize("Top Level Drawing Lists");

    /// Load image filename data in vectors
    g_counter_i = 0;

    int idx_i = 0;
    for (int i = 1; i < f_argc_i; ++i)
    {
        if (std::string(f_argv_p[i]) == std::string("--left"))
            idx_i = 0;
        else if (std::string(f_argv_p[i]) == std::string("--right"))
            idx_i = 1;
        else
        {
            g_maxImages_i = std::max((int)g_files_v[idx_i].size(), g_maxImages_i);    
            g_files_v[idx_i].push_back ( f_argv_p[i] );
        }        
    }
    
    g_maxImages_i = std::max((int)g_files_v[idx_i].size(), g_maxImages_i);

    /// Set screen count.
    setScreenCount ( cv::Size(1,1) );    
  
    /// Load first image
    loadAndShowFrame ( );
    
    /// Set callback functions
    setKeyPressedEventCBF ( &callBackKeyboard );

    int retval_i = app.exec();

    terminate();

    return retval_i;
}

bool callBackTimer( CTimerEvent * /*f_cevent_p*/ )
{
    ++g_counter_i;
    g_counter_i %= g_maxImages_i;
    loadAndShowFrame ( );
    return true;
}


bool callBackKeyboard( CKeyEvent *f_kevent_p )
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
    loadAndShowFrame ( );

    return true;
}

void loadAndShowFrame (  )
{
    static cv::Mat src;

    cv::Size size(0,0);

    for (unsigned int c = 0; c < g_files_v.size(); ++c)
    {
        CDrawingList *list_p = getDrawingList ( std::string("Anaglyph") + (c?"Left":"Right") );
        list_p -> clear();
        
        if ( g_counter_i < (int) g_files_v[c].size() )
        {
            src = cv::imread (  g_files_v[c][g_counter_i].c_str() );

            if ( src.size().width > 0 )
            {
                if ( c == 0 )
                    list_p -> setColorMask (0, 1, 1, 0);
                else
                    list_p -> setColorMask (1, 0, 0, 0);

                list_p -> addImage ( src );

                size = src.size();
            }
        }
    }

    setScreenSize ( size );

    updateDisplay();    
}

