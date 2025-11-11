#pragma once

#include "../events/EventSite.h"

class MouseEvent;
class MouseButtonEvent;
class ResizeEvent;
class KeyboardEvent;

class GLEventSite : public engine::EventSite {
    friend class GLEventSource;

  protected:
    virtual void windowResizeEvent(ResizeEvent* pEvent) {};
    virtual void mouseMoveEvent(MouseEvent* pEvent) {};
    virtual void mouseButtonEvent(MouseButtonEvent* pEvent) {};
    virtual void keyboardEvent(KeyboardEvent* pEvent) {};
};