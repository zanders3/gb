
#include <math.h>

#include "glwt.h"
#include "gb.h"
#include <assert.h>
#include <stdio.h>
#include "display.h"

#include "imgui.h"
#include "imgui_impl_glwt.h"

#include "memory.h"
#include <array>

#pragma warning (disable: 4996)

const int ScreenMultiplier = 5;
GLuint screenTexture = 0, tileTexture = 0, screenBgTexture = 0;

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
	u32 len = (u32)ftell(file);

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

void set_texture(GLuint tex, int width, int height, const u8* pixels)
{
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(pixels));
}

GLuint make_texture(int width, int height)
{
    GLuint tex;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return tex;
}

void glwt_setup()
{
    screenTexture = make_texture(SCREEN_WIDTH, SCREEN_HEIGHT);
    screenBgTexture = make_texture(SCREEN_WIDTH, SCREEN_HEIGHT);
    tileTexture = make_texture(TILES_WIDTH, TILES_HEIGHT);

    ImGui_ImplGlfwGL3_Init();
    imgui_stylesetup();
}

struct Breakpoints
{
    u32 numLocations;
    std::array<u16, 32> locations;

    Breakpoints() : numLocations(0) 
    {}

    inline bool hasHit(u16 loc) const
    {
        for (u32 i = 0; i < numLocations; i++)
            if (locations[i] == loc)
                return true;
        return false;
    }

    void add(u16 loc)
    {
        if (hasHit(loc) || numLocations >= locations.size())
            return;
        locations[numLocations] = loc;
        numLocations++;
    }

    void remove(u16 loc)
    {
        for (u32 i = 0; i < numLocations; i++)
            if (locations[i] == loc)
            {
                locations[i] = locations[numLocations - 1];
                numLocations--;
                return;
            }
    }
} g_breakpoints;

enum class RunState
{
    Running,
    Paused,
    StepInto,
    StepOver,
    MemBreakpoint
};
RunState g_runState = RunState::Running;
u16 g_stepOverLoc = 0xFFFF;

void imgui_drawgrid(ImVec2 imgPos, int xw, int xh, float w, float h)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    for (int x = 0; x <= xw; x++)
        draw_list->AddLine(ImVec2(imgPos.x + x * 16.f, imgPos.y), ImVec2(imgPos.x + x * 16.f, imgPos.y + h), IM_COL32(200, 123, 123, 255));
    for (int y = 0; y <= xh; y++)
        draw_list->AddLine(ImVec2(imgPos.x, imgPos.y + y * 16.f), ImVec2(imgPos.x + w, imgPos.y + y * 16.f), IM_COL32(200, 123, 123, 255));
}

void imgui_drawram()
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(100.f, 100.f), ImVec2(800.f, 800.f));
    ImGui::Begin("RAM");
    {
        bool useMemBreakpoint = g_runState == RunState::MemBreakpoint;
        if (ImGui::Checkbox("Memory Breakpoint", &useMemBreakpoint) && useMemBreakpoint)
            g_runState = RunState::MemBreakpoint;
        ImGui::SameLine();

        static char memBreakpoint[64] = { '\0' };

        if (ImGui::InputText("", memBreakpoint, 16, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_EnterReturnsTrue))
        {
            sscanf(memBreakpoint, "%04X", (unsigned int*)&g_memBreakpoint);
            sprintf(memBreakpoint, "%04X", g_memBreakpoint);
        }
        if (ImGui::IsItemClicked() || memBreakpoint[0] == '\0')
        {
            sprintf(memBreakpoint, "%02X", g_memBreakpoint);
        }

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
                for (u16 i = 0; i < locsPerLine / 2; i++)
                {
                    ImGui::SameLine();
                    ImGui::Text("%04X ", readMemory16(loc + i * 2));
                    if (ImGui::IsItemHovered() && ImGui::IsItemClicked())
                    {
                        g_memBreakpoint = g_memBreakpoint == loc ? 0xFFFF : loc;
                    }
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
}

void imgui_drawdisasm()
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(100.f, 100.f), ImVec2(800.f, 800.f));
    ImGui::Begin("Disassembly");
    {
        static bool scrollToDisasmNext = false;
        bool scrollToDisasm = g_hitMemBreakpoint && g_runState == RunState::Paused;
        {
            if (scrollToDisasmNext)
            {
                scrollToDisasmNext = false;
                scrollToDisasm = true;
            }
            if (ImGui::Button(g_runState != RunState::Running ? "Play " : "Pause"))
            {
                g_runState = g_runState == RunState::Running ? RunState::Paused : RunState::Running;
                scrollToDisasmNext = g_runState == RunState::Paused;
            }
            ImGui::SameLine();
            if (ImGui::Button("Step Into"))
            {
                g_runState = RunState::StepInto;
                scrollToDisasmNext = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Step Over"))
            {
                u8 opcode, length;
                u16 nn;
                const char* disasm = GB_disasm(gb.pc, opcode, nn, length);
                if (strncmp("CALL", disasm, 4) == 0)
                {
                    g_stepOverLoc = gb.pc + length;
                    g_runState = RunState::StepOver;
                }
                else
                    g_runState = RunState::StepInto;
                scrollToDisasmNext = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Jump to PC"))
            {
                scrollToDisasm = true;
            }
        }

        ImGui::BeginChild("##scrolling", ImVec2(0.f, -ImGui::GetItemsLineHeightWithSpacing()-100.f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        int totalLineCount = 0xFFFF / 2;
        ImGuiListClipper clipper(totalLineCount, ImGui::GetTextLineHeight());

        if (scrollToDisasm)
        {
            float targetScrollY = (gb.pc - 30) * ImGui::GetTextLineHeight() * 0.5f;
            float scrollY = ImGui::GetScrollY();
            if (fabsf(targetScrollY - scrollY) > 30.f)
                ImGui::SetScrollY(targetScrollY);
        }

        u16 memStart = clipper.DisplayStart * 2;
        u16 memEnd = clipper.DisplayEnd * 2;
        for (u16 loc = memStart; loc < memEnd;)
        {
            u8 opcode, length;
            u16 nn;
            const char* disasm = GB_disasm(loc, opcode, nn, length);

            bool hitPC = loc == gb.pc;
            bool hitBP = g_breakpoints.hasHit(loc);

            ImVec4 textCol = hitPC ? ImVec4(0.8f, 0.7f, 0.2f, 1.f) : hitBP ? ImVec4(1.f, 0.2f, 0.1f, 1.f) : ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
            ImGui::PushStyleColor(ImGuiCol_Header, textCol);
            ImGui::PushID(loc);
            if (ImGui::Selectable("##selectable", hitPC || hitBP, ImGuiSelectableFlags_SpanAllColumns))
            {
                if (hitBP)
                    g_breakpoints.remove(loc);
                else
                    g_breakpoints.add(loc);
            }
            ImGui::PopID();
            ImGui::PopStyleColor();
            if (hitBP)
            {
                ImGui::SameLine();
                ImGui::Bullet();
                ImGui::SameLine();
            }
            else
                ImGui::SameLine(13.f);
            ImGui::Text("%04X:%02X ", loc, opcode);
            ImGui::SameLine();
            ImGui::Text(disasm, nn);
            loc += length;
        }

        clipper.End();

        ImGui::PopStyleVar(2);
        ImGui::EndChild();

        ImGui::Columns(2);
        ImGui::Separator();

        ImGui::Text("Call Stack");
        ImGui::NextColumn();

        ImGui::Text("Registers");
        ImGui::Text("af = %04X\nbc = %04X\nde = %04X\nhl = %04X\nsp = %04X\npc = %04X", gb.af, gb.bc, gb.de, gb.hl, gb.sp, gb.pc);
        ImGui::Columns(1);
        ImGui::Separator();
    }
    ImGui::End();
}

void imgui_drawgpu()
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(100.f, 100.f), ImVec2(800.f, 800.f));
    ImGui::Begin("GPU");
    {
        //Tabs
        static int selectedTab = 0;
        {
            ImGui::Tab(0, "Display", &selectedTab);
            ImGui::Tab(1, "Background", &selectedTab);
            ImGui::Tab(2, "Tiles", &selectedTab);
            ImGui::Tab(3, "Sprites", &selectedTab);
        }

        ImVec2 imgPos = ImGui::GetCursorScreenPos();
        if (selectedTab == 0)
        {
            ImGui::Image((ImTextureID)screenTexture, ImVec2(SCREEN_WIDTH * 2.f, SCREEN_HEIGHT * 2.f));
        }
        else if (selectedTab == 1)
        {
            set_texture(screenBgTexture, SCREEN_WIDTH, SCREEN_HEIGHT, GB_bgscreen());
            ImGui::Image((ImTextureID)screenBgTexture, ImVec2(SCREEN_WIDTH * 2.f, SCREEN_HEIGHT * 2.f));
            imgui_drawgrid(imgPos, 20, 18, SCREEN_WIDTH * 2.f, SCREEN_HEIGHT * 2.f);
        }
        else if (selectedTab == 2)
        {
            set_texture(tileTexture, TILES_WIDTH, TILES_HEIGHT, GB_tiledata());
            ImGui::Image((ImTextureID)tileTexture, ImVec2(TILES_WIDTH * 2.f, TILES_HEIGHT * 2.f));
            imgui_drawgrid(imgPos, 16, 24, TILES_WIDTH * 2.f, TILES_HEIGHT * 2.f);
        }
        else if (selectedTab == 3)
        {
            set_texture(tileTexture, TILES_WIDTH, TILES_HEIGHT, GB_tiledata());
            const Sprite* sprites = reinterpret_cast<Sprite*>(&gb.memory[0xFE00]);
            ImGui::Columns(8);
            for (int i = 0; i < 8*5; i++)
            {
                const Sprite& sprite = sprites[i];
                ImVec2 pos = ImVec2((sprite.TileNum % 16) / 16.f, (sprite.TileNum / 16) / 24.f);
                ImGui::Image((ImTextureID)tileTexture, ImVec2(16.f * 2.f, 16.f * 2.f), pos, ImVec2(pos.x + 1.f / 16.f, pos.y + 1.f / 24.f));
                ImGui::SameLine();
                ImGui::Text("%02X\n%02X\n%02X\n%02X", sprite.YPos, sprite.XPos, sprite.TileNum, sprite.Attributes);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
        }
    }
    ImGui::End();
}

void imgui_draw()
{
    ImGui::SetNextWindowPos(ImVec2(-8, 31));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x - 8, ImGui::GetIO().DisplaySize.y - 23));
    ImGui::Begin("MainWindow", NULL, ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus);
    {
        ImGui::Image((ImTextureID)screenTexture, ImVec2(ImGui::GetIO().DisplaySize.x - 8, ImGui::GetIO().DisplaySize.y - 23));
    }
    ImGui::End();

    imgui_drawram();
    imgui_drawdisasm();
    imgui_drawgpu();
}

const u32 cpuHertz = 4190000;
i32 ticksToSimulate = 0;

void glwt_draw(float time)
{
    if (g_runState == RunState::Paused)
        ticksToSimulate = 0;
    else if (time < 1.f)
        ticksToSimulate += (i32)(time * cpuHertz);

    g_hitMemBreakpoint = false;
    if (ticksToSimulate > 0)
    {
        bool scanlineCompleted = false;
        while (ticksToSimulate > 0 || scanlineCompleted == false)
        {
            i32 ticksElapsed = 0;
            scanlineCompleted = GB_tick(ticksElapsed);
            ticksToSimulate -= ticksElapsed;
            if (g_breakpoints.hasHit(gb.pc))
            {
                g_runState = RunState::Paused;
                break;
            }
            else if (g_runState != RunState::Running)
            {
                if (g_runState == RunState::StepInto)
                {
                    g_runState = RunState::Paused;
                    break;
                }
                else if (g_runState == RunState::StepOver && gb.pc == g_stepOverLoc)
                {
                    g_runState = RunState::Paused;
                    break;
                }
                else if (g_runState == RunState::MemBreakpoint && g_hitMemBreakpoint)
                {
                    g_runState = RunState::Paused;
                    break;
                }
            }
        }
        if (scanlineCompleted)
            set_texture(screenTexture, SCREEN_WIDTH, SCREEN_HEIGHT, GB_gpuscreen());
    }

    ImGui_ImplGlfwGL3_NewFrame(SCREEN_WIDTH * ScreenMultiplier, SCREEN_HEIGHT * ScreenMultiplier);
    imgui_draw();
    ImGui::ShowTestWindow();
    
    glClearColor(0.2f, 0.2f, 0.21f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui::Render();
}
