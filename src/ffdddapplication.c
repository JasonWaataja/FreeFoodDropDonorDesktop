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

#include "ffdddapplication.h"
#include "ffdddwindow.h"

struct _FfdddApplication {
	GtkApplication parent;
};

struct _FfdddApplicationClass {
	GtkApplicationClass parent_class;
};

typedef struct _FfdddApplicationPrivate FfdddApplicationPrivate;

struct _FfdddApplicationPrivate {
	FfdddWindow *win;
};

G_DEFINE_TYPE_WITH_PRIVATE(FfdddApplication, ffddd_application,
    GTK_TYPE_APPLICATION);

static void
ffddd_application_init(FfdddApplication *app)
{
}

static void
ffddd_application_startup(GApplication *app)
{

	G_APPLICATION_CLASS(ffddd_application_parent_class)->startup(app);
}

static void
ffddd_application_activate(GApplication *app)
{
	FfdddApplicationPrivate *priv;

	priv = ffddd_application_get_instance_private(FFDDD_APPLICATION(app));
	priv->win = ffddd_window_new(FFDDD_APPLICATION(app));

	gtk_window_present(GTK_WINDOW(priv->win));
}

static void
ffddd_application_class_init(FfdddApplicationClass *kclass)
{

	G_APPLICATION_CLASS(kclass)->startup = ffddd_application_startup;
	G_APPLICATION_CLASS(kclass)->activate = ffddd_application_activate;
}

FfdddApplication *
ffddd_application_new(void)
{
	return (g_object_new(FFDDD_TYPE_APPLICATION, "application-id",
	    "com.waataja.ffddd", NULL));
}
