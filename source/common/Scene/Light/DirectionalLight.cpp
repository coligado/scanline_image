#include "common/Scene/Light/DirectionalLight.h"

const std::string Light::LIGHT_UNIFORM_NAME = "directionalLight";

DirectionalLight::Light(std::unique_ptr<LightProperties> inProperties, LightType type):
    properties(std::move(inProperties)), lightType(type), constantAttenuation(1.f), linearAttenuation(0.f), quadraticAttenuation(0.f)
{
}

DirectionalLight::~Light()
{
}