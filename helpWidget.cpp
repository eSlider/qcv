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


#include "helpWidget.h"
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>

using namespace QCV;

static QString 
addHelpLine(const QString& f_field1_str, 
            const QString& f_field2_str )
{
    static bool s_color_i = 0;

    QString res("<tr bgcolor=\"");

    if (s_color_i)
        res += "#eeffee\">";
    else
        res += "#ffffff\">";

    res += "<td><b>" + f_field1_str + QString("</b></td><td>") + f_field2_str + QString("</td></tr>\n");

    s_color_i = !s_color_i;

    return res;
}

CHelpWidget::CHelpWidget( QWidget * f_parent_p ):
    QTabWidget (        f_parent_p ),
    m_helpString (              "" ),
    m_keyboardString (          "" ),
    m_mouseString (             "" )
{
    int height_i;

    setStrings();
    setWindowTitle("Help");
    
    QPushButton* qbAbout_p = new QPushButton("About", this);
    connect(qbAbout_p, SIGNAL(clicked()), SLOT(showAbout()));
    
    setCornerWidget(qbAbout_p);
    
    QTextEdit* tab_p;

    tab_p = new QTextEdit(NULL);
    tab_p->setReadOnly(true);
    insertTab(0, tab_p, "&Keyboard and Mouse");
    tab_p->setHtml(m_keyboardString);

    height_i = tab_p->height();

    tab_p = new QTextEdit(NULL);
    tab_p->setReadOnly(true);
    insertTab(1, tab_p, "Help");
    tab_p->setHtml(m_helpString);

    if ((tab_p->height() > height_i ))
        height_i = tab_p->height();

    resize(800, height_i+40);
}

CHelpWidget::~CHelpWidget( )
{
}


void
CHelpWidget::showAbout()
{
    QMessageBox::about ( this, "About QCV", 
                         "Copyright (C) 2012 Hernan Badino\n"
                         "<hernan.badino@gmail.com>\n\n"
                         "QCV is under the terms of the GNU Lesser General Public License "
                         "version 2.1. See the GNU LGPL version 2.1 for details. "
                         "QCV is distributed \"AS IS\" without ANY WARRANTY, without even the "
                         "implied warranty of merchantability or fitness for a particular "
                         "purpose.\n\n"
                         "In no event shall the authors or contributors be liable "
                         "for any direct, indirect, incidental, special, exemplary, or "
                         "consequential damages arising in any way out of the use of this "
                         "software.\n\n"
                         "By downloading, copying, installing or using the software you agree "
                         "to this license. Do not download, install, copy or use the "
                         "software, if you do not agree to this license.");
}

void
CHelpWidget::setStrings()
{
    m_helpString = "Nothing here so far";

    QString header_str("<center><table border=\"1\" cellspacing=\"0\" cellpadding=\"4\">\n"
                   "<tr bgcolor=\"#aaccaa\"><th align=\"center\">Key/Mouse</th><th align=\"center\">Description</th></tr>\n" );
    
    QString footer_str = "</table></center>";

    m_keyboardString=header_str;
    m_keyboardString += addHelpLine("H",               "Show this help.");
    m_keyboardString += addHelpLine("G",               "Start or stop grabbing display updates. Filename format is grabbedDisplayWidgetImg_%05i.png at the current directory.");
    m_keyboardString += addHelpLine("Ctrl-G",          "Grab all display repaints. It can be deactivated by pressing key G.");
    m_keyboardString += addHelpLine("Ctrl-Alt-G",      "Grab display at regular time interval. Images will be saved after pressing G again. When saving the images, the key Esc can be pressed to cancel saving images.");
    m_keyboardString += addHelpLine("F",               "Change to full screen mode.");
    m_keyboardString += addHelpLine("&#62;",               "Zoom-in.");
    m_keyboardString += addHelpLine("&#60;",               "Zoom-out.");
    m_keyboardString += addHelpLine("F1:F35",          "Zoom a particular screen. The function value number corresponds to the nth screen in the array order. Example: on a 4-column-3-row screen, F6 corresponds to screen column 2, row 1..");
    m_keyboardString += addHelpLine("Ctrl-F1:F35",     "Move the current zoomed area to a different screen.");
    m_keyboardString += addHelpLine("Alt-Left",        "Unconstrained motion of the current zoomed area to the screen to the left.");
    m_keyboardString += addHelpLine("Alt-Right",       "Unconstrained motion of the current zoomed area to the screen to the screen to the right.");
    m_keyboardString += addHelpLine("Alt-Up",          "Unconstrained motion of the current zoomed area to the screen to the immediate upper screen.");
    m_keyboardString += addHelpLine("Alt-Down",        "Unconstrained motion of the current zoomed area to the screen to the immediate lower screen.");
    m_keyboardString += addHelpLine("Wheel",           "Zoom in/out.");
    m_keyboardString += addHelpLine("Ctrl-Wheel",      "Fast zoom in/out.");
    m_keyboardString += addHelpLine("Alt-Wheel",       "Slow zoom in/out.");
    m_keyboardString += addHelpLine("Shift-Wheel",     "Fine zoom in/out.");

    m_keyboardString += addHelpLine("Ctrl-Mouse Move", "Moved zoomed display.");

    m_keyboardString += "<tr bgcolor=\"#aaccaa\"><th align=\"center\">Sequencer</th><th align=\"center\">Description</th></tr>\n" ;

    m_keyboardString += addHelpLine("Space or P",      "Play/Pause.");
    m_keyboardString += addHelpLine("R",               "Reload current frame.");
    m_keyboardString += addHelpLine("S",               "Stop and go to first frame.");
    m_keyboardString += addHelpLine("Left",            "Previous frame.");
    m_keyboardString += addHelpLine("Right",           "Next frame.");


    m_keyboardString += footer_str;    
}
