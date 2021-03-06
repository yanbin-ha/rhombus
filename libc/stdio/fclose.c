/*
 * Copyright (C) 2009-2011 Nick Johnson <nickbjohnson4224 at gmail.com>
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>

#include <rho/natio.h>
#include <rho/mutex.h>
#include <rho/proc.h>

/****************************************************************************
 * fclose - close a stream
 *
 * The fclose() function flushes the stream pointed to by <stream> (writing 
 * any buffered output data using fflush()).
 *
 * Upon successful completion 0 is returned. Otherwise, EOF is returned. 
 * In either case further access (including another call to fclose()) to 
 * the stream results in undefined behavior.
 */

int fclose(FILE *stream) {

	fflush(stream);
	
	mutex_spin(&stream->mutex);

	if (stream->buffer) {
		free(stream->buffer);
	}

	mutex_free(&stream->mutex);

	close(stream->fd);

	free(stream);

	return 0;
}
