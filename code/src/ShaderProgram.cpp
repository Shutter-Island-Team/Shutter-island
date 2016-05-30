#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

# include <GL/glew.h>

#include "./../include/ShaderProgram.hpp"
#include "./../include/log.hpp"
#include "./../include/gl_helper.hpp"

using namespace std;

int ShaderProgram::null_location = -1;

static void
dump_shader_log( GLuint shader )
{
  GLint logLen;
  glcheck(glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLen ));
  if( logLen > 0 )
    {
      char*  log = (char* )malloc(logLen);
      GLsizei written;
      glGetShaderInfoLog( shader, logLen, &written, log);
      LOG( warning, "shader log:\n" << log );
      free(log);
    }
}

struct gpu_type {
    gpu_type ( std::string ext, GLenum n );
    std::string extension;
    GLenum      name     ;
};

gpu_type::gpu_type (std::string ext, GLenum n)
    : extension(ext), name(n)
{}

static const unsigned int NUMBER_OF_SHADER_TYPES = 6;
static gpu_type gpu_file_extensions [NUMBER_OF_SHADER_TYPES] =
    {
	gpu_type("cs",   GL_COMPUTE_SHADER        ),
	gpu_type("vert", GL_VERTEX_SHADER         ),
	gpu_type("frag", GL_FRAGMENT_SHADER       ),
	gpu_type("geom", GL_GEOMETRY_SHADER       ),
	gpu_type("tcs",  GL_TESS_CONTROL_SHADER   ),
	gpu_type("tes",  GL_TESS_EVALUATION_SHADER)
    };

static bool is_supported_extension(const std::string& ext, GLenum& type)
{
    for( unsigned int i = 0; i < NUMBER_OF_SHADER_TYPES; ++ i ) {
	if ( ext == gpu_file_extensions[i].extension ) {
	    type = gpu_file_extensions[i].name;
	    return true;
	}
    }
    return false;
}

static GLint
check_program_status(GLuint program)
{
  int     info_log_length = 0;
  int     chars_written  = 0;
  GLint   status;

  glcheck(glGetProgramiv(program, GL_LINK_STATUS, &status));
  glcheck(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length));

  if (info_log_length > 1)
  {
      std::vector<char> info_log(info_log_length+1);
      glcheck(glGetProgramInfoLog(program, info_log_length, &chars_written, &info_log[0]));
      LOG( info, "program log:\n" << &info_log[0] );
  }
  return status;
}

static GLuint
compile_shader( const std::string& gpu_name, GLuint type )
{
  // open the shader file
  std::ifstream gpu_file( gpu_name );
  if ( !gpu_file.is_open() )
    {
      LOG( error, "cannot open shader file " << gpu_name << ". Are you in the right directory?" );
      return 0;
    }

  // create a new shader object
  glcheck(GLuint shader = glCreateShader( type ));
  if ( !shader )
    {
      LOG( error, "error when creating shader [" << gpu_name << "]");
      return 0;
    }

  // load the shader source in one string
  std::stringstream gpu_data;
  gpu_data << gpu_file.rdbuf();
  gpu_file.close(); // no more needed, so close it now
  const std::string gpu_string = gpu_data.str();

  // set the source of the shader (as one big cstring)
  const char*  strShaderVar = gpu_string.c_str();
  GLint iShaderLen = gpu_string.size();
  glcheck(glShaderSource ( shader, 1,(const GLchar**) &strShaderVar, (GLint*)&iShaderLen ));

  // compile the shader
  glcheck(glCompileShader( shader ));
  GLint result;
  glcheck(glGetShaderiv( shader, GL_COMPILE_STATUS, &result ));
  if (GL_FALSE == result) {
      LOG( error, "shader [" << gpu_name << "] compilation failed!");
      GLint logLen;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
      if(logLen > 0) {
	  char*  log = (char* )malloc(logLen);
	  GLsizei written;
	  glGetShaderInfoLog(shader, logLen, &written, log);
	  LOG(warning, "shader log:\n" << log);
	  free(log);
      }
      glDeleteShader(shader);
      return 0;
  }
  return shader;
}

ShaderProgram::ShaderProgram()
  : m_programId{0}
{}


ShaderProgram::ShaderProgram(const std::list< std::string >& shader_sources)
    : m_source_filenames{ shader_sources }, m_programId{ 0 }
  {
      reload();
  }

ShaderProgram::~ShaderProgram()
{
  if( glIsProgram(m_programId) )
    glcheck(glDeleteProgram(m_programId));
}

void
ShaderProgram::load(const std::list< std::string >& shader_sources)
{
    m_source_filenames = shader_sources;
    reload();
}


std::string
ShaderProgram::getExtension(const std::string &s) {
   size_t i = s.rfind('.', s.length());
   if (i != string::npos) {
      return(s.substr(i+1, s.length() - i));
   }
   return("");
}

void
ShaderProgram::reload()
{
    if( m_programId ) {
	glcheck(glDeleteShader(m_programId));
    }

    m_programId = 0;
    bool valid = true;
    for(auto& filename : m_source_filenames) {
	if(!m_programId) {
	    glcheck( m_programId = glCreateProgram());
	}
	GLenum type;
	if( !is_supported_extension(getExtension(filename), type) ) {
	    LOG( error, "unknown filename extension: " << filename );
	    valid = false;
	    break;
	} else {
	    GLuint shader = compile_shader(filename, type);
	    if(shader) {
		glcheck(glAttachShader(m_programId, shader));
		glcheck(glDeleteShader(shader));
	    } else {
		valid = false;
		break;
	    }
	}
    }

    if(valid && m_programId) {
	glcheck(glLinkProgram( m_programId ));
	GLint status;
	glcheck(glGetProgramiv( m_programId, GL_LINK_STATUS, &status ));
	if(GL_FALSE == status) {
	    LOG( error, "failed to link shader program " << m_programId );
	    GLint logLen;
	    glGetProgramiv( m_programId, GL_INFO_LOG_LENGTH, &logLen);
	    if(logLen > 0 ) {
		    char *log = (char *)malloc(logLen);
		    GLsizei written;
		    glGetProgramInfoLog(m_programId, logLen, &written, log);
		    LOG(error, "program log:\n" << log);
		    free(log);
		}
	    valid = false;
	}
    }
    if(valid)
	ressources_introspection();
    else {
	glcheck(glDeleteProgram(m_programId));
	m_programId = 0;
    }
}


void
ShaderProgram::bind()
{
  glcheck(glUseProgram(m_programId));
}

void
ShaderProgram::unbind()
{
  glcheck(glUseProgram(0));
}

static const GLenum uniform_properties[3] = {
    GL_BLOCK_INDEX,
    GL_NAME_LENGTH,
    GL_LOCATION
};

static const GLenum attribute_properties[2] = {
    GL_NAME_LENGTH,
    GL_LOCATION
};


ShaderProgram::operator unsigned int() const
{
    return m_programId;
}
GLuint ShaderProgram::programId()
{
    return m_programId;
}


void
ShaderProgram::ressources_introspection()
{
    LOG( info, "variable introspection of shader program " << m_programId );
    LOG( info, "this shader program was compiled from the following shaders:");
    for( auto& file : m_source_filenames )
	{
	    LOG( info, "  - " << file );
	}

    //Clean the maps
    m_uniforms.clear();
    m_attributes.clear();

    GLint values[3];

    GLint num_uniforms = 0;
    glcheck(glGetProgramInterfaceiv( m_programId, GL_UNIFORM, GL_ACTIVE_RESOURCES, &num_uniforms));
    LOG( info, "  * " << num_uniforms << " uniform(s)");
    for(int unif = 0; unif < num_uniforms; ++unif)
	{
	    glcheck(glGetProgramResourceiv( m_programId, GL_UNIFORM, unif, 3, uniform_properties, 3, NULL, values));

	    //Skip any uniforms that are in a block.
	    if(values[0] != -1)
		{
		    LOG( info, "    - uniform #" << unif << " is in a block");
		    continue;
		}

	    char* name = new char[values[1]];
	    glcheck(glGetProgramResourceName(m_programId, GL_UNIFORM, unif, values[1], NULL, &name[0]));
	    m_uniforms.insert( {{name, values[2]}});

	    LOG( info, "    - uniform #" << unif << ": " << name );
	    delete[] name;
	}

    GLint num_attributes = 0;
    glcheck(glGetProgramInterfaceiv( m_programId, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &num_attributes ));
    LOG( info, "  * " << num_attributes << " attribute(s)");
    for( int att = 0; att < num_attributes; ++ att )
	{
	    glcheck(glGetProgramResourceiv( m_programId, GL_PROGRAM_INPUT, att, 2, attribute_properties, 2, NULL, values ));
	    char* name = new char[values[0]];
	    glcheck(glGetProgramResourceName(m_programId, GL_PROGRAM_INPUT, att, values[0], NULL, &name[0]));
	    m_attributes.insert( {{name, values[1] }} );
	    LOG( info, "    - attribute #" << att << ": " << name );
	    delete[]name;
	}
}

GLint ShaderProgram::getUniformLocation( const std::string& name ) const
{
  std::unordered_map< std::string, int >::const_iterator search = m_uniforms.find( name );
  if( search != m_uniforms.end() )
    return search->second;
  //LOG( error, "there is no uniform named \"" << name << "\" in this program " << this );
  return null_location;
}

GLint ShaderProgram::getAttributeLocation( const std::string& name ) const
{
  std::unordered_map< std::string, int >::const_iterator search = m_attributes.find( name );
  if( search != m_attributes.end() )
    return search->second;
  //LOG( error, "there is no attribute named \"" << name << "\" in this program " << this );
  return null_location;
}

