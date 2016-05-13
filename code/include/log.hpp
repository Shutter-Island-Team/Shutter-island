/*  Created on: Dec 13, 2015
 *      Author: T. Delame (tdelame@gmail.com)
 */
/** @file
 * @brief Log functionalities
 *
 * This file groups all the log features of the project. You are only expected
 * to use the LOG() macro.*/
# ifndef LOG_H_
# define LOG_H_
# include <iostream>
# include <ctime>


/**@brief  Log severity levels.
 *
 * A severity level refers to a particular kind of information logged in a
 * log entry by the macro #LOG(level,message). Its value reflects its
 * importance, from ::trace which is the less important to
 * ::fatal which is critical.
 */
enum severity_level {
  /** the log entry is a trace to understand the program flow */
  trace,
  /** the log entry is a debug information, that should not appear in production
   * code */
  debug,
  /** the log entry is an information that might interest the user */
  info,
  /** the log entry is a warning about something that is not an error but might
   * not be what the user expected */
  warning,
  /** the log entry is an error, something needs to be fixed */
  error,
  /** the log entry is a fatal error, terminating immediately the application */
  fatal
};

/** @brief Holds the string version of the severity levels.
 *
 * This array is used to print a string representing the severity level in the
 * log entry.
 */
extern const std::string severity_names[6];
# define LOG_WITH_LINE_FILE( level, message, line, file )                   \
{                                                                           \
  if( severity_level::level > severity_level::warning )                     \
  {                                                                         \
    std::time_t time = std::time( nullptr );                                \
    char mbstr[ 30 ];                                                       \
    std::strftime(mbstr, sizeof(mbstr), "%c", std::localtime(&time));       \
    std::cout<< '[' << mbstr << ']'                                         \
             << severity_names[severity_level::level]<< " "                 \
             << message                                                     \
             << " (in "<< file <<":"<< line <<")"                           \
             << std::endl;                                                  \
  }                                                                         \
  else                                                                      \
  {                                                                         \
    std::time_t time = std::time( nullptr );                                \
    char mbstr[ 30 ];                                                       \
    std::strftime(mbstr, sizeof(mbstr), "%c", std::localtime(&time));       \
    std::cout<< '[' << mbstr << ']'                                         \
             << severity_names[severity_level::level]<< " "                 \
             << message << std::endl;                                       \
  }                                                                         \
}

/**
 * Log an entry with a specific #severity_level. For levels above
 * severity_level::warning, the location (file and line) of the code that
 * request to log that entry is automatically included in the entry. The
 * message can be built by concatening strings and values with the stream
 * operator:
 * \code{.cpp}
 * float x = 2, y = 5;
 * LOG( debug, "the 2D point is at {" << x << ", " << y << "}");
 * \endcode
 */
# define LOG( level, message ) LOG_WITH_LINE_FILE( level, message, __LINE__, __FILE__ )

#endif /* LOG_H_ */
