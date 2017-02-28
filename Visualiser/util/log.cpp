#include "log.h"

#include <iomanip>

namespace logging {

std::ostream& logFormatTime(std::ostream &ostrm) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    return (ostrm << std::put_time(&tm, "%H:%M:%S"));
}

}
