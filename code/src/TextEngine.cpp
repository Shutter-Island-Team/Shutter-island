#include "./../include/TextEngine.hpp"
#include "./../include/log.hpp"
#include "./../include/gl_helper.hpp"

#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

// Whenever a line in the text atlas require more than this number of pixel,
// another line is started.
static const int max_texture_width = 1024;

struct TextAtlas {
  GLuint textureID;

  unsigned int textureWidth;
  unsigned int textureHeight;
  unsigned int maxCharacterHeight;

  struct CharacterInformation {
    glm::vec2 advance;
    glm::ivec2 size;
    glm::ivec2 bearing;

    float tx; // x offset of glyph in texture coordinates
    float ty; // y offset of glyph in texture coordinates
  } characterInformation[128];

  TextAtlas(FT_Face face, int height)
    : textureWidth{0}, textureHeight{0}, maxCharacterHeight{0}
  {
    unsigned int roww = 0;
    unsigned int rowh = 0;

    memset(characterInformation, 0, sizeof characterInformation);

    FT_Set_Pixel_Sizes(face, 0, height);
    FT_GlyphSlot g = face->glyph;

    /* Find minimum size for a texture holding all visible ASCII characters */
    for ( int i = 32; i < 128; i++)
      {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
          {
            LOG( error, "failed to load glyph " << i );
            continue;

          }
        if ( roww + g->bitmap.width + 1 >= max_texture_width )
          {
            textureWidth = std::max(textureWidth, roww);
            textureHeight += rowh;
            roww = 0;
            rowh = 0;
          }
      roww += g->bitmap.width + 1;
      rowh = std::max(rowh, (unsigned int)g->bitmap.rows);
      maxCharacterHeight = std::max( maxCharacterHeight, rowh );
    }

    textureWidth = std::max(textureWidth, roww);
    textureHeight += rowh;

    /* Create a texture that will be used to hold all ASCII glyphs */
    glcheck(glGenTextures(1, &textureID));
    glcheck(glBindTexture(GL_TEXTURE_2D, textureID));
    glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0 ));

    /* We require 1 byte alignment when uploading texture data */
    glcheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    /* Clamping to edges is important to prevent artifacts when scaling */
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    /* Linear filtering usually looks best for text */
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    /* Paste all glyph bitmaps into the texture, remembering the offset */
    int ox = 0;
    int oy = 0;

    rowh = 0;

    const float invTextureWidth = float(1.0/textureWidth);
    const float invTextureHeight = float(1.0/textureHeight);
    for ( int i = 32; i < 128; i++)
      {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
          {
            LOG(error, "loading character " << i << " failed" );
            continue;
          }

        if ( ox + g->bitmap.width + 1 >= max_texture_width )
          {
            oy += rowh;
            rowh = 0;
            ox = 0;
          }

        glcheck(glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer));
        characterInformation[i].advance = glm::ivec2(float(g->advance.x)*0.015625f, float(g->advance.y)*0.015625f);
        characterInformation[i].size = glm::ivec2( g->bitmap.width, g->bitmap.rows);
        characterInformation[i].bearing = glm::ivec2( g->bitmap_left, g->bitmap_top );
        characterInformation[i].tx = float(ox) * invTextureWidth;
        characterInformation[i].ty = float(oy) * invTextureHeight;

        rowh = std::max(rowh, (unsigned int)g->bitmap.rows);
        ox += g->bitmap.width + 1;
      }
    LOG( info, "generated a " << textureWidth << " x " << textureHeight << " (" << textureWidth * textureHeight / 1024 << " kb) texture atlas" );
  }

  ~TextAtlas() {
    glcheck(glDeleteTextures(1, &textureID));
  }
};

TextEngine::TextEngine()
  : m_atlas{nullptr},
    m_vboVertices{0}, m_vertexLocation{-1},
    m_projectionLocation{-1}, m_colorLocation{-1},
    m_samplerLocation{-1}
{}

void TextEngine::init()
{
    FT_Library ft;
    if( FT_Init_FreeType(&ft) )
     {
        LOG(fatal, "[FreeType] cannot initialize library");
        exit( EXIT_FAILURE );
     }
    FT_Face face;
    if( FT_New_Face( ft, "./../fonts/Anonymous_Pro.ttf", 0, &face))
      {
        LOG(fatal, "[FreeType] cannot load font");
        exit( EXIT_FAILURE );
      }

    // init program and acquire GPU variables locations (names)
    m_program.load(std::list<std::string>{"./../shaders/text_vertex.vert",
		"./../shaders/text_fragment.frag"});
    m_projectionLocation = m_program.getUniformLocation( "projection" );
    m_colorLocation = m_program.getUniformLocation("text_color");
    m_samplerLocation = m_program.getUniformLocation("tex");
    m_vertexLocation = m_program.getAttributeLocation("vertex");

    // init the atlas with a 24 pixel size
    m_atlas = new TextAtlas( face, 24 );

    // create VBO
    glcheck(glGenBuffers(1, &m_vboVertices)); // get a buffer pointer name
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices)); // allocate memory for the pointer
}

TextEngine::~TextEngine()
{
  delete m_atlas;
}

void TextEngine::setWindowDimensions( float width, float height )
{
  m_projection = glm::ortho(float(0), width, float(0), height );
}

void TextEngine::render( const std::string& msg, const glm::vec2& pixelCoordinates,
             const glm::vec3& textColor, float scale )
{
  const int nb_triangles = msg.length() * 2;
  std::vector< float > vertices( nb_triangles * 12, 0.0f );
  {
    size_t float_idx = 0;
    glm::vec2 characterCoordinates = pixelCoordinates;
    // this is done for two reasons:
    // 1) a multiplication is faster than a division
    // 2) for N characters, each of the division are computer 3*N times
    const float invTextureWidth = float(1.0/m_atlas->textureWidth);
    const float invTextureHeight = float(1.0/m_atlas->textureHeight);
    for( const char& c : msg )
      {
        TextAtlas::CharacterInformation& charInfo = m_atlas->characterInformation[ c ];

        // actual dimension in pixels of the character
        float charWidth = charInfo.size.x * scale;
        float charHeight = charInfo.size.y * scale;

        // position of the lower left corner of the character bounding box
        float x2 =  characterCoordinates.x + charInfo.bearing.x * scale;
        float y2 =  characterCoordinates.y - (charInfo.size.y - charInfo.bearing.y) * scale;

        // compute the
        characterCoordinates.x += charInfo.advance.x * scale;

        // skip non renderable glyphs
        if( !charWidth || !charHeight )
          {
            if( c == '\n' )
              {
                characterCoordinates.y -= 1.25 * scale * m_atlas->maxCharacterHeight;
                characterCoordinates.x  = pixelCoordinates.x;
              }
            continue;
          }

        // compute the vertex info for the 2 triangles making the quad we will texture
        // note: the atlas is stored upside down, thus when the y coordinate increase,
        // the texture y coordinate (v) decrease.
        vertices[ float_idx ++ ] = x2;
        vertices[ float_idx ++ ] = y2 + charHeight;
        vertices[ float_idx ++ ] = charInfo.tx;
        vertices[ float_idx ++ ] = charInfo.ty;

        vertices[ float_idx ++ ] = x2;
        vertices[ float_idx ++ ] = y2;
        vertices[ float_idx ++ ] = charInfo.tx;
        vertices[ float_idx ++ ] = charInfo.ty + float(charInfo.size.y) * invTextureHeight;

        vertices[ float_idx ++ ] = x2 + charWidth;
        vertices[ float_idx ++ ] = y2;
        vertices[ float_idx ++ ] = charInfo.tx + float(charInfo.size.x) * invTextureWidth;
        vertices[ float_idx ++ ] = charInfo.ty + float(charInfo.size.y) * invTextureHeight;

        vertices[ float_idx ++ ] = x2;
        vertices[ float_idx ++ ] = y2 + charHeight;
        vertices[ float_idx ++ ] = charInfo.tx;
        vertices[ float_idx ++ ] = charInfo.ty;

        vertices[ float_idx ++ ] = x2 + charWidth;
        vertices[ float_idx ++ ] = y2;
        vertices[ float_idx ++ ] = charInfo.tx + float(charInfo.size.x) * invTextureWidth;
        vertices[ float_idx ++ ] = charInfo.ty + float(charInfo.size.y) * invTextureHeight;

        vertices[ float_idx ++ ] = x2 + charWidth;
        vertices[ float_idx ++ ] = y2 + charHeight;
        vertices[ float_idx ++ ] = charInfo.tx + float(charInfo.size.x) * invTextureWidth;
        vertices[ float_idx ++ ] = charInfo.ty;
      }
  }

  // the quads will be blended with what is behind (the 3D scene)
  glcheck(glEnable(GL_BLEND));
  glcheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // the quads are rendered in the right order, no need to work with the two sides of each quad
  glcheck(glEnable(GL_CULL_FACE));

  m_program.bind();

  // deal with the texture
  glcheck(glActiveTexture(GL_TEXTURE0)); //make texture unit 0 active
  glcheck(glBindTexture(GL_TEXTURE_2D, m_atlas->textureID)); //put our altas on this texture unit
  glcheck(glUniform1i(m_samplerLocation, 0)); //tell the sampler to use texture unit 0

  // deal with the uniforms
  glcheck(glUniformMatrix4fv(m_projectionLocation, 1, GL_FALSE, glm::value_ptr(m_projection)));
  glcheck(glUniform3fv(m_colorLocation, 1, glm::value_ptr(textColor)));

  // transfer our vertex data to be used by the vertex attrib array #0
  glcheck(glBindBuffer(GL_ARRAY_BUFFER,m_vboVertices)); // bind our VBO to ARRAY_BUFFER
  glcheck(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW)); // transmit data to the GPU
  glcheck(glEnableVertexAttribArray(m_vertexLocation));
  glcheck(glVertexAttribPointer(m_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));

  glcheck(glDrawArrays(GL_TRIANGLES, 0, nb_triangles * 3 ));

  // restore the previous openGL state
  glcheck(glBindTexture(GL_TEXTURE_2D, 0));
  glcheck(glDisableVertexAttribArray(m_vertexLocation));
  glcheck(glDisable(GL_BLEND));
  glcheck(glDisable(GL_CULL_FACE));
}
