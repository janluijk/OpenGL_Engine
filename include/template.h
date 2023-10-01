class Object {
  unsigned int VBO, VAO, EBO;
  Shader *objectShader;

  float Zoom;
  float Scale;
  glm::vec2 Transpose;
  glm::vec2 Position;
  glm::vec3 Color;

public:
  Object(glm::vec2 position = glm::vec2(0.0f, 0.0f), float scale = 1.0f,
         glm::vec2 transpose = glm::vec2(0.0f, 0.0f), float zoom = 1.0f,
         glm::vec3 color = glm::vec3(0.0f, 1.0f, 1.0f)) {

    Shader objectShader("build/shaders/line.vert", "build/shaders/line.frag");

    vertices = {};
    indices = {};

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

    Zoom = zoom;
    Scale = scale;
    Transpose = transpose;
    Position = position;
    Color = color;
  }

  int setScale(float scale) {
    Scale = scale;
    return 1;
  }

  int setPosition(glm::vec2 position) {
    Position = position;
    return 1;
  }

  int setZoom(float zoom) {
    Zoom = zoom;
    return 1;
  }

  int setTranspose(glm::vec2 transpose) {
    Transpose = transpose;
    return 1;
  }

  int setColor(glm::vec3 color) {
    Color = color;
    return 1;
  }

  int draw() {
    objectShader.use();
    objectShader->setFloat("zoom", Zoom);
    objectShader->setVec2("transpose", Transpose);
    objectShader->setVec3("color", Color);
    objectShader->setFloat("scale", Scale);
    objectShader->setPosition("position", Position);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
    return 1;
  }

  ~Object() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
  }
};
