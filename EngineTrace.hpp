#pragma once

#include <d3dx9math.h>
#include "Util_Vector.hpp"

#define DISPSURF_FLAG_SURFACE (1<<0)
#define DISPSURF_FLAG_WALKABLE (1<<1)
#define DISPSURF_FLAG_BUILDABLE (1<<2)
#define DISPSURF_FLAG_SURFPROP1 (1<<3)
#define DISPSURF_FLAG_SURFPROP2 (1<<4)

class GTrace;
typedef GTrace trace_t;

enum class TraceType_t {

	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

struct cplane_t
{
	vec3_t normal;
	float dist;
	BYTE type;
	BYTE signbits;
	BYTE pad[2];
};

struct csurface_t
{
	const char* name;
	short surfaceProps;
	unsigned short flags;
};

class BTrace
{
public:

	inline bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	inline bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	inline bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	inline bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	inline bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:

	vec3_t startpos;
	vec3_t endpos;
	cplane_t plane;
	float fraction;
	int contents;
	unsigned short dispFlags;
	bool allsolid;
	bool startsolid;

	BTrace() {}
};

class GTrace : public BTrace
{
public:

	bool DHW() const;
	bool DHNWE() const;
	int GEI() const;
	bool DH() const;
	bool IV() const;

public:

	float fractionleftsolid;
	csurface_t surface;
	int hitgroup;
	short physicsbone;
	unsigned short worldSurfaceIndex;
	void *m_pEntityHit;
	int hitbox;

	GTrace() {}

private:

	GTrace(const GTrace &other) : fractionleftsolid(other.fractionleftsolid), surface(other.surface), hitgroup(other.hitgroup), physicsbone(other.physicsbone), worldSurfaceIndex(other.worldSurfaceIndex), m_pEntityHit(other.m_pEntityHit), hitbox(other.hitbox)
	{
		startpos = other.startpos;
		endpos = other.endpos;
		plane = other.plane;
		fraction = other.fraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allsolid = other.allsolid;
		startsolid = other.startsolid;
	}

	GTrace &GTrace::operator=(const GTrace &other)
	{
		startpos = other.startpos;
		endpos = other.endpos;
		plane = other.plane;
		fraction = other.fraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allsolid = other.allsolid;
		startsolid = other.startsolid;
		fractionleftsolid = other.fractionleftsolid;
		surface = other.surface;
		hitgroup = other.hitgroup;
		physicsbone = other.physicsbone;
		worldSurfaceIndex = other.worldSurfaceIndex;
		m_pEntityHit = other.m_pEntityHit;
		hitbox = other.hitbox;
		return *this;
	}
};

inline bool GTrace::DH() const {

	return fraction < 1 || allsolid || startsolid;
}

inline bool GTrace::IV() const {

	return fraction > 0.97f;
}

struct Ray_t
{
	VectorAligned  m_Start;
	VectorAligned  m_Delta;
	VectorAligned  m_StartOffset;
	VectorAligned  m_Extents;

	const   D3DXMATRIX *m_pWorldAxisTransform;

	bool    m_IsRay;
	bool    m_IsSwept;

	Ray_t() : m_pWorldAxisTransform(NULL) { }

	void Init(vec3_t &vecStart, vec3_t &vecEnd)
	{
		m_Delta = vecEnd - vecStart;

		m_IsSwept = (m_Delta.lengthSqr() != 0);

		m_Extents.clear();

		m_pWorldAxisTransform = NULL;

		m_IsRay = true;

		m_StartOffset.clear();

		m_Start = vecStart;
	}
};

class TraceFilter
{
public:

	virtual bool ShouldHitEntity(void *pEntity, int contentsMask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class TraceFilterSkipTwoEntities : public TraceFilter
{
public:

	TraceFilterSkipTwoEntities(void *pPassEnt1, void *pPassEnt2);

	virtual bool ShouldHitEntity(void *pEntityHandle, int contentsMask);

	virtual TraceType_t GetTraceType() const;

	void *pSkip1;
	void *pSkip2;
};

class TraceFilterSkipCurrentEntity : public TraceFilter
{
public:

	TraceFilterSkipCurrentEntity(void *pPassEnt);

	virtual bool ShouldHitEntity(void *pEntityHandle, int contentsMask);

	virtual TraceType_t GetTraceType() const;

	void *pSkip;
};

class EngineTrace
{
public:

	void TraceRay(const Ray_t &ray, unsigned int fMask, TraceFilter *pTraceFilter, GTrace *pTrace);
};

extern EngineTrace *p_EngineTrace;