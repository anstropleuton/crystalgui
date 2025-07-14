/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A modern GUI framework for raylib.
///
/// This source file contains implementations for node functionality of
/// Crystal GUI.
///
/// This project is licensed under the terms of MIT license.

#include <string.h>

#include "crystalgui.h"
#include "raylib.h"

int       cguiNameCounter = 0;
CguiNode *cguiFocusedNode = NULL;

// Node management

CguiNode *CguiCreateNode(void)
{
    return CguiCreateNodeEx(TextFormat("CguiUnnamedNode #%d", ++cguiNameCounter));
}

CguiNode *CguiCreateNodeEx(const char *name)
{
    if (!name)
    {
        name = TextFormat("CguiUnnamedNode #%d", ++cguiNameCounter);
    }

    CguiNode *node = CG_MALLOC(sizeof(CguiNode));
    if (!node)
    {
        return NULL;
    }

    node->name = CG_MALLOC(strlen(name) + 1);
    if (!node->name)
    {
        CG_FREE_NULL(node);
        return NULL;
    }

    strcpy(node->name, name);

    node->enabled = true;
    node->visible = true;
    node->rebound = true;

    return node;
}

CguiNode *CguiCreateNodePro(const char *name, int type, const void *data, int dataSize)
{
    if (!name)
    {
        name = TextFormat("CguiUnnamedNode #%d", ++cguiNameCounter);
    }

    CguiNode *node = CguiCreateNodeEx(name);
    if (!node)
    {
        return NULL;
    }

    node->data = CG_MALLOC(dataSize);
    if (!node->data)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    node->type     = type;
    node->dataSize = dataSize;
    if (data) memcpy(node->data, data, dataSize);

    return node;
}

void CguiDeleteNode(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->children)
    {
        for (int i = node->childrenCount - 1; i >= 0; i--)
        {
            node->children[i]->parent = NULL; // Optimization: reduce unnecessary searching for child in current and unnecessary reallocations when deleting
            CguiDeleteNode(node->children[i]);
        }

        CG_FREE_NULL(node->children);
        node->childrenCount    = 0;
        node->childrenCapacity = 0;
    }

    CguiDeleteNodeSelf(node);
}

void CguiDeleteNodeSelf(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->deleteNodeData)
    {
        node->deleteNodeData(node);
    }

    // Detach from parent if it is still attached
    if (node->parent)
    {
        CguiRemoveChild(node->parent, node);
        node->parent = NULL;
    }

    if (node->name)
    {
        CG_FREE_NULL(node->name);
    }

    if (node->data)
    {
        CG_FREE_NULL(node->data);
        node->dataSize = 0;
    }

    CG_FREE_NULL(node);
}

bool CguiRenameNode(CguiNode *node, const char *newName)
{
    if (!node)
    {
        return false;
    }

    char *name = CG_REALLOC(node->name, strlen(newName) + 1);
    if (!name)
    {
        return false;
    }

    node->name = name;
    strcpy(node->name, newName);
    return true;
}

void CguiTransformNode(CguiNode *node, bool rebound)
{
    if (!node)
    {
        return;
    }

    // Rebound propegates down the tree to all children
    rebound |= CguiTransformNodeSelf(node, rebound);

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiTransformNode(node->children[i], rebound);
    }
}

bool CguiTransformNodeSelf(CguiNode *node, bool rebound)
{
    if (!node)
    {
        return false;
    }

    if (node->transform)
    {
        rebound |= node->transform(node);
    }

    rebound |= node->rebound;

    // If node transformation was changed, recalculate bounds
    if (rebound)
    {
        node->bounds  = CguiComputeNodeBounds(node);
        node->rebound = false;
    }

    return rebound;
}

void CguiUpdateNode(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    CguiUpdatePreNodeSelf(node);

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiUpdateNode(node->children[i]);
    }

    CguiUpdatePostNodeSelf(node);
}

void CguiUpdatePreNodeSelf(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->updatePre)
    {
        node->updatePre(node);
    }
}

void CguiUpdatePostNodeSelf(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->updatePost)
    {
        node->updatePost(node);
    }
}

void CguiDrawNode(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    CguiDrawPreNodeSelf(node);

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiDrawNode(node->children[i]);
    }

    CguiDrawPostNodeSelf(node);
}

void CguiDrawPreNodeSelf(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->drawPre)
    {
        node->drawPre(node);
    }
}

void CguiDrawPostNodeSelf(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->drawPost)
    {
        node->drawPost(node);
    }
}

void CguiDebugDrawNode(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    CguiDebugDrawNodeSelf(node);

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiDebugDrawNode(node->children[i]);
    }
}

void CguiDebugDrawNodeSelf(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->debugDraw)
    {
        node->debugDraw(node);
    }
    else
    {
        // Draw basic debug info
        DrawRectangleLinesEx(node->bounds, 1.0f, GRAY);
        DrawText(TextFormat("%s", node->name), node->bounds.x, node->bounds.y - 10, 10, GRAY);
        DrawText(TextFormat("%.0f, %.0f, %.0f, %.0f", node->bounds.x, node->bounds.y, node->bounds.width, node->bounds.height), node->bounds.x, node->bounds.y, 10, GRAY);
    }
}

CguiNode *CguiCloneNode(CguiNode *node)
{
    if (!node)
    {
        return NULL;
    }

    CguiNode *newNode = CguiCloneNodeSelf(node);
    if (!newNode)
    {
        return NULL;
    }

    if (node->children && node->childrenCount > 0)
    {
        newNode->children = CG_MALLOC(sizeof(CguiNode *) * node->childrenCount);
        if (!newNode->children)
        {
            CguiDeleteNode(newNode);
            return NULL;
        }

        newNode->childrenCount    = node->childrenCount;
        newNode->childrenCapacity = node->childrenCount;

        for (int i = 0; i < node->childrenCount; i++)
        {
            newNode->children[i] = CguiCloneNode(node->children[i]);
            if (!newNode->children[i])
            {
                CguiDeleteNode(newNode);
                return NULL;
            }

            newNode->children[i]->parent = newNode;
        }
    }

    return newNode;
}

CguiNode *CguiCloneNodeSelf(CguiNode *node)
{
    if (!node)
    {
        return NULL;
    }

    CguiNode *newNode = CguiCreateNodeEx(TextFormat("%s (Clone #%d)", node->name, ++cguiNameCounter));
    if (!newNode)
    {
        return NULL;
    }

    if (!CguiCopyNodeValues(node, newNode))
    {
        CguiDeleteNode(newNode);
        return NULL;
    }

    return newNode;
}

// Node children management

CguiNode *CguiCreateChild(CguiNode *parent)
{
    if (!parent)
    {
        return NULL;
    }

    CguiNode *node = CguiCreateNode();
    if (!node)
    {
        return NULL;
    }

    if (!CguiInsertChild(parent, node))
    {
        CguiDeleteNode(node);
        return NULL;
    }

    return node;
}

CguiNode *CguiCreateChildAt(CguiNode *parent, int childIndex)
{
    if (!parent || childIndex < 0 || childIndex > parent->childrenCount)
    {
        return NULL;
    }

    CguiNode *node = CguiCreateNode();
    if (!node)
    {
        return NULL;
    }

    if (!CguiInsertChildAt(parent, node, childIndex))
    {
        CguiDeleteNode(node);
        return NULL;
    }

    return node;
}

bool CguiInsertChild(CguiNode *parent, CguiNode *child)
{
    return CguiInsertChildAt(parent, child, parent->childrenCount);
}

bool CguiInsertChildAt(CguiNode *parent, CguiNode *child, int childIndex)
{
    if (!parent || !child || childIndex < 0 || childIndex > parent->childrenCount)
    {
        return false;
    }

    // Prevent duplicates
    int foundChildIndex = CguiFindChildIndex(parent, child);
    if (foundChildIndex != -1)
    {
        return false;
    }

    // Resize capacity if full
    if (parent->childrenCount == parent->childrenCapacity)
    {
        if (!CguiSetChildrenCapacity(parent, parent->childrenCapacity * 2))
        {
            return false;
        }
    }

    // Shift elements right to insert element
    memmove(&parent->children[childIndex + 1], &parent->children[childIndex], sizeof(CguiNode *) * (parent->childrenCount - childIndex));

    parent->children[childIndex] = child;
    parent->childrenCount++;
    child->parent = parent;

    return true;
}

bool CguiRemoveChild(CguiNode *parent, CguiNode *child)
{
    int foundChildIndex = CguiFindChildIndex(parent, child);
    if (foundChildIndex == -1)
    {
        return false;
    }

    return CguiRemoveChildAt(parent, foundChildIndex);
}

bool CguiRemoveChildAt(CguiNode *parent, int childIndex)
{
    if (!parent || childIndex < 0 || childIndex >= parent->childrenCount)
    {
        return false;
    }

    parent->children[childIndex]->parent = NULL;

    // Shift elements left to remove element
    memmove(&parent->children[childIndex], &parent->children[childIndex + 1], sizeof(CguiNode *) * (parent->childrenCount - childIndex));
    parent->childrenCount--;

    // Reduce capacity if < 25% used
    if (parent->childrenCapacity > 1 && parent->childrenCount < parent->childrenCapacity / 4)
    {
        // Ignore reallocation failure
        CguiSetChildrenCapacity(parent, parent->childrenCapacity / 2);
    }

    return true;
}

bool CguiDeleteChild(CguiNode *parent, CguiNode *child)
{
    int childIndex = CguiFindChildIndex(parent, child);
    if (childIndex == -1)
    {
        return false;
    }

    return CguiDeleteChildAt(parent, childIndex);
}

bool CguiDeleteChildAt(CguiNode *parent, int childIndex)
{
    if (!parent || childIndex < 0 || childIndex >= parent->childrenCount)
    {
        return false;
    }

    CguiNode *child = parent->children[childIndex];

    if (!CguiRemoveChildAt(parent, childIndex))
    {
        return false;
    }

    CguiDeleteNode(child);
    return true;
}

bool CguiRemoveAllChildren(CguiNode *parent)
{
    if (!parent || !parent->children)
    {
        return false;
    }

    for (int i = 0; i < parent->childrenCount; i++)
    {
        parent->children[i]->parent = NULL;
        parent->children[i]         = NULL;
    }

    parent->childrenCount = 0;

    // Optimization: Preserve capacity as-is
    return true;
}

bool CguiDeleteAllChildren(CguiNode *parent)
{
    if (!parent || parent->childrenCount == 0)
    {
        return false;
    }

    for (int i = parent->childrenCount - 1; i >= 0; i--)
    {
        parent->children[i]->parent = NULL; // Optimization: reduce unnecessary searching for child in current and unnecessary reallocations when deleting
        CguiDeleteNode(parent->children[i]);
        parent->children = NULL;
    }

    parent->childrenCount = 0;

    // Optimization: Preserve capacity as-is
    return true;
}

bool CguiSetChildrenCapacity(CguiNode *node, int newCapacity)
{
    if (!node || newCapacity < node->childrenCount)
    {
        return false;
    }

    // At least one capacity needed
    if (newCapacity == 0)
    {
        newCapacity = 1;
    }

    CguiNode **newChildren = CG_REALLOC(node->children, sizeof(CguiNode *) * newCapacity);
    if (!newChildren)
    {
        return false;
    }

    node->children         = newChildren;
    node->childrenCapacity = newCapacity;
    return true;
}

bool CguiReserveChildrenCapacity(CguiNode *node, int capacity)
{
    return CguiSetChildrenCapacity(node, node->childrenCapacity + capacity);
}

bool CguiShrinkChildrenCapacity(CguiNode *node)
{
    return CguiSetChildrenCapacity(node, node->childrenCount);
}

bool CguiCloneAllChildren(CguiNode *fromParent, CguiNode *toParent)
{
    if (!fromParent || !toParent)
    {
        return false;
    }

    bool result = true;
    for (int i = 0; i < fromParent->childrenCount; i++)
    {
        result &= CguiInsertChild(toParent, CguiCloneNode(fromParent->children[i]));
    }

    return result;
}

bool CguiCloneChildrenRange(CguiNode *fromParent, int indexBegin, int indexEnd, CguiNode *toParent)
{
    if (!fromParent || !toParent ||
        indexBegin < 0 || indexBegin >= fromParent->childrenCount ||
        indexEnd < 0 || indexEnd >= fromParent->childrenCount ||
        indexBegin >= indexEnd)
    {
        return false;
    }

    bool result = true;
    for (int i = indexBegin; i <= indexEnd; i++)
    {
        result &= CguiInsertChild(toParent, CguiCloneNode(fromParent->children[i]));
    }

    return result;
}

bool CguiTransferChild(CguiNode *fromParent, CguiNode *child, CguiNode *toParent)
{
    if (!fromParent || !child || !toParent)
    {
        return false;
    }

    if (!CguiInsertChild(toParent, child))
    {
        return false;
    }

    if (!CguiRemoveChild(fromParent, child))
    {
        CguiRemoveChild(toParent, child);
        return false;
    }

    return true;
}

bool CguiTransferChildAt(CguiNode *fromParent, int childIndex, CguiNode *toParent)
{
    if (!fromParent || childIndex < 0 || childIndex >= fromParent->childrenCount || !toParent)
    {
        return false;
    }

    if (!CguiInsertChild(toParent, fromParent->children[childIndex]))
    {
        return false;
    }

    if (!CguiRemoveChildAt(fromParent, childIndex))
    {
        CguiRemoveChild(toParent, fromParent->children[childIndex]);
        return false;
    }

    return true;
}

bool CguiTransferAllChildren(CguiNode *fromParent, CguiNode *toParent)
{
    if (!fromParent || !toParent)
    {
        return false;
    }

    bool result = true;
    for (int i = 0; i < fromParent->childrenCount; i++)
    {
        result &= CguiInsertChild(toParent, CguiCloneNode(fromParent->children[i]));
    }

    return result;
}

bool CguiTransferChildrenRange(CguiNode *fromParent, int indexBegin, int indexEnd, CguiNode *toParent)
{
    if (!fromParent || !toParent ||
        indexBegin < 0 || indexBegin >= fromParent->childrenCount ||
        indexEnd < 0 || indexEnd >= fromParent->childrenCount ||
        indexBegin >= indexEnd)
    {
        return false;
    }

    bool result = true;
    for (int i = indexBegin; i <= indexEnd; i++)
    {
        result &= CguiInsertChild(toParent, CguiCloneNode(fromParent->children[i]));
    }

    return result;
}

// Transformation helpers

CguiTransformation CguiTAbsolute(Vector2 pos, Vector2 size)
{
    return (CguiTransformation) {
        .position           = pos,
        .size               = size,
        .isRelativePosition = { 0.0f, 0.0f },
        .isRelativeSize     = { 0.0f, 0.0f },
        .anchor             = { 0.0f, 0.0f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTOffset(Vector2 offset, Vector2 size)
{
    return (CguiTransformation) {
        .position           = offset,
        .size               = size,
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 0.0f, 0.0f },
        .anchor             = { 0.0f, 0.0f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTScale(Vector2 pos, Vector2 scale)
{
    return (CguiTransformation) {
        .position           = pos,
        .size               = scale,
        .isRelativePosition = { 0.0f, 0.0f },
        .isRelativeSize     = { 1.0f, 1.0f },
        .anchor             = { 0.0f, 0.0f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTOffsetScale(Vector2 offset, Vector2 scale)
{
    return (CguiTransformation) {
        .position           = offset,
        .size               = scale,
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 1.0f, 1.0f },
        .anchor             = { 0.0f, 0.0f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTCentered(Vector2 size)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f },
        .size               = size,
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 0.0f, 0.0f },
        .anchor             = { 0.5f, 0.5f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTPadded(float padding)
{
    return (CguiTransformation) {
        .position           = { 0.0f,           0.0f           },
        .size               = { 1.0f,           1.0f           },
        .isRelativePosition = { 0.0f,           0.0f           },
        .isRelativeSize     = { 1.0f,           1.0f           },
        .anchor             = { 0.0f,           0.0f           },
        .shrink             = { padding * 2.0f, padding * 2.0f }
    };
}

bool CguiIsTransformationEqual(CguiTransformation a, CguiTransformation b)
{
    return (a.position.x == b.position.x && a.position.y == b.position.y) &&
           (a.size.x == b.size.x && a.size.y == b.size.y) &&
           (a.isRelativePosition.x == b.isRelativePosition.x && a.isRelativePosition.y == b.isRelativePosition.y) &&
           (a.isRelativeSize.x == b.isRelativeSize.x && a.isRelativeSize.y == b.isRelativeSize.y) &&
           (a.anchor.x == b.anchor.x && a.anchor.y == b.anchor.y) &&
           (a.shrink.x == b.shrink.x && a.shrink.y == b.shrink.y);
}

// Misc.

bool CguiCopyNodeValues(CguiNode *fromNode, CguiNode *toNode)
{
    if (!fromNode || !toNode)
    {
        return false;
    }

    CguiNode copyNode = *fromNode;

    // Exclude name and hierarchy from copy
    copyNode.name             = toNode->name;
    copyNode.parent           = toNode->parent;
    copyNode.children         = toNode->children;
    copyNode.childrenCount    = toNode->childrenCount;
    copyNode.childrenCapacity = toNode->childrenCapacity;

    if (fromNode->data && fromNode->dataSize > 0)
    {
        copyNode.data = CG_MALLOC(fromNode->dataSize);
        if (!copyNode.data)
        {
            return false;
        }

        memcpy(copyNode.data, fromNode->data, fromNode->dataSize);
        copyNode.dataSize = fromNode->dataSize;
    }

    if (toNode->data)
    {
        CG_FREE_NULL(toNode->data);
        toNode->dataSize = 0;
    }

    *toNode = copyNode;

    return true;
}

int CguiFindChildIndex(CguiNode *parent, CguiNode *child)
{
    if (!parent || !child)
    {
        return -1;
    }

    for (int i = 0; i < parent->childrenCount; i++)
    {
        if (parent->children[i] == child)
        {
            return i;
        }
    }

    return -1;
}

bool CguiIsDescendantOf(CguiNode *parent, CguiNode *child)
{
    return CguiIsAncestorOf(child, parent);
}

bool CguiIsAncestorOf(CguiNode *child, CguiNode *parent)
{
    if (!parent || !child)
    {
        return false;
    }

    if (child->parent == parent)
    {
        return true;
    }

    if (child->parent)
    {
        return CguiIsAncestorOf(child->parent, parent);
    }

    return false;
}

CguiNode *CguiFindTypeInChildren(CguiNode *parent, int type)
{
    if (!parent)
    {
        return NULL;
    }

    if (parent->type == type)
    {
        return parent;
    }

    for (int i = 0; i < parent->childrenCount; i++)
    {
        CguiNode *found = CguiFindTypeInChildren(parent->children[i], type);
        if (found)
        {
            return found;
        }
    }

    return NULL;
}

CguiNode *CguiFindTypeInParents(CguiNode *child, int type)
{
    if (!child)
    {
        return NULL;
    }

    if (child->type == type)
    {
        return child;
    }

    CguiNode *found = CguiFindTypeInParents(child->parent, type);
    if (found)
    {
        return found;
    }

    return NULL;
}

Rectangle CguiComputeNodeBounds(CguiNode *node)
{
    if (!node)
    {
        return CguiRecZero();
    }

    Rectangle bounds  = CguiRecZero();
    Rectangle pBounds = node->parent ? node->parent->bounds : CguiGetAppSizeRec();

    CguiTransformation t = node->transformation;

    bounds.width  = (t.size.x * (pBounds.width - t.shrink.x)) * t.isRelativeSize.x + t.size.x * (1.0f - t.isRelativeSize.x);
    bounds.height = (t.size.y * (pBounds.height - t.shrink.y)) * t.isRelativeSize.y + t.size.y * (1.0f - t.isRelativeSize.y);

    bounds.x = (pBounds.x + pBounds.width * t.anchor.x - bounds.width * t.anchor.x + t.position.x) * t.isRelativePosition.x + t.position.x * (1.0f - t.isRelativePosition.x);
    bounds.y = (pBounds.y + pBounds.height * t.anchor.y - bounds.height * t.anchor.y + t.position.y) * t.isRelativePosition.y + t.position.y * (1.0f - t.isRelativePosition.y);

    return bounds;
}

CguiNode *CguiCheckCollision(CguiNode *node, Vector2 point)
{
    if (!node)
    {
        return NULL;
    }

    // Check the deepest collision first
    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiNode *collided = CguiCheckCollision(node->children[i], point);
        if (collided)
        {
            return collided;
        }
    }

    if (CheckCollisionPointRec(point, node->bounds))
    {
        return node;
    }

    return NULL;
}

void CguiRequestFocus(CguiNode *node)
{
    cguiFocusedNode = node;
}

void CguiRequestUnfocus(void)
{
    cguiFocusedNode = NULL;
}

CguiNode *CguiGetFocusedNode(void)
{
    return cguiFocusedNode;
}
