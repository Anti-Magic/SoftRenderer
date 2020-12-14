#include <stdio.h>
#include <string>
#include <vector>
#include <glad/glad.h>
#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Mathf.h"
#include "Device.h"
#include "SceneTest.h"
#include "SceneTest2.h"

using namespace SoftRenderer;

static std::shared_ptr<Texture2D> LoadTexture2D(const std::string& path, bool isFlipY)
{
    SDL_Surface* img = IMG_Load(path.c_str());
    uint8_t* src = (uint8_t*)img->pixels;
    std::shared_ptr<Texture2D> res = std::make_shared<Texture2D>(Vec2(img->w, img->h), Vec4());
    uint8_t r, g, b, a;
    for (int y = 0; y < res->size.y; y++)
    {
        for (int x = 0; x < res->size.x; x++)
        {
            int srcIndex = (x + y * img->w) * img->format->BytesPerPixel;
            if (isFlipY)
            {
                srcIndex = (x + (img->h - y - 1) * img->w) * img->format->BytesPerPixel;
            }
            int dstIndex = (x + y * res->size.x) * 4;
            uint32_t* pixel = (uint32_t*)(src + srcIndex);
            SDL_GetRGBA(*pixel, img->format, &r, &g, &b, &a);
            res->d[dstIndex] = r / 255.0f;
            res->d[dstIndex + 1] = g / 255.0f;
            res->d[dstIndex + 2] = b / 255.0f;
            res->d[dstIndex + 3] = a / 255.0f;
        }
    }
    return res;
}

static Mesh LoadMesh(const std::string& path)
{
    return Mesh();
}

class TextureRenderer
{
public:
    GLuint shaderID;
    GLuint textureID;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    int width;
    int height;
    uint8_t* buffer;

    TextureRenderer(int _width, int _height)
        : width(_width)
        , height(_height)
        , buffer(new uint8_t[_width * _height * 4])
    {
        initShader();
        initMesh();
        initTexture();
    }

    TextureRenderer(const TextureRenderer&) = delete;
    TextureRenderer(const TextureRenderer&&) = delete;

    ~TextureRenderer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteTextures(1, &textureID);
    }

    void render(const std::vector<float>& src)
    {
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int index = (x + y * width) * 4;
                buffer[index] = floor(src[index] * 255);
                buffer[index + 1] = floor(src[index + 1] * 255);
                buffer[index + 2] = floor(src[index + 2] * 255);
                buffer[index + 3] = floor(src[index + 3] * 255);
            }
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

        // render container
        glUseProgram(shaderID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

private:
    bool initShader()
    {
        std::string vShaderCode = vertShaderCode();
        std::string fShaderCode = fragShaderCode();
        const char* vShaderCodeC = vShaderCode.c_str();
        const char* fShaderCodeC = fShaderCode.c_str();
        GLint success;
        GLchar infoLog[1024];
        // vertex shader
        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCodeC, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
            printf("error: %s\n", infoLog);
            return false;
        }
        // fragment Shader
        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCodeC, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
            printf("error: %s\n", infoLog);
            return false;
        }
        // shader Program
        shaderID = glCreateProgram();
        glAttachShader(shaderID, vertex);
        glAttachShader(shaderID, fragment);
        glLinkProgram(shaderID);
        glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
            printf("error: %s\n", infoLog);
            return false;
        }
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return true;
    }

    void initMesh()
    {
        float vertices[] = {
            // positions         // texture coords
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // bottom left
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, // top left 
             1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // top right
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom right
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };
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
    }

    void initTexture()
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        // texture wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glUseProgram(shaderID);
        glUniform1i(glGetUniformLocation(shaderID, "tex"), 0);
    }

    std::string vertShaderCode()
    {
        return R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
void main()
{
	gl_Position = vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
)";
    }

    std::string fragShaderCode()
    {
        return R"(#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D tex;
void main()
{
	FragColor = texture(tex, TexCoord);
}
)";
    }
};

KeyCode SDLKeyConvert(SDL_Keycode key)
{
    switch (key)
    {
    case SDLK_SPACE:
        return KeyCode::Space;
    case SDLK_w:
        return KeyCode::W;
    case SDLK_a:
        return KeyCode::A;
    case SDLK_s:
        return KeyCode::S;
    case SDLK_d:
        return KeyCode::D;
    case SDLK_q:
        return KeyCode::Q;
    case SDLK_e:
        return KeyCode::E;
    default:
        return KeyCode::Unknown;
    }
}

MouseButton SDLMouseConvert(int b)
{
    switch (b)
    {
    case SDL_BUTTON_LEFT:
        return MouseButton::Left;
    case SDL_BUTTON_MIDDLE:
        return MouseButton::Middle;
    case SDL_BUTTON_RIGHT:
        return MouseButton::Right;
    default:
        return MouseButton::None;
    }
}

int main(int argc, char* argv[])
{
    int width = 854;
    int height = 480;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#ifdef __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_Window* window = SDL_CreateWindow("SoftRenderer", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        width, height, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(0); // vsync

    // Initialize OpenGL loader
    bool err = gladLoadGL() == 0;
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    TextureRenderer tr(width, height);
    std::shared_ptr<Device> device = std::make_shared<Device>(Vec2(width, height));
    device->LoadTexture2D = LoadTexture2D;
    device->LoadMesh = LoadMesh;
    device->setScene(std::make_shared<SceneTest>(device));

    // main loop
    uint32_t lastTime = SDL_GetTicks();
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
            {
                done = true;
            }
            else if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                {
                    done = true;
                }
            }
            else if (event.type == SDL_KEYDOWN)
            {
                device->onKeyDown(SDLKeyConvert(event.key.keysym.sym));
            }
            else if (event.type == SDL_KEYUP)
            {
                device->onKeyUp(SDLKeyConvert(event.key.keysym.sym));
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                device->onMouseDown(SDLMouseConvert(event.button.button));
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                device->onMouseUp(SDLMouseConvert(event.button.button));
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                device->onMouseMotion(Vec2(event.motion.x, event.motion.y));
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        uint32_t now = SDL_GetTicks();
        float deltaTime = (now - lastTime) / 1000.0f;
        lastTime = now;
        device->loop(deltaTime);

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClear(GL_COLOR_BUFFER_BIT);

        tr.render(device->frameBuffer.color->d);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
