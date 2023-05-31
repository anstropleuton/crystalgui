#include "raylib.h"

// Check if two text string are equal till the length
bool IsTextEqualTillLength(const char *text1, const char *text2, int length);

// Checks are performed for every character/string
bool MarkdownTextCheck(const char *text, const char *check, int *offset);

// All the font used by markdown renderer
typedef struct FontCollection
{
    Font regular;
    Font italic;
    Font bold;
    Font boldItalic;
    Font monoRegular;
    Font monoItalic;
    Font monoBold;
    Font monoBoldItalic;
} FontCollection;

// Colors required by markdown renderer
typedef struct ColorCollection
{
    Color regular;
    Color shapeColor;
    Color backgroundColor;
} ColorCollection;

// Markdown render header state
typedef enum MarkdownSize
{
    MARKDOWN_HEADER_SIZE_NORMAL,
    MARKDOWN_HEADER_SIZE_ONE,
    MARKDOWN_HEADER_SIZE_TWO,
    MARKDOWN_HEADER_SIZE_THREE,
    MARKDOWN_HEADER_SIZE_FOUR,
    MARKDOWN_HEADER_SIZE_FIVE,
    MARKDOWN_HEADER_SIZE_SIX,
    MARKDOWN_HEADER_SIZE_MAX
} MarkdownSize;

// Draw text with markdown formats (only basics)
void DrawTextMarkdown(FontCollection *fontCollection, const char *text, Rectangle area, float fontSizes[MARKDOWN_HEADER_SIZE_MAX], Vector2 spacing, float tabWidth, ColorCollection *colorCollection);
