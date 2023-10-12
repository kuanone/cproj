#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include "version.h"
#include "3rd/cJSON/cJSON.h"
#include <jemalloc/jemalloc.h>
#include <vector>
#include <unistd.h>
#include <ctime>

void print_version();

int equal(int v) {
    if (v != 0) {
        return 0;
    } else {
        return 1;
    }
}

int str_split(char *str, char *delim, char **tokens) {
    int i = 0;
    char *token;

    // 遍历字符串
    while ((token = strsep(&str, delim)) != NULL) {
        // 将子字符串存储到 tokens 数组中
        tokens[i++] = token;
    }

    // 返回子字符串的数量
    return i;
}

/**
 * str_trim
 * str_trim 只需要执行一次 strspn() 和一次 strndup() 操作，而 strtok 需要执行多次 strtok() 操作。strtok() 每次操作都会将字符串指针向前移动，因此 strtok 的性能会随着字符串长度的增加而下降。
 * str_trim 的性能会显著优于 strtok：
 * 字符串长度较长
 * 字符串中“-”字符出现的次数较多
 * strtok 的性能可能会优于 str_trim：
 * 字符串长度较短
 * 字符串中“-”字符出现的次数较少
 * @param str
 * @return char * new str
 * **/
char *str_trim(char *str) {
    unsigned int len = strspn(str, "-");
    char *new_str = strndup(str + len, strlen(str) - len);
    return new_str;
}

void print_json() {
    cJSON *cjson_test = NULL;
    cJSON *cjson_address = NULL;
    cJSON *cjson_skill = NULL;
    char *str = NULL;

    /* 创建一个JSON数据对象(链表头结点) */
    cjson_test = cJSON_CreateObject();

    /* 添加一条字符串类型的JSON数据(添加一个链表节点) */
    cJSON_AddStringToObject(cjson_test, "name", "mculover666");

    /* 添加一条整数类型的JSON数据(添加一个链表节点) */
    cJSON_AddNumberToObject(cjson_test, "age", 22);

    /* 添加一条浮点类型的JSON数据(添加一个链表节点) */
    cJSON_AddNumberToObject(cjson_test, "weight", 55.5);

    /* 添加一个嵌套的JSON数据（添加一个链表节点） */
    cjson_address = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_address, "country", "China");
    cJSON_AddNumberToObject(cjson_address, "zip-code", 111111);
    cJSON_AddItemToObject(cjson_test, "address", cjson_address);

    /* 添加一个数组类型的JSON数据(添加一个链表节点) */
    cjson_skill = cJSON_CreateArray();
    cJSON_AddItemToArray(cjson_skill, cJSON_CreateString("C"));
    cJSON_AddItemToArray(cjson_skill, cJSON_CreateString("Java"));
    cJSON_AddItemToArray(cjson_skill, cJSON_CreateString("Python"));
    cJSON_AddItemToObject(cjson_test, "skill", cjson_skill);

    /* 添加一个值为 False 的布尔类型的JSON数据(添加一个链表节点) */
    cJSON_AddFalseToObject(cjson_test, "student");

    /* 打印JSON对象(整条链表)的所有数据 */
    str = cJSON_PrintUnformatted(cjson_test);
    printf("%s\n", str);
}

class Widget {
        public:
                Widget() { }
                ~Widget() {}
                void hello() { std::cout << "hello, world" << std::endl;}
        private:
                std::string str_;
                char buf[1024];
                int i;
                int j;
                std::string str2_;
};

void jemalloc_example(){
     time_t t = time(NULL);
        for (int i = 0; i < 100000000; i++) {
                Widget *w = new Widget();
                delete w;
        }

        std::cout << "cost:" << time(NULL) - t << "s" << std::endl;
}

int main(int argc, char *argv[]) { // argc: argument count, argv: argument vector
    if (argc == 2 && (equal(strcmp(argv[1], "-v")) || equal(strcmp(argv[1], "--version")))) {
        print_version();
        return 0;
    } else if (argc == 2 && (equal(strcmp(argv[1], "-h")) || equal(strcmp(argv[1], "--help")))) {
        printf("Usage:\n");
        printf(" -v, --version：show version\n");
        printf(" -h, --help：show help\n");
        return 0;
    }

    for (int i = 1; i < argc; ++i) {
        printf("Argument: %s\n", argv[i]);
        char *str[2];
//        int count = str_split(strtok(argv[i], "-"), "=", str);
        int count = str_split(str_trim(argv[i]), "=", str);
        if (count == 2) {
            printf("Key: %s, Value: %s\n", str[0], str[1]);
        } else {
            printf("Key: %s, Value: %s\n", str[0], "");
        }
    }

    print_json();
    jemalloc_example();
    return 0;
}

void print_version() {
    printf("Version：%d.%d.%d\n", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
}


