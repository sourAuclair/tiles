#include "server.hpp"

Server::Server() : listenPort(55001){
}

Server::Server(std::string hostName, int listenPort) : listenPort(listenPort){
  connectedPlayers.push_back(hostName);
}

Server& Server::operator = (const Server& rhs){
  if(&rhs == this){
    return *this;
  }
  listenPort = rhs.listenPort;
  connectedPlayers = rhs.connectedPlayers;
}

void Server::waitForConnection(){
  listener.listen(listenPort);
  listener.accept(socket);
  // Look for name collision
  sf::Packet packet;
  socket.receive(packet);
  std::string name;
  packet >> name;
  bool validName = true;
  for(int i = 0; i < connectedPlayers.size(); i++){
    if(name == connectedPlayers[i]){
      validName = false;
      listener.close();
    }
  }
  if(validName){
    connectedPlayers.push_back(name);
  }
}

void Server::sendCompleteBoard(const Field& field){
  sf::Packet packet;
  packet << "completeBoard";
  sf::Uint32 height, width, mines;
  height = field.getHeight();
  width = field.getWidth();
  mines = field.getMines();
  packet << height << width << mines;

  // (x, y, isOpen, isMine, isFlagged, flagger, surrounding)
  for(sf::Uint32 x = 0; x < width; x++){
    for(sf::Uint32 y = 0; y < height; y++){
      bool isOpen = field.isOpen(x, y);
      bool isMine = field.isMine(x, y);
      bool isFlagged = field.isFlagged(x, y);
      std::string flagger = field.getFlagger(x, y);
      sf::Uint8 surrounding = field.getSurroundingMines(x, y);
      packet << x << y << isOpen << isMine << isFlagged << flagger;
    }
  }

  // Send startTime and endTime
  sf::Uint32 secondsSinceStart = field.getSecondsSinceStart();
  packet << secondsSinceStart;

  socket.send(packet);
}
