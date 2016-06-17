#ifndef GUIGAME_HPP
#define GUIGAME_HPP
#include <SFML/Graphics.hpp>
#include "field.hpp"

#include <ostream> ///

enum class Gamestate{
  Won, Lost, Playing, Pending
};

class GuiGame{
public:
  GuiGame(int height, int width, int mines);
  ~GuiGame();
  bool isOpen() const;
  void getEvent();
  void display();
  Gamestate getState();
  friend std::ostream& operator << (std::ostream& stream, GuiGame& object); ///
private:
  bool changed; ///
  void clickAt(int x, int y, sf::Mouse::Button button);
  void drawLabel(int x, int y);
  int height;
  int width;
  Field field;
  Gamestate state;
  sf::Font font;
  sf::RenderWindow *window;
};

#endif
