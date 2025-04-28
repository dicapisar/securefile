cmake_minimum_required(VERSION 3.30)
project(securefile)

set(CMAKE_CXX_STANDARD 20)

if(APPLE)
    message(STATUS "MacOS Detected - Using Homebrew")
    set(OPENSSL_ROOT_DIR "/opt/homebrew/Cellar/openssl@3/3.4.1")
    set(SQLITE_ROOT_DIR "/opt/homebrew/Cellar/sqlite/3.49.1")
    set(BOOST_ROOT "/opt/homebrew/Cellar/boost/1.87.0_1")
elseif(WIN32)
    message(STATUS "Windows Detected")

    if(NOT DEFINED OPENSSL_ROOT_DIR)
        message(STATUS "OPENSSL DIDN'T DEFINED")
        set(OPENSSL_ROOT_DIR "C:/path/to/openssl")
    endif()

    if(NOT DEFINED SQLITE_ROOT_DIR)
        message(STATUS "SQLITE DIDN'T DEFINED")
        set(SQLITE_ROOT_DIR "C:/path/to/sqlite")
    endif()

    if(NOT DEFINED BOOST_ROOT)
        message(STATUS "BOOST DIDN'T DEFINED")
        set(BOOST_ROOT "C:/path/to/boost")
    endif()

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

target_link_libraries(securefile PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        sqlite3
        Boost::filesystem
)