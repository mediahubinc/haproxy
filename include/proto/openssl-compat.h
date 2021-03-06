#ifndef _PROTO_OPENSSL_COMPAT_H
#define _PROTO_OPENSSL_COMPAT_H
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#if (defined SSL_CTRL_SET_TLSEXT_STATUS_REQ_CB && !defined OPENSSL_NO_OCSP)
#include <openssl/ocsp.h>
#endif
#ifndef OPENSSL_NO_DH
#include <openssl/dh.h>
#endif

#if (OPENSSL_VERSION_NUMBER < 0x1000000fL)
/*
 * Functions introduced in OpenSSL 1.0.1
 */
static inline int SSL_SESSION_set1_id_context(SSL_SESSION *s, const unsigned char *sid_ctx, unsigned int sid_ctx_len)
{
	s->sid_ctx_length = sid_ctx_len;
	memcpy(s->sid_ctx, sid_ctx, sid_ctx_len);
	return 1;
}

static inline int EVP_PKEY_base_id(const EVP_PKEY *pkey)
{
	return EVP_PKEY_type(pkey->type);
}

static inline const unsigned char *SSL_SESSION_get_id(const SSL_SESSION *sess, unsigned int *sid_length)
{
	*sid_length = sess->session_id_length;
	return sess->session_id;
}

static inline X509_NAME_ENTRY *X509_NAME_get_entry(const X509_NAME *name, int loc)
{
	return sk_X509_NAME_ENTRY_value(name->entries, loc);
}

static inline ASN1_OBJECT *X509_NAME_ENTRY_get_object(const X509_NAME_ENTRY *ne)
{
	return ne->object;
}

static inline ASN1_STRING *X509_NAME_ENTRY_get_data(const X509_NAME_ENTRY *ne)
{
	return ne->value;
}

static inline int ASN1_STRING_length(const ASN1_STRING *x)
{
	return x->length;
}

static inline int X509_NAME_entry_count(X509_NAME *name)
{
	return sk_X509_NAME_ENTRY_num(name->entries)
}

int X509_PUBKEY_get0_param(ASN1_OBJECT **ppkalg, const unsigned char **pk, int *ppklen, X509_ALGOR **pa, X509_PUBKEY *pub)
{
	*ppkalg = pub->algor->algorithm;
	return 1;
}

static inline void X509_ALGOR_get0(ASN1_OBJECT **paobj, int *pptype, const void **ppval, const X509_ALGOR *algor)
{
	*paobj = algor->algorithm;
}

#ifndef X509_get_X509_PUBKEY
#define X509_get_X509_PUBKEY(x) ((x)->cert_info->key
#endif

#endif

#if (OPENSSL_VERSION_NUMBER < 0x1010000fL)
/*
 * Functions introduced in OpenSSL 1.1.0
 */

static inline const unsigned char *SSL_SESSION_get0_id_context(const SSL_SESSION *sess, unsigned int *sid_ctx_length)
{
	*sid_ctx_length = sess->sid_ctx_length;
	return sess->sid_ctx;
}

static inline int SSL_SESSION_set1_id(SSL_SESSION *s, const unsigned char *sid, unsigned int sid_len)
{
	s->session_id_length = sid_len;
	memcpy(s->session_id, sid, sid_len);
	return 1;
}

static inline const OCSP_CERTID *OCSP_SINGLERESP_get0_id(const OCSP_SINGLERESP *single)
{
	return single->certId;
}

static inline pem_password_cb *SSL_CTX_get_default_passwd_cb(SSL_CTX *ctx)
{
	return ctx->default_passwd_callback;
}

static inline void *SSL_CTX_get_default_passwd_cb_userdata(SSL_CTX *ctx)
{
	return ctx->default_passwd_callback_userdata;
}

#ifndef OPENSSL_NO_DH
static inline int DH_set0_pqg(DH *dh, BIGNUM *p, BIGNUM *q, BIGNUM *g)
{
	/* Implements only the bare necessities for HAProxy */
	dh->p = p;
	dh->g = g;
	return 1;
}
#endif

static inline const unsigned char *ASN1_STRING_get0_data(const ASN1_STRING *x)
{
	return x->data;
}

static inline X509_ALGOR *X509_get0_tbs_sigalg(const X509 *x)
{
	return x->cert_info->signature;
}

#endif

#if (OPENSSL_VERSION_NUMBER >= 0x1010000fL)
#define __OPENSSL_110_CONST__ const
#else
#define __OPENSSL_110_CONST__
#endif

#endif /* _PROTO_OPENSSL_COMPAT_H */
