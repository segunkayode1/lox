# Additional targets to perform clang-format/clang-tidy
# Get all project files
file(GLOB_RECURSE
     ALL_CXX_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp 
    ${PROJECT_SOURCE_DIR}/include/*.hpp
     )

# Adding clang-format target if executable is found
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  add_custom_target(
    clang-format
    COMMAND clang-format
    -i
    -style=file
    ${ALL_CXX_SOURCE_FILES}
    )
endif()

# Adding clang-tidy target if executable is found
find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
  add_custom_target(
    clang-tidy
    COMMAND clang-tidy
    ${ALL_CXX_SOURCE_FILES}
    -p ${PROJECT_BINARY_DIR}
    -header-filter=.*
    )
endif()