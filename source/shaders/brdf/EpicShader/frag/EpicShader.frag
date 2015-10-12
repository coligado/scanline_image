#version 400

in vec4 fragmentColor;
in vec4 vertexWorldPosition;
in vec3 vertexWorldNormal;

out vec4 finalColor;

uniform InputMaterial {
//    vec4 matDiffuse;
//    vec4 matSpecular;
//    float matShininess;
//    vec4 matAmbient;
    float metallic;
    float roughness;
    float specular;
} material;

struct LightProperties {
    vec4 diffuseColor;
    vec4 specularColor;
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

subroutine vec4 lightingSubroutine(vec4 worldPosition, vec3 worldNormal);
subroutine uniform lightingSubroutine inputLightSubroutine;

subroutine(lightingSubroutine) vec4 pointLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    // Normal to the surface
    vec4 N = vec4(normalize(worldNormal), 0.f);
    
    // Direction from the surface to the light
    vec4 L = normalize(pointLight.pointPosition - worldPosition);

    // Direction from the surface to the eye
    vec4 E = normalize(cameraPosition - worldPosition);

    // Direction of maximum highlights (see paper!)
    vec4 H = normalize(L + E);

    // Amount of diffuse reflection
    float d = max(0, dot(N, L));
    vec4 diffuseColor = d * genericLight.diffuseColor * material.matDiffuse;
    
    // Amount of specular reflection
    float s = pow(max(0, dot(N, H)), material.matShininess);
    vec4 specularColor = s * genericLight.specularColor * material.matSpecular;

//    return diffuseColor + specularColor;
    return vec4(0,0,0,0);
}

subroutine(lightingSubroutine) vec4 globalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
//    return material.matAmbient;
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
    finalColor = AttenuateLight(inputLightSubroutine(vertexWorldPosition, vertexWorldNormal), vertexWorldPosition) * fragmentColor;
}
