#include "LWGL/events/GLEventSource.h"
#include "LWGL/events/GLEventSite.h"

#include "LWGL/events/GLEvents.h"

void GLEventSource::fireWindowResizeEvent(ResizeEvent* pEvent) {
    engine::EventSource::fireEvent(&GLEventSite::windowResizeEvent, pEvent);
};

void GLEventSource::fireMouseMoveEvent(MouseEvent* pEvent) {
    engine::EventSource::fireEvent(&GLEventSite::mouseMoveEvent, pEvent);
}

void GLEventSource::fireKeyboardEvent(KeyboardEvent* pEvent) {
    engine::EventSource::fireEvent(&GLEventSite::keyboardEvent, pEvent);
}