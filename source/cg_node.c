/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This source file contains implementations for basic node.
///
/// This project is licensed under the terms of MIT license.

#include <string.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"

extern int cguiNameCounter;

// Node management

CguiNode *CguiCreateNode(void)
{
    return CguiCreateNodeEx(CguiTZeroSize(), NULL);
}

CguiNode *CguiCreateNodeEx(CguiTransformation transformation, const char *name)
{
    if (!name)
    {
        name = TextFormat("CguiUnnamedNode #%d", ++cguiNameCounter);
    }

    CguiNode *node = CG_MALLOC_NULL(sizeof(CguiNode));
    if (!node)
    {
        return NULL;
    }

    node->name = CG_MALLOC_NULL(strlen(name) + 1);
    if (!node->name)
    {
        CG_FREE_NULL(node);
        return NULL;
    }

    strcpy(node->name, name);

    node->enabled        = true;
    node->rebound        = true;
    node->transformation = transformation;

    CG_LOG_TRACE("Created node: %s", node->name);

    return node;
}

CguiNode *CguiCreateNodePro(CguiTransformation transformation, const char *name, int type, const void *data, int dataSize)
{
    CguiNode *node = CguiCreateNodeEx(transformation, name);
    if (!node)
    {
        return NULL;
    }

    node->type = type;
    if (dataSize > 0)
    {
        node->data = CG_MALLOC_NULL(dataSize);
        if (!node->data)
        {
            CguiDeleteNode(node);
            return NULL;
        }

        node->dataSize = dataSize;
        if (data) memcpy(node->data, data, dataSize);
    }

    return node;
}

CguiNode *CguiCreateNodeProMax(CguiTransformation transformation, const char *name, int type, const void *data, int dataSize, const void *instanceData, int instanceDataSize)
{
    CguiNode *node = CguiCreateNodePro(transformation, name, type, data, dataSize);
    if (!node)
    {
        return NULL;
    }

    node->instanceData = CG_MALLOC_NULL(instanceDataSize);
    if (!node->instanceData)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    node->type             = type;
    node->instanceDataSize = instanceDataSize;
    if (instanceData) memcpy(node->instanceData, instanceData, instanceDataSize);

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

    CG_LOG_TRACE("Deleted node: %s", node->name);

    if (node->deleteNodeData)
    {
        node->deleteNodeData(node);
    }

    // Detach from parent if it is still attached
    if (node->parent)
    {
        CguiRemoveChild(node->parent, node);
    }

    // Unlink from template source if it is still linked
    if (node->templateSource)
    {
        CguiUnlinkTemplate(node);
    }

    CG_FREE_NULL(node->name);

    CG_FREE_NULL(node->data);
    node->dataSize = 0;

    CG_FREE_NULL(node->instanceData);
    node->instanceDataSize = 0;

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
        DrawText(TextFormat("%s [%x]", node->name, node->type), node->bounds.x, node->bounds.y - 10, 10, GRAY);
        DrawText(TextFormat("{%.0f,%.0f,%.0f,%.0f}, B:%c, H:%c:%d, S:%c", node->bounds.x, node->bounds.y, node->bounds.width, node->bounds.height, node->rebound ? 'T' : 'F', node->parent != NULL ? 'T' : 'F', node->childrenCount, node->resync ? 'T' : 'F'), node->bounds.x, node->bounds.y, 10, GRAY);
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
        newNode->children = CG_MALLOC_NULL(sizeof(CguiNode *) * node->childrenCount);
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

    CguiNode *newNode = CguiCreateNodeEx(CguiTZeroSize(), TextFormat("%s (Clone #%d)", node->name, ++cguiNameCounter));
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

CguiNode *CguiCreateInstance(CguiNode *templateNode)
{
    if (!templateNode)
    {
        return NULL;
    }

    CguiNode *instance = CguiCreateInstanceSelf(templateNode);
    if (!instance)
    {
        return NULL;
    }

    // Create instance of children
    // Each children's template source is the template node's children rather
    // than the template node itself
    if (templateNode->children && templateNode->childrenCount > 0)
    {
        instance->children = CG_MALLOC_NULL(sizeof(CguiNode *) * templateNode->childrenCount);
        if (!instance->children)
        {
            CguiDeleteNode(instance);
            return NULL;
        }

        instance->childrenCount    = templateNode->childrenCount;
        instance->childrenCapacity = templateNode->childrenCount;

        for (int i = 0; i < templateNode->childrenCount; i++)
        {
            instance->children[i] = CguiCreateInstance(templateNode->children[i]);
            if (!instance->children[i])
            {
                CguiDeleteNode(instance);
                return NULL;
            }

            instance->children[i]->parent = instance;
        }
    }

    return instance;
}

CguiNode *CguiCreateInstanceSelf(CguiNode *templateNode)
{
    if (!templateNode)
    {
        return NULL;
    }

    // Optimization: prevent copying data twice
    // Instance data is still implanted
    CguiNode *instance = CguiCreateNodeProMax(CguiTZeroSize(), TextFormat("%s (Instance #%d)", templateNode->name, ++cguiNameCounter), templateNode->type, NULL, templateNode->dataSize, templateNode->instanceData, templateNode->instanceDataSize);
    if (!instance)
    {
        return NULL;
    }

    if (!CguiCopyNodeValuesNoTi(templateNode, instance))
    {
        CguiDeleteNode(instance);
        return NULL;
    }

    if (!CguiLinkTemplate(instance, templateNode))
    {
        CguiDeleteNode(instance);
        return NULL;
    }

    return instance;
}

void CguiApplyTemplateResync(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    node->resync = true;

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiApplyTemplateResync(node->children[i]);
    }
}

bool CguiLinkTemplate(CguiNode *node, CguiNode *templateNode)
{
    if (!node || !templateNode)
    {
        return false;
    }

    // Prevent duplicates
    int foundInstanceIndex = CguiFindInstanceIndex(templateNode, node);
    if (foundInstanceIndex != -1)
    {
        return false;
    }

    if (templateNode->instancesCount == templateNode->instancesCapacity)
    {
        if (!CguiSetInstancesCapacity(templateNode, templateNode->instancesCapacity * 2))
        {
            return false;
        }
    }

    // Order does not matter, insert at end
    templateNode->instances[templateNode->instancesCount] = node;
    templateNode->instancesCount++;
    node->templateSource = templateNode;

    return true;
}

bool CguiUnlinkTemplate(CguiNode *node)
{
    if (!node || !node->templateSource)
    {
        return false;
    }

    int foundInstanceIndex = CguiFindInstanceIndex(node->templateSource, node);
    if (foundInstanceIndex == -1)
    {
        return false;
    }

    CguiNode *templateSource = node->templateSource;
    node->templateSource     = NULL;

    // Shift element left to remove element
    memmove(&templateSource->instances[foundInstanceIndex], &templateSource->instances[foundInstanceIndex + 1], sizeof(CguiNode *) * (templateSource->instancesCount - foundInstanceIndex));
    templateSource->instancesCount--;

    // Reduce capacity if < 25% used
    if (templateSource->instancesCapacity > 1 && templateSource->instancesCount < templateSource->instancesCapacity / 4)
    {
        // Ignore reallocation failure
        CguiSetInstancesCapacity(templateSource, templateSource->instancesCapacity / 2);
    }

    return true;
}

bool CguiSetInstancesCapacity(CguiNode *node, int newCapacity)
{
    if (!node || newCapacity < node->instancesCount)
    {
        return false;
    }

    // At least one capacity needed
    if (newCapacity == 0)
    {
        newCapacity = 1;
    }

    CguiNode **newInstances = CG_REALLOC(node->instances, sizeof(CguiNode *) * newCapacity);
    if (!newInstances)
    {
        return false;
    }

    node->instances         = newInstances;
    node->instancesCapacity = newCapacity;
    return true;
}

int CguiFindInstanceIndex(CguiNode *templateSource, CguiNode *instance)
{
    if (!templateSource || !instance)
    {
        return -1;
    }

    for (int i = 0; i < templateSource->instancesCount; i++)
    {
        if (templateSource->instances[i] == instance)
        {
            return i;
        }
    }

    return -1;
}

void CguiSyncInstances(CguiNode *node, bool resync)
{
    if (!node)
    {
        return;
    }

    bool instanceResync = CguiSyncInstancesSelf(node, resync);

    for (int i = 0; i < node->instancesCount; i++)
    {
        CguiSyncInstances(node->instances[i], instanceResync);
    }
}

bool CguiSyncInstancesSelf(CguiNode *node, bool resync)
{
    if (!node)
    {
        return false;
    }

    if (!node->templateSource)
    {
        return resync;
    }

    if (resync)
    {
        if (!CguiCopyNodeValuesNoTi(node->templateSource, node))
        {
            return false;
        }

        if (node->override)
        {
            node->override(node);
        }
    }

    node->templateSource->resync = false;
    return node->resync || resync;
}

void CguiSyncHierarchy(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    CguiSyncInstances(node, node->resync);

    for (int i = 0; i < node->childrenCount; i++)
    {
        CguiSyncHierarchy(node->children[i]);
    }
}

// Node children management

bool CguiInsertChild(CguiNode *parent, CguiNode *child)
{
    if (!parent || !child)
    {
        return false;
    }

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
    parent->rebound = true;
    child->parent   = parent;

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
    parent->rebound = true;

    // Reduce capacity if < 25% used
    if (parent->childrenCapacity > 1 && parent->childrenCount < parent->childrenCapacity / 4)
    {
        // Ignore reallocation failure
        CguiSetChildrenCapacity(parent, parent->childrenCapacity / 2);
    }

    return true;
}

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
    parent->rebound       = true;

    // Optimization: Preserve capacity as-is
    return true;
}

bool CguiDeleteAllChildren(CguiNode *parent)
{
    if (!parent)
    {
        return false;
    }

    // Ignore empty nodes
    if (parent->childrenCount == 0)
    {
        return true;
    }

    for (int i = parent->childrenCount - 1; i >= 0; i--)
    {
        parent->children[i]->parent = NULL; // Optimization: reduce unnecessary searching for child in current and unnecessary reallocations when deleting
        CguiDeleteNode(parent->children[i]);
    }

    parent->childrenCount = 0;
    parent->rebound       = true;

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

CguiNode *CguiInsertChildren(CguiNode *parent, ...)
{
    if (!parent)
    {
        return NULL;
    }

    va_list args;
    va_start(args, parent);
    CguiNode *result = CguiInsertChildrenV(parent, args);
    va_end(args);
    return result;
}

CguiNode *CguiInsertChildrenAt(CguiNode *parent, int childIndex, ...)
{
    if (!parent)
    {
        return NULL;
    }

    va_list args;
    va_start(args, childIndex);
    CguiNode *result = CguiInsertChildrenAtV(parent, childIndex, args);
    va_end(args);
    return result;
}

CguiNode *CguiInsertChildrenV(CguiNode *parent, va_list args)
{
    if (!parent)
    {
        return NULL;
    }

    return CguiInsertChildrenAtV(parent, parent->childrenCount, args);
}

CguiNode *CguiInsertChildrenAtV(CguiNode *parent, int childIndex, va_list args)
{
    if (!parent)
    {
        return NULL;
    }

    bool      result = true;
    CguiNode *child  = NULL;

    for (int i = 0; (child = va_arg(args, CguiNode *)) != NULL; i++)
    {
        result &= CguiInsertChildAt(parent, child, childIndex + i);
    }

    return result ? parent : NULL;
}

// Transformation helpers

CguiTransformation CguiTZeroSize(void)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f },
        .size               = { 0.0f, 0.0f },
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 0.0f, 0.0f },
        .anchor             = { 0.0f, 0.0f },
        .shrink             = { 0.0f, 0.0f }
    };
}

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

CguiTransformation CguiTCenter(Vector2 size)
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

CguiTransformation CguiTTop(Vector2 size)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f },
        .size               = size,
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 0.0f, 0.0f },
        .anchor             = { 0.5f, 0.0f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTBottom(Vector2 size)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f },
        .size               = size,
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 0.0f, 0.0f },
        .anchor             = { 0.5f, 1.0f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTLeft(Vector2 size)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f },
        .size               = size,
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 0.0f, 0.0f },
        .anchor             = { 0.0f, 0.5f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTRight(Vector2 size)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f },
        .size               = size,
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 0.0f, 0.0f },
        .anchor             = { 1.0f, 0.5f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTTopLeft(Vector2 size)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f },
        .size               = size,
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 0.0f, 0.0f },
        .anchor             = { 0.0f, 0.0f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTTopRight(Vector2 size)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f },
        .size               = size,
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 0.0f, 0.0f },
        .anchor             = { 1.0f, 0.0f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTBottomLeft(Vector2 size)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f },
        .size               = size,
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 0.0f, 0.0f },
        .anchor             = { 0.0f, 1.0f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTBottomRight(Vector2 size)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f },
        .size               = size,
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 0.0f, 0.0f },
        .anchor             = { 1.0f, 1.0f },
        .shrink             = { 0.0f, 0.0f }
    };
}

CguiTransformation CguiTDockTop(float height)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f   },
        .size               = { 1.0f, height },
        .isRelativePosition = { 1.0f, 1.0f   },
        .isRelativeSize     = { 1.0f, 0.0f   },
        .anchor             = { 0.5f, 0.0f   },
        .shrink             = { 0.0f, 0.0f   }
    };
}

CguiTransformation CguiTDockBottom(float height)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f   },
        .size               = { 1.0f, height },
        .isRelativePosition = { 1.0f, 1.0f   },
        .isRelativeSize     = { 1.0f, 0.0f   },
        .anchor             = { 0.5f, 1.0f   },
        .shrink             = { 0.0f, 0.0f   }
    };
}

CguiTransformation CguiTDockLeft(float width)
{
    return (CguiTransformation) {
        .position           = { 0.0f,  0.0f },
        .size               = { width, 1.0f },
        .isRelativePosition = { 1.0f,  1.0f },
        .isRelativeSize     = { 0.0f,  1.0f },
        .anchor             = { 0.0f,  0.5f },
        .shrink             = { 0.0f,  0.0f }
    };
}

CguiTransformation CguiTDockRight(float width)
{
    return (CguiTransformation) {
        .position           = { 0.0f,  0.0f },
        .size               = { width, 1.0f },
        .isRelativePosition = { 1.0f,  1.0f },
        .isRelativeSize     = { 0.0f,  1.0f },
        .anchor             = { 1.0f,  0.5f },
        .shrink             = { 0.0f,  0.0f }
    };
}

CguiTransformation CguiTMargin(float margin)
{
    return (CguiTransformation) {
        .position           = { 0.0f,          0.0f          },
        .size               = { 1.0f,          1.0f          },
        .isRelativePosition = { 1.0f,          1.0f          },
        .isRelativeSize     = { 1.0f,          1.0f          },
        .anchor             = { 0.5f,          0.5f          },
        .shrink             = { margin * 2.0f, margin * 2.0f }
    };
}

CguiTransformation CguiTFillParent(void)
{
    return (CguiTransformation) {
        .position           = { 0.0f, 0.0f },
        .size               = { 1.0f, 1.0f },
        .isRelativePosition = { 1.0f, 1.0f },
        .isRelativeSize     = { 1.0f, 1.0f },
        .anchor             = { 0.0f, 0.0f },
        .shrink             = { 0.0f, 0.0f }
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

void CguiSetTransformation(CguiNode *node, CguiTransformation t)
{
    if (!node)
    {
        return;
    }

    if (!CguiIsTransformationEqual(node->transformation, t))
    {
        node->transformation = t;
        node->rebound        = true;
    }
}

// Misc.

bool CguiIsTreeStructureEqual(CguiNode *a, CguiNode *b)
{
    if (!a || !b)
    {
        return false;
    }

    if (a->childrenCount != b->childrenCount)
    {
        return false;
    }

    for (int i = 0; i < a->childrenCount; i++)
    {
        if (!CguiIsTreeStructureEqual(a->children[i], b->children[i]))
        {
            return false;
        }
    }

    return true;
}

bool CguiIsTreeTypeEqual(CguiNode *a, CguiNode *b)
{
    if (!a || !b)
    {
        return false;
    }

    if (a->type != b->type)
    {
        return false;
    }

    if (a->childrenCount != b->childrenCount)
    {
        return false;
    }

    for (int i = 0; i < a->childrenCount; i++)
    {
        if (!CguiIsTreeStructureEqual(a->children[i], b->children[i]))
        {
            return false;
        }
    }

    return true;
}

bool CguiCopyNodeValues(CguiNode *fromNode, CguiNode *toNode)
{
    if (!fromNode || !toNode)
    {
        return false;
    }

    CguiNode copyNode = *fromNode;

    // Exclude hierarchy from copy
    copyNode.parent           = toNode->parent;
    copyNode.children         = toNode->children;
    copyNode.childrenCount    = toNode->childrenCount;
    copyNode.childrenCapacity = toNode->childrenCapacity;

    if (fromNode->name)
    {
        const char *newName = TextFormat("%s (Copied #%d)", fromNode->name, ++cguiNameCounter);
        copyNode.name       = CG_MALLOC_NULL(strlen(newName) + 1);
        if (!copyNode.name)
        {
            return false;
        }

        strcpy(copyNode.name, newName);
    }

    if (fromNode->data && fromNode->dataSize > 0)
    {
        copyNode.data = CG_MALLOC_NULL(fromNode->dataSize);
        if (!copyNode.data)
        {
            CG_FREE_NULL(copyNode.name);
            return false;
        }

        memcpy(copyNode.data, fromNode->data, fromNode->dataSize);
        copyNode.dataSize = fromNode->dataSize;
    }

    CG_FREE_NULL(toNode->data);
    toNode->dataSize = 0;

    if (fromNode->instanceData && fromNode->instanceDataSize > 0)
    {
        copyNode.instanceData = CG_MALLOC_NULL(fromNode->instanceDataSize);
        if (!copyNode.instanceData)
        {
            CG_FREE_NULL(copyNode.name);
            CG_FREE_NULL(copyNode.data);
            return false;
        }

        memcpy(copyNode.instanceData, fromNode->instanceData, fromNode->instanceDataSize);
        copyNode.instanceDataSize = fromNode->instanceDataSize;
    }

    CG_FREE_NULL(toNode->instanceData);
    toNode->instanceDataSize = 0;

    *toNode = copyNode;

    return true;
}

bool CguiCopyNodeValuesNoTi(CguiNode *fromNode, CguiNode *toNode)
{
    if (!fromNode || !toNode)
    {
        return false;
    }

    CguiNode copyNode = *fromNode;

    // Exclude hierarchy and instance data from copy
    copyNode.parent            = toNode->parent;
    copyNode.children          = toNode->children;
    copyNode.childrenCount     = toNode->childrenCount;
    copyNode.childrenCapacity  = toNode->childrenCapacity;
    copyNode.templateSource    = toNode->templateSource;
    copyNode.instances         = toNode->instances;
    copyNode.instancesCount    = toNode->instancesCount;
    copyNode.instancesCapacity = toNode->instancesCapacity;
    copyNode.resync            = toNode->resync;
    copyNode.instanceData      = toNode->instanceData;
    copyNode.instanceDataSize  = toNode->instanceDataSize;
    copyNode.override          = toNode->override;

    if (fromNode->name)
    {
        const char *newName = TextFormat("%s (Copied #%d)", fromNode->name, ++cguiNameCounter);
        copyNode.name       = CG_MALLOC_NULL(strlen(newName) + 1);
        if (!copyNode.name)
        {
            return false;
        }

        strcpy(copyNode.name, newName);
    }

    if (fromNode->data && fromNode->dataSize > 0)
    {
        copyNode.data = CG_MALLOC_NULL(fromNode->dataSize);
        if (!copyNode.data)
        {
            CG_FREE_NULL(copyNode.name);
            return false;
        }

        memcpy(copyNode.data, fromNode->data, fromNode->dataSize);
        copyNode.dataSize = fromNode->dataSize;
    }

    CG_FREE_NULL(toNode->data);
    toNode->dataSize = 0;

    *toNode = copyNode;

    return true;
}

static void CguiCopyNodeRecurse(CguiNode *fromNode, CguiNode *toNode)
{
    if (!fromNode || !toNode)
    {
        return;
    }

    if (!CguiCopyNodeValues(fromNode, toNode))
    {
        return;
    }

    for (int i = 0; i < fromNode->childrenCount; i++)
    {
        CguiCopyNodeRecurse(fromNode->children[i], toNode->children[i]);
    }
}

bool CguiCopyNode(CguiNode *fromNode, CguiNode *toNode)
{
    if (!fromNode || !toNode)
    {
        return false;
    }

    if (!CguiIsTreeStructureEqual(fromNode, toNode))
    {
        return false;
    }

    CguiCopyNodeRecurse(fromNode, toNode);

    return true;
}

static void CguiCopyNodeNoTiRecurse(CguiNode *fromNode, CguiNode *toNode)
{
    if (!fromNode || !toNode)
    {
        return;
    }

    if (!CguiCopyNodeValuesNoTi(fromNode, toNode))
    {
        return;
    }

    for (int i = 0; i < fromNode->childrenCount; i++)
    {
        CguiCopyNodeNoTiRecurse(fromNode->children[i], toNode->children[i]);
    }
}

bool CguiCopyNodeNoTi(CguiNode *fromNode, CguiNode *toNode)
{
    if (!fromNode || !toNode)
    {
        return false;
    }

    if (!CguiIsTreeStructureEqual(fromNode, toNode))
    {
        return false;
    }

    CguiCopyNodeNoTiRecurse(fromNode, toNode);

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
    // Reverse iteration to check overlaps first (top-drawn is later children)
    for (int i = node->childrenCount - 1; i >= 0; i--)
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
