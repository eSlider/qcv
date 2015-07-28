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

void computeSobel (  );
void registerDrawingLists ( );

std::vector<std::string> g_files_v;
int                      g_counter_i;

int main(int f_argc_i, char *f_argv_p[])
{
    if (f_argc_i < 2)
    {
        printf("Usage %s img1 img2 ... imgN\n", f_argv_p[0]);
        exit(1);        
    }
    
    QApplication app (f_argc_i, f_argv_p);
    
    /// Initialize application
    initialize("Top Level Drawing Lists");
    
    g_counter_i = 0;
    for (int i = 1; i < f_argc_i; ++i)
        g_files_v.push_back ( f_argv_p[i] );
    
    /// Drawing list
    registerDrawingLists();
  
    /// Load first image
    computeSobel ( );
    
    /// Set screen count.
    setScreenCount ( cv::Size(2,2));
    
    /// Set callback functions
    setKeyPressedEventCBF ( &callBackKeyboard );

    int retval_i = app.exec();

    terminate();

    return retval_i;
}

bool callBackTimer( CTimerEvent * /*f_cevent_p*/ )
{
    ++g_counter_i;
    g_counter_i %= g_files_v.size();
    computeSobel ( );
    return true;
}


bool callBackKeyboard( CKeyEvent *f_kevent_p )
{
    const int timeout_i = 1;
    static int ms_i = 0;
    
    switch (f_kevent_p -> qtKeyEvent_p -> key() )
    {
        /// Move one frame forward
    case  Qt::Key_Left:
        --g_counter_i;
        break;

        /// Move one frame backward
    case  Qt::Key_Right:
        ++g_counter_i;
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
        g_counter_i=std::min((int)g_files_v.size()-1,g_counter_i+20);
        break;

        /// Back 20 frames.
    case  Qt::Key_PageDown:
        g_counter_i=std::max(0,g_counter_i-20);
        break;
    }

    g_counter_i = ( g_counter_i + 100*g_files_v.size())%g_files_v.size();
    
    /// Compute and show now.
    computeSobel ( );
    
    
    return true;
}

void computeSobel (  )
{
    /// Lets define some clock to measure computation time.

    static cv::Mat src;
    static cv::Mat gauss, gradx, absgradx, grady, absgrady, magnitude;

    startClock("Load");
    
    src   = cv::imread ( g_files_v[g_counter_i].c_str() );

    stopClock("Load");

    startClock("Sobel Computation");

    if ( src.size().width == 0 ) return;
    
    /// Set screen size to current image size.
    setScreenSize ( src.size() );

    /// Gauss filter.
    cv::GaussianBlur( src, gauss, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );

    /// Sobel Vert
    cv::Sobel( gauss, gradx, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT );
    cv::convertScaleAbs( gradx, absgradx );

    /// Sobel Hor
    cv::Sobel( gauss, grady, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT );
    cv::convertScaleAbs( grady, absgrady );
    
    /// Approximate magnitude (faster than computing sqrt(gx^2+gy^2)
    cv::addWeighted( absgradx, 0.5, absgrady, 0.5, 0, magnitude );

    stopClock("Sobel Computation");

    startClock("Display");
    
    displayImage (src, "Original Image", true );
    displayImage (absgradx, "Grad X", true );
    displayImage (absgrady, "Grad Y", true );
    displayImage (magnitude, "Magnitude ", true );

    updateDisplay();
    updateClocks();

    stopClock("Display");    
}

void registerDrawingLists()
{
    /// Position drawing list in default location.
    /// User might want to change the order of display by
    /// drag-and-drop later.

    registerDrawingList ( "Original Image", 0, 0 );
    registerDrawingList ( "Grad X",         1, 0 );
    registerDrawingList ( "Grad Y",         0, 1 );
    registerDrawingList ( "Magnitude ",     1, 1 );
}
