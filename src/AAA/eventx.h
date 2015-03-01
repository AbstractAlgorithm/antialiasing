#ifndef _eventx_h_
#define _eventx_h_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "core.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: EventX
////////////////////////////////////////////////////////////////////////////////

class EventX
{
    struct _listener
    {
        void(*listenerF)();
        _listener* next;
        _listener() : listenerF(NULL), next(NULL) {}
    };

    char* m_name;
    _listener* m_pListeners;
    EventX* m_next;
    static EventX* pEvents;

    EventX::EventX(const char* name)
    {
        m_name = (char*)malloc(sizeof(char)* (strlen(name) + 1));
        strcpy_s(m_name, sizeof(char)* (strlen(name) + 1), name);
        m_pListeners = NULL;
        m_next = pEvents;
        pEvents = this;
    }
    EventX::~EventX()
    {
        if (m_name != NULL)
            delete[] m_name;
    }

public:

    static void EventX::signal(const char* name)
    {
        // idi kroz event-e
        // ako naidjes na event sa isitm imenom
        // zasvakog listenera -> pozovi metodu

        EventX* event = pEvents;
        while (event != NULL)
        {
            if (strcmp(event->m_name, name) == 0)
            {
                _listener* listener = event->m_pListeners;
                while (listener != NULL)
                {
                    (*listener->listenerF)();
                    listener = listener->next;
                }
                break;
            }
            event = event->m_next;
        }
    }

    static void EventX::addListener(const char* name, void(*fn)())
    {
        // idi kroz event-e
        // ako nadjes na event sa istim imenom
        // dodaj listenera
        // ako ne -> napravi event i dodaj listenra

        EventX* event = pEvents;
        EventX* match = NULL;
        while (event != NULL)
        {
            if (strcmp(event->m_name, name) == 0)
            {
                match = event;
                break;
            }
            event = event->m_next;
        }
        if (!match)
        {
            match = new EventX(name);
        }

        _listener* new_listener = new _listener;
        new_listener->listenerF = fn;
        new_listener->next = match->m_pListeners;
        match->m_pListeners = new_listener;
    }

    static void EventX::removeListener(const char* name, void(*fn)())                      // TODO: check for mem leaks
    {
        // foreach event in eventList
        // if(name = eventListItem.name)
        // foreach listener in eventListItem.listeners
        // if listener = fn
        // remove and rebind

        EventX* old = NULL;
        EventX* event = pEvents;
        EventX* match = NULL;
        while (event != NULL)
        {
            if (strcmp(event->m_name, name) == 0)
            {
                match = event;
                _listener* oldl = NULL;
                _listener* listener = event->m_pListeners;
                while (listener != NULL)
                {
                    if (fn == listener->listenerF)
                    {
                        if (oldl)
                        {
                            oldl->next = listener->next;
                            //delete listener;
                        }
                        else
                        {
                            match->m_pListeners = listener->next;
                            //delete listener;
                            if (match->m_pListeners == NULL)
                            {
                                delete[] match->m_name;
                                match->m_name = NULL;
                                //delete match;
                                if (old)
                                    old->m_next = match->m_next;
                                else
                                    pEvents = match->m_next;
                            }
                        }
                    }
                    oldl = listener;
                    listener = listener->next;
                }
            }
            old = event;
            event = event->m_next;
        }
    }

#ifdef DEBUGMODE
    static void EventX::write()
    {
        EventX* event = pEvents;
        printf("*--\n");
        while (event != NULL)
        {
            int lcnt = 0;
            _listener* listener = event->m_pListeners;
            while (listener != NULL)
            {
                lcnt++;
                listener = listener->next;
            }
            printf("  *-- %s: %d\n", event->m_name, lcnt);
            event = event->m_next;
        }
    }
#endif
};


#endif