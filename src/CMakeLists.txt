add_executable(ps1 main.cpp)
target_link_libraries(ps1 PRIVATE tpl)
install(TARGETS ps1 DESTINATION bin)
