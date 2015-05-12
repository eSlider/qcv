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

#ifndef __IOBASE_H
#define __IOBASE_H

namespace QCV
{
    class CIOBase
    {
    public:   
        virtual ~CIOBase () {}
    };
        
    template <class _T>
    class CIO: public CIOBase
    {
    public:
        CIO( _T * f_ptr ) 
        {
            m_ptr = f_ptr;
        }
            
        _T * getPtr ( ) 
        {
            return ( m_ptr );
        }
            
        _T * getPtr ( ) const
        {
            return ( m_ptr );
        }
    private:
        _T *  m_ptr;
    };
}

#endif //  __IOBASE_H

