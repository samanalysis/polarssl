/*
 *  Public Key abstraction layer
 *
 *  Copyright (C) 2006-2013, Brainspark B.V.
 *
 *  This file is part of PolarSSL (http://www.polarssl.org)
 *  Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "polarssl/config.h"

#include "polarssl/pk.h"

#if defined(POLARSSL_RSA_C)
#include "polarssl/rsa.h"
#endif
#if defined(POLARSSL_ECP_C)
#include "polarssl/ecp.h"
#endif
#if defined(POLARSSL_ECDSA_C)
#include "polarssl/ecdsa.h"
#endif

#include <stdlib.h>

/*
 * Initialise a pk_context
 */
void pk_init( pk_context *ctx )
{
    if( ctx == NULL )
        return;

    ctx->type = POLARSSL_PK_NONE;
    ctx->data = NULL;
    ctx->dont_free = 0;
}

/*
 * Free (the components of) a pk_context
 */
void pk_free( pk_context *ctx )
{
    if( ctx == NULL )
        return;

#if defined(POLARSSL_RSA_C)
    if( ctx->type == POLARSSL_PK_RSA )
        rsa_free( ctx->data );
    else
#endif
#if defined(POLARSSL_ECP_C)
    if( ctx->type == POLARSSL_PK_ECKEY || ctx->type == POLARSSL_PK_ECKEY_DH )
        ecp_keypair_free( ctx->data );
    else
#endif
#if defined(POLARSSL_ECDSA_C)
    if( ctx->type == POLARSSL_PK_ECDSA )
        ecdsa_free( ctx->data );
    else
#endif
    {
        ; /* guard for the else's above */
    }

    if( ! ctx->dont_free )
        free( ctx->data );

    ctx->type = POLARSSL_PK_NONE;
    ctx->data = NULL;
}

/*
 * Set a pk_context to a given type
 */
int pk_set_type( pk_context *ctx, pk_type_t type )
{
    size_t size;

    if( ctx->type == type )
        return( 0 );

    if( ctx->type != POLARSSL_PK_NONE )
        return( POLARSSL_ERR_PK_TYPE_MISMATCH );

#if defined(POLARSSL_RSA_C)
    if( type == POLARSSL_PK_RSA )
        size = sizeof( rsa_context );
    else
#endif
#if defined(POLARSSL_ECP_C)
    if( type == POLARSSL_PK_ECKEY || type == POLARSSL_PK_ECKEY_DH )
        size = sizeof( ecp_keypair );
    else
#endif
#if defined(POLARSSL_ECDSA_C)
    if( type == POLARSSL_PK_ECDSA )
        size = sizeof( ecdsa_context );
    else
#endif
        size = 0; /* should never be executed */

    if( ( ctx->data = malloc( size ) ) == NULL )
        return( POLARSSL_ERR_PK_MALLOC_FAILED );

    memset( ctx->data, 0, size );
    ctx->type = type;

    return( 0 );
}

#if defined(POLARSSL_RSA_C)
/*
 * Wrap an RSA context in a PK context
 */
int pk_wrap_rsa( pk_context *ctx, const rsa_context *rsa)
{
    if( ctx->type != POLARSSL_PK_NONE )
        return( POLARSSL_ERR_PK_TYPE_MISMATCH );

    ctx->type = POLARSSL_PK_RSA;
    ctx->data = (rsa_context *) rsa;
    ctx->dont_free = 1;

    return( 0 );
}
#endif
