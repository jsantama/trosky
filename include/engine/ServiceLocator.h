#ifndef SERVICE_LOCATOR_H
#define SERVICE_LOCATOR_H

#include <map>
#include <string>

/**
 * [SESSION PROTOCOL 2.111 - 3.4] Dependency Management
 * The framework uses a ServiceLocator (Core) and DI.
 */

class ServiceLocator {
public:
    static ServiceLocator& instance() {
        static ServiceLocator INSTANCE;
        return INSTANCE;
    }

    void registerService(const std::string& id, void* service) {
        services[id] = service;
    }

    template<typename T>
    T* get(const std::string& id) {
        if (services.count(id)) {
            return static_cast<T*>(services[id]);
        }
        return nullptr;
    }

private:
    std::map<std::string, void*> services;
    ServiceLocator() {}
};

#endif
