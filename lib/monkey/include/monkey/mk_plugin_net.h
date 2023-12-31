/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Monkey HTTP Server
 *  ==================
 *  Copyright 2001-2017 Eduardo Silva <eduardo@monkey.io>
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef MK_PLUGIN_NET_H
#define MK_PLUGIN_NET_H

#include <monkey/mk_core.h>

/*
 * Network plugin: a plugin that provides a network layer, eg: plain
 * sockets or SSL.
 */
struct mk_plugin;
struct mk_plugin_network {
    int (*read) (struct mk_plugin *, int, void *, int);
    int (*write) (struct mk_plugin *, int, const void *, size_t);
    int (*writev) (struct mk_plugin *, int, struct mk_iov *);
    int (*close) (struct mk_plugin *, int);
    int (*send_file) (struct mk_plugin *, int, int, off_t *, size_t);
    int buffer_size;
    struct mk_plugin *plugin;
};

#endif
