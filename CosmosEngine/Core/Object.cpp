//
// Created by shuru on 8/20/2019.
//

#include "Object.h"

void Object::Update(float deltaTime, float totalTime) {
    for (Component *component : components) {
        if (component->isActive)
            component->Update(deltaTime, totalTime);
    }
}

//Scene *Object::GetScene() {
//    return owner;
//}

Object::Object(/*Scene *scene*/)
        : id(boost::uuids::random_generator()()), name("Object"), /*owner(scene), */isHidden(false) {
    transform = AddComponent<Transform>();
}

Object::Object(/*Scene *scene, */std::string name)
        : id(boost::uuids::random_generator()()), name(std::move(name)), /*owner(scene), */isHidden(false) {
    transform = AddComponent<Transform>();
}

Object::~Object() {
    for (Component *component : components) {
        delete component;
    }
    components.clear();
}

boost::uuids::uuid Object::GetInstanceID() const {
    return id;
}

bool operator==(const Object &v1, const Object &v2) {
    return v1.id == v2.id;
}

bool operator!=(const Object &v1, const Object &v2) {
    return v1.id != v2.id;
}