#include "SyncServer.hpp"
#include <iostream>
#include <csignal>

SyncServer* server_ptr = nullptr;

void signal_handler(int signal) {
    std::cout << "\nRecebido sinal de desligamento..." << std::endl;
    if (server_ptr) {
        server_ptr->stop();
    }
}

int main() {
    try {
        SyncServer server(8080);
        server_ptr = &server;

        // Configurar handler para Ctrl+C
        std::signal(SIGINT, signal_handler);

        std::cout << "Iniciando servidor síncrono..." << std::endl;
        server.start();

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
