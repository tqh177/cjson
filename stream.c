#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stream.h"

static stream *_stream_end(stream *s);
static size_t _stream_fill(stream *s, BYTE bytes[], size_t bytes_len);

stream *_stream_end(stream *s)
{
    while (s->next)
    {
        s = s->next;
    }
    return s;
}

size_t _stream_fill(stream *s, BYTE bytes[], size_t bytes_len)
{
    if (s->length == STREAM_MAX_LEN)
    {
        return 0;
    }
    else
    {
        size_t len = STREAM_MAX_LEN - s->length;
        if (bytes_len < len)
        {
            len = bytes_len;
        }
        memcpy(s->byte + s->length, bytes, len);
        s->length += len;
        return len;
    }
}

stream *newStream()
{
    stream *s = malloc(sizeof(stream));
    memset(s, 0, sizeof(stream));
    return s;
}

void stream_push(stream *s, BYTE bytes[], size_t bytes_len)
{
    size_t len;
    s = _stream_end(s);
    len = _stream_fill(s, bytes, bytes_len);
    while (bytes_len -= len)
    {
        bytes += len;
        s->next = newStream();
        s = s->next;
        len = _stream_fill(s, bytes, bytes_len);
    }
}
void stream_clear(stream *s)
{
    if (s)
    {
        stream_clear(s->next);
        free(s);
    }
}
size_t stream_len(stream *s)
{
    size_t len = s->length;
    while (s->next)
    {
        s = s->next;
        len += s->length;
    }
    return len;
}
char *stream2string(stream *s)
{
    size_t len = stream_len(s);
    char *str = malloc(sizeof(char) * (len + 1));
    char *hstr = str;
    str[len] = 0;
    do
    {
        memcpy(str, s->byte, s->length);
        str += s->length;
        s = s->next;
    } while (s);
    return hstr;
}
