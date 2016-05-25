#ifndef State_RENDERABLE_HPP
#define State_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "MovableBoid.hpp"
#include "StateType.hpp"
#include <vector>
#include <glm/glm.hpp>

class StateRenderable : public HierarchicalRenderable
{
    public:
        ~StateRenderable();
        StateRenderable( ShaderProgramPtr program, MovableBoidPtr boid );

    private:
        void do_draw();
        void do_animate( float time );

        void do_keyPressedEvent( sf::Event& e );
        glm::vec4 stateColor();

        bool m_display;
        MovableBoidPtr m_boid;
        StateType m_state;

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec4 > m_colors;
        std::vector< glm::vec3 > m_normals;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
};

typedef std::shared_ptr<StateRenderable> StateRenderablePtr;

#endif
