#ifndef I_APP_STATE_H
#define I_APP_STATE_H

struct SensorData {
    float temperature;
    float pressure;
};

class IAppState {
public:
    virtual ~IAppState() {}
    virtual void setTemperature(float temp) = 0;
    virtual float getTemperature() const = 0;
    virtual void setPressure(float press) = 0;
    virtual float getPressure() const = 0;
    virtual void setListening(bool listening) = 0;
    virtual bool isListening() const = 0;
    virtual void setSOS(bool sos) = 0;
    virtual bool isSOS() const = 0;
};

#endif
