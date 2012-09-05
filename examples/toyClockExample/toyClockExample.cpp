/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU General Public License, version 2.
 * See the GNU General Public License, version 2 for details.
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

#include "../cinterface.h"

using namespace QCV;

bool callBackTimer    ( CTimerEvent *f_cevent_p );

int main(int f_argc_i, char *f_argv_p[])
{
    QApplication app (f_argc_i, f_argv_p);
    
    /// Initialize application.
    initialize("Top Level Drawing Lists");
    
    /// Set screen size.
    setScreenSize( cv::Size(640,480) );

    /// Set screen count.
    setScreenCount( cv::Size(1,1) );
    
    /// Set callback functions.
    setTimerEventCBF ( &callBackTimer, 10 );

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

bool callBackTimer ( CTimerEvent * /* f_clockEvent_p */ )
{
    int scrWidth_i  = getScreenWidth();
    int scrHeight_i = getScreenHeight();

    static std::vector <SLine> lines;

    if ( lines.size() == 0 )
        for (size_t i = 0; i < 30; ++i)
            lines.push_back ( SLine ( scrWidth_i, scrHeight_i) );

    CDrawingList * list_p    = getDrawingList ( "Overlay" );  
    list_p->clear();

    for (size_t i = 0; i < lines.size()-1;++i)
    {        
        lines[i].advance( scrWidth_i, scrHeight_i );
        list_p->setLineColor ( lines[i].color );
        list_p->addLine ( lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2 );
    }

    updateDisplay();
    return true;
}

