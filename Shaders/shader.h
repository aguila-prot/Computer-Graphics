#include "point.h"
#include <string>
#include <vector>
#include <fstream>
class Shader {
public:
    GLuint ident;
    static GLuint createShader(std::string vertexShaderFilename, std::string fragmentShaderFilename);
    static std::string readFile(std::string filename);
    void use();
    void unuse();
    Shader(GLuint ident);
    Shader();
};