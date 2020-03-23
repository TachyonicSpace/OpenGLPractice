#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>;

/*compile a shader (gpu program) from a string*/
static unsigned int CompileShader(unsigned int type, const std::string source) {
    //creates a shader of specified type
    unsigned int id = glCreateShader(type);
    //stores the source code as a string
    const char* src = source.c_str();
    //load source code into openGL and compiles it
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //makes sure code compiled correctly
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    //if compile error, print our the error
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile \n" << message << "\n";
        glDeleteShader(id);
        return 0;
    }
    //else return compiled code location
    return id;
}

//create the shaders from strings
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    //makes a new gpu program, and both shaders compiled code
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //attatch shaders to program and makes sure it is valid
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    //delete the shaders since they are incorporated into the program and return program
    glDeleteShader(vs);
    glDeleteShader(fs);

    return(program);
}

int main()
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
    
    //makes sure glew initilizes correctly
    if (glewInit() != GLEW_OK) {
        std::cout << "error!\n";
    }

    //an array holding the positions of the  triangle
    float positions[] = {
        -.5f, -.5f,
        0.0f, 0.5f,
        .5f, -0.5f
    };

    //makes the buffer that will hold the positons
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    //binds the buffer to the current scope
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //specifies the type, how big the array is in bytes, points to the data, and hints at openGL how the data will be used
    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    //specifies how to traverse the array
    glEnableVertexAttribArray(0);
    /*starting index in data
    how many points in each vertex
    data type
    do we need to normilize the data or not
    how far in memory do we move bevore hitting the next vertex
    a pointer*/
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(positions[0]) * 2, 0);

    //source code for the diffrent shaders
    std::string vertexshader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main(){\n"
        "   gl_Position = position;\n"
        "}\n";
    std::string fragmentshader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main(){\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";
    //assignes the program with these two shaders to be used by openGL
    unsigned int shader = CreateShader(vertexshader, fragmentshader);
    glUseProgram(shader);

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