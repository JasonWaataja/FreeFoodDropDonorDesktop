/*-
 * Copyright (c) 2016, Jason Waataja
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <glib/gi18n.h>

#include "ffdddwindow.h"
#include "ffdddgiveawaydialog.h"

struct _FfdddWindow {
	GtkApplicationWindow parent;
};

struct _FfdddWindowClass {
	GtkApplicationWindowClass parent_class;
};

typedef struct _FfdddWindowPrivate FfdddWindowPrivate;

struct _FfdddWindowPrivate {
	GtkWidget	*add_giveaway_button;
	GtkWidget	*giveaways_view;

	GtkListStore	*giveaways_store;
};

G_DEFINE_TYPE_WITH_PRIVATE(FfdddWindow, ffddd_window,
    GTK_TYPE_APPLICATION_WINDOW);

static GActionEntry win_entries[] = {
	{"quit", ffddd_window_quit_activated, NULL, NULL, NULL},
	{"about", ffddd_window_about_activated, NULL, NULL, NULL}
};

static void
ffddd_window_init(FfdddWindow *win)
{
	FfdddWindowPrivate *priv;

	priv = ffddd_window_get_instance_private(win);
	gtk_widget_init_template(GTK_WIDGET(win));

	g_signal_connect_swapped(priv->add_giveaway_button, "clicked",
	    G_CALLBACK(ffddd_window_add_new_giveaway), win);

	g_action_map_add_action_entries(G_ACTION_MAP(win), win_entries,
	    G_N_ELEMENTS(win_entries), win);
}

static void
ffddd_window_class_init(FfdddWindowClass *kclass)
{
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(kclass),
	    "/com/waataja/ffddd/ui/mainwindow.ui");
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddWindow, add_giveaway_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddWindow, giveaways_view);
}

FfdddWindow *
ffddd_window_new(FfdddApplication *app)
{
	FfdddWindow *win = g_object_new(FFDDD_TYPE_WINDOW, "application", app,
	    NULL);

	return (win);
}

void ffddd_window_quit_activated(GSimpleAction *action, GVariant *parameter,
    gpointer win)
{

	gtk_widget_destroy(GTK_WIDGET(win));
}

void ffddd_window_about_activated(GSimpleAction *action, GVariant *parameter,
    gpointer win)
{
  const gchar *authors[] = { "Jason Waataja", "Justin Frank", NULL };

  gtk_show_about_dialog(GTK_WINDOW (win),
      "program-name", "FreeFoodDropDonorDesktop",
      "title", _("About FreeFoodDropDonorDesktop"),
      "authors", authors,
      "copyright", "Copyright (C) 2016 Jason Waataja",
      "license-type", GTK_LICENSE_BSD,
      "version", "Version 0.1.0",
      "website", "https://github.com/JasonWaataja/FreeFoodDropDonorDesktop",
      "comments", "Desktop client for donors to the FreeFoodDrop system",
      NULL);
}

void
ffddd_window_add_new_giveaway(FfdddWindow *win)
{
	FfdddGiveawayDialog *dialog;

	dialog = ffddd_giveaway_dialog_new(GTK_WINDOW(win));
	gtk_dialog_run(GTK_DIALOG(dialog));
}
