#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <array>
#include <glm/vec2.hpp>
#include <random>

#include "abcg.hpp"
#include "targets.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  /* void resizeGL(int width, int height) override;
  void terminateGL() override;
 */
 private:
  Targets m_targets;
  GLuint m_vao{};
  GLuint m_vboVertices{};
  GLuint m_program{};
  void restart();

  std::array<float, 4> m_clearColor{0.1f, 0.1f, 0.1f, 0.1f};
  std::default_random_engine m_randomEngine;
};

#endif