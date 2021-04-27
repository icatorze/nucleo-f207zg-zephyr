zephyr_library()
zephyr_library_sources_ifdef(CONFIG_PCF8574 pcf8574.c)
