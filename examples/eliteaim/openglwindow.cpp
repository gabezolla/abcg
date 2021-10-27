#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include <chrono>

void OpenGLWindow::initializeGL() {  

  // Clear window
  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  /* const auto *vertexShader{R"gl(
    #version 410
    layout(location = 0) in vec2 inPosition;

    uniform vec4 color;
    uniform vec2 translation;
    uniform float rotation;

    out vec4 fragColor;

    void main() {
      float sinAngle = sin(rotation);
      float cosAngle = cos(rotation);
      vec2 rotated = vec2(inPosition.x * cosAngle - inPosition.y * sinAngle,
                          inPosition.x * sinAngle + inPosition.y * cosAngle); 

      gl_Position = vec4(rotated + translation, 0, 100);
      fragColor = color;
    }
  )gl"};

  const auto *fragmentShader{R"gl(
    #version 410
    out vec4 outColor;
    void main() { outColor = vec4(1); }
  )gl"};

  */

  // Create program to render targets
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "target.vert", getAssetsPath() + "target.frag");

  std::array<GLfloat, 2> sizes{};
#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
  abcg::glGetFloatv(GL_POINT_SIZE_RANGE, sizes.data());
#else
  abcg::glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, sizes.data());
#endif
  fmt::print("Point size: {:.2f} (min), {:.2f} (max)\n", sizes[0], sizes[1]);
}

void OpenGLWindow::paintGL() {

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);
  update();
  // Appears target every 5 seconds
  if(m_WaitTimer.elapsed() > 5) {
    std::uniform_real_distribution<float> intDistribution(-1.0f, 1.0f);
    m_randomEngine.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    m_target.paintGL();
    m_WaitTimer.restart();
  }

}

void OpenGLWindow::update() {

}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
  {
    ImGui::SetNextWindowPos(ImVec2(5, 5 + 50 + 16 + 5));
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);

    if (ImGui::Button("Clear window", ImVec2(150, 30))) {
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    ImGui::End();
  }
}

void OpenGLWindow::handleEvent(SDL_Event &event) {  
  // Mouse events
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Shoot));
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Shoot));
    if (event.button.button == SDL_BUTTON_RIGHT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Shoot));
  }

  if (event.type == SDL_MOUSEMOTION) {
    glm::ivec2 mousePosition;
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    glm::vec2 direction{glm::vec2{mousePosition.x - m_viewportWidth / 2,
                                  mousePosition.y - m_viewportHeight / 2}};
    direction.y = -direction.y;

    /* if(direction.x == target.x && direction.y == target.y) {
      // aumenta contador, some o target
    } */
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
}

/* void OpenGLWindow::setupModel() {
  
  // Release previous VBO and VAO
  abcg::glDeleteBuffers(1, &m_vboVertices);
  abcg::glDeleteVertexArrays(1, &m_vao);

  // Generate a new VBO and get the associated ID
  abcg::glGenBuffers(1, &m_vboVertices);
  // Bind VBO in order to use it
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
  // Upload data to VBO
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_P), &m_P, GL_STATIC_DRAW);
  // Unbinding the VBO is allowed (data can be released now)
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  const GLint positionAttribute{
      abcg::glGetAttribLocation(m_objectsProgram, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
} 
*/ 