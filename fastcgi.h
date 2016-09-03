/* 
 * fastcgi.h --
 *
 *	Defines for the FastCGI protocol.
 *
 *
 * Copyright (c) 1995-1996 Open Market, Inc.
 *
 * See the file "LICENSE" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * $Id: fastcgi.h,v 1.1 1997/09/16 15:36:32 stanleyg Exp $
 */

#ifndef _FASTCGI_H_
#define _FASTCGI_H_

/*
 * Listening socket file number
 */
#define FCGI_LISTENSOCK_FILENO	0

typedef struct {
	unsigned char version;
	unsigned char type;
	unsigned char request_id_b1;
	unsigned char request_id_b0;
	unsigned char content_length_b1;
	unsigned char content_length_b0;
	unsigned char padding_length;
	unsigned char reserved;
} fcgi_header;

#define FCGI_MAX_LENGTH	0xffff

/*
 * Number of bytes in a fcgi_header.
 * Future versions of the protocol will not reduce this number.
 */
#define FCGI_HEADER_LEN	8

/*
 * Value for version component of fcgi_header
 */
#define FCGI_VERSION_1	1

/*
 * Values for type component of fcgi_header
 */
#define FCGI_BEGIN_REQUEST		1
#define FCGI_ABORT_REQUEST		2
#define FCGI_END_REQUEST		3
#define FCGI_PARAMS				4
#define FCGI_STDIN				5
#define FCGI_STDOUT				6
#define FCGI_STDERR				7
#define FCGI_DATA				8
#define FCGI_GET_VALUES			9
#define FCGI_GET_VALUES_RESULT	10
#define FCGI_UNKNOWN_TYPE		11
#define FCGI_MAXTYPE			FCGI_UNKNOWN_TYPE

/*
 * Value for request_id component of fcgi_header
 */
#define FCGI_NULL_REQUEST_ID	0

typedef struct {
	unsigned char role_b1;
	unsigned char role_b0;
	unsigned char flags;
	unsigned char reserved[5];
} fcgi_begin_request_body;

typedef struct {
	fcgi_header header;
	fcgi_begin_request_body body;
} fcgi_begin_request_record;

/*
 * Mask for flags component of fcgi_begin_request_body
 */
#define FCGI_KEEP_CONN	1

/*
 * Values for role component of fcgi_begin_request_body
 */
#define FCGI_RESPONDER	1
#define FCGI_AUTHORIZER	2
#define FCGI_FILTER		3

typedef struct {
	unsigned char app_status_b3;
	unsigned char app_status_b2;
	unsigned char app_status_b1;
	unsigned char app_status_b0;
	unsigned char protocol_status;
	unsigned char reserved[3];
} fcgi_end_request_body;

typedef struct {
	fcgi_header header;
	fcgi_end_request_body body;
} fcgi_end_request_record;

/*
 * Values for protocol_status component of fcgi_end_request_body
 */
#define FCGI_REQUEST_COMPLETE	0
#define FCGI_CANT_MPX_CONN		1
#define FCGI_OVERLOADED			2
#define FCGI_UNKNOWN_ROLE		3

/*
 * Variable names for FCGI_GET_VALUES / FCGI_GET_VALUES_RESULT records
 */
#define FCGI_MAX_CONNS	"FCGI_MAX_CONNS"
#define FCGI_MAX_REQS	"FCGI_MAX_REQS"
#define FCGI_MPXS_CONNS	"FCGI_MPXS_CONNS"

typedef struct {
	unsigned char type;
	unsigned char reserved[7];
} fcgi_unknown_type_body;

typedef struct {
	fcgi_header header;
	fcgi_unknown_type_body body;
} fcgi_unknown_type_record;

#endif /* _FASTCGI_H_ */
