#include "log.h"

#include <iomanip>

std::ostream& operator<<(std::ostream &os, glm::vec3 const &m)
{
    return (os << "vec3(" << m.x << ", " << m.y << ", " << m.z << ")");
}

std::ostream& operator<<(std::ostream &os, glm::vec2 const &m)
{
    return (os << "vec2(" << m.x << ", " << m.y << ")");
}


namespace logging {

std::ostream& logFormatTime(std::ostream &ostrm) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    return (ostrm << std::put_time(&tm, "%H:%M:%S"));
}

}
