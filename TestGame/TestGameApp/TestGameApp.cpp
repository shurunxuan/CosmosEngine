#include <CosmosEngine/Logging/Logging.h>
#include "TestGameApp.h"
#include <boost/container/list.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <CosmosEngine/Core/MeshRenderer.h>
#include "../Scripts/ObjectMovement.h"
#include "../Scripts/CameraMovement.h"

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
};

const boost::container::vector<Vertex> vertices = {
        {{-0.5f, 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f,  0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f,  0.0f, 0.5f},  {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.0f, 0.5f},  {1.0f, 1.0f, 1.0f}}
};

const boost::container::vector<uint16_t> indices = {
        0, 2, 1, 2, 0, 3, 0, 1, 2, 2, 3, 0
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
    float fHPrime = fmod(fH / 60.0, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
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

    Object* testObject = CurrentActiveScene()->AddObject("TestObject");
    MeshRenderer* meshRenderer = testObject->AddComponent<MeshRenderer>();

    boost::shared_ptr<Mesh> mesh = boost::make_shared<Mesh>();
    mesh->LoadVertexData((void*) (&*vertices.begin()), sizeof(Vertex), vertices.size());
    mesh->LoadIndexData((uint16_t*) (&*indices.begin()), indices.size());

    boost::shared_ptr<Material> material = boost::make_shared<Material>();
    material->LoadVertexShader("Shaders/VertexShader.hlsl");
    material->LoadPixelShader("Shaders/shader.frag");

    meshRenderer->SetMaterial(material);
    meshRenderer->SetMesh(mesh);

    ObjectMovement* movement = testObject->AddComponent<ObjectMovement>();

    movement->movementType = 0;


    boost::container::vector<Vertex> vertices_1 =
            {
                    {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}
            };

    boost::container::vector<uint16_t> indices_1;

    float radius = 0.5f;
    int precision = 1000;

    vertices_1.reserve(precision + 1);
    indices_1.reserve(vertices_1.capacity() * 6);

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

        Vertex newVert;
        newVert.color = {r, g, b};
        newVert.pos = {cosf(theta) * radius, 0.0f, sinf(theta) * radius};

        vertices_1.push_back(newVert);
    }


    Object* testObject_1 = CurrentActiveScene()->AddObject("TestObject_1");
    MeshRenderer* meshRenderer_1 = testObject_1->AddComponent<MeshRenderer>();

    boost::shared_ptr<Mesh> mesh_1 = boost::make_shared<Mesh>();
    mesh_1->LoadVertexData((void*) (&*vertices_1.begin()), sizeof(Vertex), vertices_1.size());
    mesh_1->LoadIndexData((uint16_t*) (&*indices_1.begin()), indices_1.size());

    boost::shared_ptr<Material> material_1 = boost::make_shared<Material>();
    material_1->LoadVertexShader("Shaders/VertexShader.hlsl");
    material_1->LoadPixelShader("Shaders/shader.frag");

    meshRenderer_1->SetMaterial(material_1);
    meshRenderer_1->SetMesh(mesh_1);

    ObjectMovement* movement_1 = testObject_1->AddComponent<ObjectMovement>();

    movement_1->movementType = 1;


    CurrentActiveScene()->mainCamera->AddComponent<CameraMovement>();

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

