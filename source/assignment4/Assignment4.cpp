#include "assignment4/Assignment4.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include "common/Utility/Texture/TextureLoader.h"

Assignment4::Assignment4(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
Application(std::move(inputScene), std::move(inputCamera))
{
}

std::unique_ptr<Application> Assignment4::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment4>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment4::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(60.f, 1280.f / 720.f);
}

glm::vec2 Assignment4::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment4::SetupScene()
{
    SetupExample2();
}

void Assignment4::SetupCamera()
{
    camera->Translate(glm::vec3(0.f, 0.f, 10.f));
    //    camera->Translate(glm::vec3(0.f, 0.f, 20.f));
}

void Assignment4::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{
    if (state != SDL_KEYDOWN) {
        return;
    }
    
    switch (key.sym) {
        case SDLK_1:
            if (!repeat && state == SDL_KEYDOWN) {
                SetupExample1();
            }
            break;
        case SDLK_2:
            if (!repeat && state == SDL_KEYDOWN) {
                SetupExample2();
            }
            break;
        case SDLK_UP:
            camera->Rotate(glm::vec3(camera->GetRightDirection()), 0.1f);
            break;
        case SDLK_DOWN:
            camera->Rotate(glm::vec3(camera->GetRightDirection()), -0.1f);
            break;
        case SDLK_RIGHT:
            camera->Rotate(glm::vec3(SceneObject::GetWorldUp()), -0.1f);
            break;
        case SDLK_LEFT:
            camera->Rotate(glm::vec3(SceneObject::GetWorldUp()), 0.1f);
            break;
        case SDLK_w:
            camera->Translate(glm::vec3(camera->GetForwardDirection() * 0.3f));
            break;
        case SDLK_a:
            camera->Translate(glm::vec3(camera->GetRightDirection() * -0.3f));
            break;
        case SDLK_s:
            camera->Translate(glm::vec3(camera->GetForwardDirection() * -0.3f));
            break;
        case SDLK_d:
            camera->Translate(glm::vec3(camera->GetRightDirection() * 0.3f));
            break;
        case SDLK_SPACE:
            camera->Translate(glm::vec3(camera->GetUpDirection() * 0.3f));
            break;
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            camera->Translate(glm::vec3(camera->GetUpDirection() * -0.3f));
            break;
        default:
            Application::HandleInput(key, state, repeat, timestamp, deltaTime);
            break;
    }
}

void Assignment4::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
    std::static_pointer_cast<PerspectiveCamera>(camera)->SetAspectRatio(x / y);
}

void Assignment4::SetupExample1()
{
    scene->ClearScene();
#ifndef DISABLE_OPENGL_SUBROUTINES
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/blinnphong.frag" }
    };
#else
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.frag"}
    };
#endif
    std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
    shader->SetDiffuse(glm::vec4(0.8f, 0.8f, 0.8f, 1.f));
    shader->SetTexture(BlinnPhongShader::TextureSlots::DIFFUSE, TextureLoader::LoadTexture("brick/bricktexture.jpg"));
    shader->SetTexture(BlinnPhongShader::TextureSlots::SPECULAR, TextureLoader::LoadTexture("brick/bricktexture.jpg"));
    
    std::unique_ptr<LightProperties> lightProperties = make_unique<LightProperties>();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    
    std::shared_ptr<Light> pointLight = std::make_shared<Light>(std::move(lightProperties));
    pointLight->SetPosition(glm::vec3(10.f, -10.f, 10.f));
    scene->AddLight(pointLight);
    
    std::vector<std::shared_ptr<RenderingObject>> sphereTemplate = MeshLoader::LoadMesh(shader, "sphere.obj");
    std::shared_ptr<class SceneObject> sceneObject = std::make_shared<SceneObject>(sphereTemplate);
    sceneObject->Rotate(glm::vec3(SceneObject::GetWorldRight()), PI / 4.f);
    scene->AddSceneObject(sceneObject);
}

void Assignment4::SetupExample2()
{
    scene->ClearScene();
    
#ifndef DISABLE_OPENGL_SUBROUTINES
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/blinnphong.frag" }
    };
#else
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.frag" }
    };
#endif
    std::unique_ptr<LightProperties> lightProperties = make_unique<LightProperties>();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    
    std::shared_ptr<Light> pointLight = std::make_shared<Light>(std::move(lightProperties));
    pointLight->SetPosition(glm::vec3(10.f, 20.f, 20.f));
    scene->AddLight(pointLight);
    
    std::unique_ptr<LightProperties> lightProperties2 = make_unique<LightProperties>();
    lightProperties2->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties2->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    
    std::shared_ptr<Light> pointLight2 = std::make_shared<Light>(std::move(lightProperties2));
    pointLight2->SetPosition(glm::vec3(-10.f, -20.f, -20.f));
    scene->AddLight(pointLight2);
    
    // botos
    std::shared_ptr<BlinnPhongShader> botoShader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
    botoShader->SetDiffuse(glm::vec4(0.8f, 0.8f, 0.8f, 1.f));
    botoShader->SetTexture(BlinnPhongShader::TextureSlots::DIFFUSE, TextureLoader::LoadTexture("boto/Textures/boto.png"));
    botoShader->SetTexture(BlinnPhongShader::TextureSlots::SPECULAR, TextureLoader::LoadTexture("boto/Textures/boto.png"));
    
    std::vector<std::shared_ptr<RenderingObject>> botoMeshTemplate = MeshLoader::LoadMesh(botoShader, "boto/boto.obj");
    if (botoMeshTemplate.empty()) {
        std::cerr << "ERROR: Failed to load the model. Check your paths." << std::endl;
        return;
    }
    
    boto1 = std::make_shared<SceneObject>(botoMeshTemplate);
    boto1->AddScale(3.0f);
    boto1->Rotate(glm::vec3(SceneObject::GetWorldUp()), 1.1f);
    boto1->Translate(glm::vec3(-4.f,-3.f,-2.f));
    scene->AddSceneObject(boto1);
    
    boto2 = std::make_shared<SceneObject>(botoMeshTemplate);
    boto2->AddScale(3.0f);
    boto2->Rotate(glm::vec3(SceneObject::GetWorldUp()), -1.0f);
    boto2->Rotate(glm::vec3(SceneObject::GetWorldRight()), -0.1f);
    boto2->Translate(glm::vec3(4.f,-3.f,-1.f));
    scene->AddSceneObject(boto2);
    
    // surfboard
    std::shared_ptr<BlinnPhongShader> surfboardShader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
    surfboardShader->SetDiffuse(glm::vec4(0.8f, 0.8f, 0.8f, 1.f));
    surfboardShader->SetTexture(BlinnPhongShader::TextureSlots::DIFFUSE, TextureLoader::LoadTexture("surfboard/surfboard.png"));
    surfboardShader->SetTexture(BlinnPhongShader::TextureSlots::SPECULAR, TextureLoader::LoadTexture("surfboard/surfboard.png"));
    
    std::vector<std::shared_ptr<RenderingObject>> surfboardTemplate = MeshLoader::LoadMesh(surfboardShader, "surfboard/surfboard.obj");
    if (surfboardTemplate.empty()) {
        std::cerr << "ERROR: Failed to load the model. Check your paths." << std::endl;
        return;
    }
    
    surfboard = std::make_shared<SceneObject>(surfboardTemplate);
    surfboard->AddScale(-0.25f);
    surfboard->Rotate(glm::vec3(SceneObject::GetWorldUp()), 1.5f);
    surfboard->Rotate(glm::vec3(SceneObject::GetWorldRight()), 0.38f);
    surfboard->Translate(glm::vec3(-0.2f,-2.f,-0.3f));
    scene->AddSceneObject(surfboard);
    
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
    
    // sunset
    std::vector<std::shared_ptr<RenderingObject>> sunsetTemplate = MeshLoader::LoadMesh(nullptr, "sunset/sunset.obj", &loadedMaterials);
    for (size_t i = 0; i < sunsetTemplate.size(); ++i) {
        std::shared_ptr<BlinnPhongShader> sunsetShader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
        sunsetShader->LoadMaterialFromAssimp(loadedMaterials[i]);
        sunsetShader->SetDiffuse(glm::vec4(0.8f, 0.8f, 0.8f, 1.f));
        sunsetTemplate[i]->SetShader(std::move(sunsetShader));
    }
    sunset = std::make_shared<SceneObject>(sunsetTemplate);
    sunset->AddScale(1.3f);
    sunset->Translate(glm::vec3(0.f,-9.f,-10.f));
    scene->AddSceneObject(sunset);
    
    size_t placeholder = sunsetTemplate.size();
    
    // bottle
    std::vector<std::shared_ptr<RenderingObject>> bottleTemplate = MeshLoader::LoadMesh(nullptr, "wine_bottle/wine_bottle.obj", &loadedMaterials);
    for (size_t i = 0; i < bottleTemplate.size(); ++i) {
        std::shared_ptr<BlinnPhongShader> bottleShader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
        bottleShader->LoadMaterialFromAssimp(loadedMaterials[placeholder+i]);
        bottleTemplate[i]->SetShader(std::move(bottleShader));
    }
    
    bottle = std::make_shared<SceneObject>(bottleTemplate);
    bottle->AddScale(-0.6f);
    bottle->Rotate(glm::vec3(SceneObject::GetWorldUp()), -0.3f);
    bottle->Translate(glm::vec3(0.f,-1.5f,0.f));
    scene->AddSceneObject(bottle);
    
    placeholder += bottleTemplate.size();
    
    // glass
    std::vector<std::shared_ptr<RenderingObject>> glassTemplate = MeshLoader::LoadMesh(nullptr, "glass/glass.obj", &loadedMaterials);
    for (size_t i = 0; i < glassTemplate.size(); ++i) {
        std::shared_ptr<BlinnPhongShader> glassShader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
        glassShader->SetDiffuse(glm::vec4(0.8f, 0.8f, 0.8f, 1.f));
        glassShader->LoadMaterialFromAssimp(loadedMaterials[placeholder + i]);
        glassTemplate[i]->SetShader(std::move(glassShader));
    }
    
    glass1 = std::make_shared<SceneObject>(glassTemplate);
    glass1->AddScale(-0.96f);
    glass1->Translate(glm::vec3(-0.5f,-1.8f,0.7f));
    scene->AddSceneObject(glass1);
    
    glass2 = std::make_shared<SceneObject>(glassTemplate);
    glass2->AddScale(-0.96f);
    glass2->Translate(glm::vec3(0.3f,-2.f,0.7f));
    scene->AddSceneObject(glass2);
    
    placeholder += glassTemplate.size();
    
    // ocean
    std::vector<std::shared_ptr<RenderingObject>> oceanTemplate = MeshLoader::LoadMesh(nullptr, "ocean/big_ocean.obj", &loadedMaterials);
    for (size_t i = 0; i < oceanTemplate.size(); ++i) {
        std::shared_ptr<BlinnPhongShader> oceanShader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
        oceanShader->LoadMaterialFromAssimp(loadedMaterials[placeholder + i]);
        oceanTemplate[i]->SetShader(std::move(oceanShader));
    }
    
    ocean = std::make_shared<SceneObject>(oceanTemplate);
    ocean->Rotate(glm::vec3(SceneObject::GetWorldRight()), 0.4f);
    ocean->AddScale(2.0f);
    ocean->Translate(glm::vec3(-12.f,2.f,-10.f));
    scene->AddSceneObject(ocean);
    
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials2;
    
    std::vector<std::shared_ptr<RenderingObject>> islandTemplate = MeshLoader::LoadMesh(nullptr, "island/island.obj", &loadedMaterials2);
    for (size_t i = 0; i < islandTemplate.size(); ++i) {
        std::shared_ptr<BlinnPhongShader> islandShader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
        islandShader->LoadMaterialFromAssimp(loadedMaterials2[i]);
        islandTemplate[i]->SetShader(std::move(islandShader));
    }
    
    island = std::make_shared<SceneObject>(islandTemplate);
    island->AddScale(-0.95f);
    island->Translate(glm::vec3(0.f,1.4f,-11.9f));
    scene->AddSceneObject(island);
    
    island2 = std::make_shared<SceneObject>(islandTemplate);
    island2->AddScale(-0.95f);
    island2->Rotate(glm::vec3(SceneObject::GetWorldUp()), 0.4f);
    island2->Translate(glm::vec3(-5.f,1.6f,-11.9f));
    scene->AddSceneObject(island2);
    
    island3 = std::make_shared<SceneObject>(islandTemplate);
    island3->AddScale(-0.96f);
    island3->Translate(glm::vec3(-13.f,1.5f,-11.9f));
    scene->AddSceneObject(island3);
    
    island4 = std::make_shared<SceneObject>(islandTemplate);
    island4->AddScale(-0.95f);
    island4->Translate(glm::vec3(5.4f,1.5f,-11.9f));
    scene->AddSceneObject(island4);
    
    island5 = std::make_shared<SceneObject>(islandTemplate);
    island5->AddScale(-0.94f);
    island5->Rotate(glm::vec3(SceneObject::GetWorldUp()), 0.7f);
    island5->Translate(glm::vec3(-15.f,1.f,-12.0f));
    scene->AddSceneObject(island5);
    
    island6 = std::make_shared<SceneObject>(islandTemplate);
    island6->AddScale(-0.95f);
    island6->Rotate(glm::vec3(SceneObject::GetWorldUp()), 0.7f);
    island6->Translate(glm::vec3(15.f,1.2f,-12.0f));
    scene->AddSceneObject(island6);
    
//    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials3;
    // bubbles
//    std::vector<std::shared_ptr<RenderingObject>> bubblesTemplate = MeshLoader::LoadMesh(nullptr, "bubbles/bubbles.obj", &loadedMaterials3);
//    for (size_t i = 0; i < bubblesTemplate.size(); ++i) {
//        std::shared_ptr<BlinnPhongShader> bubblesShader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
//        bubblesShader->LoadMaterialFromAssimp(loadedMaterials3[i]);
//        bubblesTemplate[i]->SetShader(std::move(bubblesShader));
//    }
//    
//    bubbles = std::make_shared<SceneObject>(bubblesTemplate);
//    bubbles->AddScale(-0.9f);
//    bubbles->Translate(glm::vec3(-6.f,1.f,0.f));
//    scene->AddSceneObject(bubbles);
}

void Assignment4::Tick(double deltaTime)
{
}