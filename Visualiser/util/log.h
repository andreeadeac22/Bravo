#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <glm/glm.hpp>

//Support for GLM types printing
std::ostream& operator<<(std::ostream &os, glm::vec3 const &m);
std::ostream& operator<<(std::ostream &os, glm::vec2 const &m);

namespace logging {

    std::ostream& logFormatTime(std::ostream& ostrm);

}

#define _log_issue logging::logFormatTime(std::cerr) << " " << __FILE__ << ":" << __LINE__

#define log_info logging::logFormatTime(std::cout) << " [INFO] "
#define log_warn logging::logFormatTime(std::cout) << " [WARNING] "
#define log_error _log_issue << " [ERROR] "

#define log_end std::endl

#endif // LOG_H
