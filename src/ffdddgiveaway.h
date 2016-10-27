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

#ifndef _FFDDDGIVEAWAY_H_
#define _FFDDDGIVEAWAY_H_

#include <gtk/gtk.h>

#define FFDDD_TYPE_GIVEAWAY (ffddd_giveaway_get_type())
#define FFDDD_GIVEAWAY(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),		      \
	FFDDD_TYPE_GIVEAWA, FfdddGiveawa))

typedef struct _FfdddGiveaway FfdddGiveaway;
typedef struct _FfdddGiveawayClass FfdddGiveawayClass;

struct FfdddDate {
	guint	year;
	guint	month;
	guint	day;
};

void		 ffddd_date_zero(struct FfdddDate *date);
void		 ffddd_date_set(struct FfdddDate *date, unsigned int
    year, unsigned int month, unsigned int day);


GType		 ffddd_giveaway_get_type(void);
FfdddGiveaway	*ffddd_giveaway_new(void);

void		 ffddd_giveaway_get_start_date(FfdddGiveaway *giveaway, struct
    FfdddDate *date);
void		 ffddd_giveaway_set_start_date(FfdddGiveaway *giveaway, struct
    FfdddDate *date);

void		 ffddd_giveaway_get_end_date(FfdddGiveaway *giveaway, struct
    FfdddDate *date);

void		 ffddd_giveaway_set_end_date(FfdddGiveaway *giveaway, struct
    FfdddDate *date);

const gchar	*ffddd_giveaway_get_start_time(FfdddGiveaway *giveaway);
void		 ffddd_giveaway_set_start_time(FfdddGiveaway *giveaway, const
    gchar *start_time);
const gchar	*ffddd_giveaway_get_end_time(FfdddGiveaway *giveaway);
void		 ffddd_giveaway_set_end_time(FfdddGiveaway *giveaway, const
    gchar *end_time);

GList		*ffddd_giveaway_get_items_copy(FfdddGiveaway *giveaway);
void		 ffddd_giveaway_set_items(FfdddGiveaway *giveaway, GList
    *items_list);

const gchar	*ffddd_giveaway_get_info(FfdddGiveaway *giveaway);
void		 ffddd_giveaway_set_info(FfdddGiveaway *giveaway, const gchar
    *info);

const gchar	*ffddd_giveaway_get_address(FfdddGiveaway *giveaway);
void		 ffddd_giveaway_set_address(FfdddGiveaway *giveaway, const
    gchar *address);

#endif /* !_FFDDDGIVEAWAY_H_ */
