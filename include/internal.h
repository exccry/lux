#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>

#define LX_PROTOCOL               0
#define LX_SOCK_TYPE              SOCK_STREAM
#define LX_DOMAIN_IPV4            AF_INET
#define LX_LEVEL                  SOL_SOCKET
#define LX_OPTNAME                SO_REUSEADDR

#define LX_CONF_KEY_IP            "http_server_ip"
#define LX_CONF_KEY_PORT          "http_server_port"
#define LX_CONF_KEY_BACKLOG       "http_server_backlog"

#define LX_IP_ANY                 0x00000000
#define LX_CONF_DEFAULT_IP        INADDR_ANY
#define LX_CONF_DEFAULT_PORT      8080
#define LX_CONF_DEFAULT_BACKLOG   10
#define LX_CONF_DEFAULT_FILE_UNIX ".luxrc"

#define LX_MXCONF_PATH            256
#define LX_IP_LEN                 16

#define LX_HTTP_REQ_FMT           "%7s %255s %15s"

typedef struct
{
  char conf_path[256];
  int port;
  int backlog;

} ServerConfig;

typedef struct
{
  char method[8];
  char path[256];
  char http_version[16];

} HttpRequest;

typedef struct
{
  char content_type[64];
  char reason_phrase[32];
  char *body;
  size_t body_len;
  int status_code;

} HttpResponse;
