cmake_minimum_required(VERSION 3.30)
project(securefile)

set(CMAKE_CXX_STANDARD 20)

# 📌 Verifica el sistema operativo para usar Homebrew en macOS
if(APPLE)
    message(STATUS "Detectado macOS - Usando Homebrew")
    set(OPENSSL_ROOT_DIR "/opt/homebrew/Cellar/openssl@3/3.4.1")
    set(SQLITE_ROOT_DIR "/opt/homebrew/Cellar/sqlite/3.49.1")
    set(BOOST_ROOT "/opt/homebrew/Cellar/boost/1.87.0_1")
endif()

# 📌 Buscar las librerías necesarias
find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

# 📌 Agregar el código fuente
add_executable(securefile src/main.cpp)

# 📌 Incluir directorios de las librerías
target_include_directories(securefile PRIVATE
        ${OPENSSL_ROOT_DIR}/include
        ${SQLITE_ROOT_DIR}/include
        ${BOOST_ROOT}/include
)

# 📌 Enlazar las librerías necesarias
target_link_libraries(securefile PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        sqlite3
        Boost::filesystem
)