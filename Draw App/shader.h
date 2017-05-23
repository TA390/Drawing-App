//
//  shader.h
//  Draw App
//
//  Created by TA on 22/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//

#ifndef shader_h
#define shader_h

#include <iostream>
#include <GL/glew.h>

enum ShaderCompileType { vertex, fragment, geometry, program };

class Shader
{
public:
    Shader(const GLchar* vertex_source = nullptr, const GLchar* fragment_source = nullptr, const GLchar* geometry_source = nullptr);
    ~Shader();
    void use();
    void compile(const GLchar* vertex_source, const GLchar* fragment_source, const GLchar* geometry_source = nullptr);
    void checkCompileErrors(GLuint object, const ShaderCompileType& type);
private:
    GLuint program_id;
    void compileShader(GLuint& reference, const GLchar* source, GLenum type);
    std::string type_to_str(const ShaderCompileType& type);
};

#endif
