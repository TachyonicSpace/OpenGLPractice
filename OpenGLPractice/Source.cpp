#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>;
#include <fstream>
#include <string>
#include <sstream>
//if x is false then add a breakpoint to that line of code
#define ASSERT(x) if (!(x)) __debugbreak();
//clear errors from openGL, run x, then checks for errors
#define GLCall(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//clear errors from openGL
static void GLClearErrors(){
    while (glGetError() != GL_NO_ERROR);
}

//print the errors to the screen, and then return if it ran correctly
static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "{OpenGL Error} (" << error << "):" << function << " "  << file << ":" << line;
        return false;
    }return true;
}


//a struct to allow us to return multiple return types
struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

//allows us to parse in shader from files instread of string literals
static ShaderProgramSource parseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }return { ss[0].str(), ss[1].str() };
}

/*compile a shader (gpu program) from a string*/
static unsigned int CompileShader(unsigned int type, const std::string source) {
    //creates a shader of specified type
    unsigned int id = glCreateShader(type);
    //stores the source code as a string
    const char* src = source.c_str();
    //load source code into openGL and compiles it
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    //makes sure code compiled correctly
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    //if compile error, print our the error
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "failed to compile \n" << message << "\n";
        GLCall(glDeleteShader(id));
        return 0;
    }
    //else return compiled code location
    return id;
}

//create the shaders from strings
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    //makes a new gpu program, and both shaders compiled code
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //attatch shaders to program and makes sure it is valid
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    //delete the shaders since they are incorporated into the program and return program
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

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
    (glfwMakeContextCurrent(window));
    
    
    //makes sure glew initilizes correctly
    if (glewInit() != GLEW_OK) {
        std::cout << "error!\n";
    }

    //an array holding the positions of the  triangle plus one for the square
    float positions[] = {
        -.5f, -.5f,
        0.5f, -0.5f,
        .5f, 0.5f,
        -.5f, .5f,
    };

    //openGL how to read the positions to make a square out of two triangles
    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0
    };

    //makes the buffer that will hold the positons
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    //binds the buffer to the current scope
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    //specifies the type, how big the array is in bytes, points to the data, and hints at openGL how the data will be used
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    //specifies how to traverse the array
    GLCall(glEnableVertexAttribArray(0));
    /*starting index in data
    how many points in each vertex
    data type
    do we need to normilize the data or not
    how far in memory do we move bevore hitting the next vertex
    a pointer*/
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(positions[0]) * 2, 0));

    unsigned int ibo;//indexBufferObject
    GLCall(glGenBuffers(1, &ibo));
    //binds it the the gpu scope
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    //adds the index buffer to the gpu
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), indicies, GL_STATIC_DRAW));

    //makes some shader code from the file at this location
    ShaderProgramSource source = parseShader("res/shaders/Basic.shader");
    //compiles the shader and returns the location on the gpu
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    //tells openGL to use this shader
    GLCall(glUseProgram(shader));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        /*draw a triangle*/
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }
    GLCall(glDeleteProgram(shader));

    glfwTerminate();
}