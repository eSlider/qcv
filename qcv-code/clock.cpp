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
* \file  clock.cpp
* \author Hernan Badino
* \notes 
*******************************************************************************
*****          (C) COPYRIGHT Hernan Badino - All Rights Reserved          *****
******************************************************************************/

/* INCLUDES */
#include "clock.h"
#include <stdio.h>

using namespace QCV;

//// OpenMP includes.
#if defined ( _OPENMP )
#include <omp.h>
#else
#include <time.h>
#endif

CClock::CClock (  std::string f_name_str )
        : m_on_b (                      false ),
          m_name_str (             f_name_str ),
          m_count_ui (                      0 ),
          m_totalTime_d (                  0. ),
          m_startTime_d (                   0 )
{
}

/// Start measuring.
void
CClock::start()
{
    if ( m_on_b )
    {
        printf("Clock \"%s\" has already been started\n",
               m_name_str.c_str() );
        return;
    }
    m_on_b = true;
    
#if defined ( _OPENMP )
    m_startTime_d = omp_get_wtime();
#else    
    m_startTime_d = clock();
#endif
}

/// Stop measuring.
void
CClock::stop()
{
    if ( !m_on_b )
    {
        printf("Clock \"%s\" hasn't been started\n",
               m_name_str.c_str() );
        return;
    }

    double cycleTime_d;

#if defined ( _OPENMP )
    cycleTime_d = (omp_get_wtime() - m_startTime_d)*1000;
#else    
    cycleTime_d = (clock() - m_startTime_d)*(1000./(double)(CLOCKS_PER_SEC));
#endif

    m_totalTime_d += cycleTime_d;
    ++m_count_ui;

    m_on_b = false;

}

/// Reset Clock.
void
CClock::reset()
{
    m_on_b         = false;
    m_count_ui     = 0 ;
    m_totalTime_d  = 0. ;
    m_startTime_d  = 0 ;
}


/// Get number of measurements.
std::string
CClock::getName() const
{
    return m_name_str;
}

/// Get number of measurements.
unsigned int
CClock::getCount() const
{
    return m_count_ui;
}

/// Get number of measurements.
double
CClock::getTotalTime() const
{
    return m_totalTime_d;
}

/// Get loop time.
double
CClock::getLoopTime() const
{
    return m_totalTime_d/m_count_ui;
}

/// Print
void
CClock::print() const
{
    printf("Clock \"%s\" - Total Time: %lf Cycles: %u Time/Cycle: %lf\n",
           m_name_str.c_str(),
           m_totalTime_d,
           m_count_ui,
           m_totalTime_d/m_count_ui );
}

