#define CRYSTALGUI_IMPLEMENTATION
#include "crystalgui.h"
#include "raylib.h"

#define RESOURCE_PATH "res/"

void FillBackground(CguiInstance *instance, Texture backgroundTexture)
{
    BeginTextureMode(instance->blurInputRenderTexture);
    DrawTextureFullscreen(backgroundTexture, false);
    EndTextureMode();
    CguiBlurRenderTexture(instance);
}

void FillColor(CguiInstance *instance, Color backgroundColor)
{
    BeginTextureMode(instance->blurOutputRenderTexture);
    ClearBackground(backgroundColor);
    EndTextureMode();
}

// Actions performed on window resize
void OnWindowResize(CguiInstance *instance, Texture backgroundTexture)
{
    CguiUpdateResolution(instance);
}

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Crystal GUI Test");
    SetTargetFPS(60);

    CguiInstance *instance = CguiCreateInstance();
    Texture backgroundTexture = LoadTexture(RESOURCE_PATH "background.png");
    OnWindowResize(instance, backgroundTexture);
    bool backgroundColorToggle = true;
    FillBackground(instance, backgroundTexture);

    CguiButton *normalButton = CguiCreateButton(instance, (Rectangle){ 20, 20, 200, 40 }, "Normal Button", CGUI_BUTTON_TYPE_NORMAL);
    CguiButton *disabledButton = CguiCreateButton(instance, (Rectangle){ 20, 80, 200, 40 }, "Disabled Button", CGUI_BUTTON_TYPE_DISABLED);
    CguiButton *recommendedButton = CguiCreateButton(instance, (Rectangle){ 20, 140, 200, 40 }, "Recommended Button", CGUI_BUTTON_TYPE_RECOMMENDED);
    CguiButton *warningButton = CguiCreateButton(instance, (Rectangle){ 20, 200, 200, 40 }, "Warning Button", CGUI_BUTTON_TYPE_WARNING);
    CguiButton *alertButton = CguiCreateButton(instance, (Rectangle){ 20, 260, 200, 40 }, "Alert Button", CGUI_BUTTON_TYPE_ALERT);
    CguiButton *dangerousButton = CguiCreateButton(instance, (Rectangle){ 20, 320, 200, 40 }, "Dangerous Button", CGUI_BUTTON_TYPE_DANGEROUS);

    while (!WindowShouldClose())
    {
        if (IsKeyRepeated(KEY_SPACE))
        {
            backgroundColorToggle = !backgroundColorToggle;
            if (backgroundColorToggle) FillBackground(instance, backgroundTexture);
            else FillColor(instance, WHITE);
        }

        if (IsWindowResized())
        {
            OnWindowResize(instance, backgroundTexture);
            if (backgroundColorToggle) FillBackground(instance, backgroundTexture);
            else FillColor(instance, WHITE);
        }

        if (IsKeyRepeated(KEY_MINUS)) instance->colorTransparency -= 0.125f;
        if (IsKeyRepeated(KEY_EQUAL)) instance->colorTransparency += 0.125f;

        CguiUpdateButton(instance, normalButton);
        CguiUpdateButton(instance, disabledButton);
        CguiUpdateButton(instance, recommendedButton);
        CguiUpdateButton(instance, warningButton);
        CguiUpdateButton(instance, alertButton);
        CguiUpdateButton(instance, dangerousButton);

        BeginDrawing();
        ClearBackground(WHITE);
        if (backgroundColorToggle) DrawTextureFullscreen(backgroundTexture, false);

        CguiDrawButton(instance, normalButton);
        CguiDrawButton(instance, disabledButton);
        CguiDrawButton(instance, recommendedButton);
        CguiDrawButton(instance, warningButton);
        CguiDrawButton(instance, alertButton);
        CguiDrawButton(instance, dangerousButton);

        EndDrawing();
    }

    UnloadTexture(backgroundTexture);
    CguiDeleteInstance(instance);
    CloseWindow();
}
