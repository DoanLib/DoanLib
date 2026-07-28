cmake_minimum_required(VERSION 4.1)

# ============================================================================ #
#   Tests                                                                      #
# ============================================================================ #

set(TEST_SOURCE_FILES
  "memory.c"
  "string.c"
  "structs.c"
  "utility.c"
)

add_executable(DoanTests "main.c" ${TEST_SOURCE_FILES})
target_link_libraries(DoanTests PRIVATE DoanLib)
dn_target_setup(DoanTests)

add_test(NAME "DoanTest.Memory" COMMAND DoanTests -Memory)
add_test(NAME "DoanTest.Utility" COMMAND DoanTests -Utility)
