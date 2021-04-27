set(THS_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "i14 root directory")

zephyr_include_directories(include)

add_subdirectory(drivers)
