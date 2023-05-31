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
    bool backgroundColorToggle = true;

    OnWindowResize(instance, backgroundTexture);
    FillBackground(instance, backgroundTexture);

    CguiButton *regularButton = CguiCreateButton(instance, (Rectangle)
    {
        20, 20, 200, 40
    }, "Regular Button", CGUI_BUTTON_TYPE_REGULAR);
    CguiButton *disabledButton = CguiCreateButton(instance, (Rectangle)
    {
        20, 80, 200, 40
    }, "Disabled Button", CGUI_BUTTON_TYPE_DISABLED);
    CguiButton *recommendedButton = CguiCreateButton(instance, (Rectangle)
    {
        20, 140, 200, 40
    }, "Recommended Button", CGUI_BUTTON_TYPE_SUGGESTED);
    CguiButton *warningButton = CguiCreateButton(instance, (Rectangle)
    {
        20, 200, 200, 40
    }, "Warning Button", CGUI_BUTTON_TYPE_WARNING);
    CguiButton *alertButton = CguiCreateButton(instance, (Rectangle)
    {
        20, 260, 200, 40
    }, "Alert Button", CGUI_BUTTON_TYPE_ALERT);
    CguiButton *destructiveButton = CguiCreateButton(instance, (Rectangle)
    {
        20, 320, 200, 40
    }, "Destructive Button", CGUI_BUTTON_TYPE_DESTRUCTIVE);

    CguiToggle *regularToggle = CguiCreateToggle(instance, (Rectangle)
    {
        260, 20, 200, 40
    }, "Toggle", CGUI_TOGGLE_TYPE_REGULAR, false);

    CguiCheckbox *regularCheckbox = CguiCreateCheckbox(instance, (Rectangle)
    {
        260, 80, 200, 40
    }, "Checkbox", CGUI_CHECKBOX_TYPE_REGULAR, false);

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

        CguiUpdateButton(instance, regularButton);
        CguiUpdateButton(instance, disabledButton);
        CguiUpdateButton(instance, recommendedButton);
        CguiUpdateButton(instance, warningButton);
        CguiUpdateButton(instance, alertButton);
        CguiUpdateButton(instance, destructiveButton);

        CguiUpdateToggle(instance, regularToggle);

        CguiUpdateCheckbox(instance, regularCheckbox);

        BeginDrawing();
        ClearBackground(WHITE);
        if (backgroundColorToggle) DrawTextureFullscreen(backgroundTexture, false);

        CguiDrawButton(instance, regularButton);
        CguiDrawButton(instance, disabledButton);
        CguiDrawButton(instance, recommendedButton);
        CguiDrawButton(instance, warningButton);
        CguiDrawButton(instance, alertButton);
        CguiDrawButton(instance, destructiveButton);

        CguiDrawToggle(instance, regularToggle);

        CguiDrawCheckbox(instance, regularCheckbox);

        EndDrawing();
    }

    UnloadTexture(backgroundTexture);
    CguiDeleteInstance(instance);
    CloseWindow();
}
