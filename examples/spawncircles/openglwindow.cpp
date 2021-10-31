#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "abcg.hpp"

void OpenGLWindow::initializeGL() {
  // Clear window
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // Create shader program
  m_program = createProgramFromFile(getAssetsPath() + "target.vert",
                                    getAssetsPath() + "target.frag");

  m_gameData.m_state = State::Menu;

  // Start pseudo-random number generator
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);
}

void OpenGLWindow::paintGL() {
  if (m_gameData.m_state == State::Menu || m_gameData.m_diff == Difficulty::None) return;
  auto sides{12};

  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  if (m_WaitTimer.elapsed() > spawnTime) {
    m_elapsedTimer.restart();
    setupModel(sides);
    glUseProgram(m_program);

    // Choose a random xy position from (-1, -1) to (1, 1)
    std::uniform_real_distribution<float> rd1(-1.0f, 1.0f);
    position = {rd1(m_randomEngine), rd1(m_randomEngine)};

    // Crete target and push into list
    Target target;
    target.target_program = m_program;
    target.position = position;
    m_list.push_back({target});

    m_gameData.rodadas += 1;

    // Scale factor
    GLint scaleLocation{glGetUniformLocation(m_program, "scale")};
    glUniform1f(scaleLocation, circleScale);

    // Set translation
    GLint translationLocation{glGetUniformLocation(m_program, "translation")};
    glUniform2fv(translationLocation, 1, &position.x);

    // Set the rotation of the little square
    GLint rotationLocation{glGetUniformLocation(m_program, "rotation")};
    glUniform1f(rotationLocation, 183);

    // Render
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, sides + 2);
    glBindVertexArray(0);

    // Restart timer to build another target
    m_WaitTimer.restart();
  }

  // Function to delete circle spawned
  if (m_elapsedTimer.elapsed() > unspawnTime) {
    abcg::glClear(GL_COLOR_BUFFER_BIT);
    m_list = {};
    m_elapsedTimer.restart();
  }

  glUseProgram(0);
}

void OpenGLWindow::paintUI() {

  if (m_gameData.m_state != State::Menu && m_gameData.m_diff != Difficulty::None) {
    auto widgetSize{ImVec2(200, 90)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5,
                                   m_viewportHeight - widgetSize.y - 5));
    ImGui::SetNextWindowSize(widgetSize);
    auto windowFlags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin(" ", nullptr, windowFlags);

    ImGui::Text("Total de acertos: %d", m_gameData.acertos);
    ImGui::Text("Total de alvos: %d", m_gameData.rodadas);
    
    if (ImGui::Button("Voltar", ImVec2(-1, 30))) {
      glClear(GL_COLOR_BUFFER_BIT);
      m_gameData.m_diff = Difficulty::None;
      m_gameData.m_state = State::Menu;
      m_gameData.acertos = 0;
      m_gameData.rodadas = 0;
    }
    ImGui::End();
  }
  
  else if (m_gameData.m_state == State::Menu && m_gameData.m_diff == Difficulty::None) {
    auto widgetSize{ImVec2(215, 220)};
    ImGui::SetNextWindowPos(ImVec2((m_viewportWidth / 2) - 120,
                                   (m_viewportHeight / 2) - 120));
    ImGui::SetNextWindowSize(widgetSize);
    auto windowFlags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar};

    ImGui::Begin(" ", nullptr, windowFlags);
    if (ImGui::Button("Speed", ImVec2(200, 100))) {
      m_gameData.m_state = State::Speed;
      m_WaitTimer.restart();
    }

    if (ImGui::Button("Precision", ImVec2(200, 100))) {
      m_gameData.m_state = State::Precision;
      m_WaitTimer.restart();
    }
    ImGui::End();
  }

  else if(m_gameData.m_state != State::Menu && m_gameData.m_diff == Difficulty::None){
    auto widgetSize{ImVec2(215, 430)};
    ImGui::SetNextWindowPos(ImVec2((m_viewportWidth / 2) - 120,
                                   (m_viewportHeight / 2) - 220));
    ImGui::SetNextWindowSize(widgetSize);
    auto windowFlags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar};

    ImGui::Begin(" ", nullptr, windowFlags);
    if (ImGui::Button("Easy", ImVec2(200, 100))) {
      if(m_gameData.m_state == State::Speed) {
        unspawnTime = 1.0f;
        circleScale = 0.10f;
      }
      else if(m_gameData.m_state == State::Precision) {
        circleScale = 0.10f;
        unspawnTime = 1.0f;
      }
      m_gameData.m_diff = Difficulty::Easy;
      m_WaitTimer.restart();
    }

    if (ImGui::Button("Medium", ImVec2(200, 100))) {
      if(m_gameData.m_state == State::Speed) {
        unspawnTime = 0.75f;
        circleScale = 0.10f;
      }
      else if(m_gameData.m_state == State::Precision) {
        circleScale = 0.075f;
        unspawnTime = 1.0f;
      }
      m_gameData.m_diff = Difficulty::Easy;
      m_WaitTimer.restart();
    }

    if (ImGui::Button("Hard", ImVec2(200, 100))) {
      if(m_gameData.m_state == State::Speed) {
        unspawnTime = 0.5f;
        circleScale = 0.10f;
      }
      else if(m_gameData.m_state == State::Precision) {
        circleScale = 0.025f;
        unspawnTime = 1.0f;
      }
      m_gameData.m_diff = Difficulty::Easy;
      m_WaitTimer.restart();
    }

    if (ImGui::Button("Impossible", ImVec2(200, 100))) {
      if(m_gameData.m_state == State::Speed) {
        unspawnTime = 0.25f;
        circleScale = 0.10f;
      }
      else if(m_gameData.m_state == State::Precision) {
        circleScale = 0.01f;
        unspawnTime = 1.0f;
      }
      m_gameData.m_diff = Difficulty::Easy;
      m_WaitTimer.restart();
    }

    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;
}

void OpenGLWindow::terminateGL() {
  glDeleteProgram(m_program);
  glDeleteBuffers(1, &m_vboPositions);
  glDeleteBuffers(1, &m_vboColors);
  glDeleteVertexArrays(1, &m_vao);
}

void OpenGLWindow::setupModel(int sides) {
  // Release previous resources, if any
  glDeleteBuffers(1, &m_vboPositions);
  glDeleteBuffers(1, &m_vboColors);
  glDeleteVertexArrays(1, &m_vao);

  // Select random colors for the radial gradient
  std::uniform_real_distribution<float> rd(0.0f, 1.0f);
  glm::vec3 color1{rd(m_randomEngine), rd(m_randomEngine), rd(m_randomEngine)};
  glm::vec3 color2{rd(m_randomEngine), rd(m_randomEngine), rd(m_randomEngine)};

  // Minimum number of sides is 3
  sides = std::max(3, sides);

  std::vector<glm::vec2> positions(0);
  std::vector<glm::vec3> colors(0);

  // Polygon center
  positions.emplace_back(0, 0);
  colors.push_back(color1);

  // Border vertices
  auto step{M_PI * 2 / sides};
  for (auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
    colors.push_back(color2);
  }

  // Duplicate second vertex
  positions.push_back(positions.at(1));

  colors.push_back(color2);

  // Generate VBO of positions
  glGenBuffers(1, &m_vboPositions);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
               positions.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate VBO of colors
  glGenBuffers(1, &m_vboColors);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3),
               colors.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};
  GLint colorAttribute{glGetAttribLocation(m_program, "inColor")};

  // Create VAO
  glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnableVertexAttribArray(colorAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
  glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  glBindVertexArray(0);
}

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Mouse events
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
    if (event.button.button == SDL_BUTTON_RIGHT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));

    glm::ivec2 mousePosition;
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
    m_gameData.posicaoMouse = {
        glm::vec2{(mousePosition.x - m_viewportWidth / 2.0f) / 300.0f,
                  (mousePosition.y - m_viewportHeight / 2.0f) / 300.0f}};

    m_gameData.posicaoMouse.y = -m_gameData.posicaoMouse.y;
    checkHit();
  }
}

void OpenGLWindow::checkHit() {
  for (auto &target : m_list) {
    glm::vec2 position = {target.position.x, target.position.y};

    const auto distance{glm::distance(m_gameData.posicaoMouse, position)};

    if (distance < circleScale) {
      target.m_hit = true;
      m_list.remove_if([](const Target &t) { return t.m_hit; });
      m_gameData.acertos += 1;
    }
  }
}