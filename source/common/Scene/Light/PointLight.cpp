#include "common/Scene/Light/PointLight.h"

const std::string Light::LIGHT_UNIFORM_NAME = "pointLight";

PointLight::Light(std::unique_ptr<LightProperties> inProperties, LightType type):
    properties(std::move(inProperties)), lightType(type), constantAttenuation(1.f), linearAttenuation(0.f), quadraticAttenuation(0.f)
{
}

PointLight::~Light()
{
}