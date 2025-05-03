cmake_minimum_required(VERSION 3.30)
project(securefile)

set(CMAKE_CXX_STANDARD 20)

# Detect platform and set brew paths for macOS
if(APPLE)
    message(STATUS "MacOS Detected - Using Homebrew")
    set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@3")
    set(SQLITE_ROOT_DIR "/opt/homebrew/opt/sqlite")
    set(BOOST_ROOT "/opt/homebrew/opt/boost")
endif()

# copy data.sql into build root (CMAKE_BINARY_DIR)
file(COPY ${CMAKE_SOURCE_DIR}/data/data.sql DESTINATION ${CMAKE_BINARY_DIR})

include_directories(src)

find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

# define executable
add_executable(securefile
        src/main.cpp
        src/services/database.cpp
        src/services/encrypt.cpp
        src/services/file.cpp
        src/models/user.cpp
        src/models/encrypted_file.cpp
        src/models/shared_file.cpp
        src/models/metadata_file.cpp
        src/models/session.cpp
        src/core/auth/auth.cpp
        src/ui/ui.cpp
)

# in macOS add includes/links explicitly
if(APPLE)
    target_include_directories(securefile PRIVATE
            ${OPENSSL_ROOT_DIR}/include
            ${SQLITE_ROOT_DIR}/include
            ${BOOST_ROOT}/include
    )
    target_link_directories(securefile PRIVATE
            ${OPENSSL_ROOT_DIR}/lib
            ${SQLITE_ROOT_DIR}/lib
            ${BOOST_ROOT}/lib
    )
endif()

# ensure executables are placed in config-specific folder
set_target_properties(securefile PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>
)

# after build, copy data.sql next to the .exe for all platforms
add_custom_command(TARGET securefile POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/data/data.sql
        $<TARGET_FILE_DIR:securefile>/data.sql
)

# link libraries
target_link_libraries(securefile PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        SQLite::SQLite3
        Boost::filesystem
)