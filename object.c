#include "object.h"
#include "stream.h"
#include <stdlib.h>
#include <string.h>
static void _obj2stream1(object *obj, stream *s);
static void _obj2stream(object *obj, stream *s);

// 创建新的对象
object *newObject()
{
    object *obj = malloc(sizeof(object));
    memset(obj, 0, sizeof(object));
    return obj;
}

// 判断是否有key,有则推入缓冲区
static void _obj2stream1(object *obj, stream *s)
{
    if (obj == NULL)
    {
        return;
    }
    else if (obj->parent->type == OBJECT_OBJECT)
    {
        stream_push(s, "\"", 1);
        stream_push(s, obj->key, strlen(obj->key));
        stream_push(s, "\":", 2);
    }
    _obj2stream(obj, s);
    if (obj->brother)
    {
        stream_push(s, ",", 1);
        _obj2stream1(obj->brother, s);
    }
}
// 根据对象类型转json
static void _obj2stream(object *obj, stream *s)
{
    switch (obj->type)
    {
    case OBJECT_OBJECT:
        stream_push(s, "{", 1);
        _obj2stream1(obj->child, s);
        stream_push(s, "}", 1);
        break;
    case OBJECT_ARRAY:
        stream_push(s, "[", 1);
        _obj2stream1(obj->child, s);
        stream_push(s, "]", 1);
        break;
    case OBJECT_STRING:
        stream_push(s, "\"", 1);
        stream_push(s, obj->value, strlen(obj->value));
        stream_push(s, "\"", 1);
        break;
    case OBJECT_BOOL:
    {
        char *c = obj->i ? "true" : "false";
        stream_push(s, c, strlen(c));
        break;
    }
    case OBJECT_NUMBER:
    {
        char str[10];
        ltoa(obj->i, str, 10);
        stream_push(s, str, strlen(str));
        break;
    }
    case OBJECT_NULL:
        stream_push(s, "null", 4);
        break;
    default:
        break;
    }
}

char *json_encode(object *obj)
{
    stream *s = newStream();
    char *str;
    _obj2stream(obj, s);
    str = stream2string(s);
    stream_clear(s);
    return str;
}

void objectfree(object *obj)
{
    if (obj)
    {
        if(obj->parent->type==OBJECT_OBJECT)
        {
            free(obj->key);
        }
        switch (obj->type)
        {
        case OBJECT_OBJECT:
        case OBJECT_ARRAY:
            objectfree(obj->child);
            break;
        case OBJECT_STRING:
            free(obj->value);
            break;
        default:
            break;
        }
        objectfree(obj->brother);
        free(obj);
    }
}