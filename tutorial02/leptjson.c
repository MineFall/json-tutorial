#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL, strtod() */

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')

typedef struct {
    const char* json;
}lept_context;

static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

static int lept_parse_number(lept_context* c, lept_value* v) {
    char* end;
    int i;
    if(c->json[0]!='-' && !ISDIGIT(c->json[0]))//第一位不是减号，或者数字的
        return LEPT_PARSE_INVALID_VALUE;
    else if(c->json[0]=='0'  &&  c->json[1]!='\0')//第一位是零，但后面不为空的  json中没有16进位的表示，例如0x07等
        return LEPT_PARSE_INVALID_VALUE;
    for(i=0;c->json[i]!='\0';i++)
    {
        if(c->json[i]=='.' && !ISDIGIT(c->json[i+1]))//排除小数点后面一位没有数字的
            return LEPT_PARSE_INVALID_VALUE;
    }
    v->n = strtod(c->json, &end);
    if(v->n=HUGE_VAL || v->n=-HUGE_VAL)//如果提取的数值超过了double的精度大小
        return LEPT_PARSE_NUMBER_TOO_BIG;
    
    if (c->json == end)
        return LEPT_PARSE_INVALID_VALUE;
    c->json = end;
    v->type = LEPT_NUMBER;
    return LEPT_PARSE_OK;
}

static int lept_parse_literal(lept_context* c,lept_value* v){
    switch(*c->json)
    case 'n':{
        EXPECT(c,'n');
        if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
        c->json +=3;
        v->type = LEPT_NULL;
        break;
    }
    case 't':{
        EXPECT(c,'t');
        if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
        c->json +=3;
        v->type = LEPT_TRUE;
        break;
    }
    case 'f':{
        EXPECT(c,'f');
        if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3]!='e')
        return LEPT_PARSE_INVALID_VALUE;
        c->json +=4;
        v->type = LEPT_FALSE;
        break;
    }
    default : break;
}

static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 't':  return lept_parse_literal(c, v);
        case 'f':  return lept_parse_literal(c, v);
        case 'n':  return lept_parse_literal(c, v);
        default:   return lept_parse_number(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
    }
}

int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if (*c.json != '\0') {
            v->type = LEPT_NULL;
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

double lept_get_number(const lept_value* v) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    return v->n;
}
