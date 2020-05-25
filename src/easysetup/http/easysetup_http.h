/* ***************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef _EASYSETUP_HTTP_H_
#define _EASYSETUP_HTTP_H_

#ifdef __cplusplus
extern "C" {
#endif

enum cgi_type {
	D2D_GET= 0,
	D2D_POST,
	D2D_ERROR,
};

void http_packet_handle(const char *name, char **buf, char *payload, enum cgi_type type);

void es_http_init(void);

void es_http_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* _EASYSETUP_HTTP_H_ */