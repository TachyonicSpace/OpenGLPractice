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

#include "vender\glm\glm.hpp"
#include "vender\glm\gtc\matrix_transform.hpp"
#include "vender\imgui\imgui.h"
#include "vender\imgui\imgui_impl_glfw_gl3.h"


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
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
            100.f, 100.f, 0, 0,
            200.f, 100.f, 1, 0,
            200.f, 200.f, 1, 1,
            100.f, 200.f, 0, 1
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

        //the projection ont the screen, simulating depth
        glm::mat4 proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.0f, 1.0f);
        //the view matrix, simulating camera, first paramiter is an identity matrix, second is the translation
        glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(-100, 0, 0));


        Shader shader("res/shaders/BasicTexture.shader");
        //Shader shader("OpenGLPractice/OpenGLPractice/res/shaders/BasicTexture.shader");
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

        // Setup ImGui binding
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);

        // Setup style
        ImGui::StyleColorsDark();

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        glm::vec3 translation(200, 200, 0);

        //define a redColor variable and the increment for each frame
        float r = 0.0f, increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            //move the acctual object 200 to the left and 200 up
            glm::mat4 model = glm::translate(glm::mat4(1.f), translation);

            //preform the calculation to see where it is in the screen
            glm::mat4 mvp = proj * view * model;

            //re-assigns the following items to the gpu right befor drawing them
            shader.Bind();
            shader.SetUniform4f("u_color", r, 0.3f, .8f, 1.0f);
            shader.SetUniformMat4f("u_MPV", mvp);

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


            {
                
                ImGui::SliderFloat3("float", &translation.x, 0.0f, 960.0f);// Edit 1 float using a slider from 0.0f to 1.0f    
                
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }


            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}