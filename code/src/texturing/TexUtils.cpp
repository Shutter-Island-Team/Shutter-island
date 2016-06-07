#include "../../include/texturing/TexUtils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>


void sendMipMapTextures(std::vector<std::string> &filenames,
			GLuint *idPtr) {
    
    // Images to load
    std::vector<sf::Image> images;
    sf::Vector2u imageSize(0,0);
    images.resize(filenames.size());

    //Create texture
    glGenTextures(1, idPtr);

    //Bind the texture
    glBindTexture(GL_TEXTURE_2D, *idPtr);

    // Texture parameters
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

    // Loading images
    for(unsigned int i=0; i<images.size(); ++i)
	{
	    images[i].loadFromFile(filenames[i]);
	    images[i].flipVertically(); // sfml inverts the v axis... put the image in OpenGL convention: lower left corner is (0,0)
	    if(i==0) imageSize = images[i].getSize();
	}

    // Creating the mip map
    glTexStorage2D(GL_TEXTURE_2D, images.size(), GL_RGBA32F, imageSize.x, imageSize.y);

    // Pushing all sub images
    for(unsigned int i=0; i<images.size(); ++i)
    {
        glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, 
			images[i].getSize().x, images[i].getSize().y, 
			GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)images[i].getPixelsPtr());
    }
    
    //Release the texture
    glBindTexture(GL_TEXTURE_2D, 0);


}
