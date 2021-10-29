#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <random>

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

  GameData m_gameData;
  glm::vec2 position{};
  
  std::default_random_engine m_randomEngine;

  int m_delay{200};
  abcg::ElapsedTimer m_elapsedTimer;
  abcg::ElapsedTimer m_WaitTimer;


  void setupModel(int sides);
  void checkHit();
};

#endif