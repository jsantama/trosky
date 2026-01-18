#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <Arduino.h>
#include <vector>
#include <functional>
#include <map>

/**
 * [SESSION PROTOCOL 2.111 - 3.3] Decoupled Data Flow
 * All communication must be via AppState (for State) or EventBus (for Actions).
 */

enum class EventType {
    SOS_TRIGGERED,
    VOICE_START_LISTENING,
    VOICE_STOP_LISTENING,
    SENSOR_DATA_UPDATED,
    WAKE_WORD_DETECTED,
    INTENT_TAXI,
    INTENT_RECIPE
};

struct Event {
    EventType type;
    void* data;
};

typedef std::function<void(const Event&)> EventCallback;

class EventBus {
public:
    void subscribe(EventType type, EventCallback callback) {
        listeners[type].push_back(callback);
    }

    void publish(EventType type, void* data = nullptr) {
        Event event = {type, data};
        if (listeners.count(type)) {
            for (auto& callback : listeners[type]) {
                callback(event);
            }
        }
    }

private:
    std::map<EventType, std::vector<EventCallback>> listeners;
};

#endif
