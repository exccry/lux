#include "network.h"
#include "log.h"

void lx_fatal_error(int server_fd, char *message, bool should_close)
{
  lx_log(LOG_LVL_ERROR, message);
  if (should_close)
    close(server_fd);
  exit(EXIT_FAILURE);
}

// Initialize and configure the server socket
int lx_start_server(ServerConfig *config)
{
  lx_log(
    LOG_LVL_INFO,
    "Booting server on port [%d] Loading config from [%s]",
    config->port,
    config->conf_path);

  int opt = 1;
  int server_fd;
  struct sockaddr_in address;

  // Create socket
  if ((server_fd = socket(LX_DOMAIN_IPV4, LX_SOCK_TYPE, LX_PROTOCOL)) == 0)
    lx_fatal_error(server_fd, "Socket failed", 0);
  lx_log(LOG_LVL_INFO, "Socket created -> OK");

  // Config socket opts
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    lx_fatal_error(server_fd, "Setsockopt failed", 1);
  lx_log(LOG_LVL_INFO, "Socket opts config -> OK");

  address.sin_family = LX_DOMAIN_IPV4;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(config->port);

  // this would bind the server specifically and exclusively to 127.0.0.1
  // inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

  // bind socket to port
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    lx_fatal_error(server_fd, "Bind failed", 1);
  lx_log(LOG_LVL_INFO, "Bind success  -> OK");

  // Mark socket as passive, ready to accept incoming connections
  if (listen(server_fd, config->backlog) < 0)
    lx_fatal_error(server_fd, "Listen failed", 1);

  lx_log(LOG_LVL_INFO, "Server is listening on port [%d]\n", config->port);

  return server_fd;
}

void lx_accept_connections(int server_fd)
{
  struct sockaddr_in client_address;
  socklen_t client_len = sizeof(client_address);

  while (1)
  {
    int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
    if (client_fd < 0)
    {
      lx_log(LOG_LVL_ERROR, "Client accept failed");
      continue;
    }

    lx_handle_client(client_fd);
  }
}

void lx_handle_client(int client_fd)
{
  char buffer[1024];
  ssize_t bytes_read;

  bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
  if (bytes_read < 0)
  {
    lx_log(LOG_LVL_ERROR, "read failed.");
    close(client_fd);
    return;
  }
  
  lx_log(LOG_LVL_INFO, "Received request: \n%s", buffer);

  HttpRequest request;

  if (!lx_parse_request(buffer, &request))
  {
    const char *bad_request = 
      "HTTP/1.1 400 Bad Request\r\n"
      "Content-Length: 0\r\n"
      "\r\n";
    write(client_fd, bad_request, strlen(bad_request));
    close(client_fd);
    return;
  }

  lx_log(
    LOG_LVL_INFO,
    "Parsed Request: Method='%s' Path='%s' HTTP-Version='%s'\n", 
    request.method, 
    request.path,
    request.http_version);

  const char *response_body = "Hello, from lux-server !";
  char response[1024];
  const size_t response_len = snprintf(
    response, sizeof(response),
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: %zu\r\n"
    "\r\n"
    "%s",
    strlen(response_body), response_body);

  write(client_fd, response, response_len);

  close(client_fd);
}

ssize_t lx_read_request(int client_fd, char *buffer, const size_t size)
{
  ssize_t bytes_read = read(client_fd, buffer, size - 1);
  if (bytes_read < 0)
    lx_log(LOG_LVL_ERROR, "Read failed: %s", strerror(errno));
  else
    buffer[bytes_read] = '\0';

  return bytes_read;
}

bool lx_parse_request(const char *request, HttpRequest *http_request)
{
  // Init buffers to zero
  memset(http_request, 0, sizeof(HttpRequest));

  // Find end of the request line
  size_t req_line_len = strcspn(request, "\r\n");
  if (req_line_len >= 512)
  {
    lx_log(LOG_LVL_ERROR, "Request line too long");
    return false;
  }

  // Copy request line into separate buffer
  char request_line[512];
  memcpy(request_line, request, req_line_len);
  request_line[req_line_len] = '\0';

  // Parse the request line with field width specifiers
  int ret = sscanf(
    request_line, 
    LX_HTTP_REQ_FMT, 
    http_request->method, 
    http_request->path,
    http_request->http_version);

  if (ret != 3)
  {
    lx_log(LOG_LVL_ERROR, "Failed to parse request line");
    return false;
  }

  return true;
}

void lx_process_request(HttpRequest *request, HttpResponse *response, ServerConfig *config)
{
  // implement
}

void lx_send_response(int client_fd, HttpResponse *response)
{
  // implement
}

bool lx_sanititze_path(const char *path, char *sanitized_path, const size_t size)
{
  if (strstr(path, ".."))
  {
    lx_log(LOG_LVL_WARNING, "Directory traversal attempt detected: %s", path);
    return false;
  }
  
  if (path[0] != '/')
    snprintf(sanitized_path, size, "/%s", path);
  else
  {
    strncpy(sanitized_path, path, size - 1);
    sanitized_path[size - 1] = '\0';
  }

  return true;
}
