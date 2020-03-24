#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    
    //makes sure that glew starts correctly
    if (glewInit() != GLEW_OK) {
        std::cout << "error!\n";
    }

    //our data
    float positions[] = {
        -.5f, -.5f,
        0.0f, 0.5f,
        .5f, -0.5f
    };

    //enables us to astore our data onto the gpu
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    //enables us to specify how we want to interprite our data
    glEnableVertexAttribArray(0);
    /*
    how many objects are in the vector
    object type
    do we need to normilize the data
    how big is the vector
    */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(positions[0]) * 2, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /*draw a triangle*/
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}