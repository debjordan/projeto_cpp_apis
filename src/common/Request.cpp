#include "../include/common/Request.hpp"
#include <sstream>
#include <iostream>

Request Request::parse(const std::string& raw_request) {
    Request req;
    std::istringstream stream(raw_request); // Converte string em stream, ou seja, permite ler a string como se fosse um arquivo/stream(sequencia de elementos disponibilidados no decorrer do tempo), usando getline()
    std::string line;

    // Parse primeira linha: "GET /health HTTP/1.1"
    std::getline(stream, line);
    std::istringstream first_line(line);

    // O operador >> lê até encontrar espaço, perfeito para "GET /health HTTP/1.1"
    first_line >> req.method >> req.path >> req.version;

    // Parse headers: "Host: localhost"
    while (std::getline(stream, line) && line != "\r") {
        auto colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 2); // +2 para pular ": "
            if (!value.empty() && value.back() == '\r') {
                value.pop_back(); // Remove \r do final
            }
            req.headers[key] = value;
        }
    }

    // Parse body (se existir)
    std::getline(stream, req.body, '\0');

    return req;
}
