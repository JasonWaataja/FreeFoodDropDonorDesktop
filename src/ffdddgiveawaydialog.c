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

#include "ffdddgiveawaydialog.h"

struct _FfdddGiveawayDialog {
	GtkDialog parent;
};

struct _FfdddGiveawayDialogClass {
	GtkDialogClass parent_class;
};

typedef struct _FfdddGiveawayDialogPrivate FfdddGiveawayDialogPrivate;

struct _FfdddGiveawayDialogPrivate {
	GtkWidget		*giveaways_view;
	GtkWidget		*select_start_button;
	GtkWidget		*select_end_button;
	struct FfdddDate	start_date;
	struct FfdddDate	end_date;
	unsigned int		start_time; /* Start time in minutes. */
	unsigned int		end_time; /* End time in minutes. */
};

G_DEFINE_TYPE_WITH_PRIVATE(FfdddGiveawayDialog, ffddd_giveaway_dialog,
    GTK_TYPE_DIALOG);

static void
ffddd_giveaway_dialog_init(FfdddGiveawayDialog *dialog)
{
	FfdddGiveawayDialogPrivate *priv;

	priv = ffddd_giveaway_dialog_get_instance_private(dialog);
	gtk_widget_init_template(GTK_WIDGET(dialog));

	ffddd_date_zero(&(priv->start_date));
	ffddd_date_zero(&(priv->end_date));

	g_signal_connect_swapped(dialog, "response",
	    G_CALLBACK(gtk_widget_destroy), dialog);
	g_signal_connect_swapped(priv->select_start_button, "clicked",
	    G_CALLBACK(ffddd_giveaway_dialog_on_start_date_button_clicked),
	    dialog);
	g_signal_connect_swapped(priv->select_end_button, "clicked",
	    G_CALLBACK(ffddd_giveaway_dialog_on_end_date_button_clicked),
	    dialog);
}

static void
ffddd_giveaway_dialog_class_init(FfdddGiveawayDialogClass *kclass)
{

	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(kclass),
	    "/com/waataja/ffddd/ui/giveawaydialog.ui");
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddGiveawayDialog, select_start_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(kclass),
	    FfdddGiveawayDialog, select_end_button);
}

FfdddGiveawayDialog *
ffddd_giveaway_dialog_new(GtkWindow *window)
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

	return (dialog);
}

void
fffdd_giveaway_dialog_on_response(FfdddGiveawayDialog *dialog, gint
    response_id, gpointer user_data)
{
	gboolean got_date;
	struct FfdddDate date;
	
	if (response_id == GTK_RESPONSE_OK) {
		got_date = ffddd_get_date_dialog(&date, GTK_WINDOW(dialog));
	}

	gtk_widget_destroy(GTK_WIDGET(dialog));
}

gboolean
ffddd_window_check_info_consistency(FfdddGiveawayDialog *dialog)
{

	return (TRUE);
}

void
ffddd_date_zero(struct FfdddDate *date)
{

	g_assert(date != NULL);
	date->year = 0;
	date->month = 0;
	date->day = 0;
}

void
ffddd_date_set(struct FfdddDate *date, unsigned int year, unsigned int month,
    unsigned int day)
{

	g_assert(date != NULL);
	date->year = year;
	date->month = month;
	date->day = day;
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

	g_signal_connect_swapped(as_dialog, "response",
	    G_CALLBACK(gtk_widget_destroy), as_dialog);

	response_id = gtk_dialog_run(as_dialog);

	if (response_id == GTK_RESPONSE_OK) {
		ffddd_get_calendar_date(GTK_CALENDAR(calendar), date);
		return (TRUE);
	} else {
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

	priv = ffddd_giveaway_dialog_get_instance_private(dialog);

	response = ffddd_get_date_dialog(&priv->start_date, GTK_WINDOW(dialog));

	if (!response)
		ffddd_date_zero(&priv->start_date);
}

void
ffddd_giveaway_dialog_on_end_date_button_clicked(FfdddGiveawayDialog *dialog)
{
	FfdddGiveawayDialogPrivate *priv;
	gboolean response;

	priv = ffddd_giveaway_dialog_get_instance_private(dialog);

	response = ffddd_get_date_dialog(&priv->end_date, GTK_WINDOW(dialog));

	if (!response)
		ffddd_date_zero(&priv->end_date);
}
