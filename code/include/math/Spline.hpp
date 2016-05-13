#ifndef SPLINE_HPP
#define SPLINE_HPP

/**@file
 * @brief Contains the implementation of the class Spline.
 *
 * The class Spline encapsulates the eponymous mathematical object,
 * or, more precisely, of a Cardinal Spline.
 */
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#define DEFAULT_ITERATION_LEVEL 5
#define DEFAULT_CURVE_TENSION 0.5

/**
 * @brief Spline class.
 *
 * A spline is a mathematical object, a continuous curve defined thanks to
 * control points and their tangents.
 * Moreover, splines can be concatenated continuously, so as to define more
 * complex curves.
 * The class Spline stores a vector containing a list of control points, to
 * be specified by the user, and generate the curve (that is to say the points
 * it is constitutied by), then stores it.
 * Quite obviously, thanks to the attributes of this class, the parameters
 * of the encapsulated spline can be set (namely the number of subdivisions
 * of each sub-spline, and the curve tension).
 */
class Spline
{
    public:

        /** 
         * @name Constructors and Destructor of the Spline class.
         */
        /** 
         * @brief Default constructor.
         * 
         * Minimalist constructor which sets the values of the 
         * iteration level and the curve tension. 
         * This constructor uses default values defined in the 
         * header of this file if no values are provided, or 
         * if the specified ones are incorrect. 
         */
        Spline(
            int iteration_level = DEFAULT_ITERATION_LEVEL,
            float curve_tension = DEFAULT_CURVE_TENSION
        );

        /** 
         * @brief Extended constructor.
         *
         * Does the same initialisations as the minimalist constructor,
         * but a vector containing a list of control points can be provided
         * in order to initialised the vector of control points stored into
         * the instance of the class.
         */
        Spline(
            std::vector<glm::vec3>& control_points, 
            int iteration_level = DEFAULT_ITERATION_LEVEL,
            float curve_tension = DEFAULT_CURVE_TENSION
        );

        /** 
         * @brief Standard destructor.
         *
         * A default destructor, nothing special to say since no memory
         * management is done within this class.
         */
        ~Spline();

        /** 
         * @name Public interface.
         */
        /** 
         * @brief Getter onto the vector of control points.
         */ 
        std::vector<glm::vec3>& get_control_points();

        /** 
         * @brief Gives the user an interface onto the vector of control
         * points, so that he may add control points.
         */
        void add_control_point(glm::vec3 new_point);

        /** 
         * @brief Getter onto the level of iteration.
         */ 
        int get_iteration_level();

        /** 
         * @brief Setter onto the level of iteration.
         */
        void set_iteration_level(int iteration_level);

        /** 
         * @brief Getter onto the curve tension.
         */ 
        float get_curve_tension();

        /** 
         * @brief Setter onto the curve tension.
         */
        void set_curve_tension(float curve_tension);

        /** 
         * @brief Generates the splines points, and stores them into
         * the m_generated_points attribute.
         *
         * @param is_curve_closed A boolean used to specify if the curve
         * has to be closed. If so, the first three control points are inserted
         * at the end of the vector of control points so as to draw the first
         * and last spline parts.
         * This parameter is set by default at true.
         */
        void generate_spline(bool is_curve_closed = true);

        /** 
         * @brief Getter onto the vector of generated points.
         */ 
        std::vector<glm::vec3>& get_generated_points();

    private:

        /** 
         * @name Private interface.
         */
        /** 
         * @brief The level of iteration defines the number of points 
         * contained into the subdivision of a spline part.
         */
        int m_iteration_level;

        /** 
         * @brief The curve tension parameter, must be contained in [0; 1].
         */
        float m_curve_tension;

        /** 
         * @brief Contains the control points associated with the encapsulated
         * spline.
         */
        std::vector<glm::vec3> m_control_points;

        /** 
         * @brief Contains the generated points computed thanks to the method
         * generate_spline. If the latter has not been called by the user,
         * then this vector is empty.
         */
        std::vector<glm::vec3> m_generated_points;

        /** 
         * @brief Given all the parameters required, this method computes
         * a point of the generated spline.
         * This is an intern method which simply aims at making the implementation
         * clearer. Thus, it is a private method.
         */
        glm::vec3 compute_spline_point(
            float a1, float a2, float c1, float c2,
            glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3
        ); 
};

/** 
 * @brief A typedef using smart pointers so as to provided an easy way to
 * declare a pointer on a Spline.
 */
typedef std::shared_ptr<Spline> SplinePtr;

#endif
