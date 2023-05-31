#include "markdown_renderer.h"

// Check if two text string are equal till the length
bool IsTextEqualTillLength(const char *text1, const char *text2, int length)
{
    int text1Len = TextLength(text1);
    int text2Len = TextLength(text2);

    // If length of either text is lower than the desired length for comparation, return false
    if (text1Len < length || text2Len < length) return false;

    for (int i = 0; i < length; i++)
    {
        // Return when mismatched character was found
        if (text1[i] != text2[i]) return false;
    }
    return true;
}

// Checks are performed for every character/string
bool MarkdownTextCheck(const char *text, const char *check, int *offset)
{
    if (!offset) return false;
    int checkLength = TextLength(check);
    bool result = IsTextEqualTillLength(text + *offset, check, checkLength);
    if (result) *offset += checkLength;
    return result;
}

// Draw text with markdown formats (only basics)
void DrawTextMarkdown(FontCollection *fontCollection, const char *text, Rectangle area, float fontSizes[MARKDOWN_HEADER_SIZE_MAX], Vector2 spacing, float tabWidth, ColorCollection *colorCollection)
{
    int size = TextLength(text);
    int headerSize = 0;

    bool backslashSkip = false;
    bool wasNewLine = true;
    bool isCodeBlock = false; // Using ``` will make it code "block"

    // Font style states
    bool fontStyleBold = false;
    bool fontStyleItalic = false;
    bool fontStyleUnderscore = false;
    bool fontStyleStrikethrough = false;
    bool fontStyleMonospace = false;

    // Characters render positions
    float textOffsetX = 0.0f;
    float textOffsetY = 0.0f;

    for (int i = 0; i < size;)
    {
        //----------------------------------------------------------------------
        // Loop variables
        //----------------------------------------------------------------------

        Font font = fontCollection->regular;
        Color tint = colorCollection->regular;
        Vector2 position = { area.x, area.y };
        float fontSize = fontSizes[0];
        float monoFontWidth = MeasureTextEx(font, "M", fontSize, spacing.x).x;
        int previousI = i;

        //----------------------------------------------------------------------
        // Parsing
        //----------------------------------------------------------------------

        // Check for new line
        wasNewLine = false;
        if (i > 0) if (text[i - 1] == '\n')
            {
                wasNewLine = true;
            }

        // Note: No syntax highlighting for code block
        if (MarkdownTextCheck(text, "```", &i))
        {
            fontStyleMonospace = !fontStyleMonospace;
            isCodeBlock = !isCodeBlock;

            // Skip to new line
            for (int j = i; j < size; j++)
            {
                if (text[j] == '\n')
                {
                    i = j;
                    break;
                }
            }
        }
        else if (!isCodeBlock)
        {
            if (MarkdownTextCheck(text, "``", &i))
            {
                fontStyleMonospace = !fontStyleMonospace;
            }
            else if (MarkdownTextCheck(text, "`", &i))
            {
                fontStyleMonospace = !fontStyleMonospace;
            }
        }

        // Parse checks (Welcome to a large if-else-if chain)
        if (!backslashSkip && !fontStyleMonospace)
        {
            //------------------------------------------------------------------
            // Parse checks on new line
            //------------------------------------------------------------------
            if (wasNewLine)
            {
                // === Size of the text of the lines starting with number of # will be enlarged (or lowered) ===
                if (MarkdownTextCheck(text, "###### ", &i))
                {
                    headerSize = MARKDOWN_HEADER_SIZE_SIX;
                }
                else if (MarkdownTextCheck(text, "##### ", &i))
                {
                    headerSize = MARKDOWN_HEADER_SIZE_FIVE;
                }
                else if (MarkdownTextCheck(text, "#### ", &i))
                {
                    headerSize = MARKDOWN_HEADER_SIZE_FOUR;
                }
                else if (MarkdownTextCheck(text, "### ", &i))
                {
                    headerSize = MARKDOWN_HEADER_SIZE_THREE;
                }
                else if (MarkdownTextCheck(text, "## ", &i))
                {
                    headerSize = MARKDOWN_HEADER_SIZE_TWO;
                }
                else if (MarkdownTextCheck(text, "# ", &i))
                {
                    headerSize = MARKDOWN_HEADER_SIZE_ONE;
                }

                // === Line starting with --- will draw a horizontal rule ===
                else if (MarkdownTextCheck(text, "---", &i))
                {
                    // Skip to new line
                    for (int j = i; j < size; j++)
                    {
                        if (text[j] == '\n')
                        {
                            i = j;
                            break;
                        }
                    }

                    // Draw horizontal rule
                    DrawLineEx((Vector2)
                    {
                        area.x, area.y + textOffsetY + fontSize / 2.0f
                    }, (Vector2)
                    {
                        area.x + area.width, area.y + textOffsetY + fontSize / 2.0f
                    }, fontSize / 10.0f, colorCollection->shapeColor);
                }
            }

            // === Line starting with two forward slash will be skipped from being rendered ===
            if (MarkdownTextCheck(text, "//", &i))
            {
                for (int j = i; j < size; j++)
                {
                    if (text[j] == '\n')
                    {
                        i = j + 1; // We skip the new line too
                        break;
                    }
                }
            }

            // === Character > will be replaced by a block ===
            else if (MarkdownTextCheck(text, ">", &i))
            {
                DrawRectangleRec((Rectangle)
                {
                    position.x + textOffsetX, position.y + textOffsetY, monoFontWidth / 2.0f, fontSize
                }, colorCollection->shapeColor);
                textOffsetX += monoFontWidth;
            }

            // === Bold and italic using * ===
            else if (MarkdownTextCheck(text, "***", &i))
            {
                fontStyleBold = !fontStyleBold;
                fontStyleItalic = !fontStyleItalic;
            }
            else if (MarkdownTextCheck(text, "**", &i))
            {
                fontStyleBold = !fontStyleBold;
            }
            else if (MarkdownTextCheck(text, "*", &i))
            {
                fontStyleItalic = !fontStyleItalic;
            }

            // === Underscore and Strikethrough ===
            else if (MarkdownTextCheck(text, "__", &i))
            {
                fontStyleUnderscore = !fontStyleUnderscore;
            }
            else if (MarkdownTextCheck(text, "~~", &i))
            {
                fontStyleStrikethrough = !fontStyleStrikethrough;
            }

            // === Bullets points (hollow and non) ===
            else if (MarkdownTextCheck(text, "+ ", &i))
            {
                DrawCircle(position.x + textOffsetX + monoFontWidth / 4.0f, position.y + textOffsetY + fontSize / 2.0f, monoFontWidth / 4.0f, colorCollection->shapeColor);
                textOffsetX += monoFontWidth;
            }
            else if (MarkdownTextCheck(text, "- ", &i))
            {
                DrawCircleLines(position.x + textOffsetX + monoFontWidth / 4.0f, position.y + textOffsetY + fontSize / 2.0f, monoFontWidth / 4.0f, colorCollection->shapeColor);
                textOffsetX += monoFontWidth;
            }
        }
        else backslashSkip = false;

        if (isCodeBlock) fontStyleMonospace = true;

        // Go to next character check if some parse checks were found
        // And if it's not a new line
        if (i != previousI)
        {
            continue;
        }

        //----------------------------------------------------------------------
        // Apply parse checks
        //----------------------------------------------------------------------

        if (MarkdownTextCheck(text, "\\", &i))
        {
            backslashSkip = true;
        }

        if (fontStyleMonospace)
        {
            font = fontCollection->monoRegular;
            if (fontStyleBold)
            {
                if (fontStyleItalic) font = fontCollection->monoBoldItalic;
                else font = fontCollection->monoBold;
            }
            else if (fontStyleItalic) font = fontCollection->monoItalic;
        }
        else
        {
            if (fontStyleBold)
            {
                if (fontStyleItalic) font = fontCollection->boldItalic;
                else font = fontCollection->bold;
            }
            else if (fontStyleItalic) font = fontCollection->italic;
        }

        if (headerSize != 0) fontSize *= fontSizes[headerSize];
        float scaleFactor = fontSize/font.baseSize;         // Character quad scaling factor

        //----------------------------------------------------------------------
        // Draw character
        //----------------------------------------------------------------------

        // Draw character
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepointNext(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;

        // New line conversion
        if (codepoint == '\n')
        {
            if (headerSize == 0) textOffsetY += spacing.y;
            else textOffsetY += spacing.y * fontSizes[headerSize];
            textOffsetX = 0.0f;
            headerSize = MARKDOWN_HEADER_SIZE_NORMAL;

            if (isCodeBlock)
            {
                DrawRectangleRec((Rectangle)
                {
                    area.x, area.y + textOffsetY, area.width, fontSize
                }, colorCollection->backgroundColor);
            }
        }
        else
        {
            // Tempo variable for storing the width
            float nextTextOffsetX = textOffsetX;
            if (codepoint == '\t')
            {
                if (font.glyphs[index].advanceX == 0) nextTextOffsetX += ((float)font.recs[index].width * scaleFactor + spacing.x) * tabWidth;
                else nextTextOffsetX += ((float)font.glyphs[index].advanceX * scaleFactor + spacing.x) * tabWidth;
            }
            else
            {
                if (font.glyphs[index].advanceX == 0) nextTextOffsetX += ((float)font.recs[index].width * scaleFactor + spacing.x);
                else nextTextOffsetX += ((float)font.glyphs[index].advanceX * scaleFactor + spacing.x);
            }

            // Monospace block render
            if (fontStyleMonospace && !isCodeBlock)
            {
                DrawRectangleRec((Rectangle)
                {
                    position.x + textOffsetX, position.y + textOffsetY, nextTextOffsetX - textOffsetX, fontSize
                }, colorCollection->backgroundColor);
            }

            // Strike through line render
            if (fontStyleStrikethrough)
            {
                DrawLineEx((Vector2)
                {
                    position.x + textOffsetX, position.y + textOffsetY + fontSize / 2.0f
                }, (Vector2)
                {
                    position.x + nextTextOffsetX, position.y + textOffsetY + fontSize / 2.0f
                }, fontSize / 10.0f, colorCollection->regular);
            }

            // Underscore line render
            if (fontStyleUnderscore)
            {
                DrawLineEx((Vector2)
                {
                    position.x + textOffsetX, position.y + textOffsetY + fontSize
                }, (Vector2)
                {
                    position.x + nextTextOffsetX, position.y + textOffsetY + fontSize
                }, fontSize / 10.0f, colorCollection->regular);
            }

            // Character render
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                DrawTextCodepoint(font, codepoint, (Vector2)
                {
                    position.x + textOffsetX, position.y + textOffsetY
                }, fontSize, tint);
            }

            // Add new offset
            textOffsetX = nextTextOffsetX;
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }
}
