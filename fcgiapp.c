#include <string.h>

#include "fastcgi.h"

fcgi_header make_header(unsigned char type, unsigned short request_id,
						unsigned short content_length, unsigned char padding_length)
{
	fcgi_header header;

	header.version = FCGI_VERSION_1;
	header.type = type;
	header.request_id_b1 = (unsigned char)(request_id >> 8);
	header.request_id_b0 = (unsigned char)request_id;
	header.content_length_b1 = (unsigned char)(content_length >> 8);
	header.content_length_b0 = (unsigned char)content_length;
	header.padding_length = padding_length;
	header.reserved = 0;

	return header;
}

fcgi_begin_request_body make_begin_request_body(unsigned short role, unsigned char keep_connection)
{
	fcgi_begin_request_body body;

	body.role_b1 = (unsigned char)(role >> 8);
	body.role_b0 = (unsigned char)role;
	body.flags = keep_connection;
	memset(body.reserved, 0, sizeof(body.reserved));

	return body;
}

fcgi_end_request_body make_end_request_body(unsigned int app_status, unsigned char protocol_status)
{
	fcgi_end_request_body body;
	body.app_status_b3 = (unsigned char)(app_status >> 24);
	body.app_status_b2 = (unsigned char)(app_status >> 16);
	body.app_status_b1 = (unsigned char)(app_status >> 8);
	body.app_status_b0 = (unsigned char)(app_status);
	body.protocol_status = protocol_status;
	memset(body.reserved, 0, sizeof(body.reserved));

	return body;
}

fcgi_unknown_type_body make_unknown_type_body(unsigned char type)
{
	fcgi_unknown_type_body body;
	body.type = type;
	memset(body.reserved, 0, sizeof(body.reserved));

	return body;
}
