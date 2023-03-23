/*******************************************************************************************
*
*   Crystal GUI - Tests
*
*   DESCRIPTION:
*
*   Test all the functions from Cgui (not including raygui-compatible functions)
*
*   LICENSE: Unmodified MIT License.
*
*   Copyright (c) 2023 Anstro Pleuton
*
*******************************************************************************************/

#define CGUI_IMPLEMENTATION
//#define CGUI_LIGHT_THEME
#define CGUI_DARK_THEME

#include "crystalgui.h"
#include "raylib.h"

void TestTraceLog(const char *text, ...)
{
    va_list args;
    va_start(args, text);
    char buffer[4096] = { 0 };

    strcpy(buffer, "TestTraceLog: ");
    strcat(buffer, text);
    strcat(buffer, "\n");
    vprintf(buffer, args);
    fflush(stdout);

    va_end(args);
}

//------------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    // Only display warnings or above
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Crystal Gui Tests");
    SetTargetFPS(60);

    // Load the Cgui resources
    CguiLoad();
    CguiSetRoundness(5.0f);

    // Font settings
    FontProp fontProp = CguiGetFontProperty();
    fontProp.size = 20.0f;
    fontProp.font = LoadFontEx("../res/Roboto-Regular.ttf", fontProp.size, NULL, 0);
    CguiSetFontProperty(fontProp);

    // Cgui customization
    Texture background = LoadTexture("../res/background.png");
    bool useWallpaper = false, theme = false;

    // Gui variables
    CguiButton myButton = CguiCreateButton((Rectangle){ 20, 20, 200, 40 }, "Normal Button");
    CguiDropDownButton ddButton = CguiCreateDropDownButton((Rectangle){ 20, 70, 200, 40 }, (const char *[]){ "Select Tool", "Wrench", "Hammer", "Blade", "Screw you", "Tape", "Glue" }, 7);
    CguiRepeatButton rpButton = CguiCreateRepeatButton((Rectangle){ 20, 120, 200, 40 }, "Repeat Button");
    CguiHyperLinkButton hpButton = CguiCreateHyperLinkButton((Rectangle){ 20, 170, 200, 40 }, "My Repo", "https://github.com/AnstroPleuton/crystalgui");
    CguiActiveButton atButton = CguiCreateActiveButton((Rectangle){ 20, 220, 200, 40 }, "Active Button");
    CguiUnknownButton unButton = CguiCreateUnknownButton((Rectangle){ 20, 270, 200, 40 }, "Unknown Button");
    CguiDangerButton dgButton = CguiCreateDangerButton((Rectangle){ 20, 320, 200, 40 }, "Danger Button");

    // Game variables
    int buttonPressCount[6] = { 0 }, lastButtonPress = -1;
    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose())
    {
        // Cgui Updates
        //----------------------------------------------------------------------------------
        // Change Cgui settings
        if (IsKeyPressed(KEY_ENTER)) useWallpaper = !useWallpaper;
        if (IsKeyPressed(KEY_SPACE)) { theme = !theme; theme ? CguiSetLightTheme() : CguiSetDarkTheme(); }
        if (GetMouseWheelMove() != 0) fontProp.size += GetMouseWheelMoveV().x;

        // Update the gui every frame
        CguiUpdateResolution();

        // Update Cgui (specific update order)
        if (CguiUpdateDropDownButton(&ddButton)) { TestTraceLog("Drop Down selected entry at %i: %s", ddButton.selectedEntry, (*(CguiButton *)GetElement(ddButton.selectedEntry, ddButton.entries)->data).text); lastButtonPress = 6; }

        if (CguiGetConfig(CGUI_CONFIG_DROPDOWN_ACTIVE) == 0)
        {
            if (CguiUpdateButton(&myButton)) { TestTraceLog("Normal button pressed"); buttonPressCount[0]++; lastButtonPress = 0; }
            if (CguiUpdateRepeatButton(&rpButton)) { TestTraceLog("Repeat button pressed"); buttonPressCount[1]++; lastButtonPress = 1; }
            if (CguiUpdateHyperLinkButton(&hpButton)) { TestTraceLog("Hyper link button pressed"); buttonPressCount[2]++; lastButtonPress = 2; }
            if (CguiUpdateActiveButton(&atButton)) { TestTraceLog("Active button pressed"); buttonPressCount[3]++; lastButtonPress = 3; }
            if (CguiUpdateUnknownButton(&unButton)) { TestTraceLog("Unknown button pressed"); buttonPressCount[4]++; lastButtonPress = 4; }
            if (CguiUpdateDangerButton(&dgButton)) { TestTraceLog("Danger button pressed"); buttonPressCount[5]++; lastButtonPress = 5; }
        }

        // Reset the states
        CguiClearGlobalConfig();
        //----------------------------------------------------------------------------------

        // Your game updates
        //----------------------------------------------------------------------------------
        // Game updates when the mouse is not on top of the Cgui (example, zooming)
        if (CguiGetConfig(CGUI_CONFIG_MOUSE_ACTIVE) == 0)
        {
        }

        // Game updates when the keyboard is not being used by Cgui (example, no controls)
        if (CguiGetConfig(CGUI_CONFIG_KEYBOARD_ACTIVE) == 0)
        {
        }

        // Game updates when all drop down is closed
        if (CguiGetConfig(CGUI_CONFIG_DROPDOWN_ACTIVE) == 0)
        {
        }

        // Game updates when all popup is closed
        if (CguiGetConfig(CGUI_CONFIG_POPUP_ACTIVE) == 0)
        {
        }
        //----------------------------------------------------------------------------------

        // Process the background, pretty cool looks!
        CguiBeginBackground();
            if (useWallpaper) DrawTexturePro(background, (Rectangle){ 0.0f, 0.0f, (float)background.width, (float)background.height }, (Rectangle){ 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() }, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
            else DrawRectanglePro((Rectangle){ 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() }, (Vector2){ 0.0f, 0.0f }, 0.0f, ColorAlpha(CguiGetColor(CGUI_COLOR_BACKGROUND), 1.0f));
        CguiEndBackground();

        // Main Drawing
        BeginDrawing();
            // Draw the background that was used by CguiBeginBackground
            CguiDrawBackground();

            // Your game draws
            //------------------------------------------------------------------------------
            switch (lastButtonPress)
            {
            case 0:
                CguiDrawText(TextFormat("Normal button pressed %i times", buttonPressCount[0]), (Rectangle){ 270, 20, 200, 40 });
                break;
            case 6:
                CguiDrawText(TextFormat("Drop down selected entry: %s", (*(CguiButton *)GetElement(ddButton.selectedEntry, ddButton.entries)->data).text), (Rectangle){ 270, 70, 200, 40 });
                break;
            case 1:
                CguiDrawText(TextFormat("Repeat button pressed %i times", buttonPressCount[1]), (Rectangle){ 270, 120, 200, 40 });
                break;
            case 2:
                CguiDrawText(TextFormat("Hyper link button pressed %i times", buttonPressCount[2]), (Rectangle){ 270, 170, 200, 40 });
                break;
            case 3:
                CguiDrawText(TextFormat("Active button pressed %i times", buttonPressCount[3]), (Rectangle){ 270, 220, 200, 40 });
                break;
            case 4:
                CguiDrawText(TextFormat("Unknown button pressed %i times", buttonPressCount[4]), (Rectangle){ 270, 270, 200, 40 });
                break;
            case 5:
                CguiDrawText(TextFormat("Danger button pressed %i times", buttonPressCount[5]), (Rectangle){ 270, 320, 200, 40 });
                break;
            }
            //------------------------------------------------------------------------------

            // Cgui Draws
            //------------------------------------------------------------------------------
            // Draw Cgui (Draw in reverse order)
            CguiDrawButton(&myButton);
            ddButton.button.text = (*(CguiButton *)GetElement(ddButton.selectedEntry, ddButton.entries)->data).text;
            CguiDrawButton(&ddButton.button);
            CguiDrawRepeatButton(&rpButton);
            CguiDrawHyperLinkButton(&hpButton);
            CguiDrawActiveButton(&atButton);
            CguiDrawUnknownButton(&unButton);
            CguiDrawDangerButton(&dgButton);

            // Last these gui elements draws
            CguiDrawDropDownButton(&ddButton);
            //------------------------------------------------------------------------------
        EndDrawing();
    }

    // De-initialization
    //--------------------------------------------------------------------------------------
    CguiDeleteButton(&myButton);
    CguiDeleteDropDownButton(&ddButton);
    CguiDeleteRepeatButton(&rpButton);
    CguiDeleteHyperLinkButton(&hpButton);
    CguiDeleteActiveButton(&atButton);
    CguiDeleteUnknownButton(&unButton);
    CguiDeleteDangerButton(&dgButton);
    UnloadTexture(background);
    CguiUnload();        // Unload the Cgui resources
    CloseWindow();
    //--------------------------------------------------------------------------------------
}
