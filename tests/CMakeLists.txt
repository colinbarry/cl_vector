add_executable(test-runner ./main.c)

add_test(NAME Test COMMAND ./test-runner)
