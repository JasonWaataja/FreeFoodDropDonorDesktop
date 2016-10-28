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

#ifndef _FFDDDWINDOW_H_
#define _FFDDDWINDOW_H_

#include <gtk/gtk.h>

#include "ffdddapplication.h"

#define FFDDD_TYPE_WINDOW (ffddd_window_get_type())
#define FFDDD_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), FFDDD_TYPE_WINDOW\
	FfdddWindow))

typedef struct _FfdddWindow FfdddWindow;
typedef struct _FfdddWindowClass FfdddWindowClass;

GType		 ffddd_window_get_type(void);
FfdddWindow	*ffddd_window_new(FfdddApplication *app);

void		 ffddd_window_quit_activated(GSimpleAction *action, GVariant
    *parameter, gpointer win);
void		 ffddd_window_about_activated(GSimpleAction *action, GVariant
    *parameter, gpointer win);

void		 ffddd_window_add_new_giveaway(FfdddWindow *win);
void		 ffddd_window_init_giveaways_view(FfdddWindow *win);

void		 ffddd_window_on_send_button_clicked(FfdddWindow *window);

#endif /* _FFDDDWINDOW_H_ */
