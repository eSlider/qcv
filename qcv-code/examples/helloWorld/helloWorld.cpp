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

int main(int f_argc_i, char *f_argv_p[])
{
    QApplication app (f_argc_i, f_argv_p);
    
    /// Initialize application
    initialize("Top Level Drawing Lists");
    
    /// Set screen size.
    setScreenSize( cv::Size( 640, 480 ) );

    /// Set screen count.
    setScreenCount ( cv::Size(1,1) );

    /// Get drawing list.
    CDrawingList *list_p = getDrawingList ( "My Hello World Label" );
    
    /// Set line color. By default is black
    list_p -> setLineColor ( cv::Vec3b (255, 255, 255) );
    
    /// Add some text.
    list_p->addText ( "Hello World!", 100, 200, 64, false );

    int retval_i = app.exec();

    terminate();

    return retval_i;
}

