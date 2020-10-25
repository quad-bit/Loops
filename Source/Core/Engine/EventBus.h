#pragma once

#include <map>
#include <list>
#include <typeindex>
#include "HandlerFunctionBase.h"

using namespace std;

typedef std::list<HandlerFunctionBase*> HandlerList;

class EventBus 
{
public:
	template<typename EventType>
	void publish(EventType * evnt) {
		HandlerList * handlers = subscribers[typeid(EventType)];

		if (handlers == nullptr) {
			return;
		}

		for (auto & handler : *handlers) {
			if (handler != nullptr) {
				handler->exec(evnt);
			}
		}
	}

	template<class T, class EventType>
	void subscribe(T * instance, void (T::*memberFunction)(EventType *)) {
		HandlerList * handlers = subscribers[typeid(EventType)];

		//First time initialization
		if (handlers == nullptr) {
			handlers = new HandlerList();
			subscribers[typeid(EventType)] = handlers;
		}

		handlers->push_back(new MemberFunctionHandler<T, EventType>(instance, memberFunction));
	}
private:
	std::map<std::type_index, HandlerList*> subscribers;
};


////////////////////  reference 
/*
struct CollisionEvent : public Event {
	CollisionEvent(Entity a, Entity b) : entityA{ a }, entityB{ b } {}
	Entity entityA;
	Entity entityB;
}

class CombatSystem : public System {
public:
	void init() {
		eventBus->subscribe(this, &CombatSystem::onCollisionEvent);
	}

	void onCollisionEvent(CollisionEvent * collision) {
		world->destroyEntity(collision->entityA);
		world->destroyEntity(collision->entityB);
	}
};


// Physics System
// For each pair of entities
if (checkCollision(entityA, entityB)) {
	eventBus->publish(new CollisionEvent(entityA, entityB));
}

*/