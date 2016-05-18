#ifndef HIERARCHICAL_RENDERABLE_HPP
#define HIERARCHICAL_RENDERABLE_HPP

/**@file
 *@brief Define the hierarchical renderable interface.
 *
 * The HierarchicalRenderable class is the base class for all the 3D objects that will
 * be made of several renderables.
 */

#include "Renderable.hpp"
#include "./dynamics/Particle.hpp"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class HierarchicalRenderable;
typedef std::shared_ptr<HierarchicalRenderable> HierarchicalRenderablePtr;

/**
 * @brief HierarchicalRenderable interface.
 *
 * A hierarchical renderable is used to describe objects made of several renderables.
 * The hierarchy is used to position objects relatively to others.

 * It becomes a powerful tool if the object is animated.
 * In this case, animating a node of the hierarchy will implicitly animate its children.
 * 
 * The hierarchy is described by specifying the parent and the children of the instance.
 * If the instance is the root of the hierarchy then it has no parent.
 * If the instance is a leaf of the hierarchy then it has no children.
 * 
 * A HierarchicalRenderable instance has two specific matrices:
 * - m_parentTransform that defines the transformation with respect to the parent.
 * The compositions of the parent transforms up to the root provides the local
 * frame of the instance, in world space. The result of this composition is named
 * the total parent transform.
 * - m_localTransform that defines an additional transformation, applied to the
 * local frame of the instance, in order to obtain the model matrix. This transformation
 * is not transmitted to the children of the instance. This allows to define a
 * scale for example, without needing to apply the reverse operation to all its
 * children.
 *
 * Only the root instance is meant to be added to the Viewer instance: that root
 * will take care itself to draw and animate all the hierarchy. However, if you want
 * to interact with all the hierarchy, you will have to propagate yourself the
 * interaction calls, such as do_keyPressedEvent() to the children. This is not
 * the default behavior as it could be easier to let the root the only instance
 * that deals with interaction.
 */
class HierarchicalRenderable : public Renderable
{
public :
    /**@brief Instance destructor.
     *
     * Since this class is meant to be derived (as it is abstract because we do
     * not give an implementation for Renderable::do_draw() for example), the
     * destructor should be either public and virtual or protected and nonvirtual.
     * We chose to make it public and virtual to let you delete concrete final
     * class instances with a pointer to a HierarchichalRenderable.
     */
    virtual ~HierarchicalRenderable();

    /**@brief Instance constructor.
     *
     * Initialize the local and parent transforms to identity, the model matrix to
     * identity and the \ref m_shaderProgram to \a shaderProgram.
     * @param shaderProgram Shader program to use to draw this.
     * \sa ShaderProgram
     */
    HierarchicalRenderable(ShaderProgramPtr shaderProgram);

    /** @brief Set hierarchical relationship between two HierarchicalRenderable instances.
     *
     * This function adds \a child to the m_children of \a parent and set the m_parent member
     * of \a child to \a parent.
     *
     * \param parent A pointer to the parent.
     * \param child A pointer to the child.
     *
     * Note: the children and the parents of an instance are stored in a shared pointer.
     * If we want to have a member function for this functionality, we would try to use
     * "this", either to add it to the children of an instance or to set it to be the
     * parent of another instance. However, "this" is not a shared pointer, and if we
     * try to create a shared pointer in the member function, that would mess completely
     * with the memory management done in a shared pointer.
     */
    static void addChild(HierarchicalRenderablePtr parent, HierarchicalRenderablePtr child);

    /** @brief Compute the model matrix of the instance.
     *
     * Since a model matrix is the transformation from the object coordinates to the world
     * coordinates, it should be computed thanks to the hierarchy and the
     * matrices \ref m_parentTransform. This computation is done in this function and should be
     * typically applied before drawing a hierarchical renderable. The result is stored
     * in \ref m_model.
     */
    void updateModelMatrix();

    /** @brief Compute the total parent transformation.
     *
     * This function computes recursively the total parent transformation until
     * it reaches the root of the hierarchy.
     *
     * \return The total parent transformation matrix.
     */
    glm::mat4 computeTotalParentTransform() const;

    /** \brief Read only access to the parent transformation.
     *
     * Allows to read the value of \ref m_parentTransform.
     *
     * \return A read only reference to \ref m_parentTransform, the parent transformation matrix.
     */
    const glm::mat4& getParentTransform() const;

    /** \brief Set the value of the parent transformation.
     *
     * Write to the \ref m_parentTransform matrix. Avoid to have non uniform scale
     * here: prefer to make it in the local transform. Otherwise, you will obtain
     * shearing.
     *
     * \param parentTransform The new parent transformation matrix.
     */
    void setParentTransform( const glm::mat4& parentTransform );

    /** \brief Read only access to the local transformation.
     *
     * Write to the \ref m_localTransform matrix.
     *
     * \return A read only reference to \ref m_localTransform, the local transformation matrix.
     */
    const glm::mat4& getLocalTransform() const;

    /** \brief Set the value of the local transformation.
     *
     * Write to the \ref m_localTransform matrix.
     *
     * \param localTransform The new local transformation matrix.
     */
    void setLocalTransform(const glm::mat4& localTransform);

    /**@brief Access to the children of this renderable.
     *
     * Get the children of this hierarchical renderable.
     * @return A vector of hierarchical renderable shared pointers. */
    std::vector< HierarchicalRenderablePtr > & getChildren();

    
    
private:

    /**@brief Pointer to the parent renderable.
     *
     * If it has no parent then the pointer value is nullptr.
     * This is the case if the instance is the root of the hierarchy.
     *
     * The instance is positioned relatively to the parent.
     */
    HierarchicalRenderablePtr m_parent;

    /**@brief List of the children of the instance.
     *
     * This list contains all the children of the instance. Those
     * children all have this as a parent. */
    std::vector< HierarchicalRenderablePtr > m_children;

    /**@brief Transformation with respect to the parent.
     *
     * This matrix gives the transformation of this instance relatively
     * to its parent. If it has no parent the the matrix is set to identity.
     * Keep in mind that this transformation will be used by the children.
     */
    glm::mat4 m_parentTransform;

    /**@brief Local transformation of this instance.
     *
     * This transformation is applied to the global parent transformation to give
     * the model matrix. It is particularly useful to deform the geometry of the
     * object using scaling. Keep in mind that this transformation will NOT be used
     * by the children.
     */
    glm::mat4 m_localTransform;

    /**\brief Perform computations before do_draw()
     */
    virtual void beforeDraw();

    /**
     * \brief Perform computations after do_draw()
     */
    virtual void afterDraw();

    /**
     * \brief Perform computations after do_animate()
     */
    virtual void afterAnimate( float time );



};
typedef std::shared_ptr<HierarchicalRenderable> HierarchicalRenderablePtr;

#endif
