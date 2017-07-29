
#include "glwt.h"
#include "gb.h"
#include <assert.h>
#include <stdio.h>
#include "display.h"

#include "imgui.h"
#include "imgui_impl_glwt.h"

#include "memory.h"
#include <vector>

const int ScreenMultiplier = 6;
GLuint screenTexture = 0;

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

void imgui_stylesetup()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.Alpha = 1.0f;
    style.FrameRounding = 3.0f;
    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
    style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
    style.Colors[ImGuiCol_ComboBg] = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.50f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
    style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

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

    ImGui_ImplGlfwGL3_Init();
    imgui_stylesetup();
}

u16 g_breakpoint;

void imgui_draw()
{
    ImGui::SetNextWindowPos(ImVec2(-8, 31));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x - 8, ImGui::GetIO().DisplaySize.y - 23));
    ImGui::Begin("MainWindow", NULL, ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus);
    {
        ImGui::Image((ImTextureID)screenTexture, ImVec2(ImGui::GetIO().DisplaySize.x - 8, ImGui::GetIO().DisplaySize.y - 23));
    }
    ImGui::End();

    ImGui::SetNextWindowSizeConstraints(ImVec2(100.f, 100.f), ImVec2(800.f, 800.f));
    ImGui::Begin("RAM");
    {
        ImGui::BeginChild("##scrolling", ImVec2(0.f, -ImGui::GetItemsLineHeightWithSpacing()));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        float glyphWidth = ImGui::CalcTextSize("FFFF ").x;
        float windowSize = ImGui::GetWindowSize().x * 0.7f - glyphWidth;
        int locsPerLine = max(1, (int)(windowSize / glyphWidth)) * 2;

        int totalLineCount = (int)0xFFFF / locsPerLine;
        ImGuiListClipper clipper(totalLineCount, ImGui::GetTextLineHeight());
        {
            u16 memStart = clipper.DisplayStart * locsPerLine;
            u16 memEnd = clipper.DisplayEnd * locsPerLine;
            for (u16 loc = memStart; loc < memEnd; loc += locsPerLine)
            {
                ImGui::Text("%04X:", loc);
                for (u16 i = 0; i < locsPerLine/2; i++)
                {
                    ImGui::SameLine();
                    ImGui::Text("%04X ", readMemory16(loc + i*2));
                }

                for (u8 i = 0; i < locsPerLine; i++)
                {
                    ImGui::SameLine();
                    u8 c = readMemory(loc + i);
                    ImGui::Text("%c", (c >= 32 && c < 128) ? c : '.');
                }
            }
        }
        clipper.End();
        ImGui::PopStyleVar(2);
        ImGui::EndChild();
    }
    ImGui::End();

    ImGui::SetNextWindowSizeConstraints(ImVec2(100.f, 100.f), ImVec2(800.f, 800.f));
    ImGui::Begin("Disassembly");
    {
        bool scrollToDisasm = false;
        {
            if (ImGui::SmallButton(gb.pc == g_breakpoint ? "Play" : "Pause"))
            {
                g_breakpoint = gb.pc == g_breakpoint ? 0xFFFF : gb.pc;
                scrollToDisasm = g_breakpoint != 0xFFFF;
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("Step Into"))
            {
            }
            ImGui::SameLine();
            ImGui::SmallButton("Step Over"); ImGui::SameLine();
            ImGui::SmallButton("Step Out");
        }

        ImGui::BeginChild("##scrolling", ImVec2(0.f, -ImGui::GetItemsLineHeightWithSpacing()));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        int totalLineCount = 0xFFFF / 2;
        ImGuiListClipper clipper(totalLineCount, ImGui::GetTextLineHeight());

        if (scrollToDisasm)
            ImGui::SetScrollY((g_breakpoint - 30) * ImGui::GetTextLineHeight() * 0.5f);

        u16 memStart = clipper.DisplayStart * 2;
        u16 memEnd = clipper.DisplayEnd * 2;
        for (u16 loc = memStart; loc < memEnd;)
        {
            u8 opcode, length;
            u16 nn;
            const char* disasm = GB_disasm(loc, opcode, nn, length);

            ImVec4 textCol = loc == gb.pc ? ImVec4(0.8f, 0.7f, 0.2f, 1.f) : loc == g_breakpoint ? ImVec4(1.f, 0.2f, 0.1f, 1.f) : ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
            ImGui::PushStyleColor(ImGuiCol_Header, textCol);
            ImGui::Selectable("##selectable", loc == gb.pc || loc == g_breakpoint, ImGuiSelectableFlags_SpanAllColumns);
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::Text("%04d:%02X ", loc, opcode);
            ImGui::SameLine();
            ImGui::Text(disasm, nn);
            loc += length;
        }

        clipper.End();

        ImGui::PopStyleVar(2);
        ImGui::EndChild();

        ImGui::Text("Call Stack");
    }
    ImGui::End();
}

const u32 cpuHertz = 4190000;
i32 ticksToSimulate = 0;

void glwt_draw(float time)
{
    if (gb.pc == g_breakpoint)
        ticksToSimulate = 0;
    else
        ticksToSimulate += (i32)(time * cpuHertz);

    if (ticksToSimulate > 0)
    {
        bool scanlineCompleted = false;
        while (ticksToSimulate > 0 || scanlineCompleted == false)
        {
            i32 ticksElapsed = 0;
            scanlineCompleted = GB_tick(ticksElapsed);
            ticksToSimulate -= ticksElapsed;
            if (gb.pc == g_breakpoint)
                break;
        }
        if (scanlineCompleted)
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(GB_gpuscreen()));
    }

    ImGui_ImplGlfwGL3_NewFrame(SCREEN_WIDTH * ScreenMultiplier, SCREEN_HEIGHT * ScreenMultiplier);
    imgui_draw();
    ImGui::ShowTestWindow();
    
    glClearColor(0.2f, 0.2f, 0.21f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui::Render();
}
