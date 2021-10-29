#include "target.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Targets::initializeGL(GLuint program, int quantity) {
  terminateGL();

  // Start pseudo-random number generator
  m_randomEngine.seed(std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create Target
  m_Targets.clear();
  m_Targets.resize(quantity);

  for (auto &target : m_Targets) {
    target = createTarget();

    // Make sure the target won't collide with the ship
    do {
      target.m_translation = {m_randomDist(m_randomEngine),
                                m_randomDist(m_randomEngine)};
    } while (glm::length(target.m_translation) < 0.5f);
  }
}

void Targets::paintGL() {
  abcg::glUseProgram(m_program);

  for (const auto &target : m_Targets) {
    abcg::glBindVertexArray(target.m_vao);

    abcg::glUniform4fv(m_colorLoc, 1, &target.m_color.r);
    abcg::glUniform1f(m_scaleLoc, target.m_scale);
    abcg::glUniform1f(m_rotationLoc, target.m_rotation);

    for (auto i : {-2, 0, 2}) {
      for (auto j : {-2, 0, 2}) {
        abcg::glUniform2f(m_translationLoc, target.m_translation.x + j,
                          target.m_translation.y + i);

        abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, target.m_polygonSides + 2);
      }
    }

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Targets::terminateGL() {
  for (auto target : m_Targets) {
    abcg::glDeleteBuffers(1, &target.m_vbo);
    abcg::glDeleteVertexArrays(1, &target.m_vao);
  }
}

void Target::update(float deltaTime) {
  
}

Target::Target Target::createTarget(glm::vec2 translation,
                                              float scale) {
  Target target;

  auto &re{m_randomEngine};  // Shortcut

  // Randomly choose the number of sides
  std::uniform_int_distribution<int> randomSides(6, 20);
  target.m_polygonSides = randomSides(re);

  // Choose a random color (actually, a grayscale)
  std::uniform_real_distribution<float> randomIntensity(0.5f, 1.0f);
  target.m_color = glm::vec4(1) * randomIntensity(re);

  target.m_color.a = 1.0f;
  target.m_rotation = 0.0f;
  target.m_scale = scale;
  target.m_translation = translation;

  // Choose a random angular velocity
  target.m_angularVelocity = m_randomDist(re);

  // Choose a random direction
  glm::vec2 direction{m_randomDist(re), m_randomDist(re)};
  target.m_velocity = glm::normalize(direction) / 7.0f;

  // Create geometry
  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  const auto step{M_PI * 2 / target.m_polygonSides};
  std::uniform_real_distribution<float> randomRadius(0.8f, 1.0f);
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    const auto radius{randomRadius(re)};
    positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO
  abcg::glGenBuffers(1, &target.m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, target.m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &target.m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(target.m_vao);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, target.m_vbo);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return target;
}