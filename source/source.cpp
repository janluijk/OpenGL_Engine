// Standard classes
#include <iostream>

// Self-made classes
#include <shader.h>
#include <camera.h>

// Very useful classes
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <GLFW/glfw3.h>

// Not required
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

// Functions declaration
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

/*--------------------------GLOBAL_VARIABLES--------------------------*/#pragma region
// Window and viewport sizes
const unsigned int SCR_WIDTH   = 1920;
const unsigned int SCR_HEIGHT  = 1080;   

// Trigonometry
const float twoPi = glm::pi<float>() * 2.0f;
const float pi = glm::pi<float>();
const float halfPi = glm::pi<float>() * 0.5f;

// Timing - calculate FPS
float deltaTime = 0.0f;
float lastFrame = 0.0f; 

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

#pragma endregion

int main()
{
    /*--------------------------INITIALIZING--------------------------*/ #pragma region
    // GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialise callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1; 
    }

    // Shader compiing
    Shader cubeShader("shaders/cube.vert", "shaders/cube.frag");
    Shader lightShader("shaders/light.vert", "shaders/light.frag");
    Shader planeShader("shaders/plane.vert", "shaders/plane.frag");

    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);

    #pragma endregion
    /*--------------------------SHADER_DATA---------------------------*/ #pragma region
    float vertices[] = 
    {
    // Pos                  Normal vector
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,  

    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
    

    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,


     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,


    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
    };

    unsigned int indices[] = 
    {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
    };
    
    float plane_vertices[] = {
       -1.0f, 0.0f, -1.0f,
       -1.0f, 0.0f,  1.0f,
        1.0f, 0.0f, -1.0f,
        1.0f, 0.0f,  1.0f
    };

    unsigned int plane_indices[] = 
    {
        0, 1, 2,
        1, 2, 3
    };
    
    #pragma endregion
    /*--------------------------BUFFER_BINDING------------------------*/ #pragma region
    
    // CUBE
    unsigned int cubeVAO, cubeVBO, cubeEBO;

    glGenVertexArrays(1,    &cubeVAO);
    glGenBuffers(1,         &cubeVBO);
    glGenBuffers(1,         &cubeEBO);  

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // LIGHT
    unsigned int lightVAO;

    glGenVertexArrays(1,        &lightVAO);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);


    // PLANE
    unsigned int planeVAO, planeVBO,planeEBO;

    glGenVertexArrays(1,    &planeVAO);
    glGenBuffers(1,         &planeVBO);
    glGenBuffers(1,         &planeEBO);

    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_indices), plane_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    #pragma endregion
    /*--------------------------TEXTURE_BINDING------------------------*/ #pragma region
    
    
    #pragma endregion
    /*--------------------------PRECOMPUTATION------------------------*/ #pragma region  
    
    
    
    #pragma endregion
    
    while (!glfwWindowShouldClose(window))
    {
        // Calls the callback functions
        processInput(window);

        /*--------------------------BUFFER_CLEARING--------------------------*/ #pragma region
        glClearColor(0.015, 0.101, 0.486, 1.0f); // Color of sky

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        #pragma endregion
        /*--------------------------CALCULATION------------------------------*/  #pragma region
        float gl_time = (float)glfwGetTime();
        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f);

        #pragma endregion
       

        // Cube
        /*--------------------------PRE-UNIFORM------------------------------*/  #pragma region
        cubeShader.use();
        #pragma endregion       
        /*--------------------------UNIFORMS---------------------------------*/  #pragma region
        cubeShader.setMat4("projection"         , projection        );
        cubeShader.setMat4("view"               , view              );
        cubeShader.setVec3("viewPos"            , camera.Position   ); 
        #pragma endregion
        /*--------------------------DRAWING----------------------------------*/  #pragma region

        glBindVertexArray(cubeVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //(pre)Drawing commands
        for (int x = -5; x < 5; x++)
        {
            for(int y = -5; y < 5; y++) 
            {
                for(int z = -5; z < 5; z++)
                {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(x, y + 10, z));
                    
                    if ((x + y + z) % 3 == 0) 
                    {
                        model = glm::rotate(model, 1.0f * gl_time, glm::vec3(1.0f, 0.3f, 0.5f));
                        model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4));
                        glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(model)));

                        cubeShader.setMat4("model"    , model);
                        cubeShader.setMat3("normal"   , normal); 

                        cubeShader.setVec3("material.ambient"   , 1.0f, 0.5f, 0.31f );
                        cubeShader.setVec3("material.diffuse"   , 1.0f, 0.5f, 0.31f );
                        cubeShader.setVec3("material.specular"  , 0.5f, 0.5f, 0.5f  );
                        cubeShader.setFloat("material.shininess", 64.0f             );
                        cubeShader.setVec3("light.ambient"      ,  0.2f, 0.2f, 0.2f );
                        cubeShader.setVec3("light.diffuse"      ,  0.5f, 0.5f, 0.5f );
                        cubeShader.setVec3("light.specular"     , 1.0f, 1.0f, 1.0f  ); 
                        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
                    }
                    else
                    {
                        //model = glm::rotate(model, 1.0f * gl_time, glm::vec3(1.0f, 0.3f, 0.5f));
                        
                        model = glm::scale(model, glm::vec3(0.8, 0.8, 0.8));
                        glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(model)));
                        cubeShader.setMat4("model"     , model);
                        cubeShader.setMat3("normal"    , normal); 
                        
                        cubeShader.setVec3("material.ambient"   , 0.05375f, 0.05f, 0.06625f);
                        cubeShader.setVec3("material.diffuse"   , 0.18275f, 0.17f, 0.22525f);
                        cubeShader.setVec3("material.specular"  , 0.332741f, 0.328634f, 0.346435f);
                        cubeShader.setFloat("material.shininess", 16.0f             );
                        cubeShader.setVec3("light.ambient"      ,  1.0f, 1.0f, 1.0f );
                        cubeShader.setVec3("light.diffuse"      ,  1.0f, 1.0f, 1.0f );
                        cubeShader.setVec3("light.specular"     ,  1.0f, 1.0f, 1.0f ); 
                        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
                    }
                }
            }
        }  
        #pragma endregion

        
        // Light
        /*--------------------------PRE-UNIFORM------------------------------*/ #pragma region
        lightShader.use();

        glm::vec3 position(0.0f, 0.0f, 0.0f);
        glm::vec4 lightColor = glm::vec4(1.0f);

        model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(0.2f));

        // Light  rotating around cubes 
        float radius = 6.0f;
        position.x = cosf(gl_time * 2.3) * radius;
        position.y = sinf(gl_time * 2.9) * radius + 10.0f;
        position.z = sin(gl_time * 3.1) * radius;
        position = glm::vec3(model * glm::vec4(position, 1.0)); 

        #pragma endregion
        /*--------------------------UNIFORMS---------------------------------*/  #pragma region
        lightShader.setMat4("model"       , model);
        lightShader.setMat4("projection"  , projection);
        lightShader.setMat4("view"        , view);
        lightShader.setVec4("lightColor"        , lightColor);
        
        
        #pragma endregion
        /*--------------------------DRAWING----------------------------------*/  #pragma region
        glBindVertexArray(lightVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        #pragma endregion
        

        // Plane
        /*--------------------------PRE-UNIFORM------------------------------*/ #pragma region
        planeShader.use();

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(50.0f));

        #pragma endregion
        /*--------------------------UNIFORMS---------------------------------*/ #pragma region
        planeShader.setMat4("model"       , model);
        planeShader.setMat4("projection"  , projection);
        planeShader.setMat4("view"        , view);
        #pragma endregion
        /*--------------------------DRAWING----------------------------------*/ #pragma region
        glBindVertexArray(planeVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        #pragma endregion
        

        /*--------------------------UNIFORM_UNIFORMS-------------------------*/ #pragma region
        cubeShader.use();
        cubeShader.setVec3("light.position", position);

        #pragma endregion

        // Now lets swap the first buffer to the second buffer
        glfwSwapBuffers(window);
              
        // After rendering calculate the time it took to render the frame
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // This function checks for events and activates the callback functions on events
        glfwPollEvents();
    }
    
    /*--------------------------CLEANING_UP---------------------------*/#pragma region
    // Cube
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // Light
    glDeleteVertexArrays(1, &lightVAO);
    
    // Plane
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeEBO);
    glDeleteBuffers(1, &planeVBO);
    
    // Termination
    glfwTerminate();
    return 0;

    #pragma endregion
}

/*--------------------------FUNCTIONS------------------------------*/ #pragma region
// process inputs
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    { 
        glfwSetWindowShouldClose(window, true);
    }
    // Player movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
}

// resizing of window
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// movement with mouse
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// zoom using scrollwheel
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

#pragma endregion