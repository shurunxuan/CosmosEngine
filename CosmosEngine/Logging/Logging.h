//
// Created by shuru on 7/13/2019.
//

#ifndef COSMOSENGINE_LOGGING_H
#define COSMOSENGINE_LOGGING_H

#include "../Export.h"

#include <fstream>
#include <ostream>
#include <iostream>
#include <iomanip>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/filesystem.hpp>

#define LOG_LOGGER(logger, sev) BOOST_LOG_SEV(logger, sev) \
  << boost::log::add_value<int>("Line", __LINE__)      \
  << boost::log::add_value("File", boost::filesystem::relative(boost::filesystem::absolute(__FILE__, __BINARY_DIR__), __SOLUTION_DIR__).string())\
  << boost::log::add_value("Function", BOOST_CURRENT_FUNCTION)

#define LOG(sev) LOG_LOGGER(global_logger::get(), boost::log::trivial::sev)

#define LOG_TRACE LOG(trace)
#define LOG_DEBUG LOG(debug)
#define LOG_INFO LOG(info)
#define LOG_WARNING LOG(warning)
#define LOG_ERROR LOG(error)
#define LOG_FATAL LOG(fatal)

/**
 * @brief Define the global logger
 */
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(global_logger,
                                       boost::log::sources::severity_logger<boost::log::trivial::severity_level>)

/**
 * @brief Initialize global logger
 *
 * Should be called only once at the very beginning of the program
 */
void ENGINE_API InitLogger();

/**
 * @brief Stop async global loggers
 *
 * Should be called only once at the very end of the program
 */
void ENGINE_API StopLogger();

#endif //COSMOSENGINE_LOGGING_H
