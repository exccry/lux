#include "network.h"
#include "config.h"
#include "log.h"

#define CONF_PATH "lux_config.ini"

int main(void)
{
  ServerConfig config = lx_load_config(CONF_PATH);
  int server_fd = lx_start_server(&config);

  if (server_fd < 0)
  {
    lx_log(LOG_LVL_ERROR, "error unable to start server\n");
    exit(EXIT_FAILURE);
  }

  lx_accept_connections(server_fd);
}
