
#include "Processing.hpp"
#include "Utils.hpp"

using namespace std::literals::string_literals;

namespace core {
    Request Processing::parseRequest(Raw &req) {
        std::string msg;
        std::transform(req.begin(), req.end(), std::back_inserter(msg),
                       [](auto c) { return static_cast<char>(c); });

        auto lines = Utils::split(msg, "\r\n");

        auto arg = Utils::split(lines[0], ' ');
        std::string sMethod = arg[0];
        std::string uri = arg[1];
        std::string sVersion = arg[2];

        std::cout << "Version: " << sVersion << std::endl;
        std::cout << "Method: " << sMethod << std::endl;
        std::cout << "URI: " << uri << std::endl;

        zia::api::http::Method method;
        if (sMethod == "GET")
            method = zia::api::http::Method::get;
        else if (sMethod == "PUT")
            method = zia::api::http::Method::put;
        else if (sMethod == "POST")
            method = zia::api::http::Method::post;
        else if (sMethod == "HEAD")
            method = zia::api::http::Method::head;
        else if (sMethod == "DELETE")
            method = zia::api::http::Method::delete_;
        else if (sMethod == "TRACE")
            method = zia::api::http::Method::trace;
        else if (sMethod == "CONNECT")
            method = zia::api::http::Method::connect;
        else if (sMethod == "OPTIONS")
            method = zia::api::http::Method::options;
        else throw std::exception();

        zia::api::http::Version version;
        if (sVersion == "HTTP/1.1")
            version = zia::api::http::Version::http_1_1;
        else if (sVersion == "HTTP/1.0")
            version = zia::api::http::Version::http_1_0;
        else if (sVersion == "HTTP/0.9")
            version = zia::api::http::Version::http_0_9;
        else throw std::exception();

        Request request{version, method, uri};

        unsigned i;
        for (i = 1; i < lines.size() && !lines[i].empty(); i++) {
            auto h = Utils::split(lines[i], ": ");
            auto values = Utils::split(h[1], ',');
            for (const auto &val: values)
                request.headers[h[0]].push_back(val);
        }

        std::vector<std::string> vecBody;
        for (i += 1; i < lines.size(); i++) {
            vecBody.push_back(lines[i]);
        }
        std::string body = Utils::join(vecBody, "\r\n");

        std::cout << body << std::endl;

        std::transform(body.begin(), body.end(), std::back_inserter(request.rawBody),
                       [](auto c) { return static_cast<std::byte>(c); });

        return request;
    }

    Raw Processing::createResponse(Response &response) {
        std::string resp;
        switch (response.version) {
            case zia::api::http::Version::http_0_9:
                resp = "HTTP/0.9";
                break;
            case zia::api::http::Version::http_2_0:
                resp = "HTTP/2.0";
                break;
            case zia::api::http::Version::http_1_0:
                resp = "HTTP/1.0";
                break;
            case zia::api::http::Version::http_1_1:
            default:
                resp = "HTTP/1.1";
                break;
        }
        resp += " "s + std::to_string(response.statusCode) + " "s + response.statusReason + "\r\n";
        for (const auto &entry: response.headers) {
            resp += entry.first + " "s + Utils::join(entry.second, ",") + "\r\n";
        }
        resp += "\r\n";
        resp += response.body;

        Raw rawResp;
        std::transform(resp.begin(), resp.end(), std::back_inserter(rawResp),
                       [](auto c) { return static_cast<std::byte>(c); });

        return rawResp;
    }
}
