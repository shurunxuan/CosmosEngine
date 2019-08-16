//
// Created by shuru on 8/15/2019.
//

#include "Scene.h"
#include "../Logging/Logging.h"

Scene::Scene()
{
    mainCamera = new Camera(this);

    device = nullptr;
    context = nullptr;
}

Scene::~Scene()
{
    delete mainCamera;
    for (Object* object : allObjects)
    {
        delete object;
    }
    allObjects.clear();
    for (Light* light : lights)
    {
        delete light;
    }
    lights.clear();
}

void Scene::SetD3D11Device(ID3D11Device* d, ID3D11DeviceContext* c)
{
    device = d;

    context = c;
}

void Scene::SetDefaultShader(SimpleVertexShader* vs, SimplePixelShader* ps)
{
    defaultVS = vs;
    defaultPS = ps;
}

Object* Scene::AddObject(std::string name)
{
    Object* newObj = new Object(this, name);
    allObjects.push_back(newObj);
    return newObj;
}


Object* Scene::Instantiate(Object* obj)
{
}

void Scene::DestroyObject(Object* obj)
{
    const auto result = std::find(allObjects.begin(), allObjects.end(), obj);
    LOG_DEBUG << "Destroied object: " << obj->name;
    if (result == allObjects.end())
    { return; }
    allObjects.erase(result);
    delete obj;
}

std::list<Object*> Scene::GetAllObjects()
{
    return allObjects;
}

Object* Scene::LoadModelFile(const std::string& filename)
{
    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll 
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(filename,
                                             aiProcess_CalcTangentSpace
                                             | aiProcess_Triangulate
                                             | aiProcess_JoinIdenticalVertices
                                             | aiProcess_SortByPType
                                             | aiProcess_ConvertToLeftHanded
                                             | aiProcess_GenSmoothNormals
            //| aiProcess_PreTransformVertices
    );

    // If the import failed, report it
    if (!scene)
    {
        LOG_ERROR << importer.GetErrorString();
        return nullptr;
    }

    aiNode* currentNode = scene->mRootNode;

    Object* newObj = AddObjectWithNode(filename, scene, currentNode, nullptr);

    importer.FreeScene();

    return newObj;
}

void Scene::AddLight(LightData light)
{
    lightData.push_back(light);

    Light* newLight = new Light(&lightData.back(), device, context, mainCamera, &aabb);
    lights.push_back(newLight);
}

Light* Scene::GetLightAt(int index)
{
    auto it = lights.begin();
    std::advance(it, index);
    return *it;
}

int Scene::GetLightCount()
{
    return int(lightData.size());
}

void Scene::RemoveLightAt(int index)
{
    lightData.erase(lightData.begin() + index);

    auto it = lights.begin();
    std::advance(it, index);
    Light* lightToBeRemoved = *it;
    delete lightToBeRemoved;
    lights.erase(it);
}

void Scene::Update(float deltaTime, float totalTime)
{
    mainCamera->Update(deltaTime, totalTime);

    const float inf = std::numeric_limits<float>::infinity();
    DirectX::XMVECTOR lower = DirectX::XMVectorSet(inf, inf, inf, inf);
    DirectX::XMVECTOR upper = DirectX::XMVectorSet(-inf, -inf, -inf, -inf);

    for (Object* object : allObjects)
    {
        object->Update(deltaTime, totalTime);

        // Update AABB
        std::list<MeshRenderer*> meshRenderers = object->GetComponents<MeshRenderer>();
        for (MeshRenderer* renderer : meshRenderers)
        {
            //renderer->GetMesh()->aabb;
            DirectX::BoundingBox transformedAABB;
            renderer->GetMesh()->aabb.Transform(transformedAABB, renderer->object->transform->GetGlobalWorldMatrix());

            DirectX::XMVECTOR center = XMLoadFloat3(&transformedAABB.Center);
            DirectX::XMVECTOR extend = XMLoadFloat3(&transformedAABB.Extents);

            DirectX::XMVECTOR minPt = DirectX::XMVectorSubtract(center, extend);
            DirectX::XMVECTOR maxPt = DirectX::XMVectorAdd(center, extend);

            lower = DirectX::XMVectorMin(lower, minPt);
            upper = DirectX::XMVectorMax(upper, maxPt);
        }
    }

    DirectX::BoundingBox::CreateFromPoints(aabb, lower, upper);

    for (Light* light : lights)
    {
        light->UpdateMatrices();
    }
}

Object* Scene::FindObjectByName(std::string name)
{
    for (Object* obj : allObjects)
    {
        if (obj->name == name)
        {
            return obj;
        }
    }
    return nullptr;
}

std::list<Object*> Scene::FindObjectsByName(std::string name)
{
    std::list<Object*> result;
    for (Object* obj : allObjects)
    {
        if (obj->name == name)
        {
            result.push_back(obj);
        }
    }
    return result;
}

Object* Scene::AddObjectWithNode(const std::string& modelFileName, const aiScene* scene, aiNode* node, Object* parent)
{
    Object* newObj = AddObject(node->mName.C_Str());

    // Set Parent
    if (parent != nullptr)
    {
        newObj->transform->SetParent(parent->transform);
    }

    // Add MeshRenderer to this object
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        const unsigned int meshIndex = node->mMeshes[i];

        aiMesh* aMesh = scene->mMeshes[meshIndex];

        std::vector<Vertex> vertices;
        std::vector<int> indices;

        vertices.reserve(aMesh->mNumVertices);
        unsigned int indicesCount = 0;
        for (UINT c = 0; c < aMesh->mNumFaces; c++)
        {
            indicesCount += aMesh->mFaces[c].mNumIndices;
        }
        indices.reserve(indicesCount);

        for (UINT j = 0; j < aMesh->mNumVertices; j++)
        {
            Vertex newVtx;
            newVtx.Position = {aMesh->mVertices[j].x, aMesh->mVertices[j].y, aMesh->mVertices[j].z};
            if (aMesh->HasNormals())
            {
                newVtx.Normal = {aMesh->mNormals[j].x, aMesh->mNormals[j].y, aMesh->mNormals[j].z};
            }
            if (aMesh->HasTangentsAndBitangents())
            {
                newVtx.Tangent = {aMesh->mTangents[j].x, aMesh->mTangents[j].y, aMesh->mTangents[j].z};
            }
            if (aMesh->HasTextureCoords(0))
            {
                newVtx.UV = {aMesh->mTextureCoords[0][j].x, aMesh->mTextureCoords[0][j].y};
            }

            vertices.push_back(newVtx);
        }

        for (UINT c = 0; c < aMesh->mNumFaces; c++)
        {
            for (UINT e = 0; e < aMesh->mFaces[c].mNumIndices; e++)
            {
                indices.push_back(int(aMesh->mFaces[c].mIndices[e]));
            }
        }

        // MeshRenderer
        auto* meshRendererComponent = newObj->AddComponent<MeshRenderer>();

        // Material
        std::shared_ptr<PBRMaterial> pbrMaterial = std::make_shared<PBRMaterial>(defaultVS, defaultPS, device, context);

        // Load Textures
        aiMaterial* aMaterial = scene->mMaterials[aMesh->mMaterialIndex];

        // Diffuse Texture
        unsigned int diffuseTextureCount = aMaterial->GetTextureCount(aiTextureType_DIFFUSE);
        if (diffuseTextureCount > 0)
        {
            aiString as;
            aiReturn ar;
            // Only get the first texture
            ar = aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &as);
            if (ar == aiReturn_SUCCESS)
            {
                // Try load the texture assuming the path is absolute
                if (!pbrMaterial->LoadDiffuseTexture(as.C_Str()))
                {
                    // Try the path relative to model file
                    boost::filesystem::path modelPath(modelFileName);
                    boost::filesystem::path modelFolder = modelPath.parent_path();
                    boost::filesystem::path relativeTexture(as.C_Str());
                    relativeTexture = modelFolder / relativeTexture;
                    pbrMaterial->LoadDiffuseTexture(relativeTexture.generic_string());
                }
            }
        }

        // Normal Texture
        unsigned int normalTextureCount = aMaterial->GetTextureCount(aiTextureType_NORMALS);
        if (normalTextureCount > 0)
        {
            aiString as;
            aiReturn ar;
            // Only get the first texture
            ar = aMaterial->GetTexture(aiTextureType_NORMALS, 0, &as);
            if (ar == aiReturn_SUCCESS)
            {
                // Try load the texture assuming the path is absolute
                if (!pbrMaterial->LoadNormalTexture(as.C_Str()))
                {
                    // Try the path relative to model file
                    boost::filesystem::path modelPath(modelFileName);
                    boost::filesystem::path modelFolder = modelPath.parent_path();
                    boost::filesystem::path relativeTexture(as.C_Str());
                    relativeTexture = modelFolder / relativeTexture;
                    pbrMaterial->LoadNormalTexture(relativeTexture.generic_string());
                }
            }
        }
        aiColor3D color(0.f, 0.f, 0.f);
        aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        pbrMaterial->parameters.albedo = DirectX::XMFLOAT3(color.r, color.g, color.b);

        float opacity = 1.0f;
        aMaterial->Get(AI_MATKEY_OPACITY, opacity);
        if (opacity < 1.0f)
        {
            pbrMaterial->transparent = true;
            D3D11_RENDER_TARGET_BLEND_DESC blendDesc;
            ZeroMemory(&blendDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
            blendDesc.BlendEnable = TRUE;
            blendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
            blendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            blendDesc.BlendOp = D3D11_BLEND_OP_ADD;
            blendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
            blendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
            blendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
            blendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            pbrMaterial->SetBlendMode(blendDesc);
            pbrMaterial->parameters.transparency = 1 - opacity;
        }
        meshRendererComponent->SetMaterial(pbrMaterial);

        // Mesh
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(&*vertices.begin(), int(vertices.size()), &*indices.begin(),
                                                            int(indices.size()), device);
        meshRendererComponent->SetMesh(mesh);
    }

    // Add child nodes
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        Object* childObj = AddObjectWithNode(modelFileName, scene, node->mChildren[i], newObj);
    }

    return newObj;
}