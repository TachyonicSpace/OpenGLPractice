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

#include "tests\TestClearColor.h"


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


        Renderer renderer;

        // Setup ImGui binding
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);

        // Setup style
        ImGui::StyleColorsDark();

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;
        

        testMenu->RegisterTest<test::TestClearColor>("clear color");

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.f, 0.f, 0.f, 1.f))
            /* Render here */
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();
            if (currentTest) {
                currentTest->OnUpdate(0.f);
                currentTest->OnRender();
                ImGui::Begin("test");

                if (currentTest != testMenu && ImGui::Button("<-")) 
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();

                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        delete currentTest;
        if (currentTest != testMenu) {
            delete testMenu;
        }
    }
    // Cleanup
    
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}