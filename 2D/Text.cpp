#include "Text.h"

// static variables
Texture* Text::fontTexture;

// static function
void Text::init() {
    fontTexture = new Texture();
    fontTexture->loadFromFile("images/font.png", TEXTURE_PIXEL_FORMAT_RGBA);
    fontTexture->setMagFilter(GL_NEAREST);
    fontTexture->setMinFilter(GL_NEAREST);
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

    uvs[0] = ((diff%8)*FONT_SIZE_IN_TEXTURE)/(float)fontTexture->width();

    int row = diff/8;
    uvs[1] = (row*FONT_SIZE_IN_TEXTURE)/(float)fontTexture->height();
}

Text::Text(string texto, ShaderProgram *program) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	shaderProgram = program;
	position = glm::vec2(0.f, 0.f);

    mesh(texto);
}

Text* Text::createText(string texto, ShaderProgram *program) {
    Text *t = new Text(texto, program);

    return t;
}

void Text::mesh(string texto) {
    struct vertex {
        float pos[2];
        float texCoords[2];
    };

    text = texto;

    vector<vertex> vertices;

    float texSize = fontTexture->width();
    int size = texto.size();
    float uvStep = FONT_SIZE_IN_TEXTURE/texSize;
    for (int i = 0; i < size; ++i) {
        char c = texto[i];
        vertex v;

        float base_uvs[2];
        getUVsFromChar(c, base_uvs);
        if (base_uvs[0] == -1) {
            cout << "ERROR: character " << c << " not found in font" << endl;
            exit(-1);
        }

        // FIRST TRIANGLE
 
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

        // SECOND TRIANGLE

            // down left vertex
            v.pos[0] = i*FONT_SIZE;
            v.pos[1] = FONT_SIZE;
            v.texCoords[0] = base_uvs[0];
            v.texCoords[1] = base_uvs[1] + uvStep;
            vertices.push_back(v);

            // up right vertex
            v.pos[0] = i*FONT_SIZE + FONT_SIZE;
            v.pos[1] = 0;
            v.texCoords[0] = base_uvs[0] + uvStep;
            v.texCoords[1] = base_uvs[1];
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
	fontTexture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6*text.size());
	glDisable(GL_TEXTURE_2D);

}

