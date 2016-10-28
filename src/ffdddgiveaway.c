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

#include "ffdddgiveaway.h"

struct _FfdddGiveaway {
	GObject	parent;
};

struct _FfdddGiveawayClass {
	GObjectClass	parent_class;
};

typedef struct _FfdddGiveawayPrivate FfdddGiveawayPrivate;

struct _FfdddGiveawayPrivate {
	struct FfdddDate start_date;
	struct FfdddDate end_date;
	gchar	*start_time;
	gchar	*end_time;
	gchar	*address;
	gchar	*info;
	GList	*items_list;
};

G_DEFINE_TYPE_WITH_PRIVATE(FfdddGiveaway, ffddd_giveaway, G_TYPE_OBJECT);

static void
ffddd_giveaway_init(FfdddGiveaway *giveaway)
{
	FfdddGiveawayPrivate *priv;
	
	priv = ffddd_giveaway_get_instance_private(giveaway);

	ffddd_date_zero(&priv->start_date);
	ffddd_date_zero(&priv->end_date);

	priv->start_time = NULL;
	priv->end_time = NULL;
	priv->address = NULL;
	priv->info = NULL;
	priv->items_list = NULL;
};

static void
ffddd_giveaway_class_init(FfdddGiveawayClass *kclass)
{
}

FfdddGiveaway *
ffddd_giveaway_new()
{

	return (g_object_new(FFDDD_TYPE_GIVEAWAY, NULL));
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
ffddd_date_equal(struct FfdddDate *d1, struct FfdddDate *d2)
{

	g_assert(d1 != NULL && d2 != NULL);

	return (d1->year == d2->year && d1->month == d2->month && d1->day ==
	    d2->day);
}

void
ffddd_giveaway_get_start_date(FfdddGiveaway *giveaway, struct FfdddDate *date)
{
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);

	g_assert(date != NULL);
	date->year = priv->start_date.year;
	date->month = priv->start_date.month;
	date->day = priv->start_date.day;
}

void
ffddd_giveaway_set_start_date(FfdddGiveaway *giveaway, struct FfdddDate *date)
{
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);

	g_assert(date != NULL);
	priv->start_date.year = date->year;
	priv->start_date.month = date->month;
	priv->start_date.day = date->day;
}

void
ffddd_giveaway_get_end_date(FfdddGiveaway *giveaway, struct FfdddDate *date)
{
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);

	g_assert(date != NULL);
	date->year = priv->end_date.year;
	date->month = priv->end_date.month;
	date->day = priv->end_date.day;
}

void
ffddd_giveaway_set_end_date(FfdddGiveaway *giveaway, struct FfdddDate *date)
{
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);

	g_assert(date != NULL);
	priv->end_date.year = date->year;
	priv->end_date.month = date->month;
	priv->end_date.day = date->day;
}

const gchar *
ffddd_giveaway_get_start_time(FfdddGiveaway *giveaway)
{
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);
	return (priv->start_time);
}

void
ffddd_giveaway_set_start_time(FfdddGiveaway *giveaway, const gchar *start_time)
{
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);
	g_free(priv->start_time);
	priv->start_time = g_strdup(start_time);
}

const gchar *
ffddd_giveaway_get_end_time(FfdddGiveaway *giveaway)
{
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);
	return (priv->end_time);
}

void
ffddd_giveaway_set_end_time(FfdddGiveaway *giveaway, const gchar *end_time)
{
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);
	g_free(priv->end_time);
	priv->end_time = g_strdup(end_time);
}

GList *
ffddd_giveaway_get_items_copy(FfdddGiveaway *giveaway)
{
	GList *copy;
	GList *list_item;
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);

	copy = NULL;
	for (list_item = g_list_first(priv->items_list); list_item != NULL;
	    list_item = g_list_next(list_item)) {
		copy = g_list_append(copy, g_strdup(list_item->data));
	}

	return (copy);
}

void
ffddd_giveaway_set_items(FfdddGiveaway *giveaway, GList *items_list)
{
	FfdddGiveawayPrivate *priv;
	GList *temp_item;

	priv = ffddd_giveaway_get_instance_private(giveaway);

	g_list_free_full(priv->items_list, (GDestroyNotify)g_free);
	priv->items_list = NULL;
	for (temp_item = g_list_first(items_list); temp_item != NULL;
	    temp_item = g_list_next(temp_item)) {
		priv->items_list = g_list_append(priv->items_list,
		    g_strdup(temp_item->data));
	}
}

const gchar *
ffddd_giveaway_get_info(FfdddGiveaway *giveaway)
{
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);
	return (priv->info);
}

void
ffddd_giveaway_set_info(FfdddGiveaway *giveaway, const gchar *info)
{
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);
	g_free(priv->info);
	priv->info = g_strdup(info);
}

const gchar *
ffddd_giveaway_get_address(FfdddGiveaway *giveaway)
{
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);
	return (priv->address);
}

void
ffddd_giveaway_set_address(FfdddGiveaway *giveaway, const gchar *address)
{
	FfdddGiveawayPrivate *priv;

	priv = ffddd_giveaway_get_instance_private(giveaway);
	g_free(priv->address);
	priv->address = g_strdup(address);
}
