//
// Created by shuru on 6/6/2020.
//

#ifndef GAMEENGINE_SERIALIZATION_H
#define GAMEENGINE_SERIALIZATION_H

#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/archives/json.hpp>

#define SERIALIZABLE_CLASS friend class cereal::access
#define SERIALIZATION_VERSION(class, version) CEREAL_CLASS_VERSION(class, version)
#define SERIALIZER template<class Archive> void serialize(Archive& ar, const unsigned int version)
#define SERIALIZED_FIELD(field) CEREAL_NVP(field)
#define ADD_FIELD(field) ar(SERIALIZED_FIELD(field))

typedef cereal::JSONOutputArchive SerializerO;
typedef cereal::JSONInputArchive SerializerI;

#endif //GAMEENGINE_SERIALIZATION_H
