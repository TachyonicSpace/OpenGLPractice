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
#include "Rocket.h"

#include "vender\glm\glm.hpp"
#include "vender\glm\gtc\matrix_transform.hpp"
#include "vender\imgui\imgui.h"
#include "vender\imgui\imgui_impl_glfw_gl3.h"

#define Width 960.0f
#define Height 540.0f
#define popsize 250



bool restart = false;

struct population {
    std::vector<Rocket> rockets;
    std::vector<Rocket> matingPool;
    

    population(float width, float height) {
        Rocket::count = 0;
        for (int i = 0; i < popsize; i++) {
            rockets.push_back( *(new Rocket(width, height)) );
        }
    }

    bool update() {
        bool notFinished = true;
        for (Rocket r : rockets) {
            notFinished = notFinished && r.update();
        }return notFinished;
    }

    void pos(glm::vec2 pos) {
        for (Rocket r : rockets) {
            r.Pos = pos;
        }
    }

    float eval() {
        float bestFit = rockets[0].fitness();
        for (int i = 0; i < popsize; i++) {
            bestFit = __max(rockets[i].fitness(), bestFit);
        }
        for (int i = 0; i < popsize; i++) {
            //rockets[i].Fitness /= (bestFit + .00000001);
        }
        matingPool.resize(0);
        //matingPool.clear();
        //matingPool.shrink_to_fit();
        for (int i = 0; i < popsize; i++) {
            float fitness = rockets[i].Fitness;
            auto n = clamp(fitness, 0.f, 100.f);
            for (int j = 0; j < n; j++) {
                matingPool.push_back(rockets[i]);
            }
        }
        matingPool.shrink_to_fit();
        return bestFit;
    }

    void selection() {
        /*
        population* newpop = new population();*/
        if (matingPool.size() <= 0) {
            matingPool = rockets;
            std::cout << "Error: MatingPool empty, making the mating pool equal to the population\n\n";
        }
        for (int i = 0; i < popsize; i++) {
            auto index = Rand((matingPool.size() - 1), 0);
            auto partnerA = (matingPool[index]);

            index = Rand((matingPool.size() - 1), 0);
            auto partnerB = (matingPool[index]);

            auto child = partnerA.crossover(partnerB);

            this->rockets[i] = child;
        }
    }


};
////fix the selection function

int main()
{
    GLFWwindow* window;
    
    Renderer::height = Height;
    Renderer::width = Width;
    Rocket::mutationRate = .1f;
    Rocket::mutationpow = 1.1f;
    float rocketWidth = 25;
    float rocketHeight = 5;
    float ox = Renderer::width / 2.0;
    float oy = Renderer::height / 2.0;
    float ow = 200;
    float oh = 10;

    population pop(rocketWidth, rocketHeight);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //specifies version 3.3 and using the core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(Width, Height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    ////fps
    //glfwSwapInterval(1);
    
    
    //makes sure glew initilizes correctly
    if (glewInit() != GLEW_OK) {
        std::cout << "error!\n";
    }
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA));
    GLCall(glEnable(GL_BLEND));

    {
        //an array holding the positions of the  triangle plus one for the square

        //openGL how to read the positions to make a square out of two triangles
        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };


        VertexArray targetArray;
        VertexBuffer targetBuffer(Rocket::rect(10, 10), 4 * 4 * sizeof(float));
        VertexBufferLayout targetLayout;
        targetLayout.push<float>(2);
        targetLayout.push<float>(2);
        targetArray.addBuffer(targetBuffer, targetLayout);


        VertexArray obstacleArray;
        VertexBuffer obstacleBuffer(Rocket::rect(ow, oh), 4 * 4 * sizeof(float));
        VertexBufferLayout obstacleLayout;
        obstacleLayout.push<float>(2);
        obstacleLayout.push<float>(2);
        obstacleArray.addBuffer(obstacleBuffer, obstacleLayout);


        IndexBuffer targetib(indicies, 6);
        targetArray.UnBind();
        targetBuffer.unBind();
        targetib.unBind();


        IndexBuffer obstacleib(indicies, 6);
        obstacleArray.UnBind();
        obstacleBuffer.unBind();
        obstacleib.unBind();


        VertexArray va;
        //makes the buffer that will hold the positons and binds it to the gpu
        VertexBuffer vb(pop.rockets[0].rect(), 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        va.addBuffer(vb, layout);
        va.Bind();
        




        //makes an index buffer object and binds it to the program
        IndexBuffer ib(indicies, 6);

        //the projection ont the screen, simulating depth
        glm::mat4 proj = glm::ortho(0.f, Width, Height, 0.f, -250.0f, 250.0f);
        //the view matrix, simulating camera, first paramiter is an identity matrix, second is the translation
        glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));


        Shader shader("res/shaders/Basic.shader");
        //Shader shader("OpenGLPractice/OpenGLPractice/res/shaders/BasicTexture.shader");
        shader.Bind();



        //pop.pos(glm::vec2(width/2, 0));

        //removes all the following items from the gpu to simulate animating multiple objects
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

        glm::vec3 translationA(200, 200, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
            va.Bind();
            ib.bind();

            //re-assigns the following items to the gpu right befor drawing them
            ImGui_ImplGlfwGL3_NewFrame();

            if (restart) {
                auto tmp = 
                pop = *(new population(rocketWidth, rocketHeight));
                restart = false;
            }
            
            bool notFinished = false;
            for (int i = 0; i < popsize; i++) {
                Rocket* r = &pop.rockets[i];
                glm::mat4 model = glm::translate(glm::mat4(1.f), r->pos());
                model = model * glm::rotate(glm::mat4(1.f), r->angle, glm::vec3(0, 0, 1));
                vec4 obstical(ox, oy, ow, oh);
                bool updated = r->update(&obstical);

                notFinished = notFinished || updated;

                //preform the calculation to see where it is in the screen
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MPV", mvp);
                shader.SetUniform4f("u_color", r->r, r->g, r->b, r->a);
                /*draw our item*/
                renderer.Draw(va, ib, shader);

            }

            if (!notFinished) {
                Rocket::count = DNA::lifespan - 1;
            }

            
            {


                ImGui::SliderFloat("mutationchance", &Rocket::mutationRate, 0.0f, 1.f);// Edit 1 float using a slider from 0.0f to 1.0f  
                ImGui::SliderFloat("mutationpow", &Rocket::mutationpow, 0.0f, 2.f);// Edit 1 float using a slider from 0.0f to 1.0f  

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

                ImGui::Checkbox("restart population", &restart);

                ImGui::SliderFloat("x", &ox, 0.f, Renderer::width);
                ImGui::SliderFloat("y", &oy, 0.f, Renderer::height);
                ImGui::SliderFloat("w", &ow, 0.f, Renderer::width);
                ImGui::SliderFloat("h", &oh, 0.f, Renderer::height);

                ImGui::SliderInt("count", &Rocket::count, 0.f, DNA::lifespan);

                //ImGui::
            }
            



            {
                targetArray.Bind();
                targetBuffer.bind();
                targetib.bind();

                Rocket* r = new Rocket();
                glm::mat4 model = glm::translate(glm::mat4(1.f), r->target);
                model = model * glm::rotate(glm::mat4(1.f), r->angle++, glm::vec3(0, 0, 1));
                r->update();

                //preform the calculation to see where it is in the screen
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MPV", mvp);
                shader.SetUniform4f("u_color", 0, 1, 1, .8);
                /*draw our item*/
                renderer.Draw(targetArray, targetib, shader);
                free(r);
            }

            
                {
                    obstacleArray.Bind();
                    obstacleBuffer.bind();
                    obstacleib.bind();

                    Rocket* r = new Rocket();
                    glm::mat4 model = glm::translate(glm::mat4(1.f), vec3(ox, oy, 0));
                    model = model * glm::rotate(glm::mat4(1.f), r->angle++, glm::vec3(0, 0, 1));
                    r->update();

                    //preform the calculation to see where it is in the screen
                    glm::mat4 mvp = proj * view * model;
                    shader.Bind();
                    shader.SetUniformMat4f("u_MPV", mvp);
                    shader.SetUniform4f("u_color", 1, .2, .2, .8);
                    /*draw our item*/
                    renderer.Draw(obstacleArray, obstacleib, shader);
                    free(r);
                }
            


            Rocket::count++;
            if (Rocket::count >= DNA::lifespan) {
                Rocket::count = 0;
                pop.eval();
                pop.selection();
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
    //std::cin.get();
}