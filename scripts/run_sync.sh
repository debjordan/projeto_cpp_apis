#!/bin/bash

echo "=== Iniciando Servidor Síncrono ==="

# Navegar para diretório do projeto
cd "$(dirname "$0")/.."

# Executar servidor
./build/src/sync_server/sync_server
