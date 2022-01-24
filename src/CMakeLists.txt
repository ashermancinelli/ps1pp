add_executable(ps1 main.cpp)
target_link_libraries(ps1 PRIVATE tpl)
install(TARGETS ps1 DESTINATION bin)

execute_process(COMMAND uname -m OUTPUT_VARIABLE UNAME_M)
message(STATUS "Using arch name \"${UNAME_M}\"")

add_custom_command(
  TARGET ps1
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ps1> ${PROJECT_BINARY_DIR}/ps1
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ps1>
          ${PROJECT_BINARY_DIR}/ps1-${UNAME_M})
