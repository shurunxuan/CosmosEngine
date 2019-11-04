//
// Created by shuru on 11/3/2019.
//
#include <boost/container/list.hpp>
#include "PhysicsSystem.h"
#include "../Threading/JobSystem.h"
#include "../App/App.h"
#include "SphereCollider.h"

#define USE_MULTICORE

PhysicsSystem::PhysicsSystem()
{

}

PhysicsSystem::~PhysicsSystem()
{

}

bool PhysicsSystem::StartUp()
{
	return true;
}


struct rootData
{
	Job* selfPointer;
	boost::container::list<SphereCollider*>* colliders;
};

struct originData
{
	Job* selfPointer;
	boost::container::list<SphereCollider*>* colliders;
	SphereCollider* originCollider;
};
struct targetData
{
	Job* selfPointer;
	boost::container::list<SphereCollider*>* colliders;
	SphereCollider* originCollider;
	SphereCollider* targetCollider;
};

void originJobFunc(Job* job)
{
	originData* data = reinterpret_cast<originData*>(job->data);

	for (auto i = data->colliders->begin(); i != data->colliders->end(); ++i)
	{
		SphereCollider* targetCollider = *i;
		if (targetCollider == data->originCollider) continue;


		if (data->originCollider->CollideWithDiscrete(targetCollider))
		{
			data->originCollider->CalculateCollisionResponse(targetCollider);
		}
	}
}

void rootJobFunc(Job* job)
{
	rootData* data = reinterpret_cast<rootData*>(job->data);

	for (auto i = data->colliders->begin(); i != data->colliders->end(); ++i)
	{
		Job* origin = jobSystem->CreateJobAsChild(data->selfPointer, originJobFunc);

		originData oData{};
		oData.selfPointer = origin;
		oData.colliders = data->colliders;
		oData.originCollider = *i;

		memcpy(origin->data, &oData, sizeof(oData));

		jobSystem->Run(origin);
	}
}

void PhysicsSystem::Update(float deltaTime, float totalTime)
{
	boost::container::list<SphereCollider*> colliders;
	auto allObjects = App->CurrentActiveScene()->GetAllObjects();
	for (auto object : allObjects)
	{
		colliders.merge(object->GetComponents<SphereCollider>());
	}
	
	//LOG_DEBUG << &colliders;
	
	for (auto collider : colliders)
	{
		// Add to acc vector
		collider->acceleration = glm::vec3(0.0f, -9.8f, 0.0f) - collider->velocity * 0.001f;
		collider->velocity += collider->acceleration * deltaTime;
	}

#ifndef USE_MULTICORE
	for (auto i = colliders.begin(); i != colliders.end(); ++i)
	{
		for (auto j = colliders.begin(); j != colliders.end(); ++j)
		{
			if (i == j) continue;
			SphereCollider* a = *i;
			SphereCollider* b = *j;

			if (a->CollideWithDiscrete(b))
			{
				a->CalculateCollisionResponse(b);
			}
		}
	}
#else

	Job* root = jobSystem->CreateJob(rootJobFunc);

	rootData data{};
	data.selfPointer = root;
	data.colliders = &colliders;
	memcpy(root->data, &data, sizeof(data));

	jobSystem->Run(root);
	jobSystem->Wait(root);
#endif

}

void PhysicsSystem::Shutdown()
{

}
