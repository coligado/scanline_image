//
//  DirectionalLight.h
//  cs148-opengl4
//
//  Created by Lea Coligado on 10/11/15.
//
//

#ifndef DirectionalLight_h
#define DirectionalLight_h

#include "Light.h"

class DirectionalLight: public Light{
public:
    DirectionalLight(std::unique_ptr<struct LightProperties> inProperties, LightType type = Light::LightType::POINT);
    virtual ~PointLight();
};

#endif /* DirectionalLight_h */
