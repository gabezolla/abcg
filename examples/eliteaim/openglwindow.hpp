#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <array>
#include <glm/vec2.hpp>
#include <random>

#include "gamedata.hpp"
#include "target.hpp"
#include "abcg.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override;
  void handleEvent(SDL_Event& event) override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:

 // Identificador do par de shaders objects.vert e objects.frag
  GLuint m_objectsProgram{};

  GameData m_gameData;
  Targets m_target;

  // ImFont* m_font{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  std::default_random_engine m_randomEngine;

  abcg::ElapsedTimer m_WaitTimer;

  const std::array<glm::vec2, 3> m_points{ glm::vec2( 0,  1), glm::vec2(-1, -1), glm::vec2( 1, -1) };
  
  void update();
  void setupModel();
};
#endif