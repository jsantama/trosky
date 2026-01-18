#ifndef APP_STATE_H
#define APP_STATE_H

#include "IAppState.h"

class AppState : public IAppState {
public:
  AppState();

  void setTemperature(float t) override;
  float getTemperature() const override;

  void setPressure(float p) override;
  float getPressure() const override;

  void setListening(bool l) override;
  bool isListening() const override;

  void setSOS(bool s) override;
  bool isSOS() const override;

private:
  float temp;
  float press;
  bool listening;
  bool sos;
};

#endif
