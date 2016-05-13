# ifndef SHADER_HPP
# define SHADER_HPP

/**@file
 * @brief Define a shader program.
 *
 * This file define a ShaderProgram, which can be understood as a program
 * executed on the GPU to perform the rendering of a Renderable.
 */

# include <string>
# include <memory>
# include <unordered_map>
# include <list>

/**@brief Assembly of the graphics pipeline programmable steps.
 *
 * A shader program is the assembly of the graphics pipeline programmable steps.
 * In particular, for our practicals, it will hold the result of the compilation
 * and the linking of the vertex shader step and the fragment shader step.
 *
 * We decided to keep this class as simple as possible for these practicals.
 * However, we chose to add a nice feature: instrospection.
 * Simply put, this class examines the shader program to find the location of
 * vertex attributes and uniforms. This way, we do not have to ask the GPU what
 * is the location (if found) of a particular variable name (attribute or uniform).
 * Instead, an instance of the shader program will look into its known variables
 * for a match. This results into a faster rendering as we do not have to send
 * our queries at each from to the GPU to know those locations (remember, the
 * bus between the CPU and the GPU is quite slow, better not it efficiently).
 */
class ShaderProgram{
public:


    typedef int identifier;
    static const identifier null_identifier;

    /**@brief Construct a null shader program.
     *
     * Null shader program constructor. Perfectly valid shader program, but does
     * nothing.
     */
    ShaderProgram();

    /**@brief Construct a shader program from specified file names.
     *
     * Shader program constructor from GLSL shader files. In the context of this
     * course, we consider only the vertex shader and the fragment shader.
     * However, there exist other programmable stage in the pipeline (see
     * www.opengl.org/sdk/docs/man/html/glCreateShader.xhtml).
     *
     * If the shaders are invalid or describe an invalid program, this is
     * initialized to the null shader program.
     *
     * @param vertex_file_path Path to the vertex shader file
     * @param fragment_file_path Path to the fragment shader file.
     */
    // MODIFIED
    //ShaderProgram(const std::string& vertex_file_path, const std::string& fragment_file_path );
    ShaderProgram(const std::list< std::string >& shader_sources);

    /** @brief Destruction
     *
     * Instance destruction.
     */
    ~ShaderProgram();

    /**@brief Load new shaders from source files.
     *
     * Load shaders into this shader program. If the shaders are valid
     * (compilation stage) and they describe a valid program (linking stage),
     * this shader program would be valid. Otherwise, this remains unchanged.
     *
     * @param vertex_file_path Path to the vertex shader file
     * @param fragment_file_path Path to the fragment shader file.
     */

    void load(const std::list< std::string >& shader_sources);
    /** @brief Reload the shader sources
     *
     * Reload existing shader sources into this shader program. This is useful
     * if you decide to modify the shader sources while you execute the binary.
     * This way, you can check, improve, debug shaders and see the results
     * immediately on the screen.
     *
     * \sa Viewer::reloadShaderPrograms()
     */
    void reload();

    /**
     * Bind this program to the GPU. This is necessary to render objects or to
     * send uniforms/attributes values.
     */
    void bind();

    /** @brief Unbind any shader program.
     *
     * After this call, the next drawing command will not use any shader program
     * for the rendering, unless another shader program is binded.
     */
    static void unbind();

    /**@brief Get the location of an uniform thanks to its name.
     *
     * Return the location of a uniform thanks to its name. We use the locations
     * obtained by an introspection done after the linking stage. As such, we do
     * not request the GPU to answer this request: we have the answer on the CPU,
     * which would be faster.
     * @param name The uniform name, as it appear in the shader sources (be aware of the
     * naming convention for blocks and arrays, see
     * https://www.opengl.org/wiki/Program_Introspection#Naming)
     * @return The uniform location, null_location if there is no uniform with such name in this program
     */
    int getUniformLocation( const std::string& name ) const;

    /**@brief Get the location of an attribute thanks to its name.
     *
     * Return the location of an attribute (a program input seen in the vertex
     * shader) thanks to its name. We use the locations obtained by an introspection done
     * after the linking stage. As such, we do not request the GPU to answer this
     * request: we have the answer on the CPU, which would be faster.
     * @param name The attribute name as it appear in the vertex shader source
     * @return The attribute location, null_location if there is no attribute with such name in this program
     */
    int getAttributeLocation( const std::string& name ) const;


    /**@brief Get the identifier of this shader program.
     *
     * Get the identifier of this shader program on the GPU.
     * @return The program ID. */
    unsigned int programId();
    // Same as previous
    operator unsigned int() const;
    /**@brief Special value to represent a null location.
     *
     * Sometimes, you can ask for a uniform or an attribute that does not exist in
     * the shader program with getUniformLocation() and getAttributeLocation().
     * As the variable does not exist, the location returned by those function
     * should not point to any existing variable. \c null_location is a special
     * location value that is guaranteed to never point to an existing variable.
     *
     * When a null location is returned, this is not always an error. Indeed, when
     * a uniform or an attribute is not used in the shader sources (the compiler can
     * be pretty smart), it is optimized out. Thus, there is no valid location for
     * this variable. It is up to you to detect if the returned value is a null_location
     * and to decide what to do (throw an error or continue happily).
     */
    static int null_location; 

private:

    void ressources_introspection();
    std::string getExtension(const std::string &s);

    unsigned int m_programId;
    
    std::unordered_map< std::string, identifier > m_uniforms;
    std::unordered_map< std::string, int > m_attributes;
 
    std::list< std::string > m_source_filenames;
    bool m_loaded;

};

typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr; /*!< Typedef for a smart pointer of ShaderProgram */

#endif
