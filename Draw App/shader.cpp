//
//  shader.cpp
//  Draw App
//
//  Created by TA on 22/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//

#include "shader.h"
#include <fstream>
#include <sstream>
#include <unistd.h>

Shader::Shader(const GLchar* vertex_source, const GLchar* fragment_source, const GLchar* geometry_source)
: program_id{0}
{
    if(vertex_source != nullptr && fragment_source != nullptr) {
        compile(vertex_source, fragment_source, geometry_source);
    }
}

Shader::~Shader()
{
    glDeleteProgram(program_id);
}

void Shader::use()
{
    glUseProgram(program_id);
}

void Shader::compile(const GLchar* vertex_source, const GLchar* fragment_source, const GLchar* geometry_source)
{
    
    GLuint vertex_shader, fragment_shader, geometry_shader;
    
    // Create and compile each shader, check for compile errors
    compileShader(vertex_shader, vertex_source, GL_VERTEX_SHADER);
    checkCompileErrors(vertex_shader, ShaderCompileType::vertex);
    compileShader(fragment_shader, fragment_source, GL_FRAGMENT_SHADER);
    checkCompileErrors(fragment_shader, ShaderCompileType::fragment);
    if(geometry_source != nullptr){
        compileShader(geometry_shader, geometry_source, GL_GEOMETRY_SHADER);
        checkCompileErrors(geometry_shader, ShaderCompileType::geometry);
    }
    
    // Create a program and attach each shader
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    if(geometry_source != nullptr) {
        glAttachShader(program_id, fragment_shader);
    }
    
    // Link the program, check for link errors
    glLinkProgram(program_id);
    checkCompileErrors(program_id, ShaderCompileType::program);
    
    // Delete the shader as they are now linked to the program.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    if(geometry_source != nullptr){
        glDeleteShader(geometry_shader);
    }
    
}

void Shader::compileShader(GLuint& shader_object, const GLchar* source, GLenum type)
{
    // Create a shader object for 'type'
    shader_object = glCreateShader(type);
    // Attach the source code to the created object
    glShaderSource(shader_object, 1, &source, nullptr);
    // Compile the shader object
    glCompileShader(shader_object);
}

void Shader::checkCompileErrors(GLuint object, const ShaderCompileType& type) {
    
    GLint success;
    GLchar infoLog[1024];
    std::string error_message{};
    
    if(type == program) {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            error_message = "Shader Program Link Error.";
        }
    } else {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            error_message = "Shader Compile Error For Type: " + type_to_str(type);
        }
    }
    
    if(!error_message.empty()) {
        throw std::runtime_error(error_message);
    }

}

std::string Shader::type_to_str(const ShaderCompileType& type)
{
    switch(type){
        case vertex: return "vertex";
        case fragment: return "fragment";
        case geometry: return "geometry";
        case program: return "program";
        default: return "unknown type";
    }
}
