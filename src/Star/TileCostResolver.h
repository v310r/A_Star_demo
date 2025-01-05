#pragma once


class BaseTile;

class TileCostResolver
{
public:
	static int32_t ResolveCost(BaseTile* from, BaseTile* to);
};