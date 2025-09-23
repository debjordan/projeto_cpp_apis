#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <unordered_map>

struct Request {
    std::string method;      // GET, POST, etc.
    std::string path;        // /health, /api/users, etc.
    std::string version;     // HTTP/1.1
    std::unordered_map<std::string, std::string> headers; // Headers HTTP: Hash table para headers (O(1) acesso)
    std::string body;        // Conteúdo (para POST)

    // Parse uma requisição HTTP raw
    static Request parse(const std::string& raw_request); // Evita cópia, garante imutabilidade
};

#endif
