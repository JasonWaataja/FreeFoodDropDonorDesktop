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
#include <curl/curl.h>
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

int
ffddd_get_location(const char *address, double *lat, double *lon)
{
	CURL *curl;
	CURLcode res;
	char *addr_formatted, *current_char, *query_str, *return_str;
	GList *lines;
	gchar **line_array;
	gchar *curr_line;

	curl = curl_easy_init();

	if (curl == NULL) {
		warn(_("Failed to init CURL"));
		return (-1);
	}

	/*curl_easy_setopt(curl, CURLOPT_URL, FFDDD_GOOGLE_URL);*/
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ffddd_curl_write_func);
	return_str = NULL;
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &return_str);

	/* Replace instances of " " with "+". */
	addr_formatted = g_strdup(address);
	for (current_char = &addr_formatted[0]; *current_char != '\0';
	    current_char++) {
		if (*current_char == ' ')
			*current_char = '+';
	}

	query_str = g_strconcat(FFDDD_GOOGLE_URL, "?address=", addr_formatted, "&key=",
	    FFDDD_GOOGLE_KEY, NULL);

	curl_easy_setopt(curl, CURLOPT_URL, query_str);

	res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		g_printerr(_("Failed to retrieve latitude and longitude: %s"),
		    curl_easy_strerror(res));
		g_free(addr_formatted);
		g_free(query_str);
		curl_easy_cleanup(curl);
		return (-1);
	}

	lines = NULL;
	printf("%s\n", return_str);

	curl_easy_cleanup(curl);

	g_free(addr_formatted);
	g_free(query_str);

	return (1);
}

size_t
ffddd_curl_write_func(void *buf, size_t size, size_t nmemb, char **message)
{
	size_t cur_len, new_len;

	g_assert(message != NULL);
	cur_len = (*message != NULL) ? strlen(*message) : 0;
	new_len = cur_len + (size * nmemb);

	*message = g_realloc(*message, new_len + 1);
	if (*message == NULL)
		err(1, NULL);

	memcpy(*message + cur_len, buf, size * nmemb);
	(*message)[new_len] = '\0';

	return (size * nmemb);
}
