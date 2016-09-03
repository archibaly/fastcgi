#ifndef _FCGIAPP_H_
#define _FCGIAPP_H_

struct fcgi_stream {
};

fcgi_header make_header(unsigned char type, unsigned short request_id,
						unsigned short content_length, unsigned char padding_length);
fcgi_begin_request_body make_begin_request_body(unsigned short role, unsigned char keep_connection);
fcgi_end_request_body make_end_request_body(unsigned int app_status, unsigned char protocol_status);
fcgi_unknown_type_body make_unknown_type_body(unsigned char type);

#endif /* _FCGIAPP_H_ */
