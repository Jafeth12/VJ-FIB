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

Text::Text(std::string texto, ShaderProgram *program, glm::vec2 pos) {
    text = texto;
    position = glm::vec2(pos.x*FONT_SIZE, pos.y*FONT_SIZE);
	shaderProgram = program;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    std::vector<text_vertex> vertices;
    std::vector<unsigned int> indices;

    mesh(vertices, indices);
}

Text::~Text() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
}

Text* Text::createText(std::string texto, ShaderProgram *program, glm::vec2 pos) {
    Text *t = new Text(texto, program, pos);

    return t;
}

void Text::addChar(std::vector<text_vertex> &vertices, std::string textt, int i) {
    float texSize = s_fontTexture->width();
    float uvStep = FONT_SIZE_IN_TEXTURE/texSize;

    float base_uvs[2];
    getUVsFromChar(textt[i], base_uvs);
    if (base_uvs[0] == -1) {
        cout << "ERROR: character " << textt[i] << " not found in font" << endl;
        exit(-1);
    }

    text_vertex v;

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

void Text::mesh(std::vector<text_vertex> &vertices, std::vector<unsigned int> &indices) {
    int size = text.size();
    for (int i = 0; i < size; ++i) {

        // first triangle
            indices.push_back(vertices.size() + 0); // up left
            indices.push_back(vertices.size() + 1); // up right
            indices.push_back(vertices.size() + 2); // down left

        // second triangle
            indices.push_back(vertices.size() + 2); // down left
            indices.push_back(vertices.size() + 1); // up right
            indices.push_back(vertices.size() + 3); // down right

        addChar(vertices, text, i);
    }

    glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(text_vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);
    numIndices = indices.size();

    posLocation = shaderProgram->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
    texCoordLocation = shaderProgram->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

void Text::updateText(std::string new_text) {
    int old_size = text.size();
    int new_size = new_text.size();
    std::string old_text = text;

    text = new_text;

    if (old_size != new_size) {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);

        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);
        std::vector<text_vertex> vertices;
        std::vector<unsigned int> indices;
        mesh(vertices, indices);

    } else {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        for (int i = 0; i < new_size; ++i) {
            if (old_text[i] == new_text[i]) continue;

            std::vector<text_vertex> vertices_for_this_char;
            addChar(vertices_for_this_char, new_text, i);

            glBufferSubData(GL_ARRAY_BUFFER, i*4*sizeof(text_vertex), 4*sizeof(text_vertex), &vertices_for_this_char[0]);
        }
    }
}

void Text::updatePosition(glm::vec2 pos) {
    position = pos;
}

glm::vec2 Text::getPosition(){
    return position;
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

