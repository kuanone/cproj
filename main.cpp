#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "version.h"
#include "3rd/cJSON/cJSON.h"
#include <jemalloc/jemalloc.h>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <curl/curl.h>

#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<netdb.h>
#include<errno.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#ifdef __linux__

#include <unistd.h>

#elif _WIN32
#include <windows.h>
#endif

int get_cpu_cores() {
#ifdef __linux__
    return sysconf(_SC_NPROCESSORS_ONLN);
#elif _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#else
    return 0; // 平台不支持获取 CPU 核数
#endif
}

unsigned long long get_total_memory() {
#ifdef __linux__
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return (unsigned long long) pages * (unsigned long long) page_size;
#elif _WIN32
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return (unsigned long long)mem_info.ullTotalPhys;
#else
    return 0; // 平台不支持获取内存大小
#endif
}

void print_version();

int equal(int v) {
    if (v != 0) {
        return 0;
    } else {
        return 1;
    }
}

int str_split(char *str, const char *delim, const char **tokens) {
    int i = 0;
    char *token;

    // 遍历字符串
    while ((token = strsep(&str, delim)) != nullptr) {
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
    cJSON *cjson_test = nullptr;
    cJSON *cjson_address = nullptr;
    cJSON *cjson_skill = nullptr;
    char *str = nullptr;

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
    free(str);
    free(cjson_test);
    free(cjson_address);
    free(cjson_skill);
}

class Widget {
public:
    Widget() {}

    ~Widget() {}

    void hello() { std::cout << "hello, world" << std::endl; }

private:
    std::string str_;
    char buf[1024];
    int i;
    int j;
    std::string str2_;
};

void jemalloc_example() {
    time_t t = time(nullptr);
    for (int i = 0; i < 100000000; i++) {
        Widget *w = new Widget();
        delete w;
    }

    std::cout << "cost:" << time(nullptr) - t << "s" << std::endl;
}

void curl_example() {
    // 初始化libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    if (curl) {
        // 设置请求的URL（示例：https://www.example.com）
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.baidu.com");
        // 执行请求并输出结果
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cout << "curl perform failed: " << curl_easy_strerror(res) << "\n" << std::endl;
        }
        // 清理资源
        curl_easy_cleanup(curl);
    }
    // 全局清理libcurl
    curl_global_cleanup();
}

char *formatBytes(unsigned long long bytes) {
    const char *suffixes[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    int suffixIndex = 0;

    auto doubleBytes = (double) bytes;

    while (doubleBytes >= 1024 && suffixIndex < 8) {
        doubleBytes /= 1024;
        suffixIndex++;
    }

    char *formatted = (char *) malloc(20); // 分配内存以存储格式化后的结果

    if (formatted != nullptr) {
        snprintf(formatted, 20, "%.2f %s", doubleBytes, suffixes[suffixIndex]);
    }

    return formatted;
}

void computer_info() {
    int cpu_cores = get_cpu_cores();
    unsigned long long total_memory = get_total_memory();

    printf("CPU Cores: %d\n", cpu_cores);
    char *formatted = formatBytes(total_memory);
    printf("Total Memory: %llu bytes, %s\n", total_memory, formatted);
    free(formatted);
}

int http_get() {
    int socket_desc;
    struct sockaddr_in server{};
    const char *message;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }

    char ip[20] = {0};
    const char *hostname = "www.cnblogs.com";
    struct hostent *hp;
    if ((hp = gethostbyname(hostname)) == nullptr) {
        close(socket_desc);
        return 1;
    }

    strcpy(ip, inet_ntoa(*(struct in_addr *) hp->h_addr_list[0]));

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(80);


    //Connect to remote server
    if (connect(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        printf("connect error： %s", errno);
        close(socket_desc);
        return 1;
    }

    puts("Connected\n");

    //Send some data
    //http 协议
    message = "GET / HTTP/1.1\r\nHost: www.cnblogs.com\r\n\r\n";

    //向服务器发送数据
    if (send(socket_desc, message, strlen(message), 0) < 0) {
        puts("Send failed");
        close(socket_desc);
        return 1;
    }
    puts("Data Send\n");

    struct timeval timeout = {3, 0};
    setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(struct timeval));

    //Receive a reply from the server
    //loop
    int size_recv, total_size = 0;
    char chunk[512];
    while (true) {
        memset(chunk, 0, 512); //clear the variable
        //获取数据
        if ((size_recv = recv(socket_desc, chunk, 512, 0)) == -1) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                // 暂时没有数据可读，可以等待或重试
                break;
            } else if (errno == EINTR) {
                printf("interrupt by signal...\n");
                continue;
            } else if (errno == ENOENT) {
                printf("recv RST segement...\n");
                break;
            } else {
                printf("unknown error: %d\n", errno);
                exit(1);
            }
        } else if (size_recv == 0) {
            printf("Connection closed by server\n");
            break;
        } else {
            total_size += size_recv;
            chunk[size_recv] = '\0';
            printf("%s", chunk);

            // 检查响应头是否包含 Content-Length 字段
            char *content_length_header = strstr(chunk, "Content-Length: ");
            if (content_length_header) {
                int content_length;
                if (sscanf(content_length_header + 16, "%d", &content_length) == 1) {
                    // 如果已经接收到的数据达到 Content-Length 指定的大小，则认为响应已完全接收
                    if (size_recv >= content_length) {
                        printf("Response fully received\n");
                        break;
                    }
                }
            }
        }
    }
    close(socket_desc);
    printf("Reply received, total_size = %d bytes\n", total_size);
    return 0;
}


int https_example() {
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
    BIO *web = NULL;

    SSL_library_init();
    ERR_load_BIO_strings();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    const char *hostname = "baidu.com";
    const char *port = "443";
    const char *request = "GET / HTTP/1.1\r\nHost: baidu.com\r\n\r\n";

    ctx = SSL_CTX_new(SSLv23_client_method());
    if (!ctx) {
        fprintf(stderr, "SSL_CTX_new error\n");
        return 1;
    }

    web = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(web, &ssl);
    if (!ssl) {
        fprintf(stderr, "BIO_get_ssl error\n");
        return 1;
    }

    BIO_set_conn_hostname(web, hostname);
    BIO_set_conn_port(web, port);

    if (BIO_do_connect(web) <= 0) {
        fprintf(stderr, "BIO_do_connect error\n");
        return 1;
    }

    if (BIO_do_handshake(web) <= 0) {
        fprintf(stderr, "BIO_do_handshake error\n");
        return 1;
    }

    BIO_puts(web, request);

    char response[4096];
    int len;
    while ((len = BIO_read(web, response, sizeof(response))) > 0) {
        fwrite(response, 1, len, stdout);
    }

    BIO_free_all(web);
    SSL_CTX_free(ctx);
    ERR_free_strings();
    EVP_cleanup();
    return 0;
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
        const char *kv[2];
//        int count = str_split(strtok(argv[i], "-"), "=", kv);
        int count = str_split(str_trim(argv[i]), "=", kv);
        if (count == 2) {
            printf("Key: %s, Value: %s\n", kv[0], kv[1]);
        } else {
            printf("Key: %s, Value: %s\n", kv[0], "");
        }
    }

    print_json();
    jemalloc_example();
    curl_example();
    computer_info();
//    http_get();
//    https_example();
    return 0;
}

void print_version() {
    printf("Version：%d.%d.%d\n", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
}