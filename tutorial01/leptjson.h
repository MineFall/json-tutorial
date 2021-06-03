#ifndef LEPTJSON_H__
#define LEPTJSON_H__

typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;

typedef struct {
    lept_type type;
}lept_value;//lept_value等于就是这个结构体类的名称，虽然这个结构体是一个无名类

enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR
};//枚举类，处理分析的错误分类

int lept_parse(lept_value* v, const char* json);//用于分析的API,  lept_value* v在分析之前没有值，而是在分析之后获得值，也就是保存分析后的结果
//并且json值不应该被改变，所以需要用const进行修饰

lept_type lept_get_type(const lept_value* v);//如果lept_value取得了值，那么就可以通过这个API获得其中的成员值

#endif /* LEPTJSON_H__ */
