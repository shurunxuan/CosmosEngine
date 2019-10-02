//
// Created by shuru on 9/8/2019.
//

#ifndef GAMEENGINE_MESHRENDERER_H
#define GAMEENGINE_MESHRENDERER_H

#include "Component.h"
#include "Mesh.h"
#include "Material.h"
#include "../Rendering/RenderingPipeline.h"
#include "../Rendering/CommandBuffer.h"

#include <boost/shared_ptr.hpp>

class ENGINE_API MeshRenderer : public Component
{
public:
    explicit MeshRenderer(Object* owner);

    /**
     * @brief Destroy the Component object
     *
     */
    ~MeshRenderer() final;

    /**
     * @brief Set the Mesh of the MeshRenderer
     *
     * @param m The mesh
     */
    void SetMesh(const boost::shared_ptr<Mesh>& m);

    /**
     * @brief Set the Material of the MeshRenderer
     *
     * @param m The material
     */
    void SetMaterial(const boost::shared_ptr<Material>& m);

    boost::shared_ptr<Material>& GetMaterial();

    boost::shared_ptr<Mesh>& GetMesh();

    /**
     * @brief Will be called as soon as the component is created
     *
     */
    void Start() final;

    /**
     * @brief Will be called when the object updates
     *
     * @param deltaTime The time that a frame costs
     * @param totalTime The total time from the beginning of the application
     */
    void Update(float deltaTime, float totalTime) final;

    void RecreateRenderingResources();

    CommandBuffer* GetCommandBuffer();

private:

    boost::shared_ptr<Mesh> mesh;

    boost::shared_ptr<Material> material;

    CommandBuffer* commandBuffer;

    void createCommandBuffer();
};

#endif //GAMEENGINE_MESHRENDERER_H
