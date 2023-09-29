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
    Text(string texto, ShaderProgram *program);
    ~Text();

    static Text* createText(string texto, ShaderProgram *program);
    static void init();
    static void destroy();

    void move(glm::vec2 pos);
    void updateText(string new_text);
    void render();


private:
    static Texture *fontTexture;

	ShaderProgram *shaderProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;

    string text;
	glm::vec2 position;

    void getUVsFromChar(char c, float *uvs);
    void mesh(string new_text);
};


#endif
