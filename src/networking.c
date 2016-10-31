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

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <glib/gi18n.h>

#include "networking.h"

int
ffddd_open_socket(GError **err)
{
	int sockfd, status;
	struct addrinfo *serv_info, hints;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(FFDDD_SERVER_ADDRESS, FFDDD_SERVER_PORT, &hints,
	    &serv_info);

	if (status == -1) {
		warn(_("Failed to create address"));
		g_set_error(err, FFDDD_CONNECTION_ERROR,
		    FFDDD_CONNECTION_ERROR_ADDRESS, _("Failed to create address: %s"),
		    g_strerror(errno));

		return (-1);
	}

	sockfd = socket(serv_info->ai_family, serv_info->ai_socktype,
	    serv_info->ai_protocol);

	if (sockfd == -1) {
		warn(_("Failed to create socket"));
		freeaddrinfo(serv_info);
		g_set_error(err, FFDDD_CONNECTION_ERROR,
		    FFDDD_CONNECTION_ERROR_SOCKET, _("Failed to create socket: %s"),
		    g_strerror(errno));
		return (-1);
	}

	status = connect(sockfd, serv_info->ai_addr, serv_info->ai_addrlen);

	if (status == -1) {
		warn(_("Failed to create connect"));
		freeaddrinfo(serv_info);
		g_set_error(err, FFDDD_CONNECTION_ERROR,
		    FFDDD_CONNECTION_ERROR_CONNECT, _("Failed to connect: %s"),
		    g_strerror(errno));
		return (-1);
	}

	freeaddrinfo(serv_info);
	
	return (sockfd);
}

GQuark
ffddd_connection_error_quark()
{

	return (g_quark_from_static_string("ffddd-connection-error-quark"));
}
