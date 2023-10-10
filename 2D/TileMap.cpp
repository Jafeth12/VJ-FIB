#include <glm/fwd.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else
				map[j*mapSize.x+i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0)
			return true;
	}
	
	return false;
}

bool TileMap::onGround(const glm::ivec2 &pos, const glm::ivec2 &size)
{
    // pSpace
    int psPlayerHeight = size.y;
    int psPlayerHeadY = pos.y;
    int psPlayerFeetY = psPlayerHeadY + psPlayerHeight;
    int psPixelUnderPlayer = psPlayerFeetY + 1;

    // tileSpace
    int tsPixelUnderPlayer = psPixelUnderPlayer / tileSize;

	int x0, x1, yFeet;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;

    for (int x = x0; x<=x1; ++x)
    {
		if(map[tsPixelUnderPlayer*mapSize.x+x] != 0)
		{
            return true;
        }
    }
    return false;
}

bool TileMap::headUnderTile(const glm::ivec2 &pos, const glm::ivec2 &size)
{
    // pSpace
    int psPlayerHeadY = pos.y;
    int psPixelOverPlayer = psPlayerHeadY - 1;

    // tileSpace
    int tsPixelOverPlayer = psPixelOverPlayer / tileSize;

	int x0, x1, yFeet;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;

    for (int x = x0; x<=x1; ++x)
    {
		if(map[tsPixelOverPlayer*mapSize.x+x] != 0)
		{
            return true;
        }
    }
    return false;
}


enum DirX { NONEX = 0, LEFT = -1, RIGHT = 1 };
enum DirY { NONEY = 0, UP = -1, DOWN = 1 };

#define TOP(pos) (pos.y)
#define BOTTOM(pos, size) (pos.y + size.y - 1)
#define LEFT(pos) (pos.x)
#define RIGHT(pos, size) (pos.x + size.x - 1)
/**
 * Check if a displacement collides with the map, and corrects such final position
 * Returns a vector indicating if there is a collision in each axis
 * De momento solamente se mira el eje Y
 */
glm::bvec2 TileMap::solveCollisions(const glm::ivec2 &pos0, glm::ivec2 &pos1, const glm::ivec2 &playerSize) {
    glm::bvec2 collisions = glm::bvec2(false, false);
    DirX dirx = ((pos0.x < pos1.x) ? RIGHT : ((pos0.x > pos1.x) ? LEFT : NONEX));
    DirY diry = ((pos0.y < pos1.y) ? DOWN : ((pos0.y > pos1.y) ? UP : NONEY));

    // Espacio donde podemos colisionar (definido por los puntos p0 y p1)
    glm::ivec2 p0, p1;

    switch (dirx) {
        case RIGHT:
        {
            p0.x = LEFT(pos0);
            p1.x = RIGHT(pos1, playerSize);
            break;
        }
        case LEFT:
        {
            p0.x = RIGHT(pos1, playerSize);
            p1.x = LEFT(pos0);
            break;
        }
        case NONEX:
        {
            p0.x = p1.x = pos0.x;
            break;
        }
    }

    switch (diry) {
        case DOWN:
        {
            p0.y = TOP(pos0);
            p1.y = BOTTOM(pos1, playerSize);
            break;
        }
        case UP:
        {
            p0.y = BOTTOM(pos1, playerSize);
            p1.y = TOP(pos0);
            break;
        }
        case NONEY:
        {
            p0.y = p1.y = pos0.y;
            break;
        }
    }

    // Pasamos de pixelSpace a tileSpace
    p0 /= tileSize;
    p1 /= tileSize;

    if (dirx == NONEX) dirx = DirX(1);
    if (diry == NONEY) diry = DirY(1);
    // FIXME: Hay que descubrir como iterar de la mejor manera la trayectoria que acabamos de calcular
    for (int y = p0.y; y != p1.y+diry; y += diry)
        for (int x = p0.x; x != p1.x+dirx; x += dirx)
            if (map[y * mapSize.x + x] != 0) {
                collisions.y = true;
                if (diry == DOWN) // DOWN: Ens coloquem just sobre la colisió
                    pos1.y = tileSize * (y) - playerSize.y;
                else // UP: Ens coloquem just sota la colisió
                    pos1.y = tileSize * (y+1);
            }
    return collisions;
}
