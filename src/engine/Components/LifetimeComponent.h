#pragma once

// Stores how long an entity should exist.
struct LifetimeComponent
{
    float TTL;
	LifetimeComponent() = default;
	LifetimeComponent(float ttl) : TTL(ttl) 
	{
	}
};