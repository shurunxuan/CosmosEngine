/**
 * @file Scene.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the scene graph
 *
 * @version 0.1
 * @date 2019/03/09
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef GAMEENGINE_SCENE_H
#define GAMEENGINE_SCENE_H

//#include <d3d11.h>
//
//#include <DirectXCollision.h>

#include <list>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Object.h"
//#include "Camera.hpp"
//#include "Light.hpp"
//#include "SimpleShader.hpp"
//#include "MeshRenderer.hpp"
//#include "PBRMaterial.hpp"
#include <boost/filesystem/path.hpp>

/**
 * @brief The class represents the scene graph
 *
 */
class Scene
{
public:

    /**
     * @brief Construct a new Scene
     *
     */
    Scene();
    /**
     * @brief Destroy the Scene
     *
     */
    ~Scene();

    /**
     * @brief Set the device and context
     *
     * @param d Direct3D 11 device
     * @param c Direct3D 11 device context
     */
    //void SetD3D11Device(ID3D11Device* d, ID3D11DeviceContext* c);
    /**
     * @brief Set the Default Shaders
     *
     * @param vs Default vertex shader of the scene
     * @param ps Default pixel shader of the scene
     */
    //void SetDefaultShader(SimpleVertexShader* vs, SimplePixelShader* ps);

    /**
     * @brief Add an object to the scene
     *
     * @param name The name of the object, default to "GameObject"
     * @return Object* The pointer of the added object
     */
    Object* AddObject(std::string name = "GameObject");
    /**
     * @brief Instantiate a new object from a current object
     *
     * @param obj The template object used to instantiate the new one
     * @return Object* The pointer of the instantiated object
     *
     * @todo THIS DOES NOTHING! DON'T FORGET TO IMPLEMENT THIS
     */
    Object* Instantiate(Object* obj);
    /**
     * @brief Destror an object in the scene graph
     *
     * Nothing happens if there's no such object
     *
     * @param obj The pointer of the object to be deleted
     *
     * @todo THIS DOES NOTHING! DON'T FORGET TO IMPLEMENT THIS
     */
    void DestroyObject(Object* obj);

    /**
     * @brief Get the list of all objects
     *
     * @return std::list<Object*> The list of all objects
     */
    std::list<Object*> GetAllObjects();

    /**
     * @brief Load a model file and add the object(s) to the scene
     *
     * @param filename The filename of the model
     * @return Object* The pointer of the parent object added to the scene from the model file
     */
    Object* LoadModelFile(const std::string& filename);

    /**
     * @brief Add a light to the scene
     *
     * @param light The data of the light
     */
    //void AddLight(LightData light);
    /**
     * @brief Get the light
     *
     * @param index The index of the light
     * @return Light* The pointer of the Light, nullptr if the index is invalid
     *
     * @todo The "nullptr" part is not implemented
     */
    //Light* GetLightAt(int index);
    /**
     * @brief Get the Light Count object
     *
     * @return int The number of Lights
     */
    int GetLightCount();
    /**
     * @brief Remove a light at a specified location
     *
     * Nothing happens if the index is invalid
     *
     * @param index The index of the light to be removed
     *
     * @todo The "invalid" part is not implemented
     */
    void RemoveLightAt(int index);

    /**
     * @brief Update the scene
     *
     * Update the camera, the objects and the lights
     *
     * @param deltaTime The time a frame costs
     * @param totalTime The total time from the beginning of the application
     */
    void Update(float deltaTime, float totalTime);

    /**
     * @brief Find an object in the scene by name
     *
     * @param name The name of desired object
     *
     * @return Object* A pointer of the found object, or nullptr
     */
    Object* FindObjectByName(std::string name);

    /**
     * @brief Find all objects in the scene with the name
     *
     * @param name The name of desired object
     *
     * @return std::list<Object*> The list of the pointers of the found objects
     */
    std::list<Object*> FindObjectsByName(std::string name);


    /**
     * @brief The main camera of the scene
     *
     */
    Camera* mainCamera;

    /**
     * @brief Direct3D 11 device
     *
     */
    //ID3D11Device* device;
    /**
     * @brief Direct3D 11 device context
     *
     */
    //ID3D11DeviceContext* context;

    /**
     * @brief The default vertex shader
     *
     */
    //SimpleVertexShader* defaultVS;
    /**
     * @brief The default pixel shader
     *
     */
    //SimplePixelShader* defaultPS;
private:
    /**
     * @brief Recursively add objects to the scene from a node of the model file
     *
     * @param modelFileName The file name of the model
     * @param scene The imported model file data structure
     * @param node Currently processing node
     * @param parent The direct parent object of the model being added
     * @return Object* The pointer of the object added to the scene
     */
    Object* AddObjectWithNode(const std::string& modelFileName, const aiScene* scene, aiNode* node, Object* parent);

    /**
     * @brief All objects
     *
     */
    std::list<Object*> allObjects;
    /**
     * @brief All light data, will be sent to the shader
     *
     */
    //std::vector<LightData> lightData;
    /**
     * @brief All lights
     *
     */
    //std::list<Light*> lights;



    /**
     * @brief The axis-aligned bounding box of the whole scene
     *
     */
    //DirectX::BoundingBox aabb;
};



#endif //GAMEENGINE_SCENE_H
