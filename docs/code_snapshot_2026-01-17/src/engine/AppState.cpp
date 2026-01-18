#include "engine/AppState.h"

AppState::AppState() : temp(0), press(0), listening(false), sos(false) {}

void AppState::setTemperature(float t) { temp = t; }
float AppState::getTemperature() const { return temp; }

void AppState::setPressure(float p) { press = p; }
float AppState::getPressure() const { return press; }

void AppState::setListening(bool l) { listening = l; }
bool AppState::isListening() const { return listening; }

void AppState::setSOS(bool s) { sos = s; }
bool AppState::isSOS() const { return sos; }
