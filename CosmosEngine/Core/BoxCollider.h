//
// Created by shuru on 10/13/2019.
//

#ifndef GAMEENGINE_BOXCOLLIDER_H
#define GAMEENGINE_BOXCOLLIDER_H

#include <glm/glm.hpp>
#include "Collider.h"

class ENGINE_API BoxCollider : public Collider
{
public:

    explicit BoxCollider(Object* owner);

    ~BoxCollider() final;

    void Start() override;

    void Update(float deltaTime, float totalTime) override;

    void SetCenter(const glm::vec3& c);

    void SetSize(const glm::vec3& s);

    glm::vec3 GetCenter();

    glm::vec3 GetSize();

private:

    btVector3 center;

    btVector3 size;

};

#endif //GAMEENGINE_BOXCOLLIDER_H
