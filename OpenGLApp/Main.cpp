#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_s.h"
#include "collisionsBlock.h"
#include "player.h"

#include "stb_image.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstring>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float animationTime = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Example_09", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int fbWidth, fbHeigh;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeigh);
    glViewport(0, 0, fbWidth, fbHeigh);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("shader.vs", "shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture 
    // -------------------------
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;

    unsigned char* data = stbi_load("sfondo_itTakesThree.png", &width, &height, &nrChannels, 0);

    if (data == NULL)
    {
        std::cout << "Failed to load texture" << std::endl;
        glfwTerminate();
        return -1;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object


    //parametri texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //creazione texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
   
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("ourTexture", 0);

    std::vector<float> verticesBlock;
    std::vector<unsigned int> indicesBlock;

    for (int i = 0; i < numObstacles; i++)
    {
        float x = obstacles[i].x;
        float y = obstacles[i].y;
        float w = obstacles[i].width;
        float h = obstacles[i].height;

        float left = 2.0f * x / width - 1.0f;
        float right = 2.0f * (x + w) / width - 1.0f;
        float top = 1.0f - 2.0f * y / height;
        float bottom = 1.0f - 2.0f * (y + h) / height;

        verticesBlock.insert(verticesBlock.end(), {
            right, top, 0.0f,
            right, bottom, 0.0f,
            left, bottom, 0.0f,
            left, top, 0.0f
            });
        
        unsigned int k = i * 4;

        indicesBlock.insert(indicesBlock.end(),
            {
                k, k+1, k+3,
                k+1, k+2, k+3
            });

    }

    //rettangoli per le collisioni
    unsigned int VAOBlock, VBOBlock, EBOBlock;

    glGenVertexArrays(1, &VAOBlock);
    glGenBuffers(1, &VBOBlock);
    glGenBuffers(1, &EBOBlock);

    glBindVertexArray(VAOBlock);

    glBindBuffer(GL_ARRAY_BUFFER, VBOBlock);
    glBufferData(GL_ARRAY_BUFFER, verticesBlock.size() * sizeof(float), verticesBlock.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOBlock);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBlock.size() * sizeof(unsigned int), indicesBlock.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    //render personaggio
    unsigned int texturePlayer;

    glGenTextures(1, &texturePlayer);
    glBindTexture(GL_TEXTURE_2D, texturePlayer); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int playerWidth, playerHeight, playerChannels;
    const char* image = selectImage();

    unsigned char* dataPlayer = stbi_load(image, &playerWidth, &playerHeight, &playerChannels, 4);

    if (dataPlayer == NULL) {
        std::cout << "Failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, playerWidth, playerHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataPlayer);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dataPlayer);

    float w = 2.0f * player.width / width;
    float h = 2.0f * player.height / height;

    float verticesPlayer[] =
    {
        // Position       // Texture
        w,    0.0f, 0.0f, 1.0f, 1.0f,
        w,    -h,   0.0f, 1.0f, 0.0f,
        0.0f, -h,   0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indicesPlayer[] =
    {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VAOPlayer, VBOPlayer, EBOPlayer;

    glGenVertexArrays(1, &VAOPlayer);
    glGenBuffers(1, &VBOPlayer);
    glGenBuffers(1, &EBOPlayer);

    glBindVertexArray(VAOPlayer);

    glBindBuffer(GL_ARRAY_BUFFER, VBOPlayer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPlayer), verticesPlayer, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOPlayer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPlayer), indicesPlayer, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);          //trasparenza del perosnaggio 

    lastFrame = static_cast<float>(glfwGetTime());

    unsigned int transformLoc =
        glGetUniformLocation(ourShader.ID, "transform");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // Delta time
        float currentFrame = static_cast<float>(glfwGetTime());

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();

        // RENDER SFONDO
        glm::mat4 transform = glm::mat4(1.0f);

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
            glm::value_ptr(transform));

        ourShader.setInt("isBlock", 0);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // CAMBIO IMMAGINE PLAYER
        const char* newImage = selectImage();

        if (strcmp(image, newImage) != 0)
        {
            image = newImage;

            unsigned char* dataPlayer = stbi_load(
                image, &playerWidth, &playerHeight,
                &playerChannels, 4);

            if (dataPlayer != NULL)
            {
                glBindTexture(GL_TEXTURE_2D, texturePlayer);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                    playerWidth, playerHeight, 0,
                    GL_RGBA, GL_UNSIGNED_BYTE, dataPlayer);

                glGenerateMipmap(GL_TEXTURE_2D);

                stbi_image_free(dataPlayer);
            }
            else
            {
                std::cout << "Failed to load: " << image << std::endl;
                return -1;
            }
        }


        // TRASLAZIONE PLAYER
        float x = 2.0f * player.x / width - 1.0f;
        float y = 1.0f - 2.0f * player.y / height;

        transform = glm::mat4(1.0f);

        transform = glm::translate(transform,
            glm::vec3(x, y, 0.0f));

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
            glm::value_ptr(transform));


        // RENDER PLAYER
        ourShader.setInt("isBlock", 0);

        glBindTexture(GL_TEXTURE_2D, texturePlayer);
        glBindVertexArray(VAOPlayer);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float velocity = player.speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        player.y -= velocity;
        player.state = WALK_UP;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        player.y += velocity;
        player.state = WALK_DOWN;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        player.x -= velocity;
        player.state = WALK_LEFT;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        player.x += velocity;
        player.state = WALK_RIGHT;
    }
    else
    {
        player.state = IDLE;
    }

    if (player.state != IDLE)
    {
        player.lastDirection = player.state;
    }

    if (player.state != IDLE)
    {
        animationTime += deltaTime;

        if (animationTime >= 0.15f)
        {
            if (player.frame == 0)
                player.frame = 1;
            else
                player.frame = 0;

            animationTime = 0.0f;
        }
    }
    else
    {
        player.frame = 0;
        animationTime = 0.0f;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}