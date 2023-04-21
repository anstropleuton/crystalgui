#define CRYSTALGUI_IMPLEMENTATION
#include "crystalgui.h"
#include "raylib.h"

void ResizeStuff(CguiInstance *instance, Texture backgroundTexture)
{
    CguiUpdateResolution(instance);
    BeginTextureMode(instance->blurInputRenderTexture);
    DrawTextureFullscreen(backgroundTexture, false);
    EndTextureMode();
    CguiBlurRenderTexture(instance);
}

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Crystal GUI Test");
    SetTargetFPS(60);

    CguiInstance *instance = CguiCreateInstanceInstance();
    Texture backgroundTexture = LoadTexture(CRYSTALGUI_RESOURCE_PATH "background.png");
    ResizeStuff(instance, backgroundTexture);

    while (!WindowShouldClose())
    {
        if (IsWindowResized()) ResizeStuff(instance, backgroundTexture);

        BeginDrawing();
        ClearBackground(WHITE);
        DrawTextureFullscreen(backgroundTexture, false);

        CguiDrawShaderRectangle(instance, (Rectangle)
        {
            20.0f, 20.0f, 200.0f, 40.0f
        }, ColorAlpha(WHITE, 0.75f), ColorAlpha(BLACK, 0.5f));

        CguiDrawShaderText(instance, "Hello there!", (Rectangle)
        {
            20.0f, 20.0f, 200.0f, 40.0f
        });

        EndDrawing();
    }

    UnloadTexture(backgroundTexture);
    CguiDeleteInstance(instance);
    CloseWindow();
}
