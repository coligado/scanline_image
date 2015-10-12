#version 330

in vec4 fragmentColor;
in vec4 vertexWorldPosition;
in vec3 vertexWorldNormal;

out vec4 finalColor;

uniform InputMaterial {
    float metallic;
    float roughness;
    float specular;
} material;

struct LightProperties {
    vec4 lightColor;
    vec4 secondaryColor;
};

uniform LightProperties genericLight;

struct PointLight {
    vec4 pointPosition;
};
uniform PointLight pointLight;

uniform vec4 cameraPosition;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

uniform int lightingType;


vec4 pointLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    // Normal to the surface
    vec4 N = vec4(normalize(worldNormal), 0.f);
    
    // Direction from the surface to the light
    vec4 L = normalize(pointLight.pointPosition - worldPosition);
    
    // Direction from the surface to the eye
    vec4 E = normalize(cameraPosition - worldPosition);
    
    // Direction of maximum highlights (see paper!)
    vec4 H = normalize(L + E);
    
    // Epic diffuse color
    float m = material.metallic;
    vec4 cDiff = fragmentColor * (1-m);
    vec4 d = cDiff / 3.14;
    
    return genericLight.lightColor * dot(N, L) * d;
}

vec4 globalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0,0,0,0);
}

vec4 AttenuateLight(vec4 originalColor, vec4 worldPosition)
{
    float lightDistance = length(pointLight.pointPosition - worldPosition);
    float attenuation = 1.0 / (constantAttenuation + lightDistance * linearAttenuation + lightDistance * lightDistance * quadraticAttenuation);
    return originalColor * attenuation;
}

void main()
{
    vec4 lightingColor = vec4(0);
    if (lightingType == 0) {
        lightingColor = globalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 1) {
        lightingColor = pointLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    }
    finalColor = AttenuateLight(lightingColor, vertexWorldPosition) * fragmentColor;
}