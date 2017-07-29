#include "drawdisplay.h"

#include "glwt.h"

u32 screenTexture;

float tris[] = {
    -1.0f, -1.0f, 0.0f, 
     1.0f, 1.0f,  0.0f, 
    -1.0f, 1.0f, 0.0f,

    -1.0f, -1.0f, 0.0f, 
     1.0f, -1.0f, 0.0f, 
     1.0f,  1.0f, 0.0f
};

const char* vertex_shader =
"#version 400\n"
"in vec3 vp;"
"out vec2 uv;"
"void main () {"
"  uv = (vp.xy * 0.5) + 0.5;"
"  uv.y = 1.0 - uv.y;"
"  gl_Position = vec4(vp * 0.3, 1.0);"
"}";

const char* fragment_shader =
"#version 400\n"
"in vec2 uv;"
"uniform sampler2D T;"
"out vec4 frag_color;"
"void main () {"
"  frag_color = texture2D(T,uv);"
"}";
int g_Width, g_Height;

void* getscreentex() { return (void*)screenTexture; }

void display_init(int width, int height)
{
    g_Width = width;
    g_Height = height;

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    u32 vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), tris, GL_STATIC_DRAW);

    u32 vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    u32 vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);
    u32 fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    u32 program = glCreateProgram();
    glAttachShader(program, fs);
    glAttachShader(program, vs);
    glLinkProgram(program);

    glUseProgram(program);

    u32 loc = glGetUniformLocation(program, "T");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glBindVertexArray(vao);
}

void display_update(const u8* gpuScreen)
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g_Width, g_Height, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(gpuScreen));
}

void display_draw()
{
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
