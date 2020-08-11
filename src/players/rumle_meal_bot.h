#ifndef TOUCHDOWN_RUMLE_MEAL_BOT_H
#define TOUCHDOWN_RUMLE_MEAL_BOT_H

#include "../game/player.h"
#include <memory>
#include <vector>
#include <utility>
#include <unordered_map>
#include <set>

typedef std::size_t nid;

#define MEAL_SAMPLE_TIME_MS 5
#define MEAL_MAX_SAMPLES 500
#define MEAL_EXPLORE 1.4

namespace meal
{
class SparseBoard
{
public:
  SparseBoard(const game::Board &);

  bool operator==(const SparseBoard &);

  std::set<std::pair<unsigned char, unsigned char>> p1Pieces;
  std::set<std::pair<unsigned char, unsigned char>> p2Pieces;
};

class SparseMove : public game::Move
{
public:
  using game::Move::Move;
  bool isValid(const SparseBoard &board, bool turn) const;
  void performMove(SparseBoard &board) const;
};

class State
{
public:
  enum Status
  {
    INPROGRESS,
    TIED,
    WIN,
    LOSS,
  };
  State(const game::Board &board) : board{board} {}
  State(const SparseBoard &board) : board{board} {}

  Status status() const;

  int simulatePlayout() const;
  std::vector<std::pair<State, SparseMove>> buildChildren() const;

  bool turn = true;
  SparseBoard board;

private:
  Status status(const std::vector<SparseMove> &) const;
  std::vector<SparseMove> validMoves() const;
  State buildChild(const SparseMove &) const;
};

struct Node
{
  Node(const State &s) : state(s) {}
  State state;
  SparseMove move = SparseMove(0, 0, 0, 0);
  nid parent = 0;
  int visits = 0;
  int wins = 0;
  std::vector<nid> children;
};

class NodeManager
{
public:
  Node &get(nid);
  nid add(Node);
  void expandNode(nid);
  void report(nid, int);
  bool empty() { return nodes.empty(); }

private:
  std::vector<Node> nodes;
};

class UCT
{
public:
  UCT(std::shared_ptr<NodeManager> nm) : nm(nm) {}
  double UCTValue(nid, int);
  nid bestUCTChild(nid);
  nid bestChild(nid);
  nid selectPromisingLeaf(nid);
  void backPropogate(nid, int);

private:
  std::shared_ptr<NodeManager> nm;
};
} // namespace meal

namespace players
{

class RumleMEALBot : public game::Player
{

public:
  RumleMEALBot() : nm{std::make_shared<meal::NodeManager>()} {}

  std::string name() const;

  bool preferToStart(const game::Board &board);

  game::Move play(const game::Board &board);

private:
  std::shared_ptr<meal::NodeManager> nm;
  nid root;
};

} // namespace players
#endif //TOUCHDOWN_RUMLE_MEAL_BOT_H
