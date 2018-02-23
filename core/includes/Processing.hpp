
#pragma once

#include <http.hpp>
#include "Usings.hpp"

namespace core {

    class Processing {
    public:
        static RequestPtr parseRequest(Raw &req);

        static Raw createResponse(ResponsePtr &response);
    };

}
