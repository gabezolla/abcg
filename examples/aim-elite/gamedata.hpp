#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Fire };
enum class State { Precision, Speed, Menu };
enum class Difficulty { None, Easy, Medium, Hard, Impossible };

struct GameData {
  State m_state{State::Menu};
  Difficulty m_diff{Difficulty::None};
  std::bitset<5> m_input;
  int targetHits = 0;
  int rounds = 0;
  glm::vec2 posicaoMouse;
};

#endif