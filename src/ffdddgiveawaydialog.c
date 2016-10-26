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
};

G_DEFINE_TYPE_WITH_PRIVATE(FfdddGiveawayDialog, ffddd_giveaway_dialog,
    GTK_TYPE_DIALOG);

static void
ffddd_giveaway_dialog_init(FfdddGiveawayDialog *win)
{
	FfdddGiveawayDialogPrivate *priv;

	priv = ffddd_giveaway_dialog_get_instance_private(win);
	gtk_widget_init_template(GTK_WIDGET(win));

	g_signal_connect_swapped(win, "response",
	    G_CALLBACK(gtk_widget_destroy), win);
}

static void
ffddd_giveaway_dialog_class_init(FfdddGiveawayDialogClass *kclass)
{

	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(kclass),
	    "/com/waataja/ffddd/ui/giveawaydialog.ui");
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
