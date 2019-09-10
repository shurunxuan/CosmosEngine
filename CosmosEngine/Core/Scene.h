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

#include <boost/container/list.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem/path.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Export.h"
#include "Object.h"
#include "Camera.h"
//#include "Light.h"
//#include "SimpleShader.h"
//#include "MeshRenderer.h"
//#include "PBRMaterial.h"

/**
 * @brief The class represents the scene graph
 *
 */
class ENGINE_API Scene
{
public:
//    friend class DSSRendering;
//    friend class DSFDirect3D;
//    friend class DSSPhysics;
//    friend class DSSAudio;

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
    Object* AddObject(boost::container::string name = "GameObject");

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
     * @brief Destroy an object in the scene graph
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
     * @return boost::container::list<Object*> The list of all objects
     */
    boost::container::list<Object*> GetAllObjects();

    /**
     * @brief Load a model file and add the object(s) to the scene
     *
     * @param filename The filename of the model
     * @return Object* The pointer of the parent object added to the scene from the model file
     */
    Object* LoadModelFile(const boost::container::string& filename);

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
    //int GetLightCount();
    /**
     * @brief Remove a light at a specified location
     *
     * Nothing happens if the index is invalid
     *
     * @param index The index of the light to be removed
     *
     * @todo The "invalid" part is not implemented
     */
    //void RemoveLightAt(int index);

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
    Object* FindObjectByName(boost::container::string name);

    /**
     * @brief Find all objects in the scene with the name
     *
     * @param name The name of desired object
     *
     * @return boost::container::list<Object*> The list of the pointers of the found objects
     */
    boost::container::list<Object*> FindObjectsByName(boost::container::string name);


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
     * @brief Recursively add objects to the scene from a no/de of the model file
     *
     * @param modelFileName The file name of the model
     * @param scene The imported model file data structure
     * @param node Currently processing node
     * @param parent The direct parent object of the model being added
     * @return Object* The pointer of the object added to the scene
     */
    Object* AddObjectWithNode(const boost::container::string& modelFileName, const aiScene* scene, aiNode* node, Object* parent);

    /**
     * @brief All objects
     *
     */
    boost::container::list<Object*> allObjects;
    /**
     * @brief All light data, will be sent to the shader
     *
     */
    //boost::container::vector<LightData> lightData;
    /**
     * @brief All lights
     *
     */
    //boost::container::list<Light*> lights;



    /**
     * @brief The axis-aligned bounding box of the whole scene
     *
     */
    //DirectX::BoundingBox aabb;
};


#endif //GAMEENGINE_SCENE_H
