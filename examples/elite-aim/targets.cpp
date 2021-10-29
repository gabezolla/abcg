#include "targets.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Targets::initializeGL(GLuint program) {
  
  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  // m_translationLoc = glGetUniformLocation(m_program, "translation");
  // m_rotationLoc = glGetUniformLocation(m_program, "rotation");

  // Clear list of targets
  m_targets.clear();

  // Create target circle
  auto sides{60};

  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  auto step{M_PI * 2 / sides};
  for (auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO of positions
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
               positions.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  glBindVertexArray(0);
}

void Targets::paintGL() {
  glUseProgram(m_program);

  glBindVertexArray(m_vao);
  glUniform4f(m_colorLoc, 1, 1, 1, 1);
  glUniform1f(m_scaleLoc, m_scale);

  for (auto &target : m_targets) {
    // glUniform2f(m_translationLoc, target.m_translation.x, target.m_translation.y);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 62);
  }

  glBindVertexArray(0);

  glUseProgram(0);

  // Start pseudo-random number generator
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);
}

void Targets::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteVertexArrays(1, &m_vao);
}

Targets::Target Targets::createTarget(int x, int y) {  
  return m_targets.emplace_back(-1 + x, 1 - y);
}