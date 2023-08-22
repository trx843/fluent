/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Chunk I/O
 *  =========
 *  Copyright 2018 Eduardo Silva <eduardo@monkey.io>
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

#ifndef CIO_FILE_ST_H
#define CIO_FILE_ST_H

#include <stdlib.h>
#include <inttypes.h>

/*
 * ChunkIO data file layout as of 2018/10/26
 *
 * - 2 first bytes as identification: 0xC1 0x00
 * - 4 bytes for checksum of content section (CRC32)
 * - Content section is composed by:
 *   - 2 bytes to specify the length of metadata
 *   - optional metadata
 *   - user data
 *
 *    +--------------+----------------+
 *    |     0xC1     |     0x00       +--> Header 2 bytes
 *    +--------------+----------------+
 *    |   4 BYTES CHECKSUM            +--> CRC32(Content)
 *    |   4 BYTES CONTENT LENGHT      +--> Content length
 *    |   12 BYTES                    +--> Padding
 *    +-------------------------------+
 *    |            Content            |
 *    |  +-------------------------+  |
 *    |  |         2 BYTES         +-----> Metadata Length
 *    |  +-------------------------+  |
 *    |  +-------------------------+  |
 *    |  |                         |  |
 *    |  |        Metadata         +-----> Optional Metadata (up to 65535 bytes)
 *    |  |                         |  |
 *    |  +-------------------------+  |
 *    |  +-------------------------+  |
 *    |  |                         |  |
 *    |  |       Content Data      +-----> User Data
 *    |  |                         |  |
 *    |  +-------------------------+  |
 *    +-------------------------------+
 */

#define CIO_FILE_ID_00                 0xc1 /* header: first byte */
#define CIO_FILE_ID_01                 0x00 /* header: second byte */
#define CIO_FILE_HEADER_MIN              24 /* 24 bytes for the header */
#define CIO_FILE_CONTENT_OFFSET          22
#define CIO_FILE_CONTENT_LENGTH_OFFSET    6 /* We store the content length
                                             * right after the checksum in
                                             * what used to be padding
                                             */

/* Return pointer to hash position */
static inline char *cio_file_st_get_hash(char *map)
{
    return map + 2;
}

/* Return metadata length */
static inline uint16_t cio_file_st_get_meta_len(char *map)
{
    return (uint16_t) ((uint8_t) map[22] << 8) | (uint8_t) map[23];
}

/* Set metadata length */
static inline void cio_file_st_set_meta_len(char *map, uint16_t len)
{
    map[22] = (uint8_t) len >> 8;
    map[23] = (uint8_t) len;
}

/* Return pointer to start point of metadata */
static inline char *cio_file_st_get_meta(char *map)
{
    return map + CIO_FILE_HEADER_MIN;
}

/* Return pointer to start point of content */
static inline char *cio_file_st_get_content(char *map)
{
    uint16_t len;

    len = cio_file_st_get_meta_len(map);
    return map + CIO_FILE_HEADER_MIN + len;
}

/* Get content length */
static inline ssize_t cio_file_st_get_content_len(char *map, size_t size)
{
    uint8_t *content_length_buffer;

    if (size < CIO_FILE_HEADER_MIN) {
        return -1;
    }

    content_length_buffer = (uint8_t *) &map[CIO_FILE_CONTENT_LENGTH_OFFSET];

    return (ssize_t) (((uint32_t) content_length_buffer[0]) << 24) |
                     (((uint32_t) content_length_buffer[1]) << 16) |
                     (((uint32_t) content_length_buffer[2]) <<  8) |
                     (((uint32_t) content_length_buffer[3]) <<  0);
}

/* Set content length */
static inline void cio_file_st_set_content_len(char *map, uint32_t len)
{
    uint8_t *content_length_buffer;

    content_length_buffer = (uint8_t *) &map[CIO_FILE_CONTENT_LENGTH_OFFSET];

    content_length_buffer[0] = (uint8_t) ((len & 0xFF000000) >> 24);
    content_length_buffer[1] = (uint8_t) ((len & 0x00FF0000) >> 16);
    content_length_buffer[2] = (uint8_t) ((len & 0x0000FF00) >>  8);
    content_length_buffer[3] = (uint8_t) ((len & 0x000000FF) >>  0);
}

#endif
