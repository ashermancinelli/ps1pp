add_subdirectory(fmt)
add_subdirectory(filesystem)

target_link_libraries(tpl INTERFACE fmt::fmt ghcFilesystem::ghc_filesystem)
target_include_directories(tpl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cpp-subprocess/include)
