#!/bin/bash

echo "=== Building C++ APIs ==="

# Limpar build anterior
rm -rf build

# Criar diretório de build
mkdir -p build
cd build

# Configurar CMake
cmake ..

# Compilar
if make -j$(nproc); then
    echo "=== Build SUCCESS ==="
    echo "Executável: ./build/src/sync_server/sync_server"
else
    echo "=== Build FAILED ==="
    exit 1
fi
