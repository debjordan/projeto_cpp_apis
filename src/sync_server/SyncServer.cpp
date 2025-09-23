#include "SyncServer.hpp"
#include "../include/common/Request.hpp"
#include "../include/common/Response.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <thread>
#include <vector>

SyncServer::SyncServer(int port) : running(false) {
    // Criar socket IPv4, TCP, protocolo padrão
    // AF_INET: IPv4
    // SOCK_STREAM: TCP (orientado a conexão)
    // 0: Protocolo padrão (TCP)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == 0) {
        throw std::runtime_error("Socket creation failed");
    }

    // Reusar endereço mesmo se socket anterior estiver em TIME_WAIT
    int opt = 1;

    // SO_REUSEADDR: Permite reusar a porta imediatamente após reiniciar o servidor
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        throw std::runtime_error("Setsockopt failed");
    }

    // Configurar endereço
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Aceita conexões de qualquer IP
    address.sin_port = htons(port);       // Converter porta para network byte order - Host to Network Short - Garante ordem de bytes correta (big-endian na rede)

    // Bind
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        throw std::runtime_error("Bind failed on port " + std::to_string(port));
    }
}

SyncServer::~SyncServer() {
    stop();
    close(server_fd);
}

void SyncServer::set_handler(Handler handler) {
    request_handler = handler;
}

void SyncServer::start() {
    // Listen
    if (listen(server_fd, 10) < 0) {
        throw std::runtime_error("Listen failed");
    }

    running = true;
    std::cout << "Servidor síncrono rodando na porta " << ntohs(address.sin_port) << std::endl;

    std::vector<std::thread> threads;

    // Padrão "Thread-per-Connection":
    // Simples de implementar
    // Problema: Não escala para milhares de conexões (custo de threads)
    // Solução futura: I/O multiplexing com epoll/select

    while (running) {
        // Accept
        int client_socket = accept(server_fd, nullptr, nullptr);
        if (client_socket < 0) {
            if (running) {
                std::cerr << "Accept failed" << std::endl;
            }
            continue;
        }

        // Criar thread para cada cliente
        threads.emplace_back([this, client_socket]() {
            this->handle_client(client_socket);
        });
    }

    // Aguardar threads terminarem
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void SyncServer::handle_client(int client_socket) {
    char buffer[4096] = {0}; // Aumentar buffer
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);

    // read(): Chamada bloqueante - thread fica esperando até receber dados
    if (bytes_read > 0) {
        std::string request_str(buffer, bytes_read);
        std::cout << "Requisição recebida:\n" << request_str << std::endl;

        std::string response_str = process_request(request_str);
        send(client_socket, response_str.c_str(), response_str.size(), 0);
    }

    close(client_socket); // IMPORTANTE: Fechar socket
}

std::string SyncServer::process_request(const std::string& request) {
    if (request_handler) {
        return request_handler(request);
    }

    // Handler padrão
    Request req = Request::parse(request);
    Response res;

    if (req.path == "/health") {
        res.body = R"({"status": "healthy", "server": "sync"})";
        res.headers["Content-Type"] = "application/json";
    } else {
        res.status_code = 404;
        res.status_text = "Not Found";
        res.body = "Route not found: " + req.path;
    }

    return res.to_string();
}

void SyncServer::stop() {
    running = false;
    shutdown(server_fd, SHUT_RDWR);
}

/*
O que outras linguagens escondem:
Gerenciamento manual de sockets (abrir/fechar)
Byte ordering (htons, ntohs)
System calls (socket, bind, listen, accept)
Thread management (criação/destruição)
Buffer management (tamanho, overflow protection)

Perguntas para Reflexão:
O que acontece se duas threads tentarem escrever no std::cout ao mesmo tempo?
Por que usamos sizeof(buffer) - 1 no read?
O que é "TIME_WAIT" e por que SO_REUSEADDR é importante?
Como o std::istringstream funciona internamente?
*/
