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

#ifndef __DEVICEOPCONNECTOR_H
#define __DEVICEOPCONNECTOR_H

/**
 *******************************************************************************
 *
 * @file deviceopconnector.h
 *
 * \class CDeviceOpConnector
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief
 *
 *******************************************************************************/

/* INCLUDES */

#include "operator.h"
#include "seqDeviceControl.h"

/* CONSTANTS */

/* PROTOTYPES */

namespace QCV
{
/* NAMESPACE PROTOTYPES */    
    class CDeviceOpConnectorBase
    {
    public:
        CDeviceOpConnectorBase ( ) {};
        virtual ~CDeviceOpConnectorBase() {};

    /// Set I/O
    public:
        
        virtual bool setOperatorInput() = 0;
        
    /// Gets/Sets
    public:
        
        virtual COperatorBase *         getRootOperator() const = 0;
        virtual CSeqDeviceControlBase * getDeviceCtrl()   const = 0;
    };

        
    template <class _InType, class _OutType, class _DevType=_InType>
    class CDeviceOpConnector: public CDeviceOpConnectorBase
    {
    public:
                
    public:
        
        CDeviceOpConnector ( COperator<_InType, _OutType> * f_rootOp_p,
                             CSeqDeviceControl<_DevType> *   f_deviceOp_p );

        virtual ~CDeviceOpConnector() {};
        
    /// Set I/O
    public:
        
        virtual bool setOperatorInput();
        
    /// Gets/Sets
    public:
        
        virtual COperatorBase *         getRootOperator() const
        {
            return m_rootOp_p;
        }
        virtual CSeqDeviceControlBase * getDeviceCtrl() const
        {
            return m_device_p;
        }

    private:

        /// Input device.
        CSeqDeviceControl<_DevType> *     m_device_p;

        /// Root operator
        COperator<_InType, _OutType> *    m_rootOp_p;

        /// Input Object
        _DevType                          m_input;
    };

    template <class _InType, class _OutType, class _DevType>
    CDeviceOpConnector<_InType, _OutType,  _DevType>::CDeviceOpConnector ( COperator<_InType, _OutType> *  f_rootOp_p,
                                                                           CSeqDeviceControl<_DevType> *   f_device_p ) :
        m_device_p  (          f_device_p ),
        m_rootOp_p  (          f_rootOp_p )
    {        
    }

    template <class _InType, class _OutType, class _DevType>
    bool CDeviceOpConnector<_InType, _OutType, _DevType>::setOperatorInput ( )
    {
        bool success_b = m_device_p -> registerOutputs ( m_input );
        
        if ( success_b )
            m_rootOp_p -> setInput ( m_input );

        return success_b;
    }
    
}

#endif // __DEVICEOPCONNECTOR_H
