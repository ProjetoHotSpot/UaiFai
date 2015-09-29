#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "webserver.h"
#include "mongoose.h"
#include "layout.h"
#include "customer.h"
#include "chuncks.h"

#define _CRT_SECURE_NO_WARNINGS

const char *html_error_page = "";

int error404(struct mg_connection *conn)
{
    char buffer[SIZE_PAGE];
    entry_menu menu[] = {
	{"In&iacute;cio", "/index.html"},
	{"", ""}
    };
    csc_web_page(buffer, sizeof(buffer), menu, "error", NULL, NULL,
		 conn->uri, "<center><h1>Page not found</h1></center>");

    //send page
    mg_send_header(conn, "Content-Type", "text/html");
    mg_printf_data(conn, buffer);
    return MG_TRUE;
}

int ping(struct mg_connection *conn)
{
    mg_send_header(conn, "Content-Type", "text/plain");
    mg_printf_data(conn, "pong");
    return MG_TRUE;
}

int index_html(struct mg_connection *conn)
{
    char buffer[SIZE_PAGE];
    char content[SIZE_PAGE];
    content[0] = '\x0';

    entry_menu menu[] = {
	{"In&iacute;cio", "/index.html"},
	{"Cadastros", "/cadastros.html"},
	{"Movimenta&ccedil;&otilde;es", "/movimentacoes.html"},
	{"Relat&oacute;rios", "/relatorios.html"},
	{"", ""}
    };

    __chuncks_welcome_message_chunck(content, sizeof(content)); 
    csc_web_page(buffer, sizeof(buffer), menu, "Inicio", NULL, NULL,
		 conn->uri, content);

    //send page
    mg_send_header(conn, "Content-Type", "text/html");
    mg_printf_data(conn, buffer);
    return MG_TRUE;
}

/************************************
*       Cookie authentication 
************************************/

static const char *s_login_uri = "/login.html";
static const char *s_secret = "sadf72l34k572njkd83j4";	// Must be known only to server 

static void generate_ssid(const char *user_name,
			  const char *expiration_date, char *ssid,
			  size_t ssid_size)
{
    char hash[33];
    mg_md5(hash, user_name, ":", expiration_date, ":", s_secret, NULL);
    snprintf(ssid, ssid_size, "%s|%s|%s", user_name, expiration_date,
	     hash);
}

static int check_auth(struct mg_connection *conn)
{
    char ssid[100], calculated_ssid[100], name[100], expire[100];

    // Always authenticate requests to login page
    if (strcmp(conn->uri, s_login_uri) == 0) {
	return MG_TRUE;
    }
    // Look for session ID in the Cookie.
    // That session ID can be validated against the database that stores
    // current active sessions.
    mg_parse_header(mg_get_header(conn, "Cookie"), "ssid", ssid,
		    sizeof(ssid));
    if (sscanf(ssid, "%[^|]|%[^|]|", name, expire) == 2) {
	generate_ssid(name, expire, calculated_ssid,
		      sizeof(calculated_ssid));
	if (strcmp(ssid, calculated_ssid) == 0) {
	    return MG_TRUE;	// Authenticate
	}
    }
    // Auth failed, do NOT authenticate, redirect to login page
    mg_printf(conn, "HTTP/1.1 302 Moved\r\nLocation: %s\r\n\r\n",
	      s_login_uri);
    return MG_FALSE;
}

static int login_html(struct mg_connection *conn)
{
    char name[100], password[100], ssid[100], expire[100],
	expire_epoch[100];
    static char page_login_html[] =
	"<!DOCTYPE html>" "<html lang=\"en\">" "<head>"
	"  <meta charset=\"utf-8\" />" "  <title>CSmallCompany</title>"
	"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />"
	"</head>" "<body>" "  <div class=\"content\">"
	"    <center>"
	"    <h1>CSmallCompany</h1>"
	"    <p>Use name \"Joe\", password \"Doe\" to login.</p>"
	"<table>"
	"    <form method=\"POST\">"
	"        <tr><td><label>Name:</label></td>"
	"        <td><input type=\"text\" name=\"name\"/></td>" "</tr>"
	"        <tr><td><label>Password:</label></td>"
	"        <td><input type=\"password\" name=\"password\"/> </td></tr>"
	"        <tr><td><input type=\"submit\" value=\"Login\"/>"
	"</td></tr>" "    </form>" "</table>" "</center>" "</body>"
	"</html>";


    if (strcmp(conn->request_method, "POST") == 0) {
	mg_get_var(conn, "name", name, sizeof(name));
	mg_get_var(conn, "password", password, sizeof(password));
	if (strcmp(name, "Joe") == 0 && strcmp(password, "Doe") == 0) {
	    // Generate expiry date
	    time_t t = time(NULL) + 3600;	// Valid for 1 hour
	    snprintf(expire_epoch, sizeof(expire_epoch), "%lu",
		     (unsigned long) t);
	    strftime(expire, sizeof(expire), "%a, %d %b %Y %H:%M:%S GMT",
		     gmtime(&t));
	    generate_ssid(name, expire_epoch, ssid, sizeof(ssid));
	    // Set "session id" cookie, there could be some data encoded in it.
	    mg_printf(conn,
		      "HTTP/1.1 302 Moved\r\n"
		      "Set-Cookie: ssid=%s; expire=\"%s\"; http-only; HttpOnly;\r\n"
		      "Location: /index.html\r\n\r\n", ssid, expire);
	    return MG_FALSE;
	}
    } else {
	mg_send_header(conn, "Content-Type", "text/html");
	mg_printf_data(conn, page_login_html);
	return MG_TRUE;
    }
}

/************************************
*              Dispatch 
************************************/

#define MAX_ENTRY_DISPATCH_TABLE 10000
#define LEN_PREFIX_DISPATCH_TABLE 1024

typedef struct entry_dispatch_t {
    char prefix[LEN_PREFIX_DISPATCH_TABLE];
    int (*fn) (struct mg_connection * conn);
} entry_dispatch;

entry_dispatch array_dispatch[MAX_ENTRY_DISPATCH_TABLE];

static int entry_to_fill = 0;
void add_entry_dispatch_table(const char *p,
			      int (*f) (struct mg_connection * conn))
{
    strncpy(array_dispatch[entry_to_fill].prefix, p,
	    LEN_PREFIX_DISPATCH_TABLE);
    array_dispatch[entry_to_fill].fn = f;
    entry_to_fill++;
}

static void init_dispatch_table()
{
    int i;
    for (i = 0; i < MAX_ENTRY_DISPATCH_TABLE; i++) {
	strcpy(array_dispatch[i].prefix, "");
	array_dispatch[i].fn = NULL;
    }
    add_entry_dispatch_table("/index.html", index_html);
    add_entry_dispatch_table("/login.html", login_html);
    add_entry_dispatch_table("/ping", ping);
    add_entry_dispatch_table("/", index_html);
}

/**
* @brief event handler for mongoose 
*
* @param conn connection struct
* @param ev event
*
*/
static int ev_handler(struct mg_connection *conn, enum mg_event ev)
{
    int i, rc;
    entry_dispatch aux = array_dispatch[0];

    switch (ev) {
    case MG_AUTH:
	return check_auth(conn);
    case MG_REQUEST:
	fprintf(stdout, "Processing %s\n", conn->uri);
	for (i = 0; aux.fn != NULL; i++) {
	    aux = array_dispatch[i];
	    if ((rc = strcmp(aux.prefix, conn->uri)) == 0) {
		return aux.fn(conn);
	    }
	}
	error404(conn);
	return MG_TRUE;
    default:
	return MG_FALSE;
    }
}

void init_resources_table();

/************************************
*              Main 
************************************/

/**
* @brief entry point for executable
*
* @param argc line command number of parameter 
* @param argv line command vector
*/
int main(int argc, char **argv)
{
    struct mg_server *server;

    // initialize dispatch table
    init_dispatch_table();
    init_resources_table();
    csc_customer_init();
    csc_naveg_init_routes_table();

    // initialize modules
    csc_finance_init();

    // Create and configure the server
    server = mg_create_server(NULL, ev_handler);
    mg_set_option(server, "listening_port", "8080");

    // Serve request. Hit Ctrl-C to terminate the program
    printf("Starting on port %s\n",
	   mg_get_option(server, "listening_port"));
    for (;;) {
	mg_poll_server(server, 1000);
    }

    // Cleanup, and free server instance
    mg_destroy_server(&server);

    return EXIT_SUCCESS;
}
