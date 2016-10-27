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

enum results_view_cols {
    ADDRESS_COLUMN = 0,
    NAME_COLUMN,
    FOOD_COLUMN,
    TIME_COLUMN,
    EXTRA_COLUMN,
    N_COLUMNS
};

static void
ffddd_window_init(FfdddWindow *win)
{
	FfdddWindowPrivate *priv;

	priv = ffddd_window_get_instance_private(win);
	gtk_widget_init_template(GTK_WIDGET(win));

	ffddd_window_init_giveaways_view(win);

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
	FfdddGiveaway *giveaway;
	const gchar *address, *start_time, *end_time, *info;
	GList *temp_list;
	gchar *items_str;
	gchar *temp_str;
	gchar *full_time_string;
	struct FfdddDate start_date;
	struct FfdddDate end_date;
	gboolean on_first;
	FfdddWindowPrivate *priv;
	GtkListStore *giveaways_store;
	GtkTreeIter tree_iter;
	gint response;

	priv = ffddd_window_get_instance_private(win);
	giveaways_store = priv->giveaways_store;

	giveaway = ffddd_giveaway_new();
	dialog = ffddd_giveaway_dialog_new(GTK_WINDOW(win), giveaway);
	response = gtk_dialog_run(GTK_DIALOG(dialog));

	if (response == GTK_RESPONSE_OK) {
		ffddd_giveaway_dialog_get_gui_info(dialog, giveaway);

		address = ffddd_giveaway_get_address(giveaway);
		start_time = ffddd_giveaway_get_start_time(giveaway);
		end_time = ffddd_giveaway_get_end_time(giveaway);
		info = ffddd_giveaway_get_info(giveaway);
		address = ffddd_giveaway_get_address(giveaway);

		temp_list = ffddd_giveaway_get_items_copy(giveaway);
		for (on_first = TRUE; temp_list != NULL; temp_list =
		    g_list_next(temp_list)) {
			if (on_first) {
				items_str = g_strdup(temp_list->data);
				on_first = FALSE;
			} else {
				temp_str = items_str;
				items_str = g_strconcat(temp_str, "\n",
				    temp_list->data, NULL);
				g_free(temp_str);
			}
		}
		g_list_free_full(temp_list, (GDestroyNotify)g_free);

		ffddd_giveaway_get_start_date(giveaway, &start_date);
		ffddd_giveaway_get_end_date(giveaway, &end_date);
		full_time_string = g_strdup_printf("%u/%u/%u, %s - %u/%u/%u, %s",
		    start_date.year, start_date.month, start_date.day,
		    start_time, end_date.year, end_date.month, end_date.day,
		    end_time);

		gtk_list_store_append(giveaways_store, &tree_iter);
		gtk_list_store_set(giveaways_store, &tree_iter, ADDRESS_COLUMN,
		    address, NAME_COLUMN, _("Temp Name"), FOOD_COLUMN,
		    items_str, TIME_COLUMN, full_time_string, EXTRA_COLUMN,
		    info, -1);

		g_free(items_str);
		g_free(full_time_string);
	}

	gtk_widget_destroy(GTK_WIDGET(dialog));

	g_object_unref(giveaway);
}

void
ffddd_window_init_giveaways_view(FfdddWindow *win)
{
	FfdddWindowPrivate *priv;
	GtkTreeViewColumn *address_col;
	GtkTreeViewColumn *name_col;
	GtkTreeViewColumn *food_col;
	GtkTreeViewColumn *time_col;
	GtkTreeViewColumn *extra_col;
	GtkCellRenderer *text_render;

	priv = ffddd_window_get_instance_private(win);
	priv->giveaways_store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING,
	    G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(priv->giveaways_view),
	    GTK_TREE_MODEL(priv->giveaways_store));

	text_render = GTK_CELL_RENDERER(gtk_cell_renderer_text_new());

	address_col = gtk_tree_view_column_new_with_attributes(_("Address"),
	    text_render, "text", ADDRESS_COLUMN, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(priv->giveaways_view),
	    address_col);
	name_col = gtk_tree_view_column_new_with_attributes(_("Name"),
	    text_render, "text", NAME_COLUMN, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(priv->giveaways_view),
	    name_col);
	food_col = gtk_tree_view_column_new_with_attributes(_("Food"),
	    text_render, "text", FOOD_COLUMN, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(priv->giveaways_view),
	    food_col);
	time_col = gtk_tree_view_column_new_with_attributes(_("Time"),
	    text_render, "text", TIME_COLUMN, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(priv->giveaways_view),
	    time_col);
	extra_col = gtk_tree_view_column_new_with_attributes(_("Information"),
	    text_render, "text", EXTRA_COLUMN, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(priv->giveaways_view),
	    extra_col);
}
