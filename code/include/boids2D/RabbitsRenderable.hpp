#ifndef RABBITS_RENDERABLE_HPP
#define RABBITS_RENDERABLE_HPP

#include <glm/glm.hpp>
#include "../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include "BoidsManager.hpp"

/**
 * @class RabbitsRenderable
 * @brief Renderable for all the rabbit in the RabbitsManager. @see Renderable
 */
class RabbitsRenderable : public HierarchicalRenderable
{
    public:
        RabbitsRenderable(ShaderProgramPtr shaderProgram, BoidsManagerPtr boidsManager, 
    		const std::string& mesh, const std::string & texture);

        ~RabbitsRenderable();
        
        void setMaterial(const MaterialPtr& material);

    private:
        void do_draw();
        void do_animate( float time );

        std::vector< glm::vec2 > m_texCoords;
	    std::vector< glm::vec3 > m_vectorBuffer;
	    std::vector< glm::mat4 > m_modelMatrix;
	    int m_nbElement;

	    unsigned int m_instanceVBO;
	    std::vector< unsigned int > m_VAOs;
	    unsigned int m_VBO;
	    unsigned int m_tBuffer;
	    unsigned int m_texId;

	    MaterialPtr m_material;
	    BoidsManagerPtr m_boidsManager;     
};

typedef std::shared_ptr<RabbitsRenderable> RabbitsRenderablePtr;

#endif