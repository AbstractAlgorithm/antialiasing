#ifndef _core_shader_h_
#define _core_shader_h_

#include "core.h"
#include "file.h"

class Shader
{
private:
    GLuint m_shader;
    int m_type;

    friend class ShaderProgram;
public:
    Shader(int st);
    ~Shader();

    operator GLuint() const { return m_shader; }

    int sourceFile(const char* filename);
    int source(const char* source);
    char* log() const;
};


class ShaderProgram
{
private:
    GLuint m_program;
    int m_attachedCnt;
    GLuint* m_attached;
public:
    ShaderProgram();
    ~ShaderProgram();

    operator GLuint() const { return m_program; }

    ShaderProgram* add(Shader* s);
    int link();
    void use() const;
    char* log() const;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: Shader
////////////////////////////////////////////////////////////////////////////////

Shader::Shader(int st)
{
    m_shader = 0;
    m_type = st;
    m_shader = glCreateShader(m_type);
}

Shader::~Shader()
{
    glDeleteShader(m_shader);
}

int Shader::sourceFile(const char* filename)
{
    char* source_txt;
    if (!(source_txt = File::loadFile(filename)))
        return 0;
    return source(source_txt);
}

int Shader::source(const char* s)
{
    int status;
    glShaderSource(m_shader, 1, &s, NULL);
    glCompileShader(m_shader);
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &status);
    return status;
}

char* Shader::log() const
{
    int infologLen = 0;
    int charsWritten = 0;
    GLchar *infoLog = NULL;
    glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &infologLen);
    if (infologLen > 0)
    {
        infoLog = (GLchar*)malloc(infologLen);
        if (infoLog == NULL)
            return "ERROR: Could not retrieve shader log.";
        glGetShaderInfoLog(m_shader, infologLen, &charsWritten, infoLog);
    }
    return infoLog;
}
////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderProgram
////////////////////////////////////////////////////////////////////////////////

ShaderProgram::ShaderProgram()
{
    m_program = glCreateProgram();
    m_attachedCnt = 0;
    m_attached = NULL;
}

ShaderProgram::~ShaderProgram()
{
    for (int i = 0; i < m_attachedCnt; glDetachShader(*this, m_attached[i++]));
    glDeleteProgram(*this);
}

ShaderProgram* ShaderProgram::add(Shader* shader)
{
    glAttachShader(m_program, *shader);
    m_attachedCnt++;
    m_attached = (GLuint*)realloc(m_attached, sizeof(GLuint)*m_attachedCnt);
    m_attached[m_attachedCnt - 1] = *shader;
    return this;
}

int ShaderProgram::link()
{
    int status;
    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    return status;
}

void ShaderProgram::use() const
{
    glUseProgram(m_program);
}

char* ShaderProgram::log() const
{
    int infologLen = 0;
    int charsWritten = 0;
    GLchar *infoLog = NULL;
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infologLen);
    if (infologLen > 0)
    {
        infoLog = (GLchar*)malloc(infologLen);
        if (infoLog == NULL)
            return "ERROR: Could not write shader program log.";
        glGetProgramInfoLog(m_program, infologLen, &charsWritten, infoLog);
    }
    return infoLog;
}

#endif