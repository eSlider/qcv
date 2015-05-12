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

#ifndef __QCVCLOCK_H
#define __QCVCLOCK_H

/*@@@**************************************************************************
 ** \file  clock.h
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****          (C) COPYRIGHT Hernan Badino - All Rights Reserved          *****
******************************************************************************/

/* INCLUDES */

/* CONSTANTS */

/* INCLUDES */
#include "clock.h"

#include <vector>
#include <string>

/* CONSTANTS */


namespace QCV
{
    class CClock
    {
    /// Data types
    public:

    /// Consructors/Destructor.
    public:

        /// Constructor.
        CClock (  std::string f_name_str = "Unnamed Clock" );
                
        /// Destructor. 
        virtual ~CClock() {};
        
    /// Handling
    public:

        /// Start measuring.
        void          start();

        /// Stop measuring.
        void          stop();        

        /// Reset clock.
        void          reset();

    /// Gets/Sets
    public:

        /// Get number of measurements.
        std::string   getName() const;

        /// Get number of measurements.
        unsigned int  getCount() const;

        /// Get number of measurements.
        double        getTotalTime() const;

        /// Get loop time.
        double        getLoopTime() const;
        
    /// Print
    public:
        
        /// Print
        void          print() const;

    /// Private data members
    private:

        /// Name.
        bool         m_on_b;

        /// Name.
        std::string  m_name_str;
        
        /// Number of measurements.
        unsigned int m_count_ui;

        /// Total current time.
        double       m_totalTime_d;  

        /// Total current time.
        double       m_startTime_d;
    };
}


#endif // __QCVCLOCK_H
