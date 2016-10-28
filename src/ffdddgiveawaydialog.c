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
#include <string.h>

#include "ffdddgiveawaydialog.h"
#include "ffdddgiveaway.h"

struct _FfdddGiveawayDialog {
	GtkDialog parent;
};

struct _FfdddGiveawayDialogClass {
	GtkDialogClass parent_class;
};

typedef struct _FfdddGiveawayDialogPrivate FfdddGiveawayDialogPrivate;

struct _FfdddGiveawayDialogPrivate {
	GtkWidget		*food_items_view;
	GtkWidget		*select_start_button;
	GtkWidget		*select_end_button;
	GtkWidget		*item_entry;
	GtkWidget		*add_food_button;
	GtkWidget		*start_time_entry;
	GtkWidget		*end_time_entry;
	GtkWidget		*address_entry;
	GtkWidget		*info_view;
	GtkWidget		*name_entry;
	GtkListStore		*food_items_store;

	FfdddGiveaway		*giveaway;
};

G_DEFINE_TYPE_WITH_PRIVATE(FfdddGiveawayDialog, ffddd_giveaway_dialog,
    GTK_TYPE_DIALOG);

static void	ffddd_giveaway_dialog_dispose(GObject *giveaway);

static void
ffddd_giveaway_dialog_init(FfdddGiveawayDialog *dialog)
{
	FfdddGiveawayDialogPrivate *priv;

	priv = ffddd_giveaway_dialog_get_instance_private(dialog);
	gtk_widget_init_template(GTK_WIDGET(dialog));

	priv->giveaway = NULL;

	ffddd_giveaway_dialog_init_food_items_view(dialog);

	/*g_signal_connect_swapped(dialog, "response",*/
	    /*G_CALLBACK(gtk_widget_destroy), dialog);*/
	g_signal_connect_swapped(priv->select_start_button, "clicked",
	    G_CALLBACK(ffddd_giveaway_dialog_on_start_date_button_clicked),
	    dialog);
	g_signal_connect_swapped(priv->select_end_button, "clicked",
	    G_CALLBACK(ffddd_giveaway_dialog_on_end_date_button_clicked),
	    dialog);
	g_signal_connect_swapped(priv->add_food_button, "clicked",
	    G_CALLBACK(ffddd_giveaway_dialog_on_add_item_button_clicked),
	    dialog);
}

static void
ffddd_giveaway_dialog_class_init(FfdddGiveawayDialogClass *kclass)
{

	G_OBJECT_CLASS(kclass)->dispose = ffddd_giveaway_dialog_dispose;

	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(kclass),
	    "/com/waataja/ffddd/ui/giveawaydialog.ui");
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddGiveawayDialog, select_start_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddGiveawayDialog, select_end_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddGiveawayDialog, item_entry);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddGiveawayDialog, add_food_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddGiveawayDialog, food_items_view);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddGiveawayDialog, start_time_entry);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddGiveawayDialog, end_time_entry);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddGiveawayDialog, info_view);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddGiveawayDialog, address_entry);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddGiveawayDialog, name_entry);
}

FfdddGiveawayDialog *
ffddd_giveaway_dialog_new(GtkWindow *window, FfdddGiveaway *giveaway)
{
	FfdddGiveawayDialog *dialog;
	GtkDialog *as_dialog;

	dialog = g_object_new(FFDDD_TYPE_GIVEAWAY_DIALOG, "parent", window,
	    "title", FFDDD_GIVEAWAY_DIALOG_TITLE, NULL);
	as_dialog = GTK_DIALOG(dialog);

	gtk_dialog_add_buttons(as_dialog, _("OK"), GTK_RESPONSE_OK,
	    _("Cancel"), GTK_RESPONSE_CANCEL, NULL);

	gtk_window_set_transient_for(GTK_WINDOW(dialog), window);
	gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

	ffddd_giveaway_dialog_set_giveaway(dialog, giveaway);

	return (dialog);
}

gboolean
ffddd_window_check_info_consistency(FfdddGiveawayDialog *dialog)
{

	return (TRUE);
}

gboolean
ffddd_get_date_dialog(struct FfdddDate *date, GtkWindow *parent)
{
	GtkWidget *dialog;
	GtkDialog *as_dialog;
	GtkWidget *calendar;
	GtkContainer *content_area;
	gint response_id;

	dialog = gtk_dialog_new_with_buttons(_("Select Date"), parent,
	    GTK_DIALOG_MODAL, _("OK"), GTK_RESPONSE_OK, _("Cancel"),
	    GTK_RESPONSE_CANCEL, NULL);

	as_dialog = GTK_DIALOG(dialog);

	calendar = gtk_calendar_new();

	content_area = GTK_CONTAINER(gtk_dialog_get_content_area(as_dialog));
	gtk_container_add(content_area, calendar);
	gtk_widget_set_visible(calendar, TRUE);

	response_id = gtk_dialog_run(as_dialog);

	if (response_id == GTK_RESPONSE_OK) {
		ffddd_get_calendar_date(GTK_CALENDAR(calendar), date);
		gtk_widget_destroy(dialog);
		return (TRUE);
	} else {
		gtk_widget_destroy(dialog);
		return (FALSE);
	}
}

void
ffddd_get_calendar_date(GtkCalendar *calendar, struct FfdddDate *date)
{

	g_assert(date != NULL && calendar != NULL);

	gtk_calendar_get_date(calendar, &(date->year), &(date->month),
	    &(date->day));
}
void
ffddd_giveaway_dialog_on_start_date_button_clicked(FfdddGiveawayDialog *dialog)
{
	FfdddGiveawayDialogPrivate *priv;
	gboolean response;
	struct FfdddDate date;
	gchar *date_str;

	priv = ffddd_giveaway_dialog_get_instance_private(dialog);

	response = ffddd_get_date_dialog(&date, GTK_WINDOW(dialog));

	if (response) {
		ffddd_giveaway_set_start_date(priv->giveaway, &date);
		date_str = g_strdup_printf("%u-%u-%u", date.year, date.month,
		    date.day);
		gtk_button_set_label(GTK_BUTTON(priv->select_start_button),
		    date_str);
		g_free(date_str);
	}
}

void
ffddd_giveaway_dialog_on_end_date_button_clicked(FfdddGiveawayDialog *dialog)
{
	FfdddGiveawayDialogPrivate *priv;
	gboolean response;
	struct FfdddDate date;
	gchar *date_str;

	priv = ffddd_giveaway_dialog_get_instance_private(dialog);

	response = ffddd_get_date_dialog(&date, GTK_WINDOW(dialog));

	if (response) {
		ffddd_giveaway_set_end_date(priv->giveaway, &date);
		date_str = g_strdup_printf("%u-%u-%u", date.year, date.month,
		    date.day);
		gtk_button_set_label(GTK_BUTTON(priv->select_end_button),
		    date_str);
		g_free(date_str);
	}
}

void
ffddd_giveaway_dialog_init_food_items_view(FfdddGiveawayDialog *dialog)
{
	FfdddGiveawayDialogPrivate *priv;
	GtkTreeView *food_items_view;
	GtkListStore *food_items_store;
	GtkTreeViewColumn *text_column;
	GtkCellRenderer *text_render;

	priv = ffddd_giveaway_dialog_get_instance_private(dialog);
	/* Create a list store with 1 column, which is a string. */
	priv->food_items_store = gtk_list_store_new(1, G_TYPE_STRING);

	food_items_view = GTK_TREE_VIEW(priv->food_items_view);
	food_items_store = GTK_LIST_STORE(priv->food_items_store);

	gtk_tree_view_set_model(food_items_view,
	    GTK_TREE_MODEL(food_items_store));

	text_render = GTK_CELL_RENDERER(gtk_cell_renderer_text_new());

	text_column = gtk_tree_view_column_new_with_attributes(_("Item"),
	    text_render, "text", 0, NULL);

	gtk_tree_view_append_column(food_items_view, text_column);
}

void
ffddd_giveaway_dialog_on_add_item_button_clicked(FfdddGiveawayDialog *dialog)
{
	FfdddGiveawayDialogPrivate *priv;
	const gchar *item_text;
	GtkTreeIter iter;

	priv = ffddd_giveaway_dialog_get_instance_private(dialog);
	item_text = gtk_entry_get_text(GTK_ENTRY(priv->item_entry));

	if (strlen(item_text) > 0) {
		gtk_list_store_append(priv->food_items_store, &iter);
		gtk_list_store_set(priv->food_items_store, &iter, 0, item_text, -1);
		gtk_entry_set_text(GTK_ENTRY(priv->item_entry), _(""));
	}
}


FfdddGiveaway *
ffddd_giveaway_dialog_get_giveaway(FfdddGiveawayDialog *dialog)
{
	FfdddGiveawayDialogPrivate *priv;

	priv = ffddd_giveaway_dialog_get_instance_private(dialog);
	return (priv->giveaway);
}

void
ffddd_giveaway_dialog_set_giveaway(FfdddGiveawayDialog *dialog, FfdddGiveaway
    *giveaway)
{
	FfdddGiveawayDialogPrivate *priv;

	priv = ffddd_giveaway_dialog_get_instance_private(dialog);
	
	g_clear_object(&priv->giveaway);

	priv->giveaway = giveaway;
	g_object_ref(giveaway);
}

static void
ffddd_giveaway_dialog_dispose(GObject *giveaway)
{
	FfdddGiveawayDialogPrivate *priv;

	priv =
	    ffddd_giveaway_dialog_get_instance_private(FFDDD_GIVEAWAY_DIALOG(giveaway));

	g_clear_object(&priv->giveaway);

	G_OBJECT_CLASS(ffddd_giveaway_dialog_parent_class)->dispose(giveaway);
}

void
ffddd_giveaway_dialog_get_gui_info(FfdddGiveawayDialog *dialog, FfdddGiveaway
    *giveaway)
{
	gboolean got_date;
	FfdddGiveawayDialogPrivate *priv;
	GtkTextBuffer *info_buffer;
	GtkTextIter start_iter, end_iter;
	GtkTreeIter list_iter;
	GtkTreeModel *items_store;
	GList *temp_list;
	gchar *temp_item;

	priv = ffddd_giveaway_dialog_get_instance_private(dialog);

	info_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(priv->info_view));
	gtk_text_buffer_get_start_iter(info_buffer, &start_iter);
	gtk_text_buffer_get_end_iter(info_buffer, &end_iter);

	items_store = GTK_TREE_MODEL(priv->food_items_store);

	/* I'm assuming the dates were already set. */
	ffddd_giveaway_set_start_time(priv->giveaway,
	    gtk_entry_get_text(GTK_ENTRY(priv->start_time_entry)));
	ffddd_giveaway_set_end_time(priv->giveaway,
	    gtk_entry_get_text(GTK_ENTRY(priv->end_time_entry)));
	ffddd_giveaway_set_address(priv->giveaway,
	    gtk_entry_get_text(GTK_ENTRY(priv->address_entry)));
	ffddd_giveaway_set_name(priv->giveaway,
	    gtk_entry_get_text(GTK_ENTRY(priv->name_entry)));
	ffddd_giveaway_set_info(priv->giveaway,
	    gtk_text_buffer_get_text(info_buffer, &start_iter, &end_iter,
		FALSE));

	temp_list = NULL;
	if (gtk_tree_model_get_iter_first(items_store, &list_iter)) {
		gtk_tree_model_get(items_store, &list_iter, 0,
		    &temp_item, -1);
		temp_list = g_list_append(temp_list, temp_item);
	}

	while (gtk_tree_model_iter_next(items_store, &list_iter)) {
		gtk_tree_model_get(items_store, &list_iter, 0,
		    &temp_item, -1);
		temp_list = g_list_append(temp_list, temp_item);
	}

	ffddd_giveaway_set_items(priv->giveaway, temp_list);
	g_list_free_full(temp_list, (GDestroyNotify)g_free);
}
