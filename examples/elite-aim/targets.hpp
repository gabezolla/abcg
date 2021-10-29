#ifndef TARGETS_HPP_
#define TARGETS_HPP_

#include <list>
#include <random>

#include "openglwindow.hpp"
#include "abcg.hpp"
// #include "gamedata.hpp"

class OpenGLWindow;

class Targets {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();
  class OpenGLWindow;
  // const GameData &gameData, float deltaTime);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_colorLoc{};
  // GLint m_rotationLoc{};
  // GLint m_translationLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};

  struct Target {
    bool m_clicked{false};
    // glm::vec2 m_translation{glm::vec2(0)};
    // glm::vec2 m_velocity{glm::vec2(0)};
    float m_scale{0.18f};
  };

  Targets::Target createTarget(int x, int y);
  void restart();  

  float m_scale{0.18f};
  int m_delay{1500};

  std::list<Target> m_targets;
  std::default_random_engine m_randomEngine;
};

#endif