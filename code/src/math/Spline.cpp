#include "./../../include/math/Spline.hpp"
#include "./../../include/gl_helper.hpp"

Spline::~Spline(){}

Spline::Spline(int iteration_level, float curve_tension)
    :   m_control_points{std::vector<glm::vec3>()},
        m_generated_points{std::vector<glm::vec3>()}
{
    if (iteration_level > 0) 
        m_iteration_level = iteration_level;

    if (curve_tension >= 0.0 && curve_tension <= 1.0)
        m_curve_tension = curve_tension;
}

Spline::Spline(
    std::vector<glm::vec3>& control_points, 
    int iteration_level,
    float curve_tension
)
    :   m_control_points{std::vector<glm::vec3>()},
        m_generated_points{std::vector<glm::vec3>()}
{
    if (iteration_level > 0)
        m_iteration_level = iteration_level;

    if (curve_tension >= 0.0 && curve_tension <= 1.0)
        m_curve_tension = curve_tension;

    for (
            auto iterator = control_points.cbegin(); 
            iterator != control_points.cend();
            iterator++
    ) {
        m_control_points.push_back(*iterator);
    }
}

std::vector<glm::vec3>& Spline::get_control_points()
{
    return m_control_points;
}

void Spline::add_control_point(glm::vec3 new_points)
{
    m_control_points.push_back(new_points);
}

int Spline::get_iteration_level()
{
    return m_iteration_level;
}

void Spline::set_iteration_level(int iteration_level)
{
    if (iteration_level > 0)
        m_iteration_level = iteration_level;
}

float Spline::get_curve_tension()
{
    return m_curve_tension;
}

void Spline::set_curve_tension(float curve_tension)
{
    if (curve_tension >= 0.0 && curve_tension <= 1.0)
        m_curve_tension = curve_tension;
}

glm::vec3 Spline::compute_spline_point(
    float a1, float a2, float c1, float c2,
    glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3
) 
{
    glm::vec3 t1 = m_curve_tension * (p2 - p0);
    glm::vec3 t2 = m_curve_tension * (p3 - p1);

    return (a1*p1 + a2*p2 + c1*t1 + c2*t2);
}

void Spline::generate_spline(bool is_curve_closed) 
{
    /*
     * If the curve is supposed to be closed, adding the first and the
     * second control points at the end of the control points list, so
     * as to generate the points for the first and last parts of the curve.
     */
    if (is_curve_closed) {
        m_control_points.push_back(m_control_points[0]);
        m_control_points.push_back(m_control_points[1]);
        m_control_points.push_back(m_control_points[2]);
    }

    /*
     * Pre-computing the coefficients used to generate the points of the
     * spline.
     */
    float** coefficients = new float* [m_iteration_level];
	for (int i = 0; i < m_iteration_level; i++) {
		coefficients[i] = new float[4];
	}

    for (int step = 0; step < m_iteration_level; step++) {
        /*
         * Computing the [0;1] subdivision, and its powers.
         */
        float u = (float)step / (float)m_iteration_level;
        float u2 = u*u;
        float u3 = u2*u;

        /*
         * Computing a temporary variable in order to make
         * the approximation coefficients' computation.
         */
        float tmp1 = 2*u3 - 3*u2;

        /*
         * Computing the approximation coefficients.
         */
        coefficients[step][0] = tmp1 + 1.0;
        coefficients[step][1] = -1.0*tmp1;
        coefficients[step][2] = u3 - 2*u2 + u;
        coefficients[step][3] = u3 - u2;
    }

    for (int count = 1; count < (m_control_points.size() - 2); count ++) {
        for (int step = 0; step < m_iteration_level; step++) {
            m_generated_points.push_back(
                compute_spline_point(
                    coefficients[step][0],
                    coefficients[step][1],
                    coefficients[step][2],
                    coefficients[step][3],
                    m_control_points[count - 1],
                    m_control_points[count],
                    m_control_points[count + 1],
                    m_control_points[count + 2]
                )
            );
        }
    }

	/*
	 * Deallocating the coefficients.
	 */
	for (int i = 0; i < m_iteration_level; i++) {
		delete[] coefficients[i];
	}
	delete[] coefficients;
}

std::vector<glm::vec3>& Spline::get_generated_points()
{
    return m_generated_points;
}
