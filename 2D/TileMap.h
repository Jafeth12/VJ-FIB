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

    // Methods for collision detection avoiding tunneling ()
    bool solveCollisionsX(const glm::ivec2 &pos0, glm::ivec2 &pos1, const glm::ivec2 &playerSize);
    bool solveCollisionsY(const glm::ivec2 &pos0, glm::ivec2 &pos1, const glm::ivec2 &playerSize);
    bool onGround(const glm::ivec2 &pos, const glm::ivec2 &size);
    bool headUnderTile(const glm::ivec2 &pos, const glm::ivec2 &size);
    bool headOnFinishingTile(const glm::ivec2 &pos, const glm::ivec2 &size);

    glm::ivec2 getCastleDoorCoords() const { return casteDoorCoords; } // TODO quitar
    glm::ivec2 getCastleDoorPos() const;
    glm::ivec2 getPoleHeadPos() const;

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

    glm::vec2 casteDoorCoords;
    glm::vec2 poleHeadCoords;

};


#endif // _TILE_MAP_INCLUDE


