# Tests

# Gather all test files
file(GLOB TEST_FILES "*.cpp")

# Add an executable for the tests
add_executable(neuron_fsm_tests ${TEST_FILES})
target_link_libraries(neuron_fsm_tests PRIVATE doctest)
target_link_libraries(neuron_fsm_tests PRIVATE neuron_fsm)

# Include the header files of the library
target_include_directories(neuron_fsm_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Make sure the library is built before the tests
add_dependencies(neuron_fsm_tests neuron_fsm)

# Register the executable with CTest
add_test(NAME Neuron_FSM_Tests COMMAND neuron_fsm_tests)
