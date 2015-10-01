#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include "mongoose.h"

#define SIZE_PAGE 10000

void add_entry_dispatch_table(const char *p,
			      int (*f) (struct mg_connection * conn));

#endif
