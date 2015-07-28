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

bool callBackKeyboard ( CKeyEvent *f_kevent_p );
bool callBackTimer    ( CTimerEvent *f_cevent_p );

int main(int f_argc_i, char *f_argv_p[])
{
    QApplication app (f_argc_i, f_argv_p);
    
    /// Initialize application.
    initialize("Top Level Drawing Lists");
    
    /// Set screen size.
    setScreenSize( cv::Size(640,640) );

    /// Set screen count.
    setScreenCount( cv::Size(1,1) );
    
    /// Set callback functions.
    setTimerEventCBF ( &callBackTimer, 10 );
    setKeyPressedEventCBF ( &callBackKeyboard );
 
    /// Run qt application.
    int retval_i = app.exec();

    /// Call terminate() to free allocated objects
    terminate();

    return retval_i;
}

static float 
rndf(float f_min_f, float f_max_f)
{
    return (f_max_f-f_min_f) * random()/(float)RAND_MAX + f_min_f;
}

static int 
rndi(int f_min_i, int f_max_i)
{
    return int((f_max_i-f_min_i) * (random()/(float)RAND_MAX)) + f_min_i;
}

static float 
rndsign()
{
    return (random() >= RAND_MAX/2)?-1:1;
}

struct SLine
{
    float x1,y1,x2,y2;
    float dx1,dy1,dx2,dy2;
    SRgb  color;

    SLine()
    {
        x1 = y1 = x2 = y2 = dx1 = dy1 = dx2 = dy2 = 0.f;
    }
    
    SLine(int scrWidth_i, int scrHeight_i )
    {
        x1 = rndf(0.1f*scrWidth_i, 0.9f*scrWidth_i);
        y1 = rndf(0.1f*scrWidth_i, 0.9f*scrWidth_i);
        x2 = rndf(0.1f*scrHeight_i, 0.9f*scrHeight_i);
        y2 = rndf(0.1f*scrHeight_i, 0.9f*scrHeight_i);
        dx1 = rndf( 0.1f, 2.f ) * rndsign();
        dy1 = rndf( 0.1f, 2.f ) * rndsign();
        dx2 = rndf( 0.1f, 2.f ) * rndsign();
        dy2 = rndf( 0.1f, 2.f ) * rndsign();

        color.r = rndi(20, 255);
        color.g = rndi(20, 255);
        color.b = rndi(20, 255);
    }

    void advance( int scrWidth_i, int scrHeight_i )
    {
        x1+=dx1; if ( x1 < 0 ) x1 = dx1 = rndf( 0.1f, 2.f ); if ( x1 > scrWidth_i  ) x1 = scrWidth_i  - (dx1 = - rndf ( 0.1f, 2.f ));
        y1+=dy1; if ( y1 < 0 ) y1 = dy1 = rndf( 0.1f, 2.f ); if ( y1 > scrHeight_i ) y1 = scrHeight_i - (dy1 = - rndf ( 0.1f, 2.f ));
        x2+=dx2; if ( x2 < 0 ) x2 = dx2 = rndf( 0.1f, 2.f ); if ( x2 > scrWidth_i  ) x2 = scrWidth_i  - (dx2 = - rndf ( 0.1f, 2.f ));
        y2+=dy2; if ( y2 < 0 ) y2 = dy2 = rndf( 0.1f, 2.f ); if ( y2 > scrHeight_i ) y2 = scrHeight_i - (dy2 = - rndf ( 0.1f, 2.f ));
    }
};

/// Moving lines drawing list
std::vector <SLine> g_lines;

/// Speed of rotation
double g_rotSpeed_d = 0.12;

bool callBackTimer ( CTimerEvent * /* f_clockEvent_p */ )
{
    int scrWidth_i  = getScreenWidth();
    int scrHeight_i = getScreenHeight();

    startClock ("Lines motion");    
    if ( g_lines.empty() )
        for (int i = 0; i < 30; ++ i)
            g_lines.push_back( SLine ( scrWidth_i, scrHeight_i) );

    for (size_t i = 0; i < g_lines.size() ; ++i)
        g_lines[i].advance( scrWidth_i, scrHeight_i );
    stopClock ("Lines motion");    

    startClock ("Drawing");    

    CDrawingList * list_p    = getDrawingList ( "Moving Lines" );  

    list_p->clear();

    list_p->setLineColor ( 255, 255, 255 );
    list_p->addRectangle ( 0,0,list_p->getScreenWidth(), list_p->getScreenHeight());

    for (size_t i = 0; i < g_lines.size(); ++i)
    {        
        list_p->setLineColor ( g_lines[i].color );
        list_p->addLine ( g_lines[i].x1, g_lines[i].y1, g_lines[i].x2, g_lines[i].y2 );
    }

    /// Scale and center
    double diagDist_d = sqrt(scrWidth_i*scrWidth_i + scrHeight_i*scrHeight_i);
    double scale_d = std::min(scrWidth_i, scrHeight_i) / diagDist_d;

    list_p -> setScale ( scale_d );
    list_p -> setOffset ( (scrWidth_i/scale_d  - scrWidth_i)/2.f, 
                          (scrHeight_i/scale_d - scrHeight_i)/2.f);
    /// Rotate
    list_p -> setRotation ( list_p->getRotation() + g_rotSpeed_d );

    /// Border drawing list.
    list_p    = getDrawingList ( "Screen Border" );
    list_p->clear();
    list_p->setLineColor ( 255, 255, 255 );
    list_p->addRectangle ( 0,0,list_p->getScreenWidth(), list_p->getScreenHeight());

    stopClock ("Drawing");    

    startClock ("GL Paint");    
    updateDisplay();
    stopClock ("GL Paint");

    updateClocks();

    return true;
}

bool callBackKeyboard( CKeyEvent *f_kevent_p )
{
    int scrWidth_i  = getScreenWidth();
    int scrHeight_i = getScreenHeight();

    int num_i = f_kevent_p -> qtKeyEvent_p -> key() - Qt::Key_0;
    
    if ( num_i > 0 && num_i < 9)
    {
        int newSize_i = (int)pow(10, num_i)*3;
        int diff_i = newSize_i - (int)g_lines.size();

        if (diff_i < 0)
            g_lines.resize ( newSize_i );
        else
        {            
            for (int i = 0; i < diff_i; ++i)
                g_lines.push_back( SLine ( scrWidth_i, scrHeight_i) );
        }
    }
    else if ( f_kevent_p -> qtKeyEvent_p -> key() == Qt::Key_F1 )    
        g_rotSpeed_d = std::max(std::min(1.1*g_rotSpeed_d, 1.2), 0.012);
    else if ( f_kevent_p -> qtKeyEvent_p -> key() == Qt::Key_F2 )
        g_rotSpeed_d = std::max(g_rotSpeed_d/1.1-0.01, 0.);

    return true;
}

