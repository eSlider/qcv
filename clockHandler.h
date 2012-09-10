#ifndef __CLOCKHANDLER_H
#define __CLOCKHANDLER_H

/**
 *******************************************************************************
 *
 * @file clockHandler.h
 *
 * \class CClockHandler
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief This class implements a handler for clock.
 *
 *******************************************************************************/

/* INCLUDES */
#include <string>
#include <map>

/* CONSTANTS */

namespace QCV
{
 /* CONSTANTS */


/* DATA TYPES */


/* PROTOTYPES */

    class CNode;
    class CClock;
    class CClockOpNode;
    
    class CClockHandler
    {
    /// Public data types.
    public:


    /// Construction, Destruction
    public:        
        CClockHandler();
        virtual ~CClockHandler();

    /// Get and sets.
    public:
        /// Get pointer to clock.
        CClock *    getClock ( std::string f_name_str, 
                                           CNode * f_op_p );

        /// Get root node.
        CClockOpNode  * getRootNode ( ) const;

        /// Set clock-changed flag.
        void              setClockUpdateFlag ( bool f_val_b );

        /// Get clock-changed flag.
        bool              mustUpdateClock ( ) const;

    private:

        /// Root node.
        CClockOpNode *          m_root_p;

        /// Flag for indicating a change in some clock.
        bool                    m_clockChanged_b;
    };    


    inline CClockOpNode  * 
    CClockHandler::getRootNode ( ) const 
    { 
        return m_root_p; 
    }

    inline void 
    CClockHandler::setClockUpdateFlag ( bool f_val_b )
    {
        m_clockChanged_b = f_val_b;
    }

    inline bool
    CClockHandler::mustUpdateClock ( ) const
    {
        return m_clockChanged_b;
    }
}

#endif // __CLOCKHANDLER_H

