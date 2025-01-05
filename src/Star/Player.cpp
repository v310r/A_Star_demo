#include "pch.h"

#include "Player.h"
#include "Map.h"
#include "Input/Input.h"
#include "Tiles/BaseTile.h"
#include "Utilities/ColorUtility.h"
#include "Tiles/BlockingTile.h"
#include "Tiles/WalkableTile.h"
#include "TileCostResolver.h"
#include "Camera/Camera.h"


Player::Player(const Map& map, const Camera& camera)
	: m_MapRef(map), m_CameraRef(camera)
{

}

void Player::Update(float deltaTime)
{
	if (m_bDemoTestFinished)
	{
		return;
	}

	if (b_UseTimerToDelaySearch)
	{
		m_SearchTimeUpdateAccumulator += deltaTime;
	}
	else
	{
		// small hack do support both blocking and non-blocking styles in an easy way
		m_SearchTimeUpdateAccumulator = m_PathSearchUpdateTime * 2;
	}

	CheckPlayerInput();

	if (!m_bStartTileChosen || !m_bEndTileChosen)
	{
		return;
	}

	if (m_bBlockingPathFinding)
	{
		FindPathToEndTileBlocking();
		PaintPathSearch();
		m_bDemoTestFinished = true;

		return;
	}

	if (!m_bPathDataInitialized)
	{
		InitPathData();
		VisitTilesFrom(m_StartTilePosition);

		return;
	}

	if (m_SearchTimeUpdateAccumulator < m_PathSearchUpdateTime)
	{
		return;
	}

	FindPathToEndTileTick();
	PaintPathSearch();

	m_SearchTimeUpdateAccumulator = 0.0f;

	if (m_bPathFound)
	{
		m_bDemoTestFinished = true;
	}
}

void Player::ImGuiFrameUpdate()
{
	if (ImGui::CollapsingHeader("Pathfinding settings", ImGuiTreeNodeFlags_None))
	{
		ImGui::Checkbox("Use blocking path finding", &m_bBlockingPathFinding);

		ImGui::InputFloat("Search Tick Update Time (in seconds)", &m_PathSearchUpdateTime);

		ImGui::Checkbox("Use A-star algorithm implementation", &m_bUseAStarAlgorithm);
	}
}

void Player::CheckPlayerInput()
{
	const std::vector<BaseTile*>& tiles = m_MapRef.GetTiles();
	const glm::vec2 mousePosition = Input::GetMousePosition();

	//LOG_INFO("MousePosition: {0}", mousePosition);
	const glm::ivec2 mouseGameWorldPosition = mousePosition + m_CameraRef.GetPositionVec2();
	//LOG_INFO("MouseGameWorldPosition: {0}", mouseGameWorldPosition);

	BaseTile* tileUnderCursor = m_MapRef.GetTileFromPosition(mouseGameWorldPosition);
	if (!tileUnderCursor)
	{
		return;
	}

	if (Input::IsMouseButtonPressed(SDL_BUTTON_LEFT))
	{
		if (!m_bStartTileChosen)
		{
			m_bStartTileChosen = true;
			m_StartTilePosition = m_MapRef.GetTileRowColIndex(tileUnderCursor);

			tileUnderCursor->SetColor(glm::vec4(ColorUtility::GREEN, 1.0f));
		}
		else if (!m_bEndTileChosen)
		{
			glm::ivec2 endTilePos = m_MapRef.GetTileRowColIndex(tileUnderCursor);
			if (endTilePos != m_StartTilePosition)
			{
				m_EndTilePosition = endTilePos;
				m_bEndTileChosen = true;
				tileUnderCursor->SetColor(glm::vec4(ColorUtility::GREEN, 1.0f));
			}
		}
	}
}

void Player::PaintPathSearch()
{
	if (m_bPathFound)
	{
		FillPathArray();
		PaintPathToEndTile();
	}
	else
	{
		PaintVisitedTilePath();
	}
}

void Player::FindPathToEndTileBlocking()
{
	if (m_bPathFound)
	{
		return;
	}

	InitPathData();

	VisitTilesFrom(m_StartTilePosition);
	for (uint32_t i = 0; i < m_MapRef.GetTiles().size(); ++i)
	{
		if (m_TilePriorityQueue.empty())
		{
			m_bPathFound = false;
			break;
		}

		const glm::ivec2 HeuristicallyClosestTileLocation = m_TilePriorityQueue.back().TileLocation;
		if (HeuristicallyClosestTileLocation == m_EndTilePosition)
		{
			m_bPathFound = true;
			break;
		}

		VisitTilesFrom(HeuristicallyClosestTileLocation);
	}
}

bool Player::FindPathToEndTileTick()
{
	if (m_bPathFound || m_TilePriorityQueue.empty())
	{
		return false;
	}

	const glm::ivec2 HeuristicallyClosestTileLocation = m_TilePriorityQueue.back().TileLocation;
	if (HeuristicallyClosestTileLocation == m_EndTilePosition)
	{
		m_bPathFound = true;
		true;
	}

	VisitTilesFrom(HeuristicallyClosestTileLocation);
}

void Player::VisitTilesFrom(glm::ivec2 sourceTileLocation)
{
	auto iter = m_TilePathMap.find(sourceTileLocation);
	if (iter == m_TilePathMap.end())
	{
		return;
	}

	if (m_TilePriorityQueue.size() > 0)
	{
		m_TilePriorityQueue.pop_back();
	}

	m_VisitedTileInfoSet.emplace(sourceTileLocation);

	ShortestKnownTileInfoPath* shortestKnownTilePathToSource = FindShortestPathFromStartToTile(sourceTileLocation);

	// try update shortest paths for adjacent
	const auto& [tileLoc, adjacentTiles] = *iter;
	for (const AdjacentTilePathData& adjacentTile : adjacentTiles)
	{
		if (!adjacentTile.TileObject)
		{
			continue;
		}

		ShortestKnownTileInfoPath* shortestKnownTilePathToAdjacentTile = FindShortestPathFromStartToTile(adjacentTile.TileLocation);
		if (!shortestKnownTilePathToAdjacentTile)
		{
			int32_t previousShortestPathCost = 0;
			glm::ivec2 previousTileLocation = sourceTileLocation;
			if (shortestKnownTilePathToSource)
			{
				previousShortestPathCost = shortestKnownTilePathToSource->PathCost;
				previousTileLocation = shortestKnownTilePathToSource->PreviousClosestTileLocation;
			}

			const int32_t shortestAdjacentTilePathAccumulatedCost = previousShortestPathCost + CalculatePathCost(sourceTileLocation, adjacentTile.TileLocation);
			m_ShortestKnownTileInfoPathMap.emplace(adjacentTile.TileLocation, ShortestKnownTileInfoPath
			{
				sourceTileLocation,
				shortestAdjacentTilePathAccumulatedCost
			});

			shortestKnownTilePathToAdjacentTile = &m_ShortestKnownTileInfoPathMap[adjacentTile.TileLocation];
		}
		else
		{
			const int32_t NewPathCostToAdjacent = shortestKnownTilePathToSource->PathCost + CalculatePathCost(sourceTileLocation, adjacentTile.TileLocation);
			if (NewPathCostToAdjacent < shortestKnownTilePathToAdjacentTile->PathCost)
			{
				shortestKnownTilePathToAdjacentTile->PathCost = NewPathCostToAdjacent;
				shortestKnownTilePathToAdjacentTile->PreviousClosestTileLocation = sourceTileLocation;
			}
		}

		QueuedTileInfo* queuedTile = FindTileInPriorityQueue(adjacentTile.TileLocation);
		if (queuedTile)
		{
			queuedTile->PathCost = shortestKnownTilePathToAdjacentTile->PathCost;
			queuedTile->TotalCost = shortestKnownTilePathToAdjacentTile->PathCost + CalculateHeuristicCost(adjacentTile.TileLocation);
		}
		else
		{
			// we shouldn't even bother enqueue tile, if it was already visited
			if (!IsVisitedTileLocation(adjacentTile.TileLocation))
			{
				m_TilePriorityQueue.emplace_back(QueuedTileInfo
				{
					adjacentTile.TileLocation,
					m_MapRef.GetTileFromRowColIndex(adjacentTile.TileLocation),
					shortestKnownTilePathToAdjacentTile->PathCost,
					shortestKnownTilePathToAdjacentTile->PathCost + CalculateHeuristicCost(adjacentTile.TileLocation)
				});
			}
		}
	}

	// higher priority should be in the back of array
	std::sort(m_TilePriorityQueue.begin(), m_TilePriorityQueue.end(), [](const QueuedTileInfo& tileInfo1, const QueuedTileInfo& tileInfo2)
	{
		return tileInfo1.TotalCost > tileInfo2.TotalCost;
	});
}

void Player::Reset()
{
	m_bStartTileChosen = false;
	m_bEndTileChosen = false;
	m_StartTilePosition = glm::ivec2{};
	m_EndTilePosition = glm::ivec2{};

	m_bPathDataInitialized = false;
	m_bPathFound = false;
	m_bDemoTestFinished = false;
	m_PathToEndTile.clear();

	m_SearchTimeUpdateAccumulator = 0.0f;

	m_ShortestKnownTileInfoPathMap.clear();
	m_VisitedTileInfoSet.clear();
	m_TilePathMap.clear();
	m_TilePriorityQueue.clear();
}

int32_t Player::CalculatePathCost(BaseTile* from, BaseTile* to) const
{
	if (!from || !to)
	{
		return MAX_PATH_COST;
	}

	if (BlockingTile* blockableTile = dynamic_cast<BlockingTile*>(to))
	{
		return MAX_PATH_COST;
	}

	return TileCostResolver::ResolveCost(from, to);
}

int32_t Player::CalculatePathCost(const glm::ivec2& from, const glm::ivec2& to) const
{
	BaseTile* fromTile = m_MapRef.GetTileFromRowColIndex(from);
	BaseTile* toTile = m_MapRef.GetTileFromRowColIndex(to);
	return CalculatePathCost(fromTile, toTile);
}

float Player::CalculateHeuristicCost(const glm::ivec2& from) const
{
	BaseTile* fromTile = m_MapRef.GetTileFromRowColIndex(from);
	if (!fromTile)
	{
		// some really big number
		return HUGE_HEURISTIC_COST;
	}

	return CalculateHeuristicCost(fromTile);
}

float Player::CalculateHeuristicCost(BaseTile* from) const
{
	if (!m_bUseAStarAlgorithm)
	{
		return 0.0f;
	}

	BaseTile* endTile = GetEndTile();
	if (!endTile)
	{
		assert(false);
		return HUGE_HEURISTIC_COST;
	}

	// distance based heuristic
	const float distanceToEndTile = glm::length(endTile->GetPositionVec2() - from->GetPositionVec2());
	return distanceToEndTile;
}

void Player::FillPathDataForTile(const glm::ivec2 sourceTileLoc)
{
	const glm::ivec2 leftUp = glm::ivec2{ sourceTileLoc.x - 1, sourceTileLoc.y - 1 };
	const glm::ivec2 up = glm::ivec2{ sourceTileLoc.x, sourceTileLoc.y - 1 };
	const glm::ivec2 rightUp = glm::ivec2{ sourceTileLoc.x + 1, sourceTileLoc.y - 1 };

	const glm::ivec2 left = glm::ivec2{ sourceTileLoc.x - 1, sourceTileLoc.y };
	const glm::ivec2 right = glm::ivec2{ sourceTileLoc.x + 1, sourceTileLoc.y };

	const glm::ivec2 leftDown = glm::ivec2{ sourceTileLoc.x - 1, sourceTileLoc.y + 1 };
	const glm::ivec2 down = glm::ivec2{ sourceTileLoc.x, sourceTileLoc.y + 1 };
	const glm::ivec2 rightDown = glm::ivec2{ sourceTileLoc.x + 1, sourceTileLoc.y + 1 };

	std::array<std::pair<glm::ivec2, EPathDirection>, 8> adjacentTileArr =
	{
		std::make_pair(leftUp,		EPathDirection::LeftUp		),
		std::make_pair(up,			EPathDirection::Up),
		std::make_pair(rightUp,		EPathDirection::RightUp),
		std::make_pair(left,		EPathDirection::Left),
		std::make_pair(right,		EPathDirection::Right),
		std::make_pair(leftDown,	EPathDirection::LeftDown),
		std::make_pair(down,		EPathDirection::Down),
		std::make_pair(rightDown,	EPathDirection::RightDown)
	};

	std::vector<AdjacentTilePathData> adjacentTiles;
	for (const auto&[adjacentTileLocation, adjacentTileRelativeDirection] : adjacentTileArr)
	{
		WalkableTile* walkableTile = dynamic_cast<WalkableTile*>(m_MapRef.GetTileFromRowColIndex(adjacentTileLocation));
		if (!walkableTile)
		{
			continue;
		}

		if (adjacentTileLocation.x > 21 || adjacentTileLocation.y > 21)
		{
			int x = 5;
		}

		adjacentTiles.emplace_back(AdjacentTilePathData
		{
			adjacentTileLocation,
			walkableTile,
			adjacentTileRelativeDirection
		});
	}

	m_TilePathMap.emplace(sourceTileLoc, std::move(adjacentTiles));
}

BaseTile* Player::GetEndTile() const
{
	return m_MapRef.GetTileFromRowColIndex(m_EndTilePosition);
}

void Player::InitPathData()
{
	for (BaseTile* tile : m_MapRef.GetTiles())
	{
		const glm::ivec2 rowColIndex = m_MapRef.GetTileRowColIndex(tile);
		FillPathDataForTile(rowColIndex);
	}

	m_ShortestKnownTileInfoPathMap.emplace(m_StartTilePosition, ShortestKnownTileInfoPath
	{
		INVALID_TILE_INDEX,
		0
	});

	m_bPathDataInitialized = true;
}

bool Player::IsTilePriorityQueueEmpty() const
{
	return m_TilePriorityQueue.empty();
}

bool Player::AllTilesAreVisited() const
{
	return (m_MapRef.GetNumberOfWalkableTiles() - 1) == m_VisitedTileInfoSet.size();
}

QueuedTileInfo* Player::FindTileInPriorityQueue(glm::ivec2 tileLocation) const
{
	auto iter = std::find_if(m_TilePriorityQueue.begin(), m_TilePriorityQueue.end(), [&tileLocation](const QueuedTileInfo& tileInfo)
	{
		return tileInfo.TileLocation == tileLocation;
	});

	if (iter == m_TilePriorityQueue.end())
	{
		return nullptr;
	}

	return const_cast<QueuedTileInfo*>(&(*iter));
}

ShortestKnownTileInfoPath* Player::FindShortestPathFromStartToTile(glm::ivec2 targetTile) const
{
	auto iter = m_ShortestKnownTileInfoPathMap.find(targetTile);
	if (iter == m_ShortestKnownTileInfoPathMap.end())
	{
		return nullptr;
	}

	auto& [tileLocation, shortestPathToTile] = *iter;
	return const_cast<ShortestKnownTileInfoPath*>(&shortestPathToTile);
}

bool Player::IsVisitedTileLocation(glm::ivec2 tileLocation) const
{
	auto iter = m_VisitedTileInfoSet.find(tileLocation);
	return iter != m_VisitedTileInfoSet.end();
}

void Player::FillPathArray()
{
	m_PathToEndTile.clear();

	FillPathArray_Recursive(m_EndTilePosition);
	m_PathToEndTile.emplace_back(m_EndTilePosition);
}

void Player::FillPathArray_Recursive(glm::ivec2 targetTileLocation)
{
	if (targetTileLocation == m_StartTilePosition)
	{
		return;
	}

	ShortestKnownTileInfoPath* shortestPathFromStartToTarget = FindShortestPathFromStartToTile(targetTileLocation);
	if (shortestPathFromStartToTarget)
	{
		FillPathArray_Recursive(shortestPathFromStartToTarget->PreviousClosestTileLocation);
		m_PathToEndTile.emplace_back(shortestPathFromStartToTarget->PreviousClosestTileLocation);
	}
	else
	{
		LOG_ERROR("Non-existent shortest path for tile: {0}", targetTileLocation);
	}
}

void Player::PaintVisitedTilePath()
{
	for (glm::ivec2 visitedTile : m_VisitedTileInfoSet)
	{
		if (visitedTile == m_StartTilePosition || visitedTile == m_EndTilePosition)
		{
			continue;
		}

		BaseTile* tile = m_MapRef.GetTileFromRowColIndex(visitedTile);
		if (tile)
		{
			tile->SetColor(glm::vec4{ ColorUtility::CYAN, 1.0f });
		}
	}
}

void Player::PaintPathToEndTile()
{
	for (uint32_t i = 0; i <= m_PathToEndTile.size() - 1; ++i)
	{
		if (m_PathToEndTile[i] == m_StartTilePosition || m_PathToEndTile[i] == m_EndTilePosition)
		{
			continue;
		}

		BaseTile* tile = m_MapRef.GetTileFromRowColIndex(m_PathToEndTile[i]);
		if (tile)
		{
			tile->SetColor(glm::vec4{ ColorUtility::BLUE, 1.0f });
		}
	}
}
