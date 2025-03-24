cmake_minimum_required(VERSION 3.30)
project(securefile)

set(CMAKE_CXX_STANDARD 20)

if(APPLE)
    message(STATUS "Detectado macOS - Usando Homebrew")
    set(OPENSSL_ROOT_DIR "/opt/homebrew/Cellar/openssl@3/3.4.1")
    set(SQLITE_ROOT_DIR "/opt/homebrew/Cellar/sqlite/3.49.1")
    set(BOOST_ROOT "/opt/homebrew/Cellar/boost/1.87.0_1")
endif()

#file(GLOB SOURCES "src/*.cpp")

#file(GLOB SOURCE "src/*.h")

file(COPY data/data.sql DESTINATION ${CMAKE_BINARY_DIR})
#file(COPY test.txt DESTINATION ${CMAKE_BINARY_DIR})

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

target_link_libraries(securefile PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        sqlite3
        Boost::filesystem
)