
#include "glwt.h"
#include "gb.h"
#include <assert.h>
#include <stdio.h>
#include "display.h"

u32 screenTexture;

float tris[] = {
	-1.0f,  4.0f,  0.0f,
	4.0f, -1.0f,  0.0f,
	-1.0f, -1.0f,  0.0f
};

const char* vertex_shader =
"#version 400\n"
"in vec3 vp;"
"out vec2 uv;"
"void main () {"
"  uv = (vp.xy * 0.5) + 0.5;"
"  uv.y = 1.0 - uv.y;"
"  gl_Position = vec4(vp, 1.0);"
"}";

const char* fragment_shader =
"#version 400\n"
"in vec2 uv;"
"uniform sampler2D T;"
"out vec4 frag_color;"
"void main () {"
"  frag_color = texture2D(T, uv);"
"}";

void glwt_setup()
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	u32 vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), tris, GL_STATIC_DRAW);

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

float acc = 0.0f;

void glwt_draw(float time)
{
	bool scanlineCompleted = false;
	while (!scanlineCompleted)
	{
		scanlineCompleted = GB_tick();
	}

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(GB_gpuscreen()));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

#ifdef _WIN32
int CALLBACK WinMain(_In_  HINSTANCE hInstance, _In_  HINSTANCE hPrevInstance, _In_  LPSTR lpCmdLine, _In_  int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
	FILE* file = nullptr;
	const char* filePath = "test_gb/01-special.gb";//"Tetris.gb";
#ifdef _WIN32
	fopen_s(&file, filePath, "rb");
#else
    file = fopen(filePath, "rb");
#endif
    
	if (file == nullptr)
		return 1;
	fseek(file, 0, SEEK_END);
	size_t len = ftell(file);

	u8* rom = new u8[len];
	fseek(file, 0, SEEK_SET);
	fread(rom, 1, len, file);
	fclose(file);

	GB_load(rom, len);
	GB_gpuinit();

	return glwt_init("GB", SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, false);
}

