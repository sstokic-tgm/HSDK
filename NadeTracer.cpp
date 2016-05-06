#include "NadeTracer.hpp"

NadeTracer::NadeTracer(DrawManager *drawManager)
{
	drawManager = drawManager;
}

void NadeTracer::addTracer(Entity* entGrenade, Color colorTrace, unsigned int maxLength)
{
	setGrenadeExists.insert(entGrenade);
	if (mapGrenades.find(entGrenade) == mapGrenades.end())
		mapGrenades[entGrenade] = TraceObj(colorTrace, maxLength);

	if (mapGrenades[entGrenade].timerPointLock.diff() > 0.025f)
	{
		mapGrenades[entGrenade].vecTracePoints.push_back(entGrenade->getViewOrigin());
		if (mapGrenades[entGrenade].vecTracePoints.size() > mapGrenades[entGrenade].maxLength)
			mapGrenades[entGrenade].vecTracePoints.erase(mapGrenades[entGrenade].vecTracePoints.begin());

		mapGrenades[entGrenade].timerPointLock.init();
	}
}

void NadeTracer::draw()
{
	vec3_t vecLastScreenPos;
	bool bInit = false;

	for (auto& traceObj : mapGrenades)
	{
		for (auto& vecPos : traceObj.second.vecTracePoints)
		{
			vec3_t vecScreenPos;
			if (mUtil->w2s(vecPos, vecScreenPos))
			{
				if (vecScreenPos.z > 0.001f)
				{
					if (bInit)
						drawManager->drawLine(vecLastScreenPos.x, vecLastScreenPos.y, vecScreenPos.x, vecScreenPos.y, traceObj.second.colorTrace);

					vecLastScreenPos = vecScreenPos;
					bInit = true;
				}
			}
		}
		bInit = false;
	}
}

void NadeTracer::clear()
{
	for (auto it = mapGrenades.begin(); it != mapGrenades.end(); ++it)
	{
		if (setGrenadeExists.find((*it).first) == setGrenadeExists.end())
		{
			it = mapGrenades.erase(it);
			if (it == mapGrenades.end())
				break;
		}
	}
	setGrenadeExists.clear();
}

TraceObj::TraceObj(Color colorTrace, unsigned int maxLength)
{
	this->colorTrace = colorTrace;
	this->maxLength = maxLength;
	timerPointLock.init();
}