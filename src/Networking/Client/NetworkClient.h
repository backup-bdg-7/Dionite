// ============================================================================
// Dionite — Networking: HTTP / WebSocket client (libcurl/cpr placeholder)
// ============================================================================
#pragma once
#include <string>
#include <functional>
#include <unordered_map>

namespace dionite::net {

struct HttpResponse {
    int statusCode;
    std::string body;
    std::unordered_map<std::string, std::string> headers;
};

class NetworkClient {
public:
    explicit NetworkClient(std::string baseUrl) : baseUrl_(std::move(baseUrl)) {}

    void setBearerToken(std::string t) { token_ = std::move(t); }

    // These are stubs to be implemented by cpr/libcurl on the target platform.
    // Methods exist so engine code can compile against the public interface.
    HttpResponse get(const std::string& path) const { return req("GET", path, ""); }
    HttpResponse post(const std::string& path, const std::string& body) const { return req("POST", path, body); }
    HttpResponse put(const std::string& path, const std::string& body) const { return req("PUT", path, body); }
    HttpResponse del(const std::string& path) const { return req("DELETE", path, ""); }

    const std::string& baseUrl() const { return baseUrl_; }
    const std::string& token()   const { return token_; }

private:
    HttpResponse req(const std::string& /*method*/, const std::string& /*path*/, const std::string& /*body*/) const {
        // Real implementation: integrate cpr or libcurl here.
        return {200, "{}", {}};
    }
    std::string baseUrl_;
    std::string token_;
};

} // namespace dionite::net
