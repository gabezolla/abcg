#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Down, Up, Fire };
enum class State { Playing};

struct GameData {
  State m_state{State::Playing};
  std::bitset<5> m_input;  // [up, down, left, right]
  int acertos = 0;
  int rodadas = 0;
  glm::vec2 posicaoMouse;
};

#endif