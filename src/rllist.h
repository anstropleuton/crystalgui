/******************************************************************************
* Copyright (c) 2023 Anstro Pleuton (@AnstroPleuton)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* A Minimal C Linked List
*   Usage and other information is provided in the README.md file from the
*   repository. Please refer to that to know how to use this library.
******************************************************************************/

#ifndef RLLIST_H
#define RLLIST_H "rllist.h"

#include <stdlib.h>

// Each element has allocated next pointer and data pointer
typedef struct Element
{
    void *data;                     // Pointer to the next element
    void *next;                     // Pointer to the allocated data with size of typesize from List
} Element;

typedef struct List
{
    Element *base;           // The base does not contain an element, the next of the base is the index of 0
    size_t size;                    // Update the size of the list when modifying externally
    size_t typesize;                // Size is used in creating elements
} List;

// Create a new linked list, typesize is used for creating and allocating memory for elements
static List *CreateList(size_t typesize)
{
    List *list = (List *) malloc(sizeof(List));

    if (!list)
    {
        return NULL;
    }

    list->size = 0;
    list->typesize = typesize;
    list->base = (Element *) malloc(sizeof(Element));

    if (!list->base)
    {
        return NULL;
    }

    list->base->data = NULL;
    list->base->next = NULL;
    return list;
}

// Get an element at the position index
static Element *GetElement(size_t index, List *list)
{
    Element *element = NULL;

    if (!list)
    {
        return NULL;
    }
    element = list->base;

    // Note, I am not using 'i <= index' because I want
    // the size to overflow in case the index was -1
    for (size_t i = 0; i < (index + 1); i++)
    {
        element = (Element *) element->next;
        if (!element)
        {
            return NULL;
        }
    }
    return element;
}

// Calculate and update internal size of the list
static int UpdateListSize(List *list)
{
    Element *element = NULL;
    size_t size = 0;

    if (!list)
    {
        return 1;
    }
    element = list->base;

    while (element)
    {
        element = (Element *) element->next;
        size++;
    }

    // The base is not considered as an element
    --size;
    return 0;
}

// Get the internal list size
static size_t GetListSize(List *list)
{
    if (!list)
    {
        return 0;
    }
    if (UpdateListSize(list))
    {
        return 0;
    }
    return list->size;
}

// Create new element
// NOTE: These functions are not supposed to be used externally
static Element *CreateElement(size_t typesize)
{
    Element *element = (Element *) malloc(sizeof(Element));
    if (!element)
    {
        return NULL;
    }

    element->next = NULL;
    element->data = malloc(typesize);
    if (!element->data)
    {
        return NULL;
    }

    return element;
}

// Delete created element
// NOTE: These functions are not supposed to be used externally
static int DeleteElement(Element *element)
{
    if (!element)
    {
        return 1;
    }
    if (!element->data)
    {
        return 2;
    }

    free(element->data);
    free(element);
    return 0;
}

// Create and insert an element to the list, You can insert element at the end of the list by giving it the size of the list
static Element *InsertElement(size_t index, List *list)
{
    Element *newElement = NULL;
    Element *prev = NULL;
    Element *temp = NULL;

    if (!list)
    {
        return NULL;
    }
    if (index > list->size)
    {
        return NULL;
    }

    prev = GetElement(index - 1, list);
    newElement = CreateElement(list->typesize);

    // There can't be no previous because the base should exist
    if (!prev)
    {
        return NULL;
    }
    if (!newElement)
    {
        return NULL;
    }

    temp = (Element *) prev->next;
    prev->next = (void *) newElement;
    newElement->next = (void *) temp;

    list->size++;
    return newElement;
}

// Remove and delete the element at the position index
static int RemoveElement(size_t index, List *list)
{
    Element *element = NULL;
    Element *prev = NULL;
    Element *next = NULL;

    if (!list)
    {
        return 1;
    }
    if (index >= list->size)
    {
        return 3;
    }

    element = GetElement(index, list);
    if (!element)
    {
        return 4;
    }
    prev = GetElement(index - 1, list);

    // To prevent unnecessary error logs, I used an 'if' check
    if (index < list->size - 1)
    {
        next = GetElement(index + 1, list);
    }
    if (!prev)
    {
        return 5;
    }

    prev->next = NULL;
    if (next)
    {
        prev->next = next;
    }
    if (DeleteElement(element))
    {
        return 6;
    }
    list->size--;

    return 0;
}

// Delete and remove all the elements of the linked list
static int ClearList(List *list)
{
    if (!list)
    {
        return 1;
    }
    if (list->size == 0)
    {
        // This is not considered an error
        return 0;
    }

    for (int i = list->size - 1; i >= 0; i--)
    {
        RemoveElement(i, list);
    }
    return 0;
}

// Delete the created linked list
static int DeleteList(List *list)
{
    if (!list)
    {
        return 1;
    }
    if (ClearList(list))
    {
        return 2;
    }
    if (!list->base)
    {
        return 3;
    }

    free(list->base);
    free(list);
    return 0;
}

#endif // RLLIST_H
