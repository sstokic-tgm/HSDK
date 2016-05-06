#include "VTManager.hpp"
#include "EngineTrace.hpp"

TraceFilterSkipTwoEntities::TraceFilterSkipTwoEntities(void *pPassEnt1, void *pPassEnt2)
{
	pSkip1 = pPassEnt1;
	pSkip2 = pPassEnt2;
}

bool TraceFilterSkipTwoEntities::ShouldHitEntity(void *pEntityHandle, int contentsMask)
{
	return  !(pEntityHandle == pSkip1 || pEntityHandle == pSkip2);
}

TraceType_t TraceFilterSkipTwoEntities::GetTraceType() const
{
	return TraceType_t::TRACE_EVERYTHING;
}

TraceFilterSkipCurrentEntity::TraceFilterSkipCurrentEntity(void *pPassEnt)
{
	pSkip = pPassEnt;
}

bool TraceFilterSkipCurrentEntity::ShouldHitEntity(void *pEntityHandle, int contentsMask)
{
	return !(pEntityHandle == pSkip);
}

TraceType_t TraceFilterSkipCurrentEntity::GetTraceType() const
{
	return TraceType_t::TRACE_EVERYTHING;
}

void EngineTrace::TraceRay(const Ray_t &ray, unsigned int fMask, TraceFilter *pTraceFilter, GTrace *pTrace)
{
	typedef void(__thiscall* o_TraceRay)(void*, const Ray_t&, unsigned int, TraceFilter *, GTrace*);
	VTManager::vfunc<o_TraceRay>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
}