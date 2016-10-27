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

#ifndef _FFDDDGIVEAWAYDIALOG_H_
#define _FFDDDGIVEAWAYDIALOG_H_

#include <gtk/gtk.h>

#include "ffdddgiveaway.h"

#define FFDDD_TYPE_GIVEAWAY_DIALOG (ffddd_giveaway_dialog_get_type())
#define FFDDD_GIVEAWAY_DIALOG(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),	      \
	FFDDD_TYPE_GIVEAWAY_DIALOG, FfdddGiveawayDialog))

#define FFDDD_GIVEAWAY_DIALOG_TITLE "FreeFoodDropDonorDesktop"

typedef struct _FfdddGiveawayDialog FfdddGiveawayDialog;
typedef struct _FfdddGiveawayDialogClass FfdddGiveawayDialogClass;

GType			 ffddd_giveaway_dialog_get_type(void);
FfdddGiveawayDialog	*ffddd_giveaway_dialog_new(GtkWindow *parent,
    FfdddGiveaway *giveaway);

gboolean
ffddd_window_check_info_consistency(FfdddGiveawayDialog *dialog);
void			 fffdd_giveaway_dialog_on_response(FfdddGiveawayDialog
    *dialog, gint response_id, gpointer user_data);

gboolean		 ffddd_get_date_dialog(struct FfdddDate *date,
    GtkWindow *parent);

void			 ffddd_get_calendar_date(GtkCalendar *calendar, struct
    FfdddDate *date);

void
ffddd_giveaway_dialog_on_start_date_button_clicked(FfdddGiveawayDialog *dialog);

void
ffddd_giveaway_dialog_on_end_date_button_clicked(FfdddGiveawayDialog *dialog);

void
ffddd_giveaway_dialog_init_food_items_view(FfdddGiveawayDialog *dialog);

void
ffddd_giveaway_dialog_on_add_item_button_clicked(FfdddGiveawayDialog *dialog);

FfdddGiveaway
*ffddd_giveaway_dialog_get_giveaway(FfdddGiveawayDialog *dialog);

void
ffddd_giveaway_dialog_set_giveaway(FfdddGiveawayDialog *dialog, FfdddGiveaway
    *giveaway);

void			 ffddd_giveaway_dialog_get_gui_info(FfdddGiveawayDialog
    *dialog, FfdddGiveaway *giveaway);

#endif /* !_FFDDDGIVEAWAYDIALOG_H_ */
