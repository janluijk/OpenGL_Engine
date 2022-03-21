// Standard classes
#include <iostream>
// Self-made classes
#include <shader.h>
#include <camera.h>
// Very useful classes
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
// Not totally necessary
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

// Functions declaration
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const* path);

/*--------------------------GLOBAL_VARIABLES-----------------------*/#pragma region
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


/* TESTING */
glm::vec3 position;
glm::vec4 lightColor = glm::vec4(0.0);
bool movingLight = false;
int drawMode = GL_TRIANGLES;
float timeSpeed = 0.0f;


int main()
{
    /*-----------------------INITIALIZING--------------------------*/ #pragma region
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
    /*-----------------------SHADER_DATA---------------------------*/ #pragma region
    float vertices[] = 
    {
    // Pos                  Normal vector           Texture coord
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,

    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
    0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
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
       -1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
       -1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f
    };

    unsigned int plane_indices[] = 
    {
        0, 1, 2,
        1, 2, 3
    };
    
    #pragma endregion
    /*-----------------------BUFFER_BINDING------------------------*/ #pragma region
    
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // LIGHT
    unsigned int lightVAO;

    glGenVertexArrays(1,        &lightVAO);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    #pragma endregion
    /*-----------------------TEXTURE_BINDING-----------------------*/ #pragma region
    const char path[] = {
        "textures/box.png"
    };
    unsigned int diffuseMap = loadTexture(path);

    const char path2[] = {
        "textures/box_specular.png"
    };
    unsigned int specularMap = loadTexture(path2);
    #pragma endregion
    /*-----------------------PRECOMPUTATION------------------------*/ #pragma region  
    cubeShader.use(); 
    cubeShader.setInt("material.diffuse", 0);
    cubeShader.setInt("material.specular", 1);
    
    #pragma endregion
    
    while (!glfwWindowShouldClose(window))
    {
        // Calls the callback functions
        processInput(window);

        /*--------------------BUFFER_CLEARING-----------------------*/ #pragma region
        glClearColor(0.015, 0.051, 0.206, 1.0f); // Color of sky

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        #pragma endregion
        /*--------------------CALCULATION---------------------------*/  #pragma region
        float gl_time = (float)glfwGetTime() * timeSpeed;

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f);

        #pragma endregion
       

        // Cube
        /*---------------------PRE-UNIFORM--------------------------*/  #pragma region
        cubeShader.use();

        // Initialize
        glm::mat4 model = glm::mat4(1.0f);


        #pragma endregion       
        /*---------------------UNIFORMS-----------------------------*/  #pragma region
        // VERTEX
        cubeShader.setMat4("projection"         , projection);
        cubeShader.setMat4("view"               , view);
        // FRAGMENT
        cubeShader.setVec3("viewPos"            , camera.Position);  
        cubeShader.setFloat("light.c0"                , 1.00);
        cubeShader.setFloat("light.c1"                , 0.01);
        cubeShader.setFloat("light.c2"                , 0.1);

        // TEXTURES
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        #pragma endregion
        /*---------------------DRAWING------------------------------*/  #pragma region

        glBindVertexArray(cubeVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //(pre)Drawing commands
        for (int x = -5; x < 5; x++)
        {
            for(int y = -5; y < 5; y++) 
            {
                for(int z = -5; z < 5; z++)
                {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(x, y + 10.0f, z));
                    
                    if ((x + y + z) % 15 == 0) 
                    {
                        model = glm::rotate(model, 0.05f * gl_time, glm::vec3(1.0f, 0.3f, 0.5f));
                        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
                        glm::mat3 normalTransform = glm::mat3(glm::transpose(glm::inverse(model)));

                        // VERTEX
                        cubeShader.setMat4("model"    , model);
                        // FRAGMENT
                        cubeShader.setMat3("normalTransform"   , normalTransform);
                        cubeShader.setFloat("material.shininess", 64.0f);
                        cubeShader.setVec3("light.ambient"      , 0.2f, 0.2f, 0.2f );
                        cubeShader.setVec3("light.diffuse"      , 0.5f, 0.5f, 0.5f );
                        cubeShader.setVec3("light.specular"     , 1.0f, 1.0f, 1.0f  );

                        glDrawElements(drawMode, 36, GL_UNSIGNED_INT, 0);
                    }
                    else if((x + y + z) % 2 == 0)
                    {
                        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
                        glm::mat3 normalTransform = glm::mat3(glm::transpose(glm::inverse(model)));
                        cubeShader.setMat4("model"     , model);
                        cubeShader.setMat3("normalTransform"    , normalTransform); 
                        cubeShader.setFloat("material.shininess", 64.0f);
                        cubeShader.setVec3("light.ambient"      , 0.2f, 0.2f, 0.2f );
                        cubeShader.setVec3("light.diffuse"      , 0.5f, 0.5f, 0.5f );
                        cubeShader.setVec3("light.specular"     , 1.0f, 1.0f, 1.0f  );
                        glDrawElements(drawMode, 36, GL_UNSIGNED_INT, 0);
                    }
                }
            }
        }  
        #pragma endregion

        
        // Light
        /*---------------------PRE-UNIFORM--------------------------*/ #pragma region
        lightShader.use();

        // Light  rotating around cubes 
        
        if(movingLight == true) 
        {
            float radius = 6.0f;
            position.x = cosf(gl_time / 11.0) * radius;
            position.y = sinf(gl_time / 13.0) * radius + 10.0f;
            position.z = sin(gl_time / 17.0) * radius; 
        }
        
        
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(0.2f));
        
        

        #pragma endregion
        /*---------------------UNIFORMS-----------------------------*/  #pragma region
        lightShader.setMat4("model"       , model);
        lightShader.setMat4("projection"  , projection);
        lightShader.setMat4("view"        , view);
        lightShader.setVec4("lightColor"        , lightColor);
        
        
        #pragma endregion
        /*---------------------DRAWING------------------------------*/  #pragma region
        glBindVertexArray(lightVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        #pragma endregion
        

        // Plane
        /*----------------------PRE-UNIFORM-------------------------*/ #pragma region
        planeShader.use();

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(100.0f));

        glm::mat3 normalTransform = glm::mat3(glm::transpose(glm::inverse(model)));

        #pragma endregion
        /*----------------------UNIFORMS----------------------------*/ #pragma region
        planeShader.setMat4("model"       , model);
        planeShader.setMat4("projection"  , projection);
        planeShader.setMat4("view"        , view);

        planeShader.setMat3("normalTransform"    , normalTransform);
        planeShader.setVec3("viewPos"            , camera.Position   );
        planeShader.setFloat("material.shininess", 32.0f);
        planeShader.setVec3("light.ambient"      , 0.2f, 0.2f, 0.2f);
        planeShader.setVec3("light.diffuse"      , 0.55f, 0.55f, 0.55f);
        planeShader.setVec3("light.specular"     , 1.0f, 1.0f, 1.0f);
        planeShader.setFloat("light.c0"                , 1.00f);
        planeShader.setFloat("light.c1"                , 0.001f);
        planeShader.setFloat("light.c2"                , 0.5f);

        #pragma endregion
        /*----------------------DRAWING-----------------------------*/ #pragma region
        glBindVertexArray(planeVAO);
        glDrawElements(drawMode, 6, GL_UNSIGNED_INT, 0);
        #pragma endregion
        

        /*----------------------UNIFORM_UNIFORMS--------------------*/ #pragma region
        
        cubeShader.use();
        cubeShader.setVec3("light.position", position);
        cubeShader.setVec4("light.color", lightColor);

        planeShader.use();
        planeShader.setVec3("light.position", position);
        planeShader.setVec4("light.color", lightColor);
        
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
    
    /*-----------------------CLEANING_UP---------------------------*/#pragma region
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

/*---------------------------FUNCTIONS-----------------------------*/ #pragma region
// Process inputs
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
    if (glfwGetKey(window, GLFW_KEY_KP_0)) {
        position = glm::vec3(7.0f, 10.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_1)) {
        position = glm::vec3(0.0f, 10.0f, 7.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2)) {
        position = glm::vec3(-7.0f, 10.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_3)) {
        position = glm::vec3(0.0f, 10.0f, -7.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_5)) {
        movingLight = !movingLight;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
        timeSpeed += 0.2f;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        timeSpeed -= 0.2f;
    }
    if(timeSpeed < 0.1f) 
    {
        timeSpeed = 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS) {
        lightColor.r += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
        lightColor.g += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS) {
        lightColor.b += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        lightColor.r -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        lightColor.g -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        lightColor.b -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        drawMode = GL_LINES;
    }
    else {
        drawMode = GL_TRIANGLES;
    }
}

// Resizing of window
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// Movement with mouse
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

// Zoom using scrollwheel
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// Load textures. Todo: Add settings as parameters
unsigned int loadTexture(char const* path) {
    // Get texture ID
    unsigned int textureID;
    glGenTextures(1, &textureID);

    // Load texture data
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    
    // If data is loaded
    if (data)
    {
        // What type is texture data
        GLenum format;
        if (nrComponents == 1)
        {
            format = GL_RED;
        }
        else if (nrComponents == 3)
        {
            format = GL_RGBA;
        }
        else if (nrComponents == 4) 
        {
            format = GL_RGBA;
        }
            
        // Bind buffer to a textureID
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Load data to buffer
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Configure texture buffer (WiP)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else 
    {
        std::cout << "::ERROR:: TEXTURE NOT LOADED CORRECTLY" << std::endl;
    }

    stbi_image_free(data);

    return textureID;
}

#pragma endregion