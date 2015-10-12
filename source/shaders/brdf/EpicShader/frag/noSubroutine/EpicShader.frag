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
    //Random variables to reference
    alpha = (material.roughness * material.roughness);

    //Normalized Vector from the Vertex to the Camera
    vec4 normalV = normalize(pointLight.pointPosition - cameraPosition);

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

    // Epic specular color
    vec4 bigDDenom = ((dot(N,H) * dot(N,H) * ((alpha * alpha) - 1) + 1);
    vec4 bigD = (alpha * alpha) / (3.14 * (bigDDenom * bigDDenom));

    vec4 bigG = GOne(L, N, k) * GOne(normalV, N, k);

    float k = ((material.roughness + 1) * (material.roughness + 1)) / 8;

    vec4 sC = material.specular
    float cSpec = mix(0.08*sC, fragmentColor, m);

    twoPowerInnards = (−5.55473*(dot(normalV, H)) − 6.98316) * dot(normalV, H);
    twoPower = Math.pow(2, twoPowerInnards);

    bigF = cSpec + (1 - cSpec) * twoPower;

    //<-- SUM -->
    s = (bigD * bigF * bigG) / (dot(N, L) * dot(N, normalV) * 4);


    return genericLight.lightColor * dot(N, L) * (d + s);


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

vec4 GOne(vec4 inputVector, vec4 NVector, float k)
{
    vec4 returnVector = (dot(NVector, inputVector)) / (dot(NVector, inputVector) * (1-k) + k)
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
