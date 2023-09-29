#include "Text.h"

// static variables
Texture* Text::s_fontTexture;

// static function
void Text::init() {
    s_fontTexture = new Texture();
    s_fontTexture->loadFromFile("images/font.png", TEXTURE_PIXEL_FORMAT_RGBA);
    s_fontTexture->setMagFilter(GL_NEAREST);
    s_fontTexture->setMinFilter(GL_NEAREST);
}

// private utils
void Text::getUVsFromChar(char c, float *uvs) {
    bool isLowerCase = c >= 'a' && c <= 'z';

    if ((c < ' ' || c > '_') && (!isLowerCase)) {
        uvs[0] = -1;
        uvs[1] = -1;
        return;
    }

    // make uppercase
    if (isLowerCase) c -= 'a' - 'A';
    int diff = c - ' '; // se resta el espacio (32 en dec) 

    int row = diff/8;
    int col = diff%8;

    uvs[0] = (col*FONT_SIZE_IN_TEXTURE)/(float)s_fontTexture->width();
    uvs[1] = (row*FONT_SIZE_IN_TEXTURE)/(float)s_fontTexture->height();
}

Text::Text(string texto, ShaderProgram *program, glm::vec2 pos) {
    text = texto;
    position = pos;
	shaderProgram = program;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

    mesh();
}

Text* Text::createText(string texto, ShaderProgram *program, glm::vec2 pos) {
    Text *t = new Text(texto, program, pos);

    return t;
}

void Text::mesh() {
    struct vertex {
        float pos[2];
        float texCoords[2];
    };

    vector<vertex> vertices;
    vector<unsigned int> indices;

    float texSize = s_fontTexture->width();
    float uvStep = FONT_SIZE_IN_TEXTURE/texSize;
    int size = text.size();
    for (int i = 0; i < size; ++i) {
        char c = text[i];
        vertex v;

        float base_uvs[2];
        getUVsFromChar(c, base_uvs);
        if (base_uvs[0] == -1) {
            cout << "ERROR: character " << c << " not found in font" << endl;
            exit(-1);
        }

        // first triangle
        indices.push_back(vertices.size() + 0);
        indices.push_back(vertices.size() + 1);
        indices.push_back(vertices.size() + 2);

        // second triangle
        indices.push_back(vertices.size() + 2);
        indices.push_back(vertices.size() + 1);
        indices.push_back(vertices.size() + 3);

        // up left vertex
        v.pos[0] = i*FONT_SIZE;
        v.pos[1] = 0;
        v.texCoords[0] = base_uvs[0];
        v.texCoords[1] = base_uvs[1];
        vertices.push_back(v);

        // up right vertex
        v.pos[0] = i*FONT_SIZE + FONT_SIZE;
        v.pos[1] = 0;
        v.texCoords[0] = base_uvs[0] + uvStep;
        v.texCoords[1] = base_uvs[1];
        vertices.push_back(v);

        // down left vertex
        v.pos[0] = i*FONT_SIZE;
        v.pos[1] = FONT_SIZE;
        v.texCoords[0] = base_uvs[0];
        v.texCoords[1] = base_uvs[1] + uvStep;
        vertices.push_back(v);

        // down right vertex
        v.pos[0] = i*FONT_SIZE + FONT_SIZE;
        v.pos[1] = FONT_SIZE;
        v.texCoords[0] = base_uvs[0] + uvStep;
        v.texCoords[1] = base_uvs[1] + uvStep;
        vertices.push_back(v);
    }

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);
    numIndices = indices.size();

	posLocation = shaderProgram->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = shaderProgram->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

void Text::move(glm::vec2 pos) {
    position = pos;
}

void Text::render() {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
    glm::mat4 view = glm::mat4(1.0f);

    shaderProgram->use();
	shaderProgram->setUniformMatrix4f("model", model);
    shaderProgram->setUniformMatrix4f("view", view);
	shaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    shaderProgram->setUniform2f("texCoordDispl", 0.f, 0.f);

	glEnable(GL_TEXTURE_2D);
	s_fontTexture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	// glDrawArrays(GL_TRIANGLES, 0, 6*text.size());
	glDisable(GL_TEXTURE_2D);

}

