#include "log.h"

#include <iomanip>

std::ostream& operator<<(std::ostream &os, osg::Vec3d const &m)
{
    return (os << "vec3d(" << m.x() << ", " << m.y() << ", " << m.z() << ")");
}

std::ostream& operator<<(std::ostream &os, osg::Vec2s const &m)
{
    return (os << "vec2s(" << m.x() << ", " << m.y() << ")");
}

std::ostream& operator<<(std::ostream &os, osg::Vec2i const &m)
{
    return (os << "vec2i(" << m.x() << ", " << m.y() << ")");
}

namespace logging {

std::ostream& logFormatTime(std::ostream &ostrm) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    return (ostrm << std::put_time(&tm, "%H:%M:%S"));
}

}
