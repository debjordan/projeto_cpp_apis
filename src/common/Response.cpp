#include "../include/common/Response.hpp"
#include <sstream>

std::string Response::to_string() const {
    std::ostringstream stream;

    // Status line
    stream << version << " " << status_code << " " << status_text << "\r\n";

    // Headers
    for (const auto& [key, value] : headers) {
        stream << key << ": " << value << "\r\n";
    }

    // Body
    if (!body.empty()) {
        stream << "Content-Length: " << body.size() << "\r\n";
    }

    stream << "\r\n"; // Linha vazia separa headers do body
    stream << body;

    return stream.str();
}
