//
// Created by 西村智 on 12/1/19.
//

#include "LoggingObjC.h"
#include "Logging.h"

void LogTrace(const char* str, int line, const char* file)
{
    BOOST_LOG_SEV(global_logger::get(), boost::log::trivial::trace)
  << boost::log::add_value("Line", line)
  << boost::log::add_value("File",
          boost::filesystem::relative(
                  boost::filesystem::absolute(file, __BINARY_DIR__),
                  __SOLUTION_DIR__).string())
  << str;
}
void LogDebug(const char* str, int line, const char* file)
{
    BOOST_LOG_SEV(global_logger::get(), boost::log::trivial::debug)
        << boost::log::add_value("Line", line)
        << boost::log::add_value("File",
                                 boost::filesystem::relative(
                                         boost::filesystem::absolute(file, __BINARY_DIR__),
                                         __SOLUTION_DIR__).string())
        << str;
}
void LogInfo(const char* str, int line, const char* file)
{
    BOOST_LOG_SEV(global_logger::get(), boost::log::trivial::info)
        << boost::log::add_value("Line", line)
        << boost::log::add_value("File",
                                 boost::filesystem::relative(
                                         boost::filesystem::absolute(file, __BINARY_DIR__),
                                         __SOLUTION_DIR__).string())
        << str;
}
void LogWarning(const char* str, int line, const char* file)
{
    BOOST_LOG_SEV(global_logger::get(), boost::log::trivial::warning)
        << boost::log::add_value("Line", line)
        << boost::log::add_value("File",
                                 boost::filesystem::relative(
                                         boost::filesystem::absolute(file, __BINARY_DIR__),
                                         __SOLUTION_DIR__).string())
        << str;
}
void LogError(const char* str, int line, const char* file)
{
    BOOST_LOG_SEV(global_logger::get(), boost::log::trivial::error)
        << boost::log::add_value("Line", line)
        << boost::log::add_value("File",
                                 boost::filesystem::relative(
                                         boost::filesystem::absolute(file, __BINARY_DIR__),
                                         __SOLUTION_DIR__).string())
        << str;
}
void LogFatal(const char* str, int line, const char* file)
{
    BOOST_LOG_SEV(global_logger::get(), boost::log::trivial::fatal)
        << boost::log::add_value("Line", line)
        << boost::log::add_value("File",
                                 boost::filesystem::relative(
                                         boost::filesystem::absolute(file, __BINARY_DIR__),
                                         __SOLUTION_DIR__).string())
        << str;
}