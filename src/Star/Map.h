#pragma once


#include "Shader/Shader.h"


class BaseTile;
class Camera;
class VertexArray;

class Map
{
public:
	Map(float inWidth, float inHeight, float inTileWidth, float inTileHeight, uint32_t inLineDelimeterThicknessInPixels = 4);

	~Map();

	void RecreateMap();

	void DeleteMap();

	void Update(float deltaTime);

	void ImGuiFrameUpdate();

	void Draw(const Camera& camera);

	const std::vector<BaseTile*>& GetTiles() const;

	bool IsOutsideTiles(glm::ivec2 worldPosition) const;

	BaseTile* GetTileFromPosition(const glm::vec2& worldPosition) const;
	BaseTile* GetTileFromRowColIndex(const glm::ivec2& rowColIndex) const;
	BaseTile* GetTileFromIndex(uint32_t tileIndex) const;

	glm::ivec2 GetTileRowColIndex(const glm::vec2& worldPosition) const;
	glm::ivec2 GetTileRowColIndex(BaseTile* tile) const;

	int GetTileIndex(const glm::vec2& worldPosition) const;
	int GetTileIndex(BaseTile* tile) const;

	uint32_t GetNumberOfWalkableTiles() const;

	uint32_t ConvertRowColIndexToContiguousArrayIndex(const glm::ivec2& tileIndex) const;

protected:
	float m_Width = 0, m_Height = 0;

	std::vector<BaseTile*> m_Tiles;

	static float s_TileData[];
	static uint32_t s_TileIndexData[];

	Shader m_TileShader;
	std::shared_ptr<VertexArray> m_TileVAO;

	uint32_t m_LineDelimeterThicknessInPixels = 0;
	uint32_t m_NumberOfTilesPerRowInteger = 0, m_NumberOfTilesPerColumnInteger = 0;

	float m_TileWidth = 32.0f, m_TileHeight = 32.0f;

	uint32_t m_NumberOfWalkableTiles = 0;

	// basically position of the tile map (it's translation)
	float m_TilePositionOffsetX = 0.0f;
	float m_TilePositionOffsetY = 0.0f;
};