/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 3. See the GNU LGPL version 3 for details.
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

#ifndef __OPERATOR_H
#define __OPERATOR_H

/**
 *******************************************************************************
 *
 * @file operator.h
 *
 * \class COperator
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Defines the  class for the operators of the framework.
 *
 * The COperator class provides a  for defining operators in the framework.
 * This class provides virtual functions for handling operations input provided by
 * the user or the device controler. The method cycle() is called every time new
 * sequence data is ready for processing. reset() is called when the device
 * controler stops operation (because of user input for example). initialize() is 
 * called when a new sequence data is loaded. exit() is called when the program 
 * exits or the current sequence is closed. Additional functions for handling 
 * mouse and keyboard actions are provided.
 *
 * An operator can have a parent and multiple children. This allows the generation
 * of tree structures. At least one root operator must exist in a functioning 
 * program.
 *
 * Operators also have a parameter set for defining the parameters of the operator.
 * 
 * An operator also might define visual output in the form of drawing lists 
 * (registerDrawingList and getDrawingList methods).
 *
 *******************************************************************************/

/* INCLUDES */
#include "operatorBase.h"

/* CONSTANTS */

/* PROTOTYPES */

/* CLASS DEFINITION */

namespace QCV
{
/* PROTOTYPES */        

    template <class _InType, class _OutType>
    class COperator: public COperatorBase
    {
    /// Constructor, Desctructors
    public:    

        /// Constructors.
        COperator ( COperatorBase * const f_parent_p = NULL, 
                    const std::string f_name_str = "Unnamed Operator" ) :
            COperatorBase ( f_parent_p, f_name_str )
        {
            
        }        

        /// Virtual destructor.
        virtual ~COperator ()
        {

        }        

    /// Get/Set methods
    public:

    /// Get/Set static methods
    public:
        
    /// I/O registration.
    public:
        /// Set the input of this operator
        virtual bool setInput  ( const _InType & f_input ) = 0;

        /// Gets the output of this operator
        virtual bool getOutput ( _OutType & f_output ) const = 0;

        /// Set the input of this operator
        virtual bool compute  ( const _InType & f_input,
                                _OutType      & f_output )
        {
            startClock ("Cycle");

            if (!setInput (f_input))
                return false;
            
            if ( !cycle() ) return false;
            
            bool res_b =  getOutput (f_output);
            
            stopClock ("Cycle");

            return res_b;
        }
        

    };

} // Namespace VIC

#endif // __OPERATOR_H
