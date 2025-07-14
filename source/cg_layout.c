/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A modern GUI framework for raylib.
///
/// This source file contains implementations for layout node functionality of
/// Crystal GUI.
///
/// This project is licensed under the terms of MIT license.

#include <raylib.h>
#include <string.h>

#include "crystalgui.h"
#include "raymath.h"

extern int cguiNameCounter;

// Refactor into a macro?

CguiNode *CguiCreateClampLayout(void)
{
    CguiNode *node = CguiCreateNodePro(TextFormat("CguiClampLayout #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_CLAMP, NULL, sizeof(CguiClampLayoutData));
    if (!node)
    {
        return NULL;
    }

    CguiClampLayoutData data = {};
    memcpy(node->data, &data, sizeof(CguiClampLayoutData));

    node->transform = CguiTransformClampLayout;

    return node;
}

CguiNode *CguiCreateClampLayoutItem(bool preserveAspectRatio, float aspectRatio, bool clampFill, Vector2 minSize, Vector2 maxSize)
{
    CguiNode *node = CguiCreateNodePro(TextFormat("CguiClampLayoutItem #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_CLAMP_ITEM, NULL, sizeof(CguiClampLayoutItemData));
    if (!node)
    {
        return NULL;
    }

    CguiClampLayoutItemData data = { .preserveAspectRatio = preserveAspectRatio, .aspectRatio = aspectRatio, .clampFill = clampFill, .minSize = minSize, .maxSize = maxSize };
    memcpy(node->data, &data, sizeof(CguiClampLayoutItemData));

    return node;
}

CguiNode *CguiCreateLinearLayout(int direction, int justify, float spacing)
{
    CguiNode *node = CguiCreateNodePro(TextFormat("CguiLinearLayout #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_LINEAR, NULL, sizeof(CguiLinearLayoutData));
    if (!node)
    {
        return NULL;
    }

    CguiLinearLayoutData data = { .direction = direction, .justify = justify, .spacing = spacing };
    memcpy(node->data, &data, sizeof(CguiLinearLayoutData));

    node->transform = CguiTransformLinearLayout;

    return node;
}

CguiNode *CguiCreateLinearLayoutItem(float weight, float minSize, float maxSize)
{
    CguiNode *node = CguiCreateNodePro(TextFormat("CguiLinearLayoutItem #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_LINEAR_ITEM, NULL, sizeof(CguiLinearLayoutItemData));
    if (!node)
    {
        return NULL;
    }

    CguiLinearLayoutItemData data = { .weight = weight, .minSize = minSize, .maxSize = maxSize };
    memcpy(node->data, &data, sizeof(CguiLinearLayoutItemData));

    return node;
}

CguiNode *CguiCreateGridLayout(int xSlotsCount, int ySlotsCount, int xJustify, int yJustify, Vector2 spacing)
{
    if (xSlotsCount < 0 || ySlotsCount < 0)
    {
        return NULL;
    }

    CguiNode *node = CguiCreateNodePro(TextFormat("CguiGridLayout #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_GRID, NULL, sizeof(CguiGridLayoutData));
    if (!node)
    {
        return NULL;
    }

    CguiGridLayoutSlotData *xSlots = NULL;
    if (xSlotsCount > 0)
    {
        xSlots = CG_MALLOC(sizeof(CguiGridLayoutSlotData) * xSlotsCount);
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
        ySlots = CG_MALLOC(sizeof(CguiGridLayoutSlotData) * ySlotsCount);
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
    if (!node || node->type != CGUI_LAYOUT_NODE_TYPE_GRID)
    {
        return;
    }

    CguiGridLayoutData layoutData = *(CguiGridLayoutData *) node->data;

    if (layoutData.xSlots)
    {
        CG_FREE_NULL(layoutData.xSlots);
    }

    if (layoutData.ySlots)
    {
        CG_FREE_NULL(layoutData.ySlots);
    }
}

static bool CguiGridLayoutInsertSlotGeneric(CguiGridLayoutSlotData **slots, int *count, int *capacity, int index, float weight, float minSize, float maxSize)
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

static bool CguiGridLayoutRemoveSlotGeneric(CguiGridLayoutSlotData **slots, int *count, int *capacity, int index)
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
    return CguiGridLayoutInsertSlotGeneric(&layoutData->xSlots, &layoutData->xSlotsCount, &layoutData->xSlotsCapacity, index, weight, minSize, maxSize);
}

bool CguiGridLayoutRemoveSlotX(CguiGridLayoutData *layoutData, int index)
{
    return CguiGridLayoutRemoveSlotGeneric(&layoutData->xSlots, &layoutData->xSlotsCount, &layoutData->xSlotsCapacity, index);
}

bool CguiGridLayoutInsertSlotY(CguiGridLayoutData *layoutData, int index, float weight, float minSize, float maxSize)
{
    return CguiGridLayoutInsertSlotGeneric(&layoutData->ySlots, &layoutData->ySlotsCount, &layoutData->ySlotsCapacity, index, weight, minSize, maxSize);
}

bool CguiGridLayoutRemoveSlotY(CguiGridLayoutData *layoutData, int index)
{
    return CguiGridLayoutRemoveSlotGeneric(&layoutData->ySlots, &layoutData->ySlotsCount, &layoutData->ySlotsCapacity, index);
}

CguiNode *CguiCreateGridLayoutItem(int xSlot, int ySlot, int xSpan, int ySpan)
{
    CguiNode *node = CguiCreateNodePro(TextFormat("CguiGridLayoutItem #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_GRID_ITEM, NULL, sizeof(CguiGridLayoutItemData));
    if (!node)
    {
        return NULL;
    }

    CguiGridLayoutItemData data = { .xSlot = xSlot, .ySlot = ySlot, .xSpan = xSpan, .ySpan = ySpan };
    memcpy(node->data, &data, sizeof(CguiGridLayoutItemData));

    return node;
}

CguiNode *CguiCreateFlowLayout(int direction, Vector2 spacing)
{
    CguiNode *node = CguiCreateNodePro(TextFormat("CguiFlowLayout #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_FLOW, NULL, sizeof(CguiFlowLayoutData));
    if (!node)
    {
        return NULL;
    }

    CguiFlowLayoutData data = { .direction = direction, .spacing = spacing };
    memcpy(node->data, &data, sizeof(CguiFlowLayoutData));

    node->transform = CguiTransformFlowLayout;

    return node;
}

CguiNode *CguiCreateFlowLayoutItem(float grow, float shrink, Vector2 minSize, Vector2 maxSize)
{
    CguiNode *node = CguiCreateNodePro(TextFormat("CguiFlowLayoutItem #%d", ++cguiNameCounter), CGUI_LAYOUT_NODE_TYPE_FLOW_ITEM, NULL, sizeof(CguiFlowLayoutItemData));
    if (!node)
    {
        return NULL;
    }

    CguiFlowLayoutItemData data = { .grow = grow, .shrink = shrink, .minSize = minSize, .maxSize = maxSize };
    memcpy(node->data, &data, sizeof(CguiFlowLayoutItemData));

    return node;
}

bool CguiTransformClampLayout(CguiNode *node)
{
    if (!node || node->type != CGUI_LAYOUT_NODE_TYPE_CLAMP)
    {
        return false;
    }

    CguiClampLayoutData layoutData = *(CguiClampLayoutData *) node->data;
    (void) layoutData; // We don't have any data in clamp layout

    Rectangle pBounds = node->bounds;
    float     pAspect = pBounds.height != 0.0f ? pBounds.width / pBounds.height : 1.0f;

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiNode *child = node->children[i];
        if (child->type != CGUI_LAYOUT_NODE_TYPE_CLAMP_ITEM)
        {
            continue;
        }

        CguiClampLayoutItemData itemData = *(CguiClampLayoutItemData *) child->data;
        CguiTransformation      t        = { 0 };

        t.isRelativePosition = (Vector2) { 1.0f, 1.0f };
        t.size               = (Vector2) { pBounds.width, pBounds.height };

        t.size.x = Clamp(t.size.x, itemData.minSize.x, itemData.maxSize.x);
        t.size.y = Clamp(t.size.y, itemData.minSize.y, itemData.maxSize.y);

        // Aspect ratio calculation
        if (itemData.preserveAspectRatio)
        {
            if (itemData.clampFill != pAspect > itemData.aspectRatio)
            {
                t.size.x = t.size.y * itemData.aspectRatio;
            }
            else
            {
                t.size.y = t.size.x / itemData.aspectRatio;
            }
        }

        t.size.x = Clamp(t.size.x, itemData.minSize.x, itemData.maxSize.x);
        t.size.y = Clamp(t.size.y, itemData.minSize.y, itemData.maxSize.y);

        t.position.x = (pBounds.width - t.size.x) / 2.0f;
        t.position.y = (pBounds.height - t.size.y) / 2.0f;

        // Optimization: Only recalculate bounds if changed
        if (!CguiIsTransformationEqual(t, child->transformation))
        {
            child->transformation = t;
            child->rebound        = true;
        }
    }

    return false;
}

bool CguiTransformLinearLayout(CguiNode *node)
{
    if (!node || node->type != CGUI_LAYOUT_NODE_TYPE_LINEAR)
    {
        return false;
    }

    CguiLinearLayoutData layoutData = *(CguiLinearLayoutData *) node->data;

    Rectangle pBounds      = node->bounds;
    bool      isHorizontal = layoutData.direction == CGUI_LAYOUT_DIRECTION_X;
    float     totalSize    = isHorizontal ? pBounds.width : pBounds.height;

    // Pass 1: Total weight

    float totalWeight = 0.0f;
    int   itemsCount  = 0;

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiNode *child = node->children[i];
        if (child->type != CGUI_LAYOUT_NODE_TYPE_LINEAR_ITEM)
        {
            continue;
        }

        CguiLinearLayoutItemData itemData = *(CguiLinearLayoutItemData *) child->data;

        totalWeight += itemData.weight;
        itemsCount++;
    }

    // No further calculations (also avoids division by zero)
    if (itemsCount == 0)
    {
        return false;
    }

    bool noWeight = totalWeight == 0.0f;
    if (noWeight) totalWeight = itemsCount;

    float totalSpacing = itemsCount > 1 ? layoutData.spacing * (itemsCount - 1) : 0.0f;

    // Pass 2: Used space

    float usedSize = 0.0f;

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiNode *child = node->children[i];
        if (child->type != CGUI_LAYOUT_NODE_TYPE_LINEAR_ITEM)
        {
            continue;
        }

        CguiLinearLayoutItemData itemData = *(CguiLinearLayoutItemData *) child->data;

        float weight = noWeight ? 1.0f : itemData.weight;
        float size   = weight / totalWeight * (totalSize - totalSpacing);
        size         = Clamp(size, itemData.minSize, itemData.maxSize);
        usedSize += size;
    }

    // Justification

    float leftover = totalSize - usedSize;
    float cursor   = 0.0f;
    float spacing  = layoutData.spacing;

    switch (layoutData.justify)
    {
        case CGUI_LAYOUT_JUSTIFY_BEGIN:
            cursor = 0.0f;
            break;
        case CGUI_LAYOUT_JUSTIFY_CENTER:
            cursor = leftover / 2.0f;
            break;
        case CGUI_LAYOUT_JUSTIFY_END:
            cursor = leftover;
            break;
        case CGUI_LAYOUT_JUSTIFY_SPACE_BETWEEN:
            spacing      = (itemsCount > 1) ? (leftover / (itemsCount - 1)) : 0.0f;
            cursor       = 0.0f;
            totalSpacing = itemsCount > 1 ? spacing * (itemsCount - 1) : 0.0f;
            break;
        default:
            break;
    }

    // Pass 3: Apply

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiNode *child = node->children[i];
        if (child->type != CGUI_LAYOUT_NODE_TYPE_LINEAR_ITEM)
        {
            continue;
        }

        CguiLinearLayoutItemData itemData = *(CguiLinearLayoutItemData *) child->data;
        CguiTransformation       t        = { 0 };
        t.isRelativePosition              = (Vector2) { 1.0f, 1.0f };

        float weight = noWeight ? 1.0f : itemData.weight;
        float size   = weight / totalWeight * (totalSize - totalSpacing);
        size         = Clamp(size, itemData.minSize, itemData.maxSize);

        if (isHorizontal)
        {
            t.position.x = cursor;
            t.size.x     = size;
            t.size.y     = pBounds.height;
        }
        else
        {
            t.position.y = cursor;
            t.size.y     = size;
            t.size.x     = pBounds.width;
        }

        cursor += size + spacing;

        // Optimization: Only recalculate bounds if changed
        if (!CguiIsTransformationEqual(t, child->transformation))
        {
            child->transformation = t;
            child->rebound        = true;
        }
    }

    return false; // Layout size itself never changes, return false
}

bool CguiTransformGridLayout(CguiNode *node)
{
    if (!node || node->type != CGUI_LAYOUT_NODE_TYPE_GRID)
    {
        return false;
    }

    CguiGridLayoutData layoutData = *(CguiGridLayoutData *) node->data;

    // No further calculations (also avoids division by zero)
    if (layoutData.xSlotsCount == 0 || layoutData.ySlotsCount == 0)
    {
        return false;
    }

    Rectangle pBounds   = node->bounds;
    Vector2   totalSize = { pBounds.width, pBounds.height };

    // Pass 1: Total weight

    Vector2 totalWeight = { 0.0f, 0.0f };

    for (int i = 0; i < layoutData.xSlotsCount; i++)
    {
        CguiGridLayoutSlotData slotData = layoutData.xSlots[i];
        totalWeight.x += slotData.weight;
    }

    for (int i = 0; i < layoutData.ySlotsCount; i++)
    {
        CguiGridLayoutSlotData slotData = layoutData.ySlots[i];
        totalWeight.y += slotData.weight;
    }

    bool xNoWeight = totalWeight.x == 0.0f;
    bool yNoWeight = totalWeight.y == 0.0f;
    if (xNoWeight) totalWeight.x = layoutData.xSlotsCount;
    if (yNoWeight) totalWeight.y = layoutData.ySlotsCount;

    Vector2 totalSpacing = Vector2Multiply(layoutData.spacing, (Vector2) { layoutData.xSlotsCount - 1, layoutData.ySlotsCount - 1 });

    // Pass 2: Used space

    Vector2 usedSize = { 0.0f, 0.0f };

    for (int i = 0; i < layoutData.xSlotsCount; i++)
    {
        CguiGridLayoutSlotData slotData = layoutData.xSlots[i];

        float weight = xNoWeight ? 1.0f : slotData.weight;
        float size   = weight / totalWeight.x * (totalSize.x - totalSpacing.x);
        size         = Clamp(size, slotData.minSize, slotData.maxSize);
        usedSize.x += size;
    }

    for (int i = 0; i < layoutData.ySlotsCount; i++)
    {
        CguiGridLayoutSlotData slotData = layoutData.ySlots[i];

        float weight = yNoWeight ? 1.0f : slotData.weight;
        float size   = weight / totalWeight.y * (totalSize.y - totalSpacing.y);
        size         = Clamp(size, slotData.minSize, slotData.maxSize);
        usedSize.y += size;
    }

    // Justification

    Vector2 leftover = Vector2Subtract(totalSize, usedSize);
    Vector2 cursor   = { 0.0f, 0.0f };
    Vector2 spacing  = layoutData.spacing;

    switch (layoutData.xJustify)
    {
        case CGUI_LAYOUT_JUSTIFY_BEGIN:
            cursor.x = 0.0f;
            break;
        case CGUI_LAYOUT_JUSTIFY_CENTER:
            cursor.x = leftover.x / 2.0f;
            break;
        case CGUI_LAYOUT_JUSTIFY_END:
            cursor.x = leftover.x;
            break;
        case CGUI_LAYOUT_JUSTIFY_SPACE_BETWEEN:
            spacing.x      = leftover.x / (layoutData.xSlotsCount - 1);
            cursor.x       = 0.0f;
            totalSpacing.x = spacing.x * (layoutData.xSlotsCount - 1);
            break;
        default:
            break;
    }

    switch (layoutData.yJustify)
    {
        case CGUI_LAYOUT_JUSTIFY_BEGIN:
            cursor.y = 0.0f;
            break;
        case CGUI_LAYOUT_JUSTIFY_CENTER:
            cursor.y = leftover.y / 2.0f;
            break;
        case CGUI_LAYOUT_JUSTIFY_END:
            cursor.y = leftover.y;
            break;
        case CGUI_LAYOUT_JUSTIFY_SPACE_BETWEEN:
            spacing.y      = leftover.y / (layoutData.ySlotsCount - 1);
            cursor.y       = 0.0f;
            totalSpacing.y = spacing.y * (layoutData.ySlotsCount - 1);
            break;
        default:
            break;
    }

    // Pass 3: Apply

    for (int i = 0; i < layoutData.xSlotsCount; i++)
    {
        CguiGridLayoutSlotData *slotData = &layoutData.xSlots[i]; // By reference

        float weight = xNoWeight ? 1.0f : slotData->weight;
        float size   = weight / totalWeight.x * (totalSize.x - totalSpacing.x);
        size         = Clamp(size, slotData->minSize, slotData->maxSize);

        slotData->position = cursor.x;
        slotData->size     = size;

        cursor.x += size + spacing.x;
    }

    for (int i = 0; i < layoutData.ySlotsCount; i++)
    {
        CguiGridLayoutSlotData *slotData = &layoutData.ySlots[i]; // By reference

        float weight = yNoWeight ? 1.0f : slotData->weight;
        float size   = weight / totalWeight.y * (totalSize.y - totalSpacing.y);
        size         = Clamp(size, slotData->minSize, slotData->maxSize);

        slotData->position = cursor.y;
        slotData->size     = size;

        cursor.y += size + spacing.y;
    }

    // Add grid items

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiNode *child = node->children[i];
        if (child->type != CGUI_LAYOUT_NODE_TYPE_GRID_ITEM)
        {
            continue;
        }

        CguiGridLayoutItemData itemData = *(CguiGridLayoutItemData *) child->data;
        CguiTransformation     t        = { 0 };
        t.isRelativePosition            = (Vector2) { 1.0f, 1.0f };

        // Invalid slot indices
        if (itemData.xSlot < 0 || itemData.xSlot >= layoutData.xSlotsCount ||
            itemData.ySlot < 0 || itemData.ySlot >= layoutData.ySlotsCount ||
            itemData.xSpan <= 0 || itemData.xSlot + itemData.xSpan > layoutData.xSlotsCount ||
            itemData.ySpan <= 0 || itemData.ySlot + itemData.ySpan > layoutData.ySlotsCount)
        {
            // Optimization: Only recalculate bounds if changed
            if (!CguiIsTransformationEqual(t, child->transformation))
            {
                child->transformation = t;
                child->rebound        = true;
            }
            continue;
        }

        t.position.x = layoutData.xSlots[itemData.xSlot].position;
        t.position.y = layoutData.ySlots[itemData.ySlot].position;
        t.size.x     = layoutData.xSlots[itemData.xSlot + itemData.xSpan - 1].position + layoutData.xSlots[itemData.xSlot + itemData.xSpan - 1].size - t.position.x;
        t.size.y     = layoutData.ySlots[itemData.ySlot + itemData.ySpan - 1].position + layoutData.ySlots[itemData.ySlot + itemData.ySpan - 1].size - t.position.y;

        // Optimization: Only recalculate bounds if changed
        if (!CguiIsTransformationEqual(t, child->transformation))
        {
            child->transformation = t;
            child->rebound        = true;
        }
    }

    return false; // Layout size itself never changes, return false
}

bool CguiTransformFlowLayout(CguiNode *node)
{
    if (!node || node->type != CGUI_LAYOUT_NODE_TYPE_FLOW)
    {
        return false;
    }

    CguiFlowLayoutData layoutData = *(CguiFlowLayoutData *) node->data;

    Rectangle pBounds = node->bounds;

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiNode *child = node->children[i];
        if (child->type != CGUI_LAYOUT_NODE_TYPE_FLOW_ITEM)
        {
            continue;
        }

        CguiFlowLayoutItemData itemData = *(CguiFlowLayoutItemData *) child->data;
        CguiTransformation     t        = { 0 };

        // Optimization: Only recalculate bounds if changed
        if (!CguiIsTransformationEqual(t, child->transformation))
        {
            child->transformation = t;
            child->rebound        = true;
        }
    }

    return false; // Layout size itself never changes, return false
}
