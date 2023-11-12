#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

struct EnemyPosition {
    glm::ivec2 initPos;
    char dir;
};

enum ObjectType { NONE, COIN, MUSHROOM, STAR };
enum BlockType { BRICK, INTERROGATION };
struct IntBlockPosition {
    glm::ivec2 pos;
    BlockType type;
    ObjectType object;
};

struct CoinPosition {
    glm::ivec2 pos;
};

class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
    enum class MapColor { OVERWORLD, UNDERWORLD };

	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();

    Texture* getTexture() { return &tilesheet; }

	int getTileSize() const { return tileSize; }
    glm::ivec2 getMapSize() const { return mapSize; }

    // Methods for collision detection avoiding tunneling ()
    bool solveCollisionsX(const glm::ivec2 &pos0, glm::ivec2 &pos1, const glm::ivec2 &playerSize);
    bool solveCollisionsY(const glm::ivec2 &pos0, glm::ivec2 &pos1, const glm::ivec2 &playerSize);
    bool onGround(const glm::ivec2 &pos, const glm::ivec2 &size);
    bool headUnderTile(const glm::ivec2 &pos, const glm::ivec2 &size);
    bool headOnFinishingTile(const glm::ivec2 &pos, const glm::ivec2 &size);

    glm::ivec2 getCastleDoorCoords() const { return casteDoorCoords; } // TODO quitar
    glm::ivec2 getCastleDoorPos() const;
    glm::ivec2 getPoleHeadPos() const;

    bool centerXUnderTile(const glm::ivec2 &pos, const glm::ivec2 &size);

    // Methods for collision detection with special tiles
    glm::ivec2 tileOverHead(const glm::ivec2 &pos, const glm::ivec2 &size) const;
    bool isSpecialTile(const glm::ivec2 &tileCoord);
    bool isCoinTile(const glm::ivec2 &tileCoord);
    bool isBrickTile(const glm::ivec2 &tileCoord);

    // Interact with the map
    void destroyBrickTile(const glm::ivec2 &tileCoord);
    void placeTile(const glm::ivec2 &tileCoord, int tileType);
    void placeTileNoRemesh(const glm::ivec2 &tileCoord, int tileType);


    MapColor getMapColor() const { return enemiesColor; }
    std::vector<EnemyPosition> getGoombas() const { return goombas; }
    std::vector<EnemyPosition> getKoopas() const { return koopas; }
    std::vector<IntBlockPosition> getInteractiveBlocks() const { return interactiveBlocks; }
    std::vector<CoinPosition> getCoins() const { return coins; }

    void remesh();

private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
    void deleteBuffers();

private:
	GLuint vao;
	GLuint vbo;
    ShaderProgram *shaderProgram;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	char *map;

    MapColor enemiesColor;
    std::vector<EnemyPosition> goombas;
    std::vector<EnemyPosition> koopas;
    glm::vec2 casteDoorCoords;
    glm::vec2 poleHeadCoords;
    std::vector<IntBlockPosition> interactiveBlocks;
    std::vector<CoinPosition> coins;
};


#endif // _TILE_MAP_INCLUDE


