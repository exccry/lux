#pragma once

#include "internal.h"

int lx_start_server(ServerConfig *config);

void lx_accept_connections(int server_fd);

void lx_fatal_error(int server_fd, char *message, bool should_close);

bool lx_sanititze_path(const char *in_str, char *sanitized_path, const size_t size);

void lx_handle_client(int client_fd);

/*ssize_t lx_request_read(int client_fd, char *buffer, );
bool lx_request_parse();
void lx_request_process();
void lx_request_send();*/

// REQUEST & RESPONSE HANDLING
ssize_t lx_read_request(int client_fd, char *buffer, const size_t size);

bool lx_parse_request(const char *request, HttpRequest *http_request);

void lx_process_request(HttpRequest *request, HttpResponse *response, ServerConfig *config);

void lx_send_response(int client_fd, HttpResponse *response);

