#include "pch.h"

#include "Map.h"
#include "Tiles/BaseTile.h"

#include "IndexBuffer/IndexBuffer.h"
#include "VertexBuffer/VertexBuffer.h"
#include "VertexArray/VertexArray.h"
#include "Utilities/ColorUtility.h"
#include "Camera/Camera.h"
#include "Input/Input.h"
#include "Collision/Collision.h"
#include "Tiles/BlockingTile.h"
#include "Tiles/WalkableTile.h"
#include "Tiles/DangerousTile.h"


float Map::s_TileData[] =
{
	-1.0f, 1.0f, 0.0f,	// top-left
	 1.0f, 1.0f, 0.0f,	// top-right
	-1.0f, -1.0f, 0.0f,	// bottom-left
	1.0f, -1.0f, 0.0f,	// bottom-right
};

uint32_t Map::s_TileIndexData[] =
{
	0, 1, 2,
	2, 1, 3
};

Map::Map(float inWidth, float inHeight, float inTileWidth, float inTileHeight, uint32_t inLineDelimeterThicknessInPixels)
	: m_TileShader("shaders/tile.vert", "shaders/tile.frag"), m_Width(inWidth), m_Height(inHeight), m_TileWidth(inTileWidth), m_TileHeight(inTileHeight), m_LineDelimeterThicknessInPixels(inLineDelimeterThicknessInPixels)
{
	m_TileVAO = std::make_shared<VertexArray>();
	{
		std::shared_ptr<VertexBuffer> tileVBO = std::make_shared<VertexBuffer>(s_TileData, sizeof(s_TileData) / sizeof(s_TileData[0]));
		tileVBO->SetLayout
		(
			BufferLayout
			{
				{ EShaderDataType::VecFloat3, "positionAttribute" }
			}
		);

		m_TileVAO->AddVertexBuffer(tileVBO);

		std::shared_ptr<IndexBuffer> tileIBO = std::make_shared<IndexBuffer>(s_TileIndexData, sizeof(s_TileIndexData) / sizeof(s_TileIndexData[0]));

		m_TileVAO->SetIndexBuffer(tileIBO);
	}

RecreateMap();
}

Map::~Map()
{
	DeleteMap();
}

void Map::RecreateMap()
{
	DeleteMap();

	m_NumberOfWalkableTiles = 0;

	const float numberOfTilesPerRow = m_Width / (m_TileWidth + m_LineDelimeterThicknessInPixels);
	const float numberOfTilesPerColumn = m_Height / (m_TileHeight + m_LineDelimeterThicknessInPixels);

	// shrink the width if not all tiles can fit
	m_NumberOfTilesPerRowInteger = (uint32_t)std::abs(numberOfTilesPerRow);
	m_NumberOfTilesPerColumnInteger = (uint32_t)std::abs(numberOfTilesPerColumn);

	m_Width = m_NumberOfTilesPerRowInteger * (m_TileWidth + m_LineDelimeterThicknessInPixels);
	m_Height = m_NumberOfTilesPerColumnInteger * (m_TileHeight + m_LineDelimeterThicknessInPixels);

	const float tileScaledWidth = m_TileWidth / 2.0f;
	const float tileScaledHeight = m_TileHeight / 2.0f;

	glm::vec3 tilePosition = glm::vec3(m_TilePositionOffsetX + tileScaledWidth + m_LineDelimeterThicknessInPixels, m_TilePositionOffsetY + tileScaledHeight + m_LineDelimeterThicknessInPixels, 0.0f);
	for (uint32_t i = 0; i < m_NumberOfTilesPerColumnInteger; ++i)
	{
		for (uint32_t j = 0; j < m_NumberOfTilesPerRowInteger; ++j)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution randRange(0, 100);

			BaseTile* tile = nullptr;
			//if (randRange(gen) <= 20.0f)
			//{
			//	tile = new BlockingTile(m_TileWidth, m_TileHeight);
			//}
			//else if (randRange(gen) >= 60.0f)
			//{
			//	tile = new DangerousTile(m_TileWidth, m_TileHeight);
			//	++m_NumberOfWalkableTiles;
			//}
			//else
			//{
			//	tile = new WalkableTile(m_TileWidth, m_TileHeight);
			//	++m_NumberOfWalkableTiles;
			//}

			if (i % 10 == 0 && j != m_NumberOfTilesPerRowInteger - 1)
			{
				tile = new DangerousTile(m_TileWidth, m_TileHeight);
				++m_NumberOfWalkableTiles;
			}
			else
			{
				tile = new WalkableTile(m_TileWidth, m_TileHeight);
				++m_NumberOfWalkableTiles;
			}

			//tile = new WalkableTile(m_TileWidth, m_TileHeight);
			//++m_NumberOfWalkableTiles;

			tile->SetPosition(tilePosition);
			tile->SetScale(glm::vec3(tileScaledWidth, tileScaledHeight, 1.0f));
			tile->SetVAO(m_TileVAO);
			m_Tiles.emplace_back(tile);

			tilePosition += glm::vec3(m_TileWidth + m_LineDelimeterThicknessInPixels, 0.0f, 0.0f);
		}

		tilePosition = glm::vec3(m_TilePositionOffsetX + tileScaledWidth + m_LineDelimeterThicknessInPixels, tilePosition.y + m_TileHeight + m_LineDelimeterThicknessInPixels, 0.0f);
	}
}

void Map::DeleteMap()
{
	for (uint32_t index = 0; index < m_Tiles.size(); ++index)
	{
		delete m_Tiles[index];
	}

	m_Tiles.clear();
}

void Map::Update(float deltaTime)
{

}

void Map::ImGuiFrameUpdate()
{
	if (ImGui::CollapsingHeader("Tile Map Settings", ImGuiTreeNodeFlags_None))
	{
		ImGui::InputFloat("TileMap offset X", &m_TilePositionOffsetX);
		ImGui::InputFloat("TileMap offset Y", &m_TilePositionOffsetY);

		ImGui::InputFloat("Tile Map Width", &m_Width);
		ImGui::InputFloat("Tile Map Height", &m_Height);

		ImGui::InputFloat("Tile Width", &m_TileWidth);
		ImGui::InputFloat("Tile Height", &m_TileHeight);

		ImGui::InputScalar("Tile Line Delimeter Thickness In Pixels", ImGuiDataType_U32, &m_LineDelimeterThicknessInPixels);
	}
}

void Map::Draw(const Camera& camera)
{
	const glm::vec2 cameraMin = { camera.GetPositionVec2().x - camera.GetOrthoWidth(), camera.GetPositionVec2().y - camera.GetOrthoHeight() };
	const glm::vec2 cameraMax = { camera.GetPositionVec2().x + camera.GetOrthoWidth(), camera.GetPositionVec2().y + camera.GetOrthoHeight() };
	for (BaseTile* tile : m_Tiles)
	{
		if (tile->GetPositionVec2().x - tile->GetWidth() / 2.0f > cameraMax.x || tile->GetPositionVec2().y - tile->GetHeight() / 2.0f > cameraMax.y)
		{
			continue;
		}

		if (tile->GetPositionVec2().x + tile->GetWidth() / 2.0f < cameraMin.x || tile->GetPositionVec2().y + tile->GetHeight() / 2.0f < cameraMin.y)
		{
			continue;
		}

		m_TileShader.Bind();
		tile->GetVAO()->Bind();

		m_TileShader.SetUniformMatrix4fv(camera.GetViewMatrix(), "view");
		m_TileShader.SetUniformMatrix4fv(camera.GetOrthographicProjectionMatrix(), "projection");
		m_TileShader.SetUniformMatrix4fv(tile->GetComputedTransform(), "model");

		m_TileShader.SetUniformVector4f(tile->GetColor(), "color");

		// kinda lame not to render tiles with instanced rendering, but who cares, 
		// this project is not about rendering ;-).
		glDrawElements(GL_TRIANGLES, (int)tile->GetVAO()->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
	}
}

const std::vector<BaseTile*>& Map::GetTiles() const
{
	return m_Tiles;
}

bool Map::IsOutsideTiles(glm::ivec2 worldPosition) const
{
	return (worldPosition.x > m_Width + m_TilePositionOffsetX || worldPosition.y > m_Height + m_TilePositionOffsetY || worldPosition.x < m_TilePositionOffsetX || worldPosition.y < m_TilePositionOffsetY);
}

BaseTile* Map::GetTileFromPosition(const glm::vec2& worldPosition) const
{
	if (IsOutsideTiles(worldPosition))
	{
		return nullptr;
	}

	const uint32_t clampedTileIndex = GetTileIndex(worldPosition);

	BaseTile* nearesTile = m_Tiles[clampedTileIndex];
	if (!nearesTile)
	{
		return nullptr;
	}

	if (Collision::IsCollisionPresent(nearesTile->GetMinBounds(), nearesTile->GetMaxBounds(), worldPosition) == false)
	{
		return nullptr;
	}
	
	// position is actualy on the tile, not on the gap between tiles
	return nearesTile;
}

BaseTile* Map::GetTileFromRowColIndex(const glm::ivec2& rowColIndex) const
{
	if ((uint32_t)rowColIndex.x >= m_NumberOfTilesPerColumnInteger || (uint32_t)rowColIndex.y >= m_NumberOfTilesPerRowInteger)
	{
		return nullptr;
	}

	if (rowColIndex.x == INVALID_INDEX || rowColIndex.y == INVALID_INDEX)
	{
		return nullptr;
	}

	const uint32_t tileIndex = ConvertRowColIndexToContiguousArrayIndex(rowColIndex);
	assert(tileIndex < m_Tiles.size());

	return m_Tiles[tileIndex];
}
BaseTile* Map::GetTileFromIndex(uint32_t tileIndex) const
{
	assert(tileIndex < m_Tiles.size());
	return m_Tiles[tileIndex];
}

glm::ivec2 Map::GetTileRowColIndex(const glm::vec2& worldPosition) const
{
	const float clampedMouseWorldX = std::clamp(worldPosition.x, m_TilePositionOffsetX, m_Width + m_TilePositionOffsetX);
	const float clampedMouseWorldY = std::clamp(worldPosition.y, m_TilePositionOffsetY, m_Height + m_TilePositionOffsetY);

	const float clampedLocalMouseX = std::clamp(clampedMouseWorldX - m_TilePositionOffsetX, 0.0f, m_Width);
	const float clampedLocalMouseY = std::clamp(clampedMouseWorldY - m_TilePositionOffsetY, 0.0f, m_Height);

	const uint32_t clampedTileXIndex = std::clamp<uint32_t>(uint32_t(clampedLocalMouseX / (m_TileWidth + m_LineDelimeterThicknessInPixels)), 0, m_NumberOfTilesPerRowInteger - 1);
	const uint32_t clampedTileYIndex = std::clamp<uint32_t>(uint32_t(clampedLocalMouseY / (m_TileHeight + m_LineDelimeterThicknessInPixels)), 0, m_NumberOfTilesPerColumnInteger - 1);

	return glm::ivec2{ clampedTileXIndex, clampedTileYIndex };
}

glm::ivec2 Map::GetTileRowColIndex(BaseTile* tile) const
{
	if (!tile)
	{
		return INVALID_TILE_INDEX;
	}

	return GetTileRowColIndex(glm::vec2{ tile->GetPosition().x, tile->GetPosition().y });
}

int Map::GetTileIndex(const glm::vec2& worldPosition) const
{
	const glm::ivec2 tileIndex = GetTileRowColIndex(worldPosition);
	const uint32_t clampedTileIndex = ConvertRowColIndexToContiguousArrayIndex(tileIndex);
	assert(clampedTileIndex < m_Tiles.size());

	return clampedTileIndex;
}

int Map::GetTileIndex(BaseTile* tile) const
{
	if (!tile)
	{
		return INVALID_INDEX;
	}

	return GetTileIndex(glm::vec2{ tile->GetPosition().x, tile->GetPosition().y });
}


uint32_t Map::GetNumberOfWalkableTiles() const
{
	return m_NumberOfWalkableTiles;
}

uint32_t Map::ConvertRowColIndexToContiguousArrayIndex(const glm::ivec2& tileIndex) const
{
	const uint32_t clampedXIndex = std::clamp<uint32_t>(tileIndex.x, 0u, m_NumberOfTilesPerColumnInteger - 1);
	const uint32_t clampedYIndex = std::clamp<uint32_t>(tileIndex.y, 0u, m_NumberOfTilesPerRowInteger - 1);
	const uint32_t clampedIndex = std::clamp<uint32_t>(uint32_t((clampedYIndex * m_NumberOfTilesPerRowInteger) + clampedXIndex), 0u, (uint32_t)m_Tiles.size() - 1);

	return clampedIndex;
}
