#ifndef TEXT_H
#define TEXT_H

#include "ShaderProgram.h"
#include "Sprite.h"

#include <string>
#include <vector>
#include <iostream>

#define FONT_SIZE_IN_TEXTURE 32
#define FONT_SIZE 16

struct text_vertex {
    float pos[2];
    float texCoords[2];
};

class Text {
public: 
    Text(std::string texto, ShaderProgram *program, glm::vec2 pos);
    ~Text();

    static Text* createText(std::string texto, ShaderProgram *program, glm::vec2 pos);
    static void init();
    static void destroy();

    void updatePosition(glm::vec2 pos);
    void updateText(string new_text);
    void render();

private:
    static Texture *s_fontTexture;

	ShaderProgram *shaderProgram;
	GLuint vao, vbo, ibo;
    unsigned int numIndices;
	GLint posLocation, texCoordLocation;

    std::string text;
    char font_size;
	glm::vec2 position;

    void getUVsFromChar(char c, float *uvs);
    void mesh(std::vector<text_vertex> &vertices, std::vector<unsigned int> &indices);
    void addChar(std::vector<text_vertex> &vertices, std::string textt, int i);
};


#endif
