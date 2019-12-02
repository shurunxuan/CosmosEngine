#include <CosmosEngine/Logging/Logging.h>
#include "TestGameApp.h"
#include <boost/container/list.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/bind.hpp>
#include <CosmosEngine/Core/MeshRenderer.h>
#include <CosmosEngine/Physics/SphereCollider.h>
#include <CosmosEngine/Core/AudioSource.h>
#include "../Scripts/ObjectMovement.h"
#include "../Scripts/CameraMovement.h"

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
};

/*! \brief Convert HSV to RGB color space

  Converts a given set of HSV values `h', `s', `v' into RGB
  coordinates. The output RGB values are in the range [0, 1], and
  the input HSV values are in the ranges h = [0, 360], and s, v =
  [0, 1], respectively.

  \param fR Red component, used as output, range: [0, 1]
  \param fG Green component, used as output, range: [0, 1]
  \param fB Blue component, used as output, range: [0, 1]
  \param fH Hue component, used as input, range: [0, 360]
  \param fS Hue component, used as input, range: [0, 1]
  \param fV Hue component, used as input, range: [0, 1]

*/
void HSVtoRGB(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV)
{
    float fC = fV * fS; // Chroma
    float fHPrime = fmod(fH / 60.0f, 6);
    float fX = fC * (1 - float(fabs(fmod(fHPrime, 2) - 1)));
    float fM = fV - fC;

    if (0 <= fHPrime && fHPrime < 1)
    {
        fR = fC;
        fG = fX;
        fB = 0;
    } else if (1 <= fHPrime && fHPrime < 2)
    {
        fR = fX;
        fG = fC;
        fB = 0;
    } else if (2 <= fHPrime && fHPrime < 3)
    {
        fR = 0;
        fG = fC;
        fB = fX;
    } else if (3 <= fHPrime && fHPrime < 4)
    {
        fR = 0;
        fG = fX;
        fB = fC;
    } else if (4 <= fHPrime && fHPrime < 5)
    {
        fR = fX;
        fG = 0;
        fB = fC;
    } else if (5 <= fHPrime && fHPrime < 6)
    {
        fR = fC;
        fG = 0;
        fB = fX;
    } else
    {
        fR = 0;
        fG = 0;
        fB = 0;
    }

    fR += fM;
    fG += fM;
    fB += fM;
}

TestGameApp::TestGameApp()
        : CEApp()
{

}

TestGameApp::~TestGameApp()
{

}

bool TestGameApp::StartUp(unsigned int screenWidth, unsigned int screenHeight)
{
    CEApp::StartUp(screenWidth, screenHeight);

    presentedInputBackend->RegisterInput("Horizontal", "d", "a", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
    presentedInputBackend->RegisterInput("Horizontal", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis, JoystickLX, -1);
    presentedInputBackend->RegisterInput("Vertical", "w", "s", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
    presentedInputBackend->RegisterInput("Vertical", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis, JoystickLY, -1);
    presentedInputBackend->RegisterInput("CameraHorizontal", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis,
                                         JoystickRX, -1);
    presentedInputBackend->RegisterInput("CameraHorizontal", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Movement,
                                         MouseX, -1);
    presentedInputBackend->RegisterInput("CameraVertical", "", "", "", "", 10.0f, 0.1f, 10.0f, true, Axis, JoystickRY,
                                         -1);
    presentedInputBackend->RegisterInput("CameraVertical", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Movement, MouseY,
                                         -1);
    presentedInputBackend->RegisterInput("ArrowHorizontal", "joystick right", "joystick left", "right", "left", 10.0f,
                                         0.1f, 10.0f, false, Button, MouseX, -1);
    presentedInputBackend->RegisterInput("ArrowVertical", "joystick up", "joystick down", "up", "down", 10.0f, 0.1f,
                                         10.0f, false, Button, MouseX, -1);
    presentedInputBackend->RegisterInput("TestMouseLeft", "mouse 0", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
    presentedInputBackend->RegisterInput("TestMouseRight", "mouse 1", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
    presentedInputBackend->RegisterInput("Wheel", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Movement, MouseWheel,
                                         -1);
    presentedInputBackend->RegisterInput("ZoomIn", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis, JoystickRT, -1);
    presentedInputBackend->RegisterInput("ZoomOut", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis, JoystickLT, -1);
    presentedInputBackend->RegisterInput("Exit", "escape", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
    presentedInputBackend->RegisterInput("Exit", "joystick b", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX,
                                         -1);


    //presentedInputBackend->DisableCursor();


    boost::container::vector<Vertex> vertices_1;

    Vertex orig = {};
    orig.color = {1.0f, 1.0f, 1.0f};

    boost::container::vector<uint16_t> indices_1;

    float radius = 0.5f;
    int precision = 1000;

    vertices_1.reserve(precision + 1);
    indices_1.reserve(vertices_1.capacity() * 6);

    vertices_1.push_back(orig);

    for (int i = 0; i < precision; ++i)
    {
        indices_1.push_back(0);
        indices_1.push_back(i % precision + 1);
        indices_1.push_back((i + 1) % precision + 1);
        indices_1.push_back(0);
        indices_1.push_back((i + 1) % precision + 1);
        indices_1.push_back(i % precision + 1);

        float h = 360.0f / precision * i;
        float s = 1.0f;
        float v = 1.0f;
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        HSVtoRGB(r, g, b, h, s, v);

        float theta = float(i) / precision * 3.14159265f * 2;

        Vertex newVert = {};
        newVert.pos = {cosf(theta) * radius, 0.0f, sinf(theta) * radius};
        newVert.color = {r, g, b};
        vertices_1.push_back(newVert);
    }

    Object* testObject = CurrentActiveScene()->AddObject("TestObject");
    MeshRenderer* meshRenderer = testObject->AddComponent<MeshRenderer>();

    boost::shared_ptr<Mesh> mesh = boost::make_shared<Mesh>();
    mesh->LoadVertexData((void*) (&*vertices_1.begin()), sizeof(Vertex), vertices_1.size());
    mesh->LoadIndexData((uint16_t*) (&*indices_1.begin()), indices_1.size());

    boost::shared_ptr<Material> material = boost::make_shared<Material>();
    material->LoadVertexShader("Shaders/shader.vert");
    material->LoadPixelShader("Shaders/shader.frag");

    meshRenderer->SetMaterial(material);
    meshRenderer->SetMesh(mesh);

    testObject->transform->SetLocalScale(5.0f, 5.0f, 5.0f);

    Object* testObject_1 = CurrentActiveScene()->LoadModelFile("Assets/Models/pokemon/Models/025_00_0/0.obj");
    testObject_1->transform->SetLocalScale(0.02f, 0.02f, 0.02f);

    ObjectMovement* movement_1 = testObject_1->AddComponent<ObjectMovement>();

    AudioSource* audioSource = testObject_1->AddComponent<AudioSource>();
    audioSource->LoadAudioFile("Assets/KibounoTsubomi.flac");
    //audioSource->LoadAudioFile("Assets/Audio/low_on.wav");
    //audioSource->Loop = true;
    //audioSource->Play();

    movement_1->audioSource = audioSource;

//    boost::random::random_device rd;
//    boost::random::mt19937 gen(rd());
//    boost::random::uniform_real_distribution<float> dis(0.0f, 1.0f);
//    auto rand = boost::bind(dis, gen);
//
//    Object* firstObject = nullptr;
//    boost::shared_ptr<Mesh> mesh = nullptr;
//    boost::shared_ptr<Material> material = nullptr;
//
//    for (int i = 0; i < 300; ++i)
//    {
//        float size = rand() * 0.8f + 0.2f;
//        Object* testObject;
//
//        if (firstObject == nullptr)
//        {
//            testObject = CurrentActiveScene()->LoadModelFile("Assets/Models/Rock/sphere.obj");
//            firstObject = testObject;
//            auto renderer = testObject->transform->GetChildAt(0)->object->GetComponent<MeshRenderer>();
//            mesh = renderer->GetMesh();
//            material = renderer->GetMaterial();
//        }
//        else
//        {
//            testObject = CurrentActiveScene()->AddObject(firstObject->name);
//            auto realObject = CurrentActiveScene()->AddObject(firstObject->transform->GetChildAt(0)->object->name);
//            realObject->transform->SetParent(testObject->transform);
//
//            auto renderer = realObject->AddComponent<MeshRenderer>();
//            renderer->SetMaterial(material);
//            renderer->SetMesh(mesh);
//        }
//
//        testObject->transform->SetLocalScale(size, size, size);
//        testObject->transform->SetLocalTranslation(rand() * 20.0f - 10.0f, rand() * 20.0f - 10.0f, rand() * 20.0f - 10.0f);
//
//        SphereCollider* collider = testObject->AddComponent<SphereCollider>();
//        collider->mass = size * 1.0f;
//        collider->radius = size * 0.5f;
//    }



    auto mainCamera = CurrentActiveScene()->mainCamera;

    mainCamera->transform->SetLocalTranslation(0.0f, 5.0f, -5.0f);
    mainCamera->transform->SetLocalRotation(glm::angleAxis(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    mainCamera->AddComponent<CameraMovement>();

    LOG_INFO << "Scene Structure:";
    boost::container::list<Object*> allObjects = CurrentActiveScene()->GetAllObjects();

    std::stack<Object*> objectStack;
    std::stack<int> objectLevel;

    for (Object* obj : boost::adaptors::reverse(allObjects))
    {
        if (obj->transform->GetParent() == nullptr)
        {
            objectStack.push(obj);
            objectLevel.push(0);
        }
    }

    while (!objectStack.empty())
    {
        // Get the object
        Object* currentObject = objectStack.top();
        int currentLevel = objectLevel.top();
        // Process DFS
        objectStack.pop();
        objectLevel.pop();
        boost::container::list<Transform*> children = currentObject->transform->GetChildren();
        for (Transform* child : boost::adaptors::reverse(children))
        {
            objectStack.push(child->object);
            objectLevel.push(currentLevel + 1);
        }

        // Output Info
        boost::container::string tabs = "\t";
        for (int i = 0; i < currentLevel; ++i)
            tabs += "\t";
        LOG_INFO << tabs << currentObject->name << "\t{" << currentObject->GetInstanceID() << "}";
    }

    return true;
}

void TestGameApp::Shutdown()
{
    CEApp::Shutdown();
}

