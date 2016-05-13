#ifndef PLANE_HPP
#define PLANE_HPP

#include <glm/glm.hpp>
#include <memory>

/**@brief Geometry representation of an infinite plane.
 *
 * Represent an infinite plane.
 */
class Plane
{
public:
  /**@brief Build a plane with a normal and a point.
   *
   * Build a plane normal to the specified normal, passing by a particular
   * point.
   * @param normal The normal of the plane
   * @param point A point belonging to the plane.
   */
  Plane( const glm::vec3& normal,
         const glm::vec3& point );

  /**@brief Build the plane passing through three points.
   *
   * Build a plane containing three points.
   * @param a The first point belonging to the plane.
   * @param b The second point belonging to the plane.
   * @param c The third point belonging to the plane.
   */
  Plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);

  /**@brief Manually set the distance from this plane to the origin.
   *
   * Set the distance min_(p in this) || p - vec3(0,0,0) ||.
   * @param d New distance to the origin.
   */
  void setDistanceToOrigin(const float& d);
  /**@brief Access to the distance to the origin.
   *
   * Get the distance between this plane and the origin.
   * @return The distance to the origin.
   */
  const float& distanceToOrigin() const;

  /**@brief Set the plane's normal.
   *
   * Set the plane's normal.
   * @param n The new plane's normal.
   */
  void setNormal(const glm::vec3& n);
  /**@brief Access the plane's normal.
   *
   * Get the plane's normal.
   * @return The plane's normal.
   */
  const glm::vec3& normal() const;

  /**@brief Get the projection of a point on this plane.
   *
   * Get the orthogonal projection of a point on this plane.
   * @param p The point to project.
   * @return The orthogonal projection of p.
   */
  glm::vec3 projectOnPlane(const glm::vec3& p);

private:
    glm::vec3 m_n; /*!< Plane normal. Points x on the plane satisfy dot(m_n,x)=m_d */
    float m_d; /*!< m_d = dot(m_n,p) for a given point p on the plane */
};

typedef std::shared_ptr<Plane> PlanePtr;

#endif //PLANE_HPP
