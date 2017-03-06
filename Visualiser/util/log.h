#pragma once

#include <iostream>

#include <osg/Vec3d>
#include <osg/Vec2s>
#include <osg/Vec2i>

#define S_ASSERT(cond, msg) if (!(cond)) throw msg;

std::ostream& operator<<(std::ostream &os, osg::Vec3d const &m);
std::ostream& operator<<(std::ostream &os, osg::Vec2s const &m);
std::ostream& operator<<(std::ostream &os, osg::Vec2i const &m);

namespace logging {

    std::ostream& logFormatTime(std::ostream& ostrm);

}

#define _log_issue logging::logFormatTime(std::cerr) << " " << __FILE__ << ":" << __LINE__

#define log_info logging::logFormatTime(std::cout) << " [INFO] "
#define log_warn logging::logFormatTime(std::cout) << " [WARNING] "
#define log_error _log_issue << " [ERROR] "

#define log_end std::endl
