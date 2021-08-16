FILE(GLOB_RECURSE app_sources ${CMAKE_CURRENT_SOURCE_DIR}/*.*)

idf_component_register(SRCS "tbwifi.cpp"
       REQUIRES "nvs_flash"
       INCLUDE_DIRS ".")