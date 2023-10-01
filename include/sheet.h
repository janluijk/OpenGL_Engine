class Sheet {
  unsigned int VBO, VAO, EBO;

  Shader *sheetShader;

  unsigned int width, height;
  float zoom;
  glm::vec2 transpose;

public:
  Sheet() {

    sheetShader =
        new Shader("build/shaders/sheet.vert", "build/shaders/sheet.frag");

    float vertices[] = {
        -1.0f, -1.0f, 0.0f, //
        1.0f,  -1.0f, 0.0f, //
        -1.0f, 1.0f,  0.0f, //
        1.0f,  1.0f,  0.0f, //
    };

    unsigned int indices[] = {
        0, 1, 2, //
        1, 2, 3  //
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  int setZoom(float z) {
    zoom = z;
    return 1;
  }

  int setResolution(int x, int y) {
    width = x;
    height = y;
    return 1;
  }

  int setTranspose(glm::vec2 trans) {
    transpose = trans;
    return 1;
  }

  int draw() {
    sheetShader->use();
    sheetShader->setiVec2("resolution", width, height);
    sheetShader->setFloat("zoom", zoom);
    sheetShader->setVec2("transpose", transpose);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    return 1;
  }

  ~Sheet() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
  }
};
