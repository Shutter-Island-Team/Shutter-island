/*  Created on: Dec 13, 2015
 *      Author: T. Delame (tdelame@gmail.com)
 */

# include "./../include/gl_helper.hpp"
# include "./../include/log.hpp"
# include <GL/glew.h>

void
check_previous_gl_errors()
{
  GLenum error;
  while( ( error=glGetError() ) != GL_NO_ERROR )
    {
      LOG_WITH_LINE_FILE( error, "[GL] previous error discovered: "
        << gluErrorString(error), __LINE__, __FILE__ );
      error = glGetError();
    }
}

void
check_gl_error( const char* call, const char* file, const int line )
{
  GLenum error = glGetError();
  if( error != GL_NO_ERROR )
    {
      LOG_WITH_LINE_FILE( error, "[GL] Error discovered when calling "
        << call << ": " << gluErrorString(error), line, file );
    }
}
