

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>

namespace warehouse {

struct Connection {
    std::string endpointAId;
    std::string endpointBId;
    double distance = 0.0;
};

} // namespace warehouse

#endif
