#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"


int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //specifies version 3.3 and using the core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    
    //makes sure glew initilizes correctly
    if (glewInit() != GLEW_OK) {
        std::cout << "error!\n";
    }
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA));
    GLCall(glEnable(GL_BLEND));

    {
        //an array holding the positions of the  triangle plus one for the square
        float positions[] = {
            -.5f, -.5f, 0, 0,
            0.5f, -0.5f, 1, 0,
            .5f, 0.5f, 1, 1,
            -.5f, .5f, 0, 1
        };

        //openGL how to read the positions to make a square out of two triangles
        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };


        VertexArray va;
        //makes the buffer that will hold the positons and binds it to the gpu
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        va.addBuffer(vb, layout);


        //makes an index buffer object and binds it to the program
        IndexBuffer ib(indicies, 6);


        Shader shader("res/shaders/BasicTexture.shader");
        shader.Bind();
        shader.SetUniform4f("u_color", 0.2f, 0.3f, .8f, 1.0f);

        Texture texture("res/textures/image.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);


        //removes all the following items from the gpu to simulate animating multiple objects
        va.UnBind();
        shader.UnBind();
        va.UnBind();
        ib.unBind();

        Renderer renderer;

        //define a redColor variable and the increment for each frame
        float r = 0.0f, increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            //re-assigns the following items to the gpu right befor drawing them
            shader.Bind();
            shader.SetUniform4f("u_color", r, 0.3f, .8f, 1.0f);

            va.Bind();
            ib.bind();

            /*draw our renderer*/
            renderer.Draw(va, ib, shader);

            //if the red color goes out of bounds, then negate the increment
            if (r > 1)
                increment = -.05f;
            else if (r < 0)
                increment = .05f;
            //increment the red variable
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
}