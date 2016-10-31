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

#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#include <glib.h>

#define	FFDDD_SERVER_ADDRESS "127.0.0.1"
#define FFDDD_SERVER_PORT "8111"
#define FFDDD_GOOGLE_URL "http://maps.googleapis.com/maps/api/geocode/json"
#define FFDDD_GOOGLE_KEY "AIzaSyBHJZR3rqsqZoB7Na1ATfJH9bQEHyARU78"

#define FFDDD_CONNECTION_ERROR ffddd_connection_error_quark()

enum FfddConnectionError {
	FFDDD_CONNECTION_ERROR_ADDRESS,
	FFDDD_CONNECTION_ERROR_SOCKET,
	FFDDD_CONNECTION_ERROR_CONNECT
};

int	ffddd_open_socket(GError **err);
GQuark	ffddd_connection_error_quark();

int	ffddd_get_location(const char *address, double *lat, double *lon);

#endif /* !_NETWORKING_H_ */
