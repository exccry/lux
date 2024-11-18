#include "config.h"
#include "log.h"

ServerConfig lx_load_config(const char *filename)
{
  ServerConfig config = { 
    LX_CONF_DEFAULT_FILE_UNIX,
    LX_CONF_DEFAULT_PORT,
    LX_CONF_DEFAULT_BACKLOG };

  if (strlen(filename))
    strcpy(config.conf_path, filename);

  FILE *f = fopen(config.conf_path, "r");
  if (!f)
  {
    lx_log(LOG_LVL_ERROR, "Unable to load config file [%s]", config.conf_path);
    lx_log(
      LOG_LVL_WARNING, 
      "Rollback to default config file [%s]",
      LX_CONF_DEFAULT_FILE_UNIX);

    strcpy(config.conf_path, LX_CONF_DEFAULT_FILE_UNIX);
    
    return config;
  }

  char line[256];
  while (fgets(line, sizeof(line), f))
  {
    char *key = strtok(line, "=");
    char *value = strtok(NULL, "\n");

    if (key && value)
    {
      if (strcmp(key, LX_CONF_KEY_PORT) == 0)
        config.port = atoi(value);
      else if (strcmp(key, LX_CONF_KEY_BACKLOG) == 0)
        config.backlog = atoi(value);
    }
  }
  fclose(f);

  return config;
}
