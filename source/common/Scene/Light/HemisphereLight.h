#ifndef HemisphereLight_h
#define HemisphereLight_h

#include "Light.h"

class HemisphereLight: public Light {
public:
    HemisphereLight(std::unique_ptr<struct LightProperties> inProperties, LightType type = Light::LightType::HEMISPHERE):
    Light::Light(std::move(inProperties), type) {}
    
};

#endif /* HemisphereLight_h */