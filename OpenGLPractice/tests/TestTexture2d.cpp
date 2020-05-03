#include "TestTexture2d.h"
#include "imgui\imgui.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace test {



	TestTexture2d::TestTexture2d(): translationA(200, 200, 0), translationB(400, 200, 0)
	{

        float positions[] = {
            -50.f, -50.f, 0, 0,
             50.f, -50.f, 1, 0,
             50.f,  50.f, 1, 1,
            -50.f,  50.f, 0, 1
        };

        //openGL how to read the positions to make a square out of two triangles
        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA));
        GLCall(glEnable(GL_BLEND));

        shader = std::make_unique<Shader>("res/shaders/BasicTexture.shader");
        va = std::make_unique<VertexArray>();

        //VertexArray va;
        //makes the buffer that will hold the positons and binds it to the gpu
        vb = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        va->addBuffer(*vb, layout);


        //makes an index buffer object and binds it to the program
        //IndexBuffer ib(indicies, 6);
        ib = std::make_unique<IndexBuffer>(indicies, 6);

        //the projection ont the screen, simulating depth
        proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -250.0f, 250.0f);
        //the view matrix, simulating camera, first paramiter is an identity matrix, second is the translation
        view = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));

        //Shader shader("res/shaders/BasicTexture.shader");
        //Shader shader("OpenGLPractice/OpenGLPractice/res/shaders/BasicTexture.shader");
        shader->Bind();
        shader->SetUniform4f("u_color", 0.2f, 0.3f, .8f, 1.0f);



        //Texture texture();
        texture = std::make_unique<Texture>("res/textures/image.png");

        texture->Bind();
        shader->SetUniform1i("u_Texture", 0);


        //removes all the following items from the gpu to simulate animating multiple objects
        va->UnBind();
        shader->UnBind();
        va->UnBind();
        ib->unBind();

	}

	TestTexture2d::~TestTexture2d()
	{
	}

	void TestTexture2d::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2d::OnRender()
	{
		GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        texture->Bind();

        {
            //move the acctual object 200 to the left and 200 up
            glm::mat4 model = glm::translate(glm::mat4(1.f), translationA);
            glm::mat4 rotate = glm::rotate(glm::mat4(1.f), rotation, glm::vec3(-1, -1, -1));

            //preform the calculation to see where it is in the screen
            glm::mat4 mvp = proj * view * model * rotate;
            shader->Bind();
            shader->SetUniformMat4f("u_MPV", mvp);
            /*draw our item*/
            renderer.Draw(*va, *ib, *shader);
        }

        {
            //move the acctual object 200 to the left and 200 up
            glm::mat4 model = glm::translate(glm::mat4(1.f), translationB);
            glm::mat4 rotate = glm::rotate(glm::mat4(1.f), rotation, glm::vec3(-1, -1, -1));

            //preform the calculation to see where it is in the screen
            glm::mat4 mvp = proj * view * model * rotate;
            shader->Bind();
            shader->SetUniformMat4f("u_MPV", mvp);
            /*draw our item*/
            renderer.Draw(*va, *ib, *shader);
        }

	}

	void TestTexture2d::OnImGuiRender()
	{
        ImGui::SliderFloat3("boxA", &translationA.x, 0.f, 960.f);
        ImGui::SliderFloat3("boxB", &translationB.x, 0.f, 960.f);
        ImGui::SliderFloat("angle", &rotation, 0.f, 2 * 3.14159f);
        ImGui::Text("application average %.3f ms/frame (%.1f fps)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}

}