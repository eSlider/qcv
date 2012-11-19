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
 * \file  textList.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "textList.h"
#include "glheader.h"
#include <stdio.h>
#include <GL/glut.h>
#include <string.h>

using namespace QCV;


CTextList::CTextList( int /* f_bufferSize_i */ )
{}

/// Destructor.
CTextList::~CTextList()
{}

// Add texts from other list.
bool
CTextList::add ( const CTextList &f_otherList )
{
    m_text_v.insert( m_text_v.begin(), 
                     f_otherList.m_text_v.begin(),
                     f_otherList.m_text_v.end() );

    return true;
}

// Draw all texts.
        // Draw all texts.
bool
CTextList::add (  std::string f_text_str,
                  float   f_u_f, 
                  float   f_v_f,
                  SRgba   f_color,
                  float   f_lineWidth_f,
                  float   f_fontSize_f,
                  bool    f_fixSize_b )
{
    SText newText;

    newText.color       = f_color;
    newText.u_f         = f_u_f;
    newText.v_f         = f_v_f;
    newText.lineWidth_f = f_lineWidth_f;
    newText.fontSize_f  = f_fontSize_f;
    newText.fixSize_b   = f_fixSize_b;
    strncpy(newText.text_str, f_text_str.c_str(), 1024);


    m_text_v.push_back(newText);

    return true;
}

// Clear all texts.
bool
CTextList::clear ()
{
    m_text_v.clear();
    return m_text_v.size();
}

// Draw all texts.
bool
CTextList::show () const
{
    std::vector< SText >::const_iterator last = m_text_v.end();

    glEnable(GL_LINE_SMOOTH);
    
    glDisable(GL_TEXTURE_2D);
    for (std::vector< SText >::const_iterator i = m_text_v.begin(); 
         i != last; ++i )
    {   
        // glLineWidth(i->lineWidth_f);
        // glColor4ub( i->color.r, i->color.g, i->color.b, i->color.a );
        // glRasterPos3f(i->u_f, i->v_f, 0.0);

        // for(int j=0; i->text_str[j]!='\0'; j++)
        // {
        //     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, i->text_str[j]);
        //     //printf("%c\n",  i->text_str[j]);
        // }

        double iScale = ((i->fontSize_f)/101.0);
        glPushMatrix ();
        glTranslatef ((i->u_f),(i->v_f),0.0);

        glLineWidth(i->lineWidth_f);
        glColor4ub( i->color.r, i->color.g, i->color.b, i->color.a );
        glScalef(iScale,iScale,iScale);
        glRotatef(180.0,1.0,0.0,0.0);

        if ( i->fixSize_b)
        {
            for(int j=0; i->text_str[j]!='\0'; j++)
            {
                glutStrokeCharacter (GLUT_STROKE_MONO_ROMAN, i->text_str[j]);
            }
        }

        else
        {
            for(int j=0; i->text_str[j]!='\0'; j++)
            {
                glutStrokeCharacter (GLUT_STROKE_ROMAN, i->text_str[j]);
            }
        }

        glLineWidth(1.0);
        glRotatef(180.0,1.0,0.0,0.0);
        glPopMatrix();
        glFlush();
    }
    glEnable(GL_TEXTURE_2D);

    glDisable(GL_LINE_SMOOTH);

    /// Todo: Check GL status and return value.
    return true;
}

bool
CTextList::write ( FILE*                f_file_p,
                   const float          f_offsetU_f /* = 0.0 */,
                   const float          f_offsetV_f /* = 0.0 */,
                   const std::string    f_prefix_str /* = "" */) const
{
    std::vector< SText >::const_iterator last = m_text_v.end();
    
    int count_i = 0;
    
    for (std::vector< SText >::const_iterator i = m_text_v.begin(); 
         i != last; ++i, ++count_i )
    {
        if ( i->fixSize_b)
        {
            fprintf( f_file_p,
                     "        <text id=\"text_%s_%i\"\n"
                     "              x=\"%f\" y=\"%f\"\n"
                     "              stroke-width=\"%f\"\n"
                     "              stroke=\"none\"\n"
                     "              fill=\"#%02x%02x%02x\"\n"
                     "              opacity=\"%f\"\n"
                     "              style=\"font-family:Bitstream Vera Sans Mono;font-weight:normal;font-style:normal;font-stretch:normal;font-variant:normal;font-size:%dpx;text-anchor:start;text-align:start;writing-mode:lr\"\n",
                     f_prefix_str.c_str(), count_i,
                     i->u_f+f_offsetU_f, i->v_f+f_offsetV_f,
                     1+i->lineWidth_f,
                     i->color.r, i->color.g, i->color.b, i->color.a/255.f,
                     int( ( i->fontSize_f * (119.05+33.33) ) / 101.0) );
        }
        else
        {
            fprintf( f_file_p,
                     "        <text id=\"text_%s_%i\"\n"
                     "              x=\"%f\" y=\"%f\"\n"
                     "              stroke-width=\"%f\"\n"
                     "              stroke=\"none\"\n"
                     "              fill=\"#%02x%02x%02x\"\n"
                     "              opacity=\"%f\"\n"
                     "              style=\"font-family:Bitstream Vera Sans;font-weight:normal;font-style:normal;font-stretch:normal;font-variant:normal;font-size:%dpx;text-anchor:start;text-align:start;writing-mode:lr\"</text>\n",
                     f_prefix_str.c_str(), count_i,
                     i->u_f+f_offsetU_f, i->v_f+f_offsetV_f,
                     1+i->lineWidth_f,
                     i->color.r, i->color.g, i->color.b, i->color.a/255.f,
                     int( ( i->fontSize_f * (119.05+33.33) ) / 101.0) );
        }
    }
    
    return true;
}

/// Return number of elements.
int
CTextList::getSize () const
{
    return m_text_v.size();    
}

