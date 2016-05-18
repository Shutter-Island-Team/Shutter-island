#include "./../include/Utils.hpp"
#include "./../include/log.hpp"

#include <chrono>
#include <random>
#include <glm/gtx/color_space.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// A random engine initialized on the current time.
// This way, the random numbers generated won't be the same at each execution.
static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count() );

float random(float a, float b)
{
    uniform_real_distribution<> distribution(a,b);
    return distribution(generator);
}

glm::vec4 randomColor()
{
    uniform_real_distribution<> distribution(0,360);
    float hue = distribution(generator), saturation = 1.0, black = 1.0;
    glm::vec3 hsvColor( hue, saturation, black);
    glm::vec3 rgbColor = glm::rgbColor(hsvColor);
    return glm::vec4(rgbColor[0], rgbColor[1], rgbColor[2], 1.0);
}

glm::vec4 getColor(const float& factor, const float& low, const float& high)
{
    float hue = 0.0, saturation = 1.0, black = 1.0;
    if(factor<low) hue = 240.0;
    else if(factor>high) hue = 0.0;
    else hue = 240 + (factor-low)/(high-low)*(0.0-240.0);
    glm::vec3 hsvColor( hue, saturation, black);
    glm::vec3 rgbColor = glm::rgbColor(hsvColor);
    return glm::vec4(rgbColor[0], rgbColor[1], rgbColor[2], 1.0);
}

void getUnitPlane(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2> &texCoords)
{
    positions.resize(6, glm::vec3(0.0,0.0,0.0));
    texCoords.resize(6, glm::vec2(0.0,0.0));
    normals.resize(6, glm::vec3(0.0,0.0,1.0));

    //First triangle
    positions[0] =glm::vec3(-0.5,-0.5,0.0);
    positions[1] =glm::vec3(0.5,-0.5,0.0);
    positions[2] =glm::vec3(0.5,0.5,0.0);

    //TODO
    texCoords[0] =glm::vec2(0.0,0.0);
    texCoords[1] =glm::vec2(1.0,0.0);
    texCoords[2] =glm::vec2(1.0,1.0);

    //Second triangle
    positions[3] =glm::vec3(-0.5,-0.5,0.0);
    positions[4] =glm::vec3(0.5,0.5,0.0);
    positions[5] =glm::vec3(-0.5,0.5,0.0);

    texCoords[3] =glm::vec2(0.0,0.0);
    texCoords[4] =glm::vec2(1.0,1.0);
    texCoords[5] =glm::vec2(0.0,1.0);
}

void getUnitIndexedCube(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::ivec3>& indices)
{
    positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    for(const glm::vec3& x : positions)
    {
        normals.push_back( glm::normalize(x) );
    }

    indices.push_back(glm::ivec3(0, 2, 1));
    indices.push_back(glm::ivec3(0, 3, 2));
    indices.push_back(glm::ivec3(1, 6, 7));
    indices.push_back(glm::ivec3(1, 2, 6));
    indices.push_back(glm::ivec3(5, 4, 7));
    indices.push_back(glm::ivec3(5, 7, 6));
    indices.push_back(glm::ivec3(4, 3, 0));
    indices.push_back(glm::ivec3(4, 5, 3));
    indices.push_back(glm::ivec3(3, 6, 2));
    indices.push_back(glm::ivec3(3, 5, 6));
    indices.push_back(glm::ivec3(4, 0, 1));
    indices.push_back(glm::ivec3(4, 1, 7));
}

void getUnitCube(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords)
{
    //Face 1
    positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(0.5, -0.5, -0.5));

    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));

    normals.push_back(glm::vec3(0.0,0.0,-1.0));
    normals.push_back(glm::vec3(0.0,0.0,-1.0));
    normals.push_back(glm::vec3(0.0,0.0,-1.0));

    //Face 2
    positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, -0.5));

    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,0.0));

    normals.push_back(glm::vec3(0.0,0.0,-1.0));
    normals.push_back(glm::vec3(0.0,0.0,-1.0));
    normals.push_back(glm::vec3(0.0,0.0,-1.0));

    //Face 3
    positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));
    texCoords.push_back(glm::vec2(0.0,1.0));

    normals.push_back(glm::vec3(1.0,0.0,0.0));
    normals.push_back(glm::vec3(1.0,0.0,0.0));
    normals.push_back(glm::vec3(1.0,0.0,0.0));

    //Face 4
    positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, 0.5));

    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));

    normals.push_back(glm::vec3(1.0,0.0,0.0));
    normals.push_back(glm::vec3(1.0,0.0,0.0));
    normals.push_back(glm::vec3(1.0,0.0,0.0));

    //Face 5
    positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,0.0));

    normals.push_back(glm::vec3(0.0,0.0,1.0));
    normals.push_back(glm::vec3(0.0,0.0,1.0));
    normals.push_back(glm::vec3(0.0,0.0,1.0));

    //Face 6
    positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(0.5, 0.5, 0.5));

    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));

    normals.push_back(glm::vec3(0.0,0.0,1.0));
    normals.push_back(glm::vec3(0.0,0.0,1.0));
    normals.push_back(glm::vec3(0.0,0.0,1.0));

    //Face 7
    positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(-0.5, -0.5, -0.5));

    texCoords.push_back(glm::vec2(1.0,1.0));
    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,0.0));

    normals.push_back(glm::vec3(-1.0,0.0,0.0));
    normals.push_back(glm::vec3(-1.0,0.0,0.0));
    normals.push_back(glm::vec3(-1.0,0.0,0.0));

    //Face 8
    positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, -0.5));

    texCoords.push_back(glm::vec2(1.0,1.0));
    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(0.0,0.0));

    normals.push_back(glm::vec3(-1.0,0.0,0.0));
    normals.push_back(glm::vec3(-1.0,0.0,0.0));
    normals.push_back(glm::vec3(-1.0,0.0,0.0));

    //Face 9
    positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(0.5, 0.5, -0.5));

    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(0.0,0.0));

    normals.push_back(glm::vec3(0.0,1.0,0.0));
    normals.push_back(glm::vec3(0.0,1.0,0.0));
    normals.push_back(glm::vec3(0.0,1.0,0.0));

    //Face 10
    positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(0.5, 0.5, 0.5));

    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));
    texCoords.push_back(glm::vec2(0.0,1.0));

    normals.push_back(glm::vec3(0.0,1.0,0.0));
    normals.push_back(glm::vec3(0.0,1.0,0.0));
    normals.push_back(glm::vec3(0.0,1.0,0.0));

    //Face 11
    positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, -0.5, -0.5));

    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,0.0));

    normals.push_back(glm::vec3(0.0,-1.0,0.0));
    normals.push_back(glm::vec3(0.0,-1.0,0.0));
    normals.push_back(glm::vec3(0.0,-1.0,0.0));

    //Face 12
    positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));

    normals.push_back(glm::vec3(0.0,-1.0,0.0));
    normals.push_back(glm::vec3(0.0,-1.0,0.0));
    normals.push_back(glm::vec3(0.0,-1.0,0.0));
}

void getUnitCylinder(std::vector<glm::vec3> &positions, std::vector<glm::vec3> &normals, const unsigned int &slices)
{
    size_t number_of_triangles = slices * 4;
    size_t number_of_vertices = number_of_triangles * 3;
    float angle_step = 2.0 * M_PI / double( slices );

    positions.resize(number_of_vertices, glm::vec3(0.0,0.0,0.0));
    normals.resize(number_of_vertices, glm::vec3(0.0,0.0,0.0));

    float previous_angle = (slices - 1) * angle_step;
    float angle = 0;
    float previous_sin = std::sin( previous_angle );
    float previous_cos = std::cos( previous_angle );
    float sin = 0;
    float cos = 1;
    for( size_t i = 0; i < slices;
            ++ i,
            previous_sin = sin, previous_cos = cos,
            angle += angle_step, cos = std::cos( angle ), sin = std::sin( angle) )
    {
        size_t voffset = 12 * i;
        positions[ voffset +  0 ] = glm::vec3{0,0,1};
        positions[ voffset +  1 ] = glm::vec3{previous_cos, previous_sin, 1};
        positions[ voffset +  2 ] = glm::vec3{cos, sin,1};

        positions[ voffset +  3 ] = glm::vec3{cos, sin,1};
        positions[ voffset +  4 ] = glm::vec3{previous_cos, previous_sin, 1};
        positions[ voffset +  5 ] = glm::vec3{previous_cos, previous_sin, 0};

        positions[ voffset +  6 ] = glm::vec3{cos, sin,1};
        positions[ voffset +  7 ] = glm::vec3{previous_cos, previous_sin, 0};
        positions[ voffset +  8 ] = glm::vec3{cos, sin,0};

        positions[ voffset +  9 ] = glm::vec3{cos, sin,0};
        positions[ voffset + 10 ] = glm::vec3{previous_cos, previous_sin, 0};
        positions[ voffset + 11 ] = glm::vec3{0, 0, 0}; //useless

        normals[ voffset +  0 ] = glm::vec3{0,0,1};
        normals[ voffset +  1 ] = glm::vec3{0,0,1};
        normals[ voffset +  2 ] = glm::vec3{0,0,1};

        normals[ voffset +  3 ] = glm::vec3{cos,sin,0};
        normals[ voffset +  4 ] = glm::vec3{previous_cos,previous_sin,0};
        normals[ voffset +  5 ] = glm::vec3{previous_cos,previous_sin,0};

        normals[ voffset +  6 ] = glm::vec3{cos,sin,0};
        normals[ voffset +  7 ] = glm::vec3{previous_cos,previous_sin,0};
        normals[ voffset +  8 ] = glm::vec3{cos,sin,0};

        normals[ voffset +  9 ] = glm::vec3{0,0,-1};
        normals[ voffset + 10 ] = glm::vec3{0,0,-1};
        normals[ voffset + 11 ] = glm::vec3{0,0,-1};
    }
}

void getUnitSphere(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, const unsigned int& strips, const unsigned int& slices)
{
    double radius=1.0;
    glm::vec3 center(0.0,0.0,0.0);

    unsigned int vertexNumber = slices*strips*6;
    positions.resize(vertexNumber);
    normals.resize(vertexNumber);

    for(int i=0; i<slices; ++i)
    {
        for(int j=0; j<strips; ++j)
        {
            double curr_theta = i*(2.0*M_PI/(double)slices);
            double curr_phi = j*(M_PI/(double)strips);

            double next_theta = (i+1)*(2.0*M_PI/(double)slices);
            double next_phi = (j+1)*(M_PI/(double)strips);

            std::array<glm::vec3,3> vTriangles;
            vTriangles[0] = center + glm::vec3(radius*cos(curr_theta)*sin(curr_phi), radius*sin(curr_theta)*sin(curr_phi), radius*cos(curr_phi));
            vTriangles[1] = center + glm::vec3(radius*cos(next_theta)*sin(curr_phi), radius*sin(next_theta)*sin(curr_phi), radius*cos(curr_phi));
            vTriangles[2] = center + glm::vec3(radius*cos(next_theta)*sin(next_phi), radius*sin(next_theta)*sin(next_phi), radius*cos(next_phi));

            positions[i*strips*6 + 6*j +0] = vTriangles[0];
            positions[i*strips*6 + 6*j +1] = vTriangles[1];
            positions[i*strips*6 + 6*j +2] = vTriangles[2];

            normals[i*strips*6 + 6*j +0] = glm::vec3(cos(curr_theta)*sin(curr_phi), sin(curr_theta)*sin(curr_phi), cos(curr_phi));
            normals[i*strips*6 + 6*j +1] = glm::vec3(cos(next_theta)*sin(curr_phi), sin(next_theta)*sin(curr_phi), cos(curr_phi));
            normals[i*strips*6 + 6*j +2] = glm::vec3(cos(next_theta)*sin(next_phi), sin(next_theta)*sin(next_phi), cos(next_phi));

            vTriangles[0] = center + glm::vec3(radius*cos(curr_theta)*sin(curr_phi), radius*sin(curr_theta)*sin(curr_phi), radius*cos(curr_phi));
            vTriangles[1] = center + glm::vec3(radius*cos(next_theta)*sin(next_phi), radius*sin(next_theta)*sin(next_phi), radius*cos(next_phi));
            vTriangles[2] = center + glm::vec3(radius*cos(curr_theta)*sin(next_phi), radius*sin(curr_theta)*sin(next_phi), radius*cos(next_phi));

            positions[i*strips*6 + 6*j +3] = vTriangles[0];
            positions[i*strips*6 + 6*j +4] = vTriangles[1];
            positions[i*strips*6 + 6*j +5] = vTriangles[2];

            normals[i*strips*6 + 6*j +3] = glm::vec3(cos(curr_theta)*sin(curr_phi), sin(curr_theta)*sin(curr_phi), cos(curr_phi));
            normals[i*strips*6 + 6*j +4] = glm::vec3(cos(next_theta)*sin(next_phi), sin(next_theta)*sin(next_phi), cos(next_phi));
            normals[i*strips*6 + 6*j +5] = glm::vec3(cos(curr_theta)*sin(next_phi), sin(curr_theta)*sin(next_phi), cos(next_phi));
        }
    }
}

void getTorus(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, const float& a, const float& c, const unsigned int& strips, const unsigned int& slices)
{
    unsigned int vertexNumber = slices*strips*6;
    positions.resize(vertexNumber);
    normals.resize(vertexNumber);

    for(int i=0; i<strips; ++i)
    {
        for(int j=0; j<slices; ++j)
        {
            double curr_u = i*(2.0*M_PI/(double)strips);
            double curr_v = j*(2.0*M_PI/(double)slices);

            double next_u = (i+1)*(2.0*M_PI/(double)strips);
            double next_v = (j+1)*(2.0*M_PI/(double)slices);

            positions[i*strips*6 + 6*j +0] = glm::vec3((c+a*cos(curr_v))*cos(curr_u),
                    (c+a*cos(curr_v))*sin(curr_u),
                    a*sin(curr_v) );
            positions[i*strips*6 + 6*j +1] = glm::vec3((c+a*cos(curr_v))*cos(next_u),
                    (c+a*cos(curr_v))*sin(next_u),
                    a*sin(curr_v) );
            positions[i*strips*6 + 6*j +2] = glm::vec3((c+a*cos(next_v))*cos(next_u),
                    (c+a*cos(next_v))*sin(next_u),
                    a*sin(next_v) );

            normals[i*strips*6 + 6*j +0] = glm::vec3((cos(curr_v))*cos(curr_u),
                    (cos(curr_v))*sin(curr_u),
                    sin(curr_v) );
            normals[i*strips*6 + 6*j +1] = glm::vec3((cos(curr_v))*cos(next_u),
                    (cos(curr_v))*sin(next_u),
                    sin(curr_v) );
            normals[i*strips*6 + 6*j +2] = glm::vec3((cos(next_v))*cos(next_u),
                    (cos(next_v))*sin(next_u),
                    sin(next_v) );

            positions[i*strips*6 + 6*j +3] = glm::vec3((c+a*cos(curr_v))*cos(curr_u),
                    (c+a*cos(curr_v))*sin(curr_u),
                    a*sin(curr_v) );
            positions[i*strips*6 + 6*j +4] = glm::vec3((c+a*cos(next_v))*cos(next_u),
                    (c+a*cos(next_v))*sin(next_u),
                    a*sin(next_v) );
            positions[i*strips*6 + 6*j +5] = glm::vec3((c+a*cos(next_v))*cos(curr_u),
                    (c+a*cos(next_v))*sin(curr_u),
                    a*sin(next_v) );

            normals[i*strips*6 + 6*j +3] = glm::vec3((cos(curr_v))*cos(curr_u),
                    (cos(curr_v))*sin(curr_u),
                    sin(curr_v) );
            normals[i*strips*6 + 6*j +4] = glm::vec3((cos(next_v))*cos(next_u),
                    (cos(next_v))*sin(next_u),
                    sin(next_v) );
            normals[i*strips*6 + 6*j +5] = glm::vec3((cos(next_v))*cos(curr_u),
                    (cos(next_v))*sin(curr_u),
                    sin(next_v) );
        }
    }
}

void getUnitCone(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, const unsigned int& strips, const unsigned int& slices)
{
    double height = 1.0;
    double radius=1.0;
    glm::vec3 center(0.0,0.0,0.0);
    int numberOfVertices = slices*strips*6 + slices*3;
    positions.resize(numberOfVertices);
    normals.resize(numberOfVertices);

    //Lateral surface
    for(int i=0; i<slices; ++i)
    {
        for(int j=0; j<strips; ++j)
        {
            double curr_theta = i*(2.0*M_PI/(double)slices);
            double curr_u = j*(height/(double)strips);
            double next_theta = (i+1)*(2.0*M_PI/(double)slices);
            double next_u = (j+1)*(height/(double)strips);
            double factor1 = (height-curr_u)/height;
            double factor2 = (height-next_u)/height;

            positions[i*strips*6 + 6*j +0] = center + glm::vec3(factor1*radius*cos(curr_theta),
                    factor1*radius*sin(curr_theta),
                    curr_u);
            positions[i*strips*6 + 6*j +1] = center + glm::vec3(factor1*radius*cos(next_theta),
                    factor1*radius*sin(next_theta),
                    curr_u);
            positions[i*strips*6 + 6*j +2] = center + glm::vec3(factor2*radius*cos(next_theta),
                    factor2*radius*sin(next_theta),
                    next_u);

            normals[i*strips*6 + 6*j +0] = glm::normalize(glm::vec3(cos(curr_theta),
                        sin(curr_theta),
                        curr_u));
            normals[i*strips*6 + 6*j +1] = glm::normalize(glm::vec3(cos(next_theta),
                        sin(next_theta),
                        curr_u));
            normals[i*strips*6 + 6*j +2] = glm::normalize(glm::vec3(cos(next_theta),
                        sin(next_theta),
                        next_u));

            positions[i*strips*6 + 6*j +3] = center + glm::vec3(factor1*radius*cos(curr_theta),
                    factor1*radius*sin(curr_theta),
                    curr_u);
            positions[i*strips*6 + 6*j +4] = center + glm::vec3(factor2*radius*cos(next_theta),
                    factor2*radius*sin(next_theta),
                    next_u);
            positions[i*strips*6 + 6*j +5] = center + glm::vec3(factor2*radius*cos(curr_theta),
                    factor2*radius*sin(curr_theta),
                    next_u);

            normals[i*strips*6 + 6*j +3] = glm::normalize(glm::vec3(cos(curr_theta),
                        sin(curr_theta),
                        curr_u));
            normals[i*strips*6 + 6*j +4] = glm::normalize(glm::vec3(cos(next_theta),
                        radius*sin(next_theta),
                        next_u));
            normals[i*strips*6 + 6*j +5] = glm::normalize(glm::vec3(cos(curr_theta),
                        radius*sin(curr_theta),
                        next_u));

        }
    }

    //Bottom faces
    unsigned int counter = slices*strips*6;
    for(int i=0; i<slices; ++i)
    {
        double curr_theta = i*(2.0*M_PI/(double)slices);
        double next_theta = (i+1)*(2.0*M_PI/(double)slices);
        double u_base = 0.0;

        positions[counter+3*i+0] = center + glm::vec3(0, 0, u_base );
        positions[counter+3*i+1] = center + glm::vec3(radius*cos(curr_theta), radius*sin(curr_theta), u_base);
        positions[counter+3*i+2] = center + glm::vec3(radius*cos(next_theta), radius*sin(next_theta), u_base);

        normals[counter+3*i+0] = glm::vec3(0, 0, -1);
        normals[counter+3*i+1] = glm::vec3(0, 0, -1);
        normals[counter+3*i+2] = glm::vec3(0, 0, -1);
    }

}
