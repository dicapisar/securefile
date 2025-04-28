cmake_minimum_required(VERSION 3.30)
project(securefile)

set(CMAKE_CXX_STANDARD 20)

if(APPLE)
    message(STATUS "MacOS Detected - Using Homebrew")

    set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@3")
    set(SQLITE_ROOT_DIR "/opt/homebrew/opt/sqlite")
    set(BOOST_ROOT "/opt/homebrew/opt/boost")
endif()

file(COPY data/data.sql DESTINATION ${CMAKE_BINARY_DIR})

include_directories(src)

find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

add_executable(securefile
        src/main.cpp
        src/services/database.cpp
        src/services/encrypt.cpp
        src/services/file.cpp
        src/models/user.cpp
        src/models/encrypted_file.cpp
        src/models/shared_file.cpp
        src/models/metadata_file.cpp
)

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

target_link_libraries(securefile PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        SQLite::SQLite3
        Boost::filesystem
)