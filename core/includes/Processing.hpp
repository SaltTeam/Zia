
#pragma once

#include <Http.hpp>
#include "Usings.hpp"

namespace core {

    class Processing {
    public:
        static Request parseRequest(Raw &req);

        static void createResponse(Raw &dest, Response &response);
    };

}
