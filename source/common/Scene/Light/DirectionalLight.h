//
//  directionalLight.h
//  cs148-opengl4
//
//  Created by Lea Coligado on 10/12/15.
//
//

#ifndef directionalLight_h
#define directionalLight_h

#include "Light.h"

class directionalLight: public Light {
public:
    directionalLight(std::unique_ptr<struct LightProperties> inProperties, LightType type = Light::LightType::DIRECTIONAL):
    Light::Light(std::move(inProperties), type) {}
    
};

#endif /* directionalLight_h */
