/*
 * This file generated automatically from res.xml by c_client.py.
 * Edit at your peril.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>  /* for offsetof() */
#include "xcbext.h"
#include "res.h"

#define ALIGNOF(type) offsetof(struct { char dummy; type member; }, member)
#include "xproto.h"

xcb_extension_t xcb_res_id = { "X-Resource", 0 };


/*****************************************************************************
 **
 ** void xcb_res_client_next
 ** 
 ** @param xcb_res_client_iterator_t *i
 ** @returns void
 **
 *****************************************************************************/
 
void
xcb_res_client_next (xcb_res_client_iterator_t *i  /**< */)
{
    --i->rem;
    ++i->data;
    i->index += sizeof(xcb_res_client_t);
}


/*****************************************************************************
 **
 ** xcb_generic_iterator_t xcb_res_client_end
 ** 
 ** @param xcb_res_client_iterator_t i
 ** @returns xcb_generic_iterator_t
 **
 *****************************************************************************/
 
xcb_generic_iterator_t
xcb_res_client_end (xcb_res_client_iterator_t i  /**< */)
{
    xcb_generic_iterator_t ret;
    ret.data = i.data + i.rem;
    ret.index = i.index + ((char *) ret.data - (char *) i.data);
    ret.rem = 0;
    return ret;
}


/*****************************************************************************
 **
 ** void xcb_res_type_next
 ** 
 ** @param xcb_res_type_iterator_t *i
 ** @returns void
 **
 *****************************************************************************/
 
void
xcb_res_type_next (xcb_res_type_iterator_t *i  /**< */)
{
    --i->rem;
    ++i->data;
    i->index += sizeof(xcb_res_type_t);
}


/*****************************************************************************
 **
 ** xcb_generic_iterator_t xcb_res_type_end
 ** 
 ** @param xcb_res_type_iterator_t i
 ** @returns xcb_generic_iterator_t
 **
 *****************************************************************************/
 
xcb_generic_iterator_t
xcb_res_type_end (xcb_res_type_iterator_t i  /**< */)
{
    xcb_generic_iterator_t ret;
    ret.data = i.data + i.rem;
    ret.index = i.index + ((char *) ret.data - (char *) i.data);
    ret.rem = 0;
    return ret;
}


/*****************************************************************************
 **
 ** xcb_res_query_version_cookie_t xcb_res_query_version
 ** 
 ** @param xcb_connection_t *c
 ** @param uint8_t           client_major
 ** @param uint8_t           client_minor
 ** @returns xcb_res_query_version_cookie_t
 **
 *****************************************************************************/
 
xcb_res_query_version_cookie_t
xcb_res_query_version (xcb_connection_t *c  /**< */,
                       uint8_t           client_major  /**< */,
                       uint8_t           client_minor  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
        /* count */ 2,
        /* ext */ &xcb_res_id,
        /* opcode */ XCB_RES_QUERY_VERSION,
        /* isvoid */ 0
    };
    
    struct iovec xcb_parts[4];
    xcb_res_query_version_cookie_t xcb_ret;
    xcb_res_query_version_request_t xcb_out;
    
    xcb_out.client_major = client_major;
    xcb_out.client_minor = client_minor;
    
    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    
    xcb_ret.sequence = xcb_send_request(c, XCB_REQUEST_CHECKED, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_res_query_version_cookie_t xcb_res_query_version_unchecked
 ** 
 ** @param xcb_connection_t *c
 ** @param uint8_t           client_major
 ** @param uint8_t           client_minor
 ** @returns xcb_res_query_version_cookie_t
 **
 *****************************************************************************/
 
xcb_res_query_version_cookie_t
xcb_res_query_version_unchecked (xcb_connection_t *c  /**< */,
                                 uint8_t           client_major  /**< */,
                                 uint8_t           client_minor  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
        /* count */ 2,
        /* ext */ &xcb_res_id,
        /* opcode */ XCB_RES_QUERY_VERSION,
        /* isvoid */ 0
    };
    
    struct iovec xcb_parts[4];
    xcb_res_query_version_cookie_t xcb_ret;
    xcb_res_query_version_request_t xcb_out;
    
    xcb_out.client_major = client_major;
    xcb_out.client_minor = client_minor;
    
    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    
    xcb_ret.sequence = xcb_send_request(c, 0, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_res_query_version_reply_t * xcb_res_query_version_reply
 ** 
 ** @param xcb_connection_t                *c
 ** @param xcb_res_query_version_cookie_t   cookie
 ** @param xcb_generic_error_t            **e
 ** @returns xcb_res_query_version_reply_t *
 **
 *****************************************************************************/
 
xcb_res_query_version_reply_t *
xcb_res_query_version_reply (xcb_connection_t                *c  /**< */,
                             xcb_res_query_version_cookie_t   cookie  /**< */,
                             xcb_generic_error_t            **e  /**< */)
{
    return (xcb_res_query_version_reply_t *) xcb_wait_for_reply(c, cookie.sequence, e);
}

int
xcb_res_query_clients_sizeof (const void  *_buffer  /**< */)
{
    char *xcb_tmp = (char *)_buffer;
    const xcb_res_query_clients_reply_t *_aux = (xcb_res_query_clients_reply_t *)_buffer;
    unsigned int xcb_buffer_len = 0;
    unsigned int xcb_block_len = 0;
    unsigned int xcb_pad = 0;
    unsigned int xcb_align_to;


    xcb_block_len += sizeof(xcb_res_query_clients_reply_t);
    xcb_tmp += xcb_block_len;
    /* clients */
    xcb_block_len += _aux->num_clients * sizeof(xcb_res_client_t);
    xcb_tmp += xcb_block_len;
    xcb_align_to = ALIGNOF(xcb_res_client_t);
    /* insert padding */
    xcb_pad = -xcb_block_len & (xcb_align_to - 1);
    xcb_buffer_len += xcb_block_len + xcb_pad;
    if (0 != xcb_pad) {
        xcb_tmp += xcb_pad;
        xcb_pad = 0;
    }
    xcb_block_len = 0;

    return xcb_buffer_len;
}


/*****************************************************************************
 **
 ** xcb_res_query_clients_cookie_t xcb_res_query_clients
 ** 
 ** @param xcb_connection_t *c
 ** @returns xcb_res_query_clients_cookie_t
 **
 *****************************************************************************/
 
xcb_res_query_clients_cookie_t
xcb_res_query_clients (xcb_connection_t *c  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
        /* count */ 2,
        /* ext */ &xcb_res_id,
        /* opcode */ XCB_RES_QUERY_CLIENTS,
        /* isvoid */ 0
    };
    
    struct iovec xcb_parts[4];
    xcb_res_query_clients_cookie_t xcb_ret;
    xcb_res_query_clients_request_t xcb_out;
    
    
    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    
    xcb_ret.sequence = xcb_send_request(c, XCB_REQUEST_CHECKED, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_res_query_clients_cookie_t xcb_res_query_clients_unchecked
 ** 
 ** @param xcb_connection_t *c
 ** @returns xcb_res_query_clients_cookie_t
 **
 *****************************************************************************/
 
xcb_res_query_clients_cookie_t
xcb_res_query_clients_unchecked (xcb_connection_t *c  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
        /* count */ 2,
        /* ext */ &xcb_res_id,
        /* opcode */ XCB_RES_QUERY_CLIENTS,
        /* isvoid */ 0
    };
    
    struct iovec xcb_parts[4];
    xcb_res_query_clients_cookie_t xcb_ret;
    xcb_res_query_clients_request_t xcb_out;
    
    
    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    
    xcb_ret.sequence = xcb_send_request(c, 0, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_res_client_t * xcb_res_query_clients_clients
 ** 
 ** @param const xcb_res_query_clients_reply_t *R
 ** @returns xcb_res_client_t *
 **
 *****************************************************************************/
 
xcb_res_client_t *
xcb_res_query_clients_clients (const xcb_res_query_clients_reply_t *R  /**< */)
{
    return (xcb_res_client_t *) (R + 1);
}


/*****************************************************************************
 **
 ** int xcb_res_query_clients_clients_length
 ** 
 ** @param const xcb_res_query_clients_reply_t *R
 ** @returns int
 **
 *****************************************************************************/
 
int
xcb_res_query_clients_clients_length (const xcb_res_query_clients_reply_t *R  /**< */)
{
    return R->num_clients;
}


/*****************************************************************************
 **
 ** xcb_res_client_iterator_t xcb_res_query_clients_clients_iterator
 ** 
 ** @param const xcb_res_query_clients_reply_t *R
 ** @returns xcb_res_client_iterator_t
 **
 *****************************************************************************/
 
xcb_res_client_iterator_t
xcb_res_query_clients_clients_iterator (const xcb_res_query_clients_reply_t *R  /**< */)
{
    xcb_res_client_iterator_t i;
    i.data = (xcb_res_client_t *) (R + 1);
    i.rem = R->num_clients;
    i.index = (char *) i.data - (char *) R;
    return i;
}


/*****************************************************************************
 **
 ** xcb_res_query_clients_reply_t * xcb_res_query_clients_reply
 ** 
 ** @param xcb_connection_t                *c
 ** @param xcb_res_query_clients_cookie_t   cookie
 ** @param xcb_generic_error_t            **e
 ** @returns xcb_res_query_clients_reply_t *
 **
 *****************************************************************************/
 
xcb_res_query_clients_reply_t *
xcb_res_query_clients_reply (xcb_connection_t                *c  /**< */,
                             xcb_res_query_clients_cookie_t   cookie  /**< */,
                             xcb_generic_error_t            **e  /**< */)
{
    return (xcb_res_query_clients_reply_t *) xcb_wait_for_reply(c, cookie.sequence, e);
}

int
xcb_res_query_client_resources_sizeof (const void  *_buffer  /**< */)
{
    char *xcb_tmp = (char *)_buffer;
    const xcb_res_query_client_resources_reply_t *_aux = (xcb_res_query_client_resources_reply_t *)_buffer;
    unsigned int xcb_buffer_len = 0;
    unsigned int xcb_block_len = 0;
    unsigned int xcb_pad = 0;
    unsigned int xcb_align_to;


    xcb_block_len += sizeof(xcb_res_query_client_resources_reply_t);
    xcb_tmp += xcb_block_len;
    /* types */
    xcb_block_len += _aux->num_types * sizeof(xcb_res_type_t);
    xcb_tmp += xcb_block_len;
    xcb_align_to = ALIGNOF(xcb_res_type_t);
    /* insert padding */
    xcb_pad = -xcb_block_len & (xcb_align_to - 1);
    xcb_buffer_len += xcb_block_len + xcb_pad;
    if (0 != xcb_pad) {
        xcb_tmp += xcb_pad;
        xcb_pad = 0;
    }
    xcb_block_len = 0;

    return xcb_buffer_len;
}


/*****************************************************************************
 **
 ** xcb_res_query_client_resources_cookie_t xcb_res_query_client_resources
 ** 
 ** @param xcb_connection_t *c
 ** @param uint32_t          xid
 ** @returns xcb_res_query_client_resources_cookie_t
 **
 *****************************************************************************/
 
xcb_res_query_client_resources_cookie_t
xcb_res_query_client_resources (xcb_connection_t *c  /**< */,
                                uint32_t          xid  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
        /* count */ 2,
        /* ext */ &xcb_res_id,
        /* opcode */ XCB_RES_QUERY_CLIENT_RESOURCES,
        /* isvoid */ 0
    };
    
    struct iovec xcb_parts[4];
    xcb_res_query_client_resources_cookie_t xcb_ret;
    xcb_res_query_client_resources_request_t xcb_out;
    
    xcb_out.xid = xid;
    
    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    
    xcb_ret.sequence = xcb_send_request(c, XCB_REQUEST_CHECKED, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_res_query_client_resources_cookie_t xcb_res_query_client_resources_unchecked
 ** 
 ** @param xcb_connection_t *c
 ** @param uint32_t          xid
 ** @returns xcb_res_query_client_resources_cookie_t
 **
 *****************************************************************************/
 
xcb_res_query_client_resources_cookie_t
xcb_res_query_client_resources_unchecked (xcb_connection_t *c  /**< */,
                                          uint32_t          xid  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
        /* count */ 2,
        /* ext */ &xcb_res_id,
        /* opcode */ XCB_RES_QUERY_CLIENT_RESOURCES,
        /* isvoid */ 0
    };
    
    struct iovec xcb_parts[4];
    xcb_res_query_client_resources_cookie_t xcb_ret;
    xcb_res_query_client_resources_request_t xcb_out;
    
    xcb_out.xid = xid;
    
    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    
    xcb_ret.sequence = xcb_send_request(c, 0, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_res_type_t * xcb_res_query_client_resources_types
 ** 
 ** @param const xcb_res_query_client_resources_reply_t *R
 ** @returns xcb_res_type_t *
 **
 *****************************************************************************/
 
xcb_res_type_t *
xcb_res_query_client_resources_types (const xcb_res_query_client_resources_reply_t *R  /**< */)
{
    return (xcb_res_type_t *) (R + 1);
}


/*****************************************************************************
 **
 ** int xcb_res_query_client_resources_types_length
 ** 
 ** @param const xcb_res_query_client_resources_reply_t *R
 ** @returns int
 **
 *****************************************************************************/
 
int
xcb_res_query_client_resources_types_length (const xcb_res_query_client_resources_reply_t *R  /**< */)
{
    return R->num_types;
}


/*****************************************************************************
 **
 ** xcb_res_type_iterator_t xcb_res_query_client_resources_types_iterator
 ** 
 ** @param const xcb_res_query_client_resources_reply_t *R
 ** @returns xcb_res_type_iterator_t
 **
 *****************************************************************************/
 
xcb_res_type_iterator_t
xcb_res_query_client_resources_types_iterator (const xcb_res_query_client_resources_reply_t *R  /**< */)
{
    xcb_res_type_iterator_t i;
    i.data = (xcb_res_type_t *) (R + 1);
    i.rem = R->num_types;
    i.index = (char *) i.data - (char *) R;
    return i;
}


/*****************************************************************************
 **
 ** xcb_res_query_client_resources_reply_t * xcb_res_query_client_resources_reply
 ** 
 ** @param xcb_connection_t                         *c
 ** @param xcb_res_query_client_resources_cookie_t   cookie
 ** @param xcb_generic_error_t                     **e
 ** @returns xcb_res_query_client_resources_reply_t *
 **
 *****************************************************************************/
 
xcb_res_query_client_resources_reply_t *
xcb_res_query_client_resources_reply (xcb_connection_t                         *c  /**< */,
                                      xcb_res_query_client_resources_cookie_t   cookie  /**< */,
                                      xcb_generic_error_t                     **e  /**< */)
{
    return (xcb_res_query_client_resources_reply_t *) xcb_wait_for_reply(c, cookie.sequence, e);
}


/*****************************************************************************
 **
 ** xcb_res_query_client_pixmap_bytes_cookie_t xcb_res_query_client_pixmap_bytes
 ** 
 ** @param xcb_connection_t *c
 ** @param uint32_t          xid
 ** @returns xcb_res_query_client_pixmap_bytes_cookie_t
 **
 *****************************************************************************/
 
xcb_res_query_client_pixmap_bytes_cookie_t
xcb_res_query_client_pixmap_bytes (xcb_connection_t *c  /**< */,
                                   uint32_t          xid  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
        /* count */ 2,
        /* ext */ &xcb_res_id,
        /* opcode */ XCB_RES_QUERY_CLIENT_PIXMAP_BYTES,
        /* isvoid */ 0
    };
    
    struct iovec xcb_parts[4];
    xcb_res_query_client_pixmap_bytes_cookie_t xcb_ret;
    xcb_res_query_client_pixmap_bytes_request_t xcb_out;
    
    xcb_out.xid = xid;
    
    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    
    xcb_ret.sequence = xcb_send_request(c, XCB_REQUEST_CHECKED, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_res_query_client_pixmap_bytes_cookie_t xcb_res_query_client_pixmap_bytes_unchecked
 ** 
 ** @param xcb_connection_t *c
 ** @param uint32_t          xid
 ** @returns xcb_res_query_client_pixmap_bytes_cookie_t
 **
 *****************************************************************************/
 
xcb_res_query_client_pixmap_bytes_cookie_t
xcb_res_query_client_pixmap_bytes_unchecked (xcb_connection_t *c  /**< */,
                                             uint32_t          xid  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
        /* count */ 2,
        /* ext */ &xcb_res_id,
        /* opcode */ XCB_RES_QUERY_CLIENT_PIXMAP_BYTES,
        /* isvoid */ 0
    };
    
    struct iovec xcb_parts[4];
    xcb_res_query_client_pixmap_bytes_cookie_t xcb_ret;
    xcb_res_query_client_pixmap_bytes_request_t xcb_out;
    
    xcb_out.xid = xid;
    
    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    
    xcb_ret.sequence = xcb_send_request(c, 0, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_res_query_client_pixmap_bytes_reply_t * xcb_res_query_client_pixmap_bytes_reply
 ** 
 ** @param xcb_connection_t                            *c
 ** @param xcb_res_query_client_pixmap_bytes_cookie_t   cookie
 ** @param xcb_generic_error_t                        **e
 ** @returns xcb_res_query_client_pixmap_bytes_reply_t *
 **
 *****************************************************************************/
 
xcb_res_query_client_pixmap_bytes_reply_t *
xcb_res_query_client_pixmap_bytes_reply (xcb_connection_t                            *c  /**< */,
                                         xcb_res_query_client_pixmap_bytes_cookie_t   cookie  /**< */,
                                         xcb_generic_error_t                        **e  /**< */)
{
    return (xcb_res_query_client_pixmap_bytes_reply_t *) xcb_wait_for_reply(c, cookie.sequence, e);
}

