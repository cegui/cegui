/***********************************************************************
    created:    24/9/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#ifndef _GTK2CEGuiRendererSelector_h_
#define _GTK2CEGuiRendererSelector_h_

#include "CEGuiRendererSelector.h"
#include <gtk/gtk.h>
#include <string>

class GTK2CEGuiRendererSelector : public CEGuiRendererSelector
{
public:
    GTK2CEGuiRendererSelector();
    ~GTK2CEGuiRendererSelector();

    bool invokeDialog();

private:
    // Renderer name strings
    static const std::string    OgreRendererName;
    static const std::string    OpenGLRendererName;
    static const std::string    IrrlichtRendererName;

    // Static member funcs
    void createDialog();
    static void on_cancelButton_clicked(GtkButton* button, gpointer user_data);
    static void on_okButton_clicked(GtkButton* button, gpointer user_data);
    static void on_combobox1_changed(GtkComboBox* combobox, gpointer user_data);

    // general data
    bool d_cancelled;

    // GTK2 widgets
    GtkWidget* d_dialog;
    GtkWidget* d_vbox1;
    GtkWidget* d_label1;
    GtkWidget* d_hbox1;
    GtkWidget* d_cancelButton;
    GtkWidget* d_okButton;
    GtkWidget* d_combobox1;
};

#endif  // end of guard _GTK2CEGuiRendererSelector_h_
