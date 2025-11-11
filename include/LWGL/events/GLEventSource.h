#pragma once

#include "../events/EventSource.h"

class ResizeEvent;
class MouseEvent;
class MouseButtonEvent;
class KeyboardEvent;

class GLEventSource : public engine::EventSource {
    friend class GLEventSite;

  protected:
    void fireWindowResizeEvent(ResizeEvent* pEvent);
    void fireMouseMoveEvent(MouseEvent* pEvent);
    void fireMouseButtonEvent(MouseButtonEvent* pEvent);
    void fireKeyboardEvent(KeyboardEvent* pEvent);
};