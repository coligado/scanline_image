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

struct DirectionalLight {
    vec4 lightDirection;
};
uniform DirectionalLight directionalLightExample;

uniform vec4 cameraPosition;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

uniform int lightingType;

float dotProduct(vec4 vOne, vec4 vTwo)
{
    return clamp(dot(vOne,vTwo), 0.f, 1.f);
}

float G1(vec4 v, vec4 N, float k)
{
    return dot(N, v) / (dot(N, v) * (1-k) + k);
}

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
    float sc = material.specular;
    float r = material.roughness;
    float useless = sc+r;
    vec4 cDiff = fragmentColor * (1-m);
    vec4 d = cDiff / 3.14;
    
    //Random variables to reference
    float alpha = r * r;
    float k = ((r + 1) * (r + 1)) / 8;
    
    float bigDDenom = (dot(N,H) * dot(N,H) * ((alpha * alpha) - 1) + 1);
    float bigD = (alpha * alpha) / (3.14 * (bigDDenom * bigDDenom));
    float bigG = G1(L, N, k) * G1(E, N, k);
    vec4 cSpec = mix(vec4(0.08 * sc), fragmentColor, m);
    
    vec4 F = cSpec + (1 - cSpec) * pow(2, (-5.55473 * dotProduct(E,H) - 6.98316) * dotProduct(E,H));
    vec4 s = bigD * bigG * F / (4 * dotProduct(N, L) * dotProduct(N, E));
    
    return genericLight.lightColor * dotProduct(N,L) * (d + s);
}

vec4 globalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0,0,0,0);
}

vec4 directionalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    // Normal to the surface
    vec4 N = vec4(normalize(worldNormal), 0.f);
    
    // Direction from the surface to the light
    vec4 L = -1 * normalize(directionalLightExample.lightDirection);
    
    // Direction from the surface to the eye
    vec4 E = normalize(cameraPosition - worldPosition);
    
    // Direction of maximum highlights (see paper!)
    vec4 H = normalize(L + E);
    
    // Epic diffuse color
    float m = material.metallic;
    float sc = material.specular;
    float r = material.roughness;
    float useless = sc+r;
    vec4 cDiff = fragmentColor * (1-m);
    vec4 d = cDiff / 3.14;
    
    //Random variables to reference
    float alpha = r * r;
    float k = ((r + 1) * (r + 1)) / 8;
    
    float bigDDenom = (dot(N,H) * dot(N,H) * ((alpha * alpha) - 1) + 1);
    float bigD = (alpha * alpha) / (3.14 * (bigDDenom * bigDDenom));
    float bigG = G1(L, N, k) * G1(E, N, k);
    vec4 cSpec = mix(vec4(0.08 * sc), fragmentColor, m);
    
    vec4 F = cSpec + (1 - cSpec) * pow(2, (-5.55473 * dotProduct(E,H) - 6.98316) * dotProduct(E,H));
    vec4 s = bigD * bigG * F / (4 * dotProduct(N, L) * dotProduct(N, E));
    
    return genericLight.lightColor * dotProduct(N,L) * (d + s);
}

vec4 hemisphereLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    // Normal to the surface
    vec4 N = vec4(normalize(worldNormal), 0.f);
    
    // Direction from the surface to the light
    vec4 L = N;
    
    // Epic diffuse color
    float m = material.metallic;
    float sc = material.specular;
    float r = material.roughness;
    float useless = sc+r;
    vec4 cDiff = fragmentColor * (1-m);
    vec4 d = cDiff / 3.14;
    
    // TODO: is the clamp portion correct?
    vec4 cLight = mix(genericLight.secondaryColor, genericLight.lightColor, clamp(N * vec4(0, 1.f, 0, 0) * 0.5 + 0.5, 0, 1));
    
    return cLight * dotProduct(N,L) * d;
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
    } else if (lightingType == 2) {
        lightingColor = directionalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    }
    finalColor = AttenuateLight(lightingColor, vertexWorldPosition) * fragmentColor;
}