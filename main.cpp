
#include "glwt.h"
#include "gb.h"
#include <assert.h>
#include <stdio.h>
#include "display.h"

#include "drawdisplay.h"
#include "imgui.h"
#include "imgui_impl_glwt.h"

const int ScreenMultiplier = 6;

#ifdef _WIN32
int CALLBACK WinMain(_In_  HINSTANCE hInstance, _In_  HINSTANCE hPrevInstance, _In_  LPSTR lpCmdLine, _In_  int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
	FILE* file = nullptr;
	//const char* filePath = "test_gb/01-special.gb";
	const char* filePath = "Tetris.gb";
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

	return glwt_init("GB Emulator", SCREEN_WIDTH * ScreenMultiplier, SCREEN_HEIGHT * ScreenMultiplier, false);
}

void glwt_setup()
{
    display_init(SCREEN_WIDTH, SCREEN_HEIGHT);
    ImGui_ImplGlfwGL3_Init();
}

bool show_test_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImColor(114, 144, 154);

//target 1/60 seconds... somehow.
void glwt_draw(float time)
{
    double endTime = glwt_getTime() + (1.0 / 2400.0);

    while (glwt_getTime() < endTime)
    {
        bool scanlineCompleted = false;
        while (!scanlineCompleted)
        {
            scanlineCompleted = GB_tick();
        }
    }

    ImGui_ImplGlfwGL3_NewFrame(SCREEN_WIDTH * ScreenMultiplier, SCREEN_HEIGHT * ScreenMultiplier);
    {
        //static float f = 0.0f;
        //ImGui::Text("Hello, world!");
        ImGui::Image(getscreentex(), ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        /*ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        if (ImGui::Button("Test Window")) show_test_window ^= 1;
        if (ImGui::Button("Another Window")) show_another_window ^= 1;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);*/
    }

    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    display_draw(GB_gpuscreen());
    ImGui::Render();
}
