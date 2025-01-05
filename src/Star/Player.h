#pragma once


class Map;
class Camera;
class BaseTile;

enum class EPathDirection : uint8_t
{
	None = 0,
	LeftUp,
	RightUp,
	Up,
	Left,
	Right,
	LeftDown,
	RightDown,
	Down
};

struct AdjacentTilePathData
{
public:
	// rowCol index based
	glm::ivec2 TileLocation;

	BaseTile* TileObject = nullptr;

	EPathDirection RelativeDirection = EPathDirection::None;
};

struct ShortestKnownTileInfoPath
{
	glm::ivec2 PreviousClosestTileLocation = INVALID_TILE_INDEX;

	// accumulated path cost from start tile to this tile
	int32_t PathCost = 0;
};

struct QueuedTileInfo
{
public:
	glm::ivec2 TileLocation;

	BaseTile* TileObject = nullptr;

	// accumulated path cost from start tile to this tile
	int32_t PathCost = 0;

	// including Heuristic
	float TotalCost = 0;
};

class Player
{
public:
	Player(const Map& map, const Camera& camera);

	void Update(float deltaTime);

	void ImGuiFrameUpdate();

	void CheckPlayerInput();

	void VisitTilesFrom(glm::ivec2 sourceTile);

	void Reset();

protected:
	struct ivec2Hash
	{
		int operator()(const glm::ivec2& v) const noexcept
		{
			return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1); // Combine x and y hashes
		}
	};

	struct ivec2Equal
	{
		bool operator()(const glm::ivec2& a, const glm::ivec2& b) const noexcept
		{
			return a.x == b.x && a.y == b.y;
		}
	};

	void PaintPathSearch();

	void FindPathToEndTileBlocking();

	bool FindPathToEndTileTick();

	int32_t CalculatePathCost(BaseTile* from, BaseTile* to) const;
	int32_t CalculatePathCost(const glm::ivec2& from, const glm::ivec2& to) const;

	float CalculateHeuristicCost(const glm::ivec2& from) const;
	float CalculateHeuristicCost(BaseTile* from) const;

	void FillPathDataForTile(const glm::ivec2 sourceTileLoc);

	BaseTile* GetEndTile() const;

	void InitPathData();

	bool IsTilePriorityQueueEmpty() const;

	// it says all, but we do now take into account the "End" (target) tile
	bool AllTilesAreVisited() const;

	QueuedTileInfo* FindTileInPriorityQueue(glm::ivec2 tileLocation) const;

	ShortestKnownTileInfoPath* FindShortestPathFromStartToTile(glm::ivec2 targetTile) const;

	bool IsVisitedTileLocation(glm::ivec2 tileLocation) const;

	void FillPathArray();
	void FillPathArray_Recursive(glm::ivec2 targetTileLocation);

	void PaintPathToEndTile();

	void PaintVisitedTilePath();

	// Only walkable tiles fill this map
	std::unordered_map<glm::ivec2, std::vector<AdjacentTilePathData>, ivec2Hash, ivec2Equal> m_TilePathMap;

	std::vector<QueuedTileInfo> m_TilePriorityQueue;

	std::unordered_set<glm::ivec2, ivec2Hash, ivec2Equal> m_VisitedTileInfoSet;

	std::unordered_map<glm::ivec2, ShortestKnownTileInfoPath, ivec2Hash, ivec2Equal> m_ShortestKnownTileInfoPathMap;

	bool m_bStartTileChosen = false, m_bEndTileChosen = false;

	bool m_bPathFound = false;

	// from start [0] to end [size -1]
	std::vector<glm::ivec2> m_PathToEndTile;

	glm::ivec2 m_StartTilePosition, m_EndTilePosition;

	const Map& m_MapRef;
	const Camera& m_CameraRef;

	// whether calculate path each tick, or calculate it in one tick all together
	bool m_bBlockingPathFinding = false;

	bool b_UseTimerToDelaySearch = true;

	float m_PathSearchUpdateTime = 0.01f;
	float m_SearchTimeUpdateAccumulator = 0.0f;

	bool m_bPathDataInitialized = false;

	// if false Dijkstra's algorithm will be used
	bool m_bUseAStarAlgorithm = true;

	bool m_bDemoTestFinished = false;
};