#pragma once

#ifndef __POINT_LIGHT__
#define __POINT_LIGHT__

#include "common/Scene/Light.h"

class PointLight: public Light
{
public:
    PointLight(std::unique_ptr<struct LightProperties> inProperties, LightType type = Light::LightType::POINT);
    virtual ~PointLight();
};

#endif
