class Line {
  unsigned int VBO, VAO, EBO;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;

  // Line specific
  glm::vec2 startPoint;
  glm::vec2 endPoint;
  glm::vec3 lineColor;

  Shader *lineShader;

public:
  Line(glm::vec2 start, glm::vec2 end) {

    startPoint = start;
    endPoint = end;

    lineShader =
        new Shader("build/shaders/line.vert", "build/shaders/line.frag");

    vertices = {
        start.x, start.y, 0.0f, //
        end.x,   end.y,   0.0f, //
    };

    indices = {0, 1};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  int setColor(glm::vec3 color) {
    this->lineColor = color;
    return 1;
  }

  int draw() {
    lineShader->use();
    lineShader->setVec3("color", lineColor);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
    return 1;
  }

  ~Line() {

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
  }
};
