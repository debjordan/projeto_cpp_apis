cmake_minimum_required(VERSION 3.15)
project(CPP_APIs VERSION 1.0.0 LANGUAGES CXX)

# Configurações do compilador
set(CMAKE_CXX_STANDARD 17)        # Usar C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Exigir C++17
set(CMAKE_CXX_EXTENSIONS OFF)     # Desativar extensões GNU

# Diretórios de include
include_directories(include)      # Onde estão os headers

# Apenas sync_server por enquanto
add_subdirectory(src/sync_server) # Compilar servidor

# Async server será adicionado depois
# add_subdirectory(src/async_server)
