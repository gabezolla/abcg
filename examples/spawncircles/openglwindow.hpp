#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <random>
#include <list>

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;
  void handleEvent(SDL_Event& event) override;  

 private:
  GLuint m_vao{};
  GLuint m_vboPositions{};
  GLuint m_vboColors{};
  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  struct Target {
    GLuint target_vao{};
    GLuint target_program{};
    bool m_hit{false};
    glm::vec2 position;
  };

  float spawnTime{3.0f};
  float unspawnTime{1.0f};
  float circleScale{0.15f};
  GameData m_gameData;
  glm::vec2 position{};

  std::list<Target> m_list{};
  
  std::default_random_engine m_randomEngine;

  abcg::ElapsedTimer m_elapsedTimer;
  abcg::ElapsedTimer m_WaitTimer;

  void removeTarget(Target target);
  void setupModel(int sides);
  void checkHit();
};

#endif