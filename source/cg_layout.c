/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This source file contains implementations for layout nodes.
///
/// This project is licensed under the terms of MIT license.

#include <math.h>
#include <string.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"
#include "raymath.h"

extern int cguiNameCounter;

// Refactor into a macro?

CguiNode *CguiCreateClampLayout(CguiTransformation transformation)
{
    CguiNode *node = CguiCreateNodePro(transformation, TextFormat("CguiClampLayout #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_CLAMP, NULL, sizeof(CguiClampLayoutData));
    if (!node)
    {
        return NULL;
    }

    CguiClampLayoutData data = {};
    memcpy(node->data, &data, sizeof(CguiClampLayoutData));

    node->transform = CguiTransformClampLayout;

    return node;
}

bool CguiTransformClampLayout(CguiNode *node)
{
    if (!node || node->type != CGUI_LAYOUT_NODE_TYPE_CLAMP || !node->data)
    {
        return false;
    }

    CguiClampLayoutData *layoutData = node->data;

    Rectangle pBounds = node->bounds;
    float     pAspect = pBounds.height != 0.0f ? pBounds.width / pBounds.height : 1.0f;

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiNode *child = node->children[i];
        if (child->type != CGUI_LAYOUT_NODE_TYPE_CLAMP_ITEM || !child->data)
        {
            continue;
        }

        CguiClampLayoutItemData *itemData = child->data;
        CguiTransformation       t        = { 0 };

        t.isRelativePosition = (Vector2) { 1.0f, 1.0f };
        t.size               = (Vector2) { pBounds.width, pBounds.height };

        t.size.x = Clamp(t.size.x, itemData->minSize.x, itemData->maxSize.x);
        t.size.y = Clamp(t.size.y, itemData->minSize.y, itemData->maxSize.y);

        // Aspect ratio calculation
        if (itemData->preserveAspectRatio)
        {
            if (itemData->clampFill != pAspect > itemData->aspectRatio)
            {
                t.size.x = t.size.y * itemData->aspectRatio;
            }
            else
            {
                t.size.y = t.size.x / itemData->aspectRatio;
            }
        }

        t.size.x = Clamp(t.size.x, itemData->minSize.x, itemData->maxSize.x);
        t.size.y = Clamp(t.size.y, itemData->minSize.y, itemData->maxSize.y);

        t.position.x = (pBounds.width - t.size.x) / 2.0f;
        t.position.y = (pBounds.height - t.size.y) / 2.0f;

        CguiSetTransformation(child, t);
    }

    return false;
}

CguiNode *CguiCreateClampLayoutItem(bool preserveAspectRatio, float aspectRatio, bool clampFill, Vector2 minSize, Vector2 maxSize)
{
    CguiNode *node = CguiCreateNodePro(CguiTZeroSize(), TextFormat("CguiClampLayoutItem #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_CLAMP_ITEM, NULL, sizeof(CguiClampLayoutItemData));
    if (!node)
    {
        return NULL;
    }

    CguiClampLayoutItemData data = { .preserveAspectRatio = preserveAspectRatio, .aspectRatio = aspectRatio, .clampFill = clampFill, .minSize = minSize, .maxSize = maxSize };
    memcpy(node->data, &data, sizeof(CguiClampLayoutItemData));

    return node;
}

CguiNode *CguiCreateLinearLayout(CguiTransformation transformation, int direction, int justify, float spacing)
{
    CguiNode *node = CguiCreateNodePro(transformation, TextFormat("CguiLinearLayout #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_LINEAR, NULL, sizeof(CguiLinearLayoutData));
    if (!node)
    {
        return NULL;
    }

    CguiLinearLayoutData data = { .direction = direction, .justify = justify, .spacing = spacing };
    memcpy(node->data, &data, sizeof(CguiLinearLayoutData));

    node->transform = CguiTransformLinearLayout;

    return node;
}

bool CguiTransformLinearLayout(CguiNode *node)
{
    if (!node || node->type != CGUI_LAYOUT_NODE_TYPE_LINEAR || !node->data)
    {
        return false;
    }

    CguiLinearLayoutData *layoutData = node->data;

    Rectangle pBounds      = node->bounds;
    bool      isHorizontal = layoutData->direction == CGUI_LAYOUT_DIRECTION_X;
    float     parentSize   = isHorizontal ? pBounds.width : pBounds.height;

    int   itemsCount   = 0;
    float totalWeight  = 0.0f;
    float totalMinsize = 0.0f;

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiNode *child = node->children[i];
        if (child->type != CGUI_LAYOUT_NODE_TYPE_LINEAR_ITEM || !child->data)
        {
            continue;
        }

        CguiLinearLayoutItemData *itemData = child->data;
        totalWeight += itemData->weight;
        totalMinsize += itemData->minSize;
        itemsCount++;
    }

    if (itemsCount == 0)
    {
        return false;
    }

    float totalSpacing   = layoutData->spacing * (itemsCount - 1);
    float availableSpace = parentSize - totalSpacing;
    float extraSpace     = fmax(availableSpace - totalMinsize, 0.0f);
    float totalItemSize  = 0.0f;

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiNode *child = node->children[i];
        if (child->type != CGUI_LAYOUT_NODE_TYPE_LINEAR_ITEM || !child->data)
        {
            continue;
        }

        CguiLinearLayoutItemData *itemData   = child->data;
        float                     proportion = totalWeight != 0.0f ? itemData->weight / totalWeight : 0.0f;
        itemData->size                       = fminf(itemData->minSize + extraSpace * proportion, itemData->maxSize);
        totalItemSize += itemData->size;
    }

    float spacing  = layoutData->spacing;
    float usedSize = totalItemSize + totalSpacing;
    float cursor   = 0.0f;

    switch (layoutData->justify)
    {
        case CGUI_LAYOUT_JUSTIFY_BEGIN:
            cursor = 0.0f;
            break;
        case CGUI_LAYOUT_JUSTIFY_CENTER:
            cursor = (parentSize - usedSize) * 0.5f;
            break;
        case CGUI_LAYOUT_JUSTIFY_END:
            cursor = parentSize - usedSize;
            break;
        case CGUI_LAYOUT_JUSTIFY_SPACE_BETWEEN:
            cursor  = 0.0f;
            spacing = (parentSize - totalItemSize) / (itemsCount - 1);
            break;
    }

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiNode *child = node->children[i];
        if (child->type != CGUI_LAYOUT_NODE_TYPE_LINEAR_ITEM || !child->data)
        {
            continue;
        }

        CguiLinearLayoutItemData *itemData = child->data;

        itemData->position = cursor;
        cursor += itemData->size + spacing;

        CguiTransformation t = { 0 };
        t.isRelativePosition = (Vector2) { 1.0f, 1.0f };

        if (isHorizontal)
        {
            t.position.x = itemData->position;
            t.size.x     = itemData->size;
            t.size.y     = pBounds.height;
        }
        else
        {
            t.position.y = itemData->position;
            t.size.y     = itemData->size;
            t.size.x     = pBounds.width;
        }

        CguiSetTransformation(child, t);
    }

    return false; // Layout size itself never changes, return false
}

CguiNode *CguiCreateLinearLayoutItem(float weight, float minSize, float maxSize)
{
    CguiNode *node = CguiCreateNodePro(CguiTZeroSize(), TextFormat("CguiLinearLayoutItem #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_LINEAR_ITEM, NULL, sizeof(CguiLinearLayoutItemData));
    if (!node)
    {
        return NULL;
    }

    CguiLinearLayoutItemData data = { .weight = weight, .minSize = minSize, .maxSize = maxSize };
    memcpy(node->data, &data, sizeof(CguiLinearLayoutItemData));

    return node;
}

CguiNode *CguiCreateGridLayout(CguiTransformation transformation, int xSlotsCount, int ySlotsCount, int xJustify, int yJustify, Vector2 spacing)
{
    if (xSlotsCount < 0 || ySlotsCount < 0)
    {
        return NULL;
    }

    CguiNode *node = CguiCreateNodePro(transformation, TextFormat("CguiGridLayout #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_GRID, NULL, sizeof(CguiGridLayoutData));
    if (!node)
    {
        return NULL;
    }

    CguiGridLayoutSlotData *xSlots = NULL;
    if (xSlotsCount > 0)
    {
        xSlots = CG_MALLOC_NULL(sizeof(CguiGridLayoutSlotData) * xSlotsCount);
        if (!xSlots)
        {
            CguiDeleteNode(node);
            return NULL;
        }

        for (int i = 0; i < xSlotsCount; i++)
        {
            xSlots[i].maxSize = INFINITY; // Default to infinity
        }
    }

    CguiGridLayoutSlotData *ySlots = NULL;
    if (ySlotsCount > 0)
    {
        ySlots = CG_MALLOC_NULL(sizeof(CguiGridLayoutSlotData) * ySlotsCount);
        if (!ySlots)
        {
            CG_FREE_NULL(xSlots);
            CguiDeleteNode(node);
            return NULL;
        }

        for (int i = 0; i < ySlotsCount; i++)
        {
            ySlots[i].maxSize = INFINITY; // Default to infinity
        }
    }

    CguiGridLayoutData data = { .xSlots = xSlots, .xSlotsCount = xSlotsCount, .xSlotsCapacity = xSlotsCount, .ySlots = ySlots, .ySlotsCount = ySlotsCount, .ySlotsCapacity = ySlotsCount, .xJustify = xJustify, .yJustify = yJustify, .spacing = spacing };
    memcpy(node->data, &data, sizeof(CguiGridLayoutData));

    node->transform      = CguiTransformGridLayout;
    node->deleteNodeData = CguiDeleteGridLayoutData;

    return node;
}

void CguiDeleteGridLayoutData(CguiNode *node)
{
    if (!node || node->type != CGUI_LAYOUT_NODE_TYPE_GRID || !node->data)
    {
        return;
    }

    CguiGridLayoutData *layoutData = node->data;

    CG_FREE_NULL(layoutData->xSlots);

    CG_FREE_NULL(layoutData->ySlots);
}

static bool CguiGridLayoutInsertSlot(CguiGridLayoutSlotData **slots, int *count, int *capacity, int index, float weight, float minSize, float maxSize)
{
    if (!slots || !count || !capacity || index < 0 || index > *count)
    {
        return false;
    }

    // Resize capacity if full
    if (*count == *capacity)
    {
        int                     newCapacity = (*capacity == 0) ? 1 : (*capacity * 2);
        CguiGridLayoutSlotData *newSlots    = (CguiGridLayoutSlotData *) CG_REALLOC(*slots, sizeof(CguiGridLayoutSlotData) * newCapacity);
        if (!newSlots)
        {
            return false;
        }
        *slots    = newSlots;
        *capacity = newCapacity;
    }

    // Shift elements right to insert element
    memmove(&(*slots)[index + 1], &(*slots)[index], sizeof(CguiGridLayoutSlotData) * (*count - index));

    (*slots)[index].weight  = weight;
    (*slots)[index].minSize = minSize;
    (*slots)[index].maxSize = maxSize;
    (*count)++;

    return true;
}

static bool CguiGridLayoutRemoveSlot(CguiGridLayoutSlotData **slots, int *count, int *capacity, int index)
{
    if (!slots || !count || !capacity || index < 0 || index >= *count)
    {
        return false;
    }

    // Shift elements left to remove element
    memmove(&(*slots)[index], &(*slots)[index + 1], sizeof(CguiGridLayoutSlotData) * (*count - index - 1));
    (*count)--;

    // Reduce capacity if < 25% used
    if (*capacity > 1 && *count < *capacity / 4)
    {
        int newCapacity = *capacity / 2;
        if (newCapacity == 0)
        {
            newCapacity = 1;
        }

        CguiGridLayoutSlotData *newSlots = (CguiGridLayoutSlotData *) CG_REALLOC(*slots, sizeof(CguiGridLayoutSlotData) * newCapacity);
        if (newSlots)
        {
            *slots    = newSlots;
            *capacity = newCapacity;
        }
        // Ignore reallocation failure
    }

    return true;
}

bool CguiGridLayoutInsertSlotX(CguiGridLayoutData *layoutData, int index, float weight, float minSize, float maxSize)
{
    return CguiGridLayoutInsertSlot(&layoutData->xSlots, &layoutData->xSlotsCount, &layoutData->xSlotsCapacity, index, weight, minSize, maxSize);
}

bool CguiGridLayoutRemoveSlotX(CguiGridLayoutData *layoutData, int index)
{
    return CguiGridLayoutRemoveSlot(&layoutData->xSlots, &layoutData->xSlotsCount, &layoutData->xSlotsCapacity, index);
}

bool CguiGridLayoutInsertSlotY(CguiGridLayoutData *layoutData, int index, float weight, float minSize, float maxSize)
{
    return CguiGridLayoutInsertSlot(&layoutData->ySlots, &layoutData->ySlotsCount, &layoutData->ySlotsCapacity, index, weight, minSize, maxSize);
}

bool CguiGridLayoutRemoveSlotY(CguiGridLayoutData *layoutData, int index)
{
    return CguiGridLayoutRemoveSlot(&layoutData->ySlots, &layoutData->ySlotsCount, &layoutData->ySlotsCapacity, index);
}

bool CguiTransformGridLayout(CguiNode *node)
{
    if (!node || node->type != CGUI_LAYOUT_NODE_TYPE_GRID || !node->data)
    {
        return false;
    }

    CguiGridLayoutData *layoutData = node->data;

    // No further calculations (also avoids division by zero)
    if (layoutData->xSlotsCount == 0 || layoutData->ySlotsCount == 0)
    {
        return false;
    }

    Rectangle pBounds    = node->bounds;
    Vector2   parentSize = { pBounds.width, pBounds.height };

    Vector2 totalWeight  = Vector2Zero();
    Vector2 totalMinsize = Vector2Zero();

    for (int i = 0; i < layoutData->xSlotsCount; i++)
    {
        CguiGridLayoutSlotData *slotData = &layoutData->xSlots[i];

        totalWeight.x += slotData->weight;
        totalMinsize.x += slotData->weight;
    }

    for (int i = 0; i < layoutData->ySlotsCount; i++)
    {
        CguiGridLayoutSlotData *slotData = &layoutData->ySlots[i];

        totalWeight.y += slotData->weight;
        totalMinsize.y += slotData->weight;
    }

    Vector2 totalSpacing   = Vector2Multiply(layoutData->spacing, (Vector2) { layoutData->xSlotsCount - 1, layoutData->ySlotsCount - 1 });
    Vector2 availableSpace = Vector2Subtract(parentSize, totalSpacing);
    Vector2 extraSpace     = Vector2Max(Vector2Subtract(availableSpace, totalMinsize), Vector2Zero());
    Vector2 totalItemSize  = Vector2Zero();

    for (int i = 0; i < layoutData->xSlotsCount; i++)
    {
        CguiGridLayoutSlotData *slotData = &layoutData->xSlots[i];

        float proportion = totalWeight.x != 0.0f ? slotData->weight / totalWeight.x : 0.0f;
        slotData->size   = fminf(slotData->minSize + extraSpace.x * proportion, slotData->maxSize);
        totalItemSize.x += slotData->size;
    }

    for (int i = 0; i < layoutData->ySlotsCount; i++)
    {
        CguiGridLayoutSlotData *slotData = &layoutData->ySlots[i];

        float proportion = totalWeight.y != 0.0f ? slotData->weight / totalWeight.y : 0.0f;
        slotData->size   = fminf(slotData->minSize + extraSpace.y * proportion, slotData->maxSize);
        totalItemSize.y += slotData->size;
    }

    Vector2 spacing  = layoutData->spacing;
    Vector2 usedSize = Vector2Add(totalItemSize, totalSpacing);
    Vector2 cursor   = Vector2Zero();

    switch (layoutData->xJustify)
    {
        case CGUI_LAYOUT_JUSTIFY_BEGIN:
            cursor.x = 0.0f;
            break;
        case CGUI_LAYOUT_JUSTIFY_CENTER:
            cursor.x = (parentSize.x - usedSize.x) * 0.5f;
            break;
        case CGUI_LAYOUT_JUSTIFY_END:
            cursor.x = parentSize.x - usedSize.x;
            break;
        case CGUI_LAYOUT_JUSTIFY_SPACE_BETWEEN:
            cursor.x  = 0.0f;
            spacing.x = (parentSize.x - totalItemSize.x) / (layoutData->xSlotsCount - 1);
            break;
    }

    switch (layoutData->yJustify)
    {
        case CGUI_LAYOUT_JUSTIFY_BEGIN:
            cursor.y = 0.0f;
            break;
        case CGUI_LAYOUT_JUSTIFY_CENTER:
            cursor.y = (parentSize.y - usedSize.y) * 0.5f;
            break;
        case CGUI_LAYOUT_JUSTIFY_END:
            cursor.y = parentSize.y - usedSize.y;
            break;
        case CGUI_LAYOUT_JUSTIFY_SPACE_BETWEEN:
            cursor.y  = 0.0f;
            spacing.y = (parentSize.y - totalItemSize.y) / (layoutData->ySlotsCount - 1);
            break;
    }

    for (int i = 0; i < layoutData->xSlotsCount; i++)
    {
        CguiGridLayoutSlotData *slotData = &layoutData->xSlots[i]; // By reference

        slotData->position = cursor.x;
        cursor.x += slotData->size + spacing.x;
    }

    for (int i = 0; i < layoutData->ySlotsCount; i++)
    {
        CguiGridLayoutSlotData *slotData = &layoutData->ySlots[i]; // By reference

        slotData->position = cursor.y;
        cursor.y += slotData->size + spacing.y;
    }

    // Add grid items

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiNode *child = node->children[i];
        if (child->type != CGUI_LAYOUT_NODE_TYPE_GRID_ITEM || !child->data)
        {
            continue;
        }

        CguiGridLayoutItemData *itemData = child->data;
        CguiTransformation      t        = { 0 };
        t.isRelativePosition             = (Vector2) { 1.0f, 1.0f };

        // Invalid slot indices
        if (itemData->xSlot < 0 || itemData->xSlot >= layoutData->xSlotsCount ||
            itemData->ySlot < 0 || itemData->ySlot >= layoutData->ySlotsCount ||
            itemData->xSpan <= 0 || itemData->xSlot + itemData->xSpan > layoutData->xSlotsCount ||
            itemData->ySpan <= 0 || itemData->ySlot + itemData->ySpan > layoutData->ySlotsCount)
        {
            CguiSetTransformation(child, t);
            continue;
        }

        t.position.x = layoutData->xSlots[itemData->xSlot].position;
        t.position.y = layoutData->ySlots[itemData->ySlot].position;
        t.size.x     = layoutData->xSlots[itemData->xSlot + itemData->xSpan - 1].position + layoutData->xSlots[itemData->xSlot + itemData->xSpan - 1].size - t.position.x;
        t.size.y     = layoutData->ySlots[itemData->ySlot + itemData->ySpan - 1].position + layoutData->ySlots[itemData->ySlot + itemData->ySpan - 1].size - t.position.y;

        CguiSetTransformation(child, t);
    }

    return false; // Layout size itself never changes, return false
}

CguiNode *CguiCreateGridLayoutItem(int xSlot, int ySlot, int xSpan, int ySpan)
{
    CguiNode *node = CguiCreateNodePro(CguiTZeroSize(), TextFormat("CguiGridLayoutItem #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_GRID_ITEM, NULL, sizeof(CguiGridLayoutItemData));
    if (!node)
    {
        return NULL;
    }

    CguiGridLayoutItemData data = { .xSlot = xSlot, .ySlot = ySlot, .xSpan = xSpan, .ySpan = ySpan };
    memcpy(node->data, &data, sizeof(CguiGridLayoutItemData));

    return node;
}
