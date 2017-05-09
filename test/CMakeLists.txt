project(tests)

set(test_source main.cpp)
add_executable(func_test ${test_source})
target_link_libraries(func_test fourier-transform)

add_test(NAME FuncTest COMMAND func_test)