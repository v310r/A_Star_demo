#include "pch.h"

#include "TileCostResolver.h"

#include "Tiles/DangerousTile.h"


#define FROM_DANGEROUS_TO_WALKABLE_COST 4
#define FROM_WALKABLE_TO_DANGEROUS_COST 16384
#define FROM_DANGEROUS_TO_DANGEROUS_COST 16384
#define FROM_WALKABLE_TO_WALKABLE_COST 4

int32_t TileCostResolver::ResolveCost(BaseTile* from, BaseTile* to)
{
	// kinda ugly and not very performant, but who cares xD
	DangerousTile* dangerousFrom = dynamic_cast<DangerousTile*>(from);
	DangerousTile* dangerousTo = dynamic_cast<DangerousTile*>(to);

	if (dangerousFrom && dangerousTo)
	{
		return FROM_DANGEROUS_TO_DANGEROUS_COST;
	}

	if (dangerousFrom && !dangerousTo)
	{
		return FROM_DANGEROUS_TO_WALKABLE_COST;
	}

	if (!dangerousFrom && !dangerousTo)
	{
		return FROM_WALKABLE_TO_WALKABLE_COST;
	}

	return FROM_WALKABLE_TO_DANGEROUS_COST;
}

