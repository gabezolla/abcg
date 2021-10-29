#include "openglwindow.hpp"

#include "targets.hpp"

#include "targets.cpp"

#include <fmt/core.h>
#include <imgui.h>
#include <chrono>

void OpenGLWindow::initializeGL() {
  auto windowSettings{getWindowSettings()};

  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  // Create program to render targets
  m_program = createProgramFromFile(getAssetsPath() + "objects.vert", getAssetsPath() + "objects.frag");

  restart();
}

void OpenGLWindow::restart() {
  m_targets.initializeGL(m_program);
}

void OpenGLWindow::paintGL() {

  // Set the clear color
  abcg::glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
  glm::vec2 m_P{};

  // Start pseudo-random number generator
  m_randomEngine.seed(std::chrono::steady_clock::now().time_since_epoch().count());

  // Randomly choose a pair of coordinates in the interval [-1; 1]
  std::uniform_real_distribution<float> realDistribution(-1.0f, 1.0f);
  
  // Create Target on random position
  // Targets.createTarget..
  m_targets.paintGL();

  // Clear window
  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);

}

void OpenGLWindow::paintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::paintUI();
}

/* void resizeGL(int width, int height) {
  int size = width + height;
}
  
void terminateGL() {
  
} */
