/*
 * Copyright 2014 James Geboski <jgeboski@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @file **/

#ifndef _FACEBOOK_API_H
#define _FACEBOOK_API_H

#include <bitlbee.h>

#include "facebook-http.h"
#include "facebook-json.h"

#define FB_API_HOST   "api.facebook.com"
#define FB_API_BHOST  "b-api.facebook.com"
#define FB_API_AGENT  "Facebook App / " PACKAGE " / " PACKAGE_VERSION
#define FB_API_KEY    "256002347743983"
#define FB_API_SECRET "374e60f8b9bb6b8cbb30f78030438895"

#define FB_API_PATH_AUTH "/method/auth.login"


/**
 * Executes one of the #fb_api_funcs.
 *
 * @param a   The #fb_api.
 * @param f   The function to execute.
 * @param ... The function arguments.
 **/
#define FB_API_FUNC(m, f, ...)                         \
    G_STMT_START {                                     \
        if (G_LIKELY((m)->funcs.f != NULL)) {          \
            (m)->funcs.f(m, ##__VA_ARGS__, (m)->data); \
        }                                              \
    } G_STMT_END


/** The #GError codes of #fb_api. **/
typedef enum fb_api_error fb_api_error_t;

/** The structure for interacting with the Facebook API. **/
typedef struct fb_api fb_api_t;

/** The main structure for #fb_api callback functions. **/
typedef struct fb_api_funcs fb_api_funcs_t;


/**
 * The #GError codes of #fb_api.
 **/
enum fb_api_error
{
    FB_API_ERROR_GENERAL /** General **/
};

/**
 * The main structure for #fb_api callback functions.
 **/
struct fb_api_funcs
{
    /**
     * The error function. This is called whenever an error occurs
     * within the #fb_api.
     *
     * @param api  The #fb_api.
     * @param err  The #GError.
     * @param data The user-defined data or NULL.
     **/
    void (*error) (fb_api_t *api, GError *err, gpointer data);

    /**
     * The auth function. This is called whenever authentication has
     * been successfully completed. This is called as a result of
     * #fb_api_auth().
     *
     * @param api  The #fb_api.
     * @param data The user-defined data or NULL.
     **/
    void (*auth) (fb_api_t *api, gpointer data);
};

/**
 * The structure for interacting with the Facebook API.
 **/
struct fb_api
{
    fb_api_funcs_t funcs; /** The #fb_api_funcs. **/
    gpointer       data;  /** The user-defined data or NULL. **/

    fb_http_t *http;      /** The #fb_http. **/
    GError    *err;       /** The #GError or NULL. **/

    gchar *token;         /** The session token. **/
};


#define FB_API_ERROR fb_api_error_quark()

GQuark fb_api_error_quark(void);

fb_api_t *fb_api_new(const fb_api_funcs_t *funcs, gpointer data);

void fb_api_free(fb_api_t *api);

void fb_api_error(fb_api_t *api, fb_api_error_t err, const gchar *fmt, ...);

void fb_api_auth(fb_api_t *api, const gchar *user, const gchar *pass);

#endif /* _FACEBOOK_API_H */
