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
    glm::vec2 pos;
    glm::vec2 texCoords;
};

class Text {
public: 
    Text(string texto, ShaderProgram *program, glm::vec2 pos);
    ~Text();

    static Text* createText(string texto, ShaderProgram *program, glm::vec2 pos);
    static void init();
    static void destroy();

    void move(glm::vec2 pos);
    void updateText(string new_text);
    void render();


private:
    static Texture *s_fontTexture;

	ShaderProgram *shaderProgram;
	GLuint vao, vbo, ibo;
    unsigned int numIndices;
	GLint posLocation, texCoordLocation;

    string text;
    char font_size;
	glm::vec2 position;

    void getUVsFromChar(char c, float *uvs);
    void mesh();
};


#endif
