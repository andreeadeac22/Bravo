#pragma once

#include <iostream>

#define S_ASSERT(cond, msg) if (!(cond)) throw msg;

namespace logging {

    std::ostream& logFormatTime(std::ostream& ostrm);

}

#define _log_issue logging::logFormatTime(std::cerr) << " " << __FILE__ << ":" << __LINE__

#define log_info logging::logFormatTime(std::cout) << " [INFO] "
#define log_warn logging::logFormatTime(std::cout) << " [WARNING] "
#define log_error _log_issue << " [ERROR] "

#define log_end std::endl
