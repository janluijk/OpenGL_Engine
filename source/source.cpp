#include <glad/glad.h>

// Standard classes
#include <iostream>
#include <math.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Very useful classes
#include <GLFW/glfw3.h>
// Not totally necessary
#include <shader.h>
#include <stb_image.h> // We want to create a function visualiser
// Self-made classes
#include <cube.h>
#include <line.h>
#include <sheet.h>

// TODO :
// Trace position using a line segment
// Save line segments and create a trail

// Approximate dP better using second derivative
// Simulate particle falling according to contraints
// GLSL shader support

// Everything object orientated

// Functions declaration
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const *path);
double abso(double x) { return (x < 0.0) ? -1.0 * x : x; }

// Window and viewport sizes
const unsigned int SCR_WIDTH = 2240;
const unsigned int SCR_HEIGHT = 1400;

// Timing - calculate FPS
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//
//
//
//
//
//
//
//
// Begin troep
int drawMode = GL_TRIANGLES;
float zoom = 1.5f;
glm::vec2 transpose = glm::vec2(0.0f, 0.0f);
glm::dvec2 pos = glm::dvec2(0.0, 0.0);
glm::dvec2 pos_old = glm::dvec2(0.0, 0.0);

double path(double x) { return -1.0 * x * x; }

void function() {
  double x = pos.x, y = pos.y;
  double dx = 0.1;
  double dy = (path(x + dx) - y);
  double dP = 0.01;

  double dist_x = (dP * dx) / sqrt(dx * dx + dy * dy);

  pos_old = pos;
  pos.y = path(x + dist_x);
  pos.x = x + dist_x;
}

//
//
//
//
//
//
//
//
//

int main() {
  // GLFW
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialise callback functions
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Objects
  Sheet sheet1;
  sheet1.setZoom(1.0f);
  sheet1.setResolution(SCR_WIDTH, SCR_HEIGHT);
  sheet1.setTranspose(transpose);

  Line line1(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));

  Cube cube1(glm::vec2(0.0f, 0.0f), 2.0f, glm::vec3(0.0f, 1.0f, 1.0f));

  while (!glfwWindowShouldClose(window)) {
    // Calls the callback functions
    processInput(window);
    // Background color
    glClearColor(0.5f, 0.6f, 0.8f, 1.0f);

    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT);

    // Objects
    sheet1.setZoom(zoom);
    sheet1.setTranspose(transpose);

    cube1.setZoom(zoom);
    cube1.setTranspose(transpose);

    sheet1.draw();
    line1.draw();
    // cube1.draw();

    // Now lets swap the first buffer to the second buffer
    glfwSwapBuffers(window);

    // After rendering calculate the time it took to render the frame
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glfwPollEvents();
  }

  // Termination
  glfwTerminate();
  return 0;
}

/*---------------------------FUNCTIONS-----------------------------*/

void processInput(GLFWwindow *window) {
  // Escape
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_EQUAL)) {
    zoom = zoom * 1.05f;
  }
  if (glfwGetKey(window, GLFW_KEY_MINUS)) {
    zoom = zoom * 0.95f;
  }
  if (glfwGetKey(window, GLFW_KEY_A)) {
    transpose.x -= 0.1f;
  }
  if (glfwGetKey(window, GLFW_KEY_D)) {
    transpose.x += 0.1f;
  }
  if (glfwGetKey(window, GLFW_KEY_W)) {
    transpose.y += 0.1f;
  }
  if (glfwGetKey(window, GLFW_KEY_S)) {
    transpose.y -= 0.1f;
  }
  if (glfwGetKey(window, GLFW_KEY_K)) {
    pos.x += 0.1f;
  }
  if (glfwGetKey(window, GLFW_KEY_J)) {
    pos.x -= 0.1f;
  }
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
    drawMode = GL_LINES;
  } else {
    drawMode = GL_TRIANGLES;
  }
}

// Resizing of window
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// Load textures. Todo: Add settings as parameters
unsigned int loadTexture(char const *path) {
  // Get texture ID
  unsigned int textureID;
  glGenTextures(1, &textureID);

  // Load texture data
  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

  // If data is loaded
  if (data) {
    // What type is texture data
    GLenum format;
    if (nrComponents == 1) {
      format = GL_RED;
    } else if (nrComponents == 3) {
      format = GL_RGBA;
    } else if (nrComponents == 4) {
      format = GL_RGBA;
    }

    // Bind buffer to a textureID
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Load data to buffer
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Configure texture buffer (WiP)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else {
    std::cout << "::ERROR:: TEXTURE NOT LOADED CORRECTLY" << std::endl;
  }

  stbi_image_free(data);

  return textureID;
}
