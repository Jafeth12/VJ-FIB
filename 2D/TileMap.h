#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();

	int getTileSize() const { return tileSize; }
    glm::ivec2 getMapSize() const { return mapSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;

    // Used for collisions instead of collisionMoveDown
    bool onGround(const glm::ivec2 &pos, const glm::ivec2 &size);
    bool headUnderTile(const glm::ivec2 &pos, const glm::ivec2 &size);
    glm::bvec2 solveCollisions(const glm::ivec2 &pos0, glm::ivec2 &pos1, const glm::ivec2 &playerSize);

private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;

};


#endif // _TILE_MAP_INCLUDE


