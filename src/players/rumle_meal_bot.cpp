#include "rumle_meal_bot.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <limits>
#include <math.h>

using namespace std;
using namespace meal;

inline static bool isOutOfBounds(unsigned char x, unsigned char y)
{
    return x >= 8 || y >= 8;
}

bool SparseMove::isValid(const SparseBoard &board, bool turn) const
{
    if (isOutOfBounds(m_fromX, m_fromY))
    {
        return false;
    }
    if (isOutOfBounds(m_toX, m_toY))
    {
        return false;
    }

    if (turn && board.p1Pieces.find({m_fromX, m_fromY}) == board.p1Pieces.end())
    {
        return false;
    }
    if (!turn && board.p2Pieces.find({m_fromX, m_fromY}) == board.p2Pieces.end())
    {
        return false;
    }
    if (turn && m_toY != m_fromY - 1)
    {
        return false;
    }
    if (!turn && m_toY != m_fromY + 1)
    {
        return false;
    }
    char dx = (char)m_toX - (char)m_fromX;

    if (abs(dx) > 1)
    {
        return false;
    }
    else if (abs(dx) == 1)
    {
        if (turn && board.p2Pieces.find({m_toX, m_toY}) == board.p2Pieces.end())
        {
            return false;
        }
        if (!turn && board.p1Pieces.find({m_toX, m_toY}) == board.p1Pieces.end())
        {
            return false;
        }
    }
    else if (board.p1Pieces.find({m_toX, m_toY}) != board.p1Pieces.end() || board.p2Pieces.find({m_toX, m_toY}) != board.p2Pieces.end())
    {
        return false;
    }

    return true;
}

void SparseMove::performMove(SparseBoard &board) const
{
    if (board.p1Pieces.find({m_fromX, m_fromY}) != board.p1Pieces.end())
    {
        board.p1Pieces.erase({m_fromX, m_fromY});
        board.p1Pieces.insert({m_toX, m_toY});
        board.p2Pieces.erase({m_toX, m_toY});
    }
    else
    {
        board.p2Pieces.erase({m_fromX, m_fromY});
        board.p2Pieces.insert({m_toX, m_toY});
        board.p1Pieces.erase({m_toX, m_toY});
    }
}

SparseBoard::SparseBoard(const game::Board &board)
{
    for (unsigned char x = 0; x < 8; ++x)
    {
        for (unsigned char y = 0; y < 8; ++y)
        {
            if (board[x][y] == 1)
                p1Pieces.insert({x, y});
            if (board[x][y] == 2)
                p2Pieces.insert({x, y});
        }
    }
}

bool SparseBoard::operator==(const SparseBoard &board)
{
    return p1Pieces == board.p1Pieces && p2Pieces == board.p2Pieces;
}

int State::simulatePlayout() const
{
    auto state = *this;
    auto moves = state.validMoves();
    auto status = state.status(moves);
    while (status == State::Status::INPROGRESS)
    {
        auto move = moves.front();
        move.performMove(state.board);
        state.turn = !state.turn;
        moves = state.validMoves();
        status = state.status(moves);
    }
    if (status == State::Status::WIN)
    {
        return 2;
    }
    else if (status == State::Status::TIED)
    {
        return 1;
    }
    return 0;
}

State::Status State::status() const
{
    for (auto p1Piece : board.p1Pieces)
    {
        if (p1Piece.second == 0)
        {
            return State::Status::WIN;
        }
    }

    for (auto p2Piece : board.p2Pieces)
    {
        if (p2Piece.second == 7)
        {
            return State::Status::LOSS;
        }
    }
    if (validMoves().empty())
    {
        return State::Status::TIED;
    }
    return State::Status::INPROGRESS;
}

State::Status State::status(const vector<SparseMove> &validMoves) const
{
    for (auto p1Piece : board.p1Pieces)
    {
        if (p1Piece.second == 0)
        {
            return State::Status::WIN;
        }
    }

    for (auto p2Piece : board.p2Pieces)
    {
        if (p2Piece.second == 7)
        {
            return State::Status::LOSS;
        }
    }
    if (validMoves.empty())
    {
        return State::Status::TIED;
    }
    return State::Status::INPROGRESS;
}

vector<SparseMove> State::validMoves() const
{
    const std::set<pair<unsigned char, unsigned char>> *pieces;
    if (turn)
        pieces = &board.p1Pieces;
    else
        pieces = &board.p2Pieces;
    std::vector<SparseMove> moves;
    for (auto piece : *pieces)
    {
        unsigned char y;
        if (turn)
            y = piece.second - 1;
        else
            y = piece.second + 1;
        auto move = SparseMove{piece.first, piece.second, piece.first, y};
        if (move.isValid(board, turn))
            moves.push_back(move);
        move = SparseMove{piece.first, piece.second, (unsigned char)(piece.first + 1), y};
        if (move.isValid(board, turn))
            moves.push_back(move);
        if (piece.first > 0)
        {
            move = SparseMove{piece.first, piece.second, (unsigned char)(piece.first - 1), y};
            if (move.isValid(board, turn))
                moves.push_back(move);
        }
    }
    random_shuffle(moves.begin(), moves.end());
    return moves;
}

State State::buildChild(const SparseMove &move) const
{
    auto newBoard = board;
    move.performMove(newBoard);
    auto newState = State(newBoard);
    newState.turn = !turn;
    return newState;
}

vector<pair<State, SparseMove>> State::buildChildren() const
{
    vector<pair<State, SparseMove>> childrens;
    for (auto move : validMoves())
        childrens.push_back({buildChild(move), move});

    return childrens;
}

void UCT::backPropogate(nid n, int result)
{
    while (n != 0)
    {
        auto &node = nm->get(n);
        n = node.parent;
        node.wins += result;
        node.visits += 1;
    }
}

double UCT::UCTValue(nid node, int parentVisits)
{
    auto visits = nm->get(node).visits;
    if (visits == 0)
    {
        return numeric_limits<double>::max();
    }
    auto wins = nm->get(node).wins;

    return (double)wins / (double)visits + MEAL_EXPLORE * sqrt(log(parentVisits) / (double)visits);
}

nid UCT::bestUCTChild(nid node)
{
    auto visits = nm->get(node).visits;
    auto best = nm->get(node).children.front();
    auto bestValue = UCTValue(best, visits);

    for (nid child : nm->get(node).children)
    {
        auto val = UCTValue(child, visits);
        if (val > bestValue)
        {
            best = child;
            bestValue = val;
        }
    }
    return best;
}
nid UCT::bestChild(nid node)
{
    auto visits = nm->get(node).visits;
    auto best = nm->get(node).children.front();
    double bestValue = (double)nm->get(nm->get(node).children.front()).wins / (double)nm->get(nm->get(node).children.front()).visits;

    for (nid child : nm->get(node).children)
    {
        auto val = (double)nm->get(child).wins / (double)nm->get(child).visits;
        if (val > bestValue)
        {
            best = child;
            bestValue = val;
        }
    }
    return best;
}

nid UCT::selectPromisingLeaf(nid node)
{
    while (nm->get(node).children.size() != 0)
    {
        node = bestUCTChild(node);
    }
    return node;
}

nid NodeManager::add(Node n)
{
    nodes.push_back(n);
    return nodes.size();
}

Node &NodeManager::get(nid i)
{
    return nodes[i - 1];
}

void NodeManager::expandNode(nid n)
{
    vector<nid> children;
    for (auto childState : get(n).state.buildChildren())
    {
        auto childNode = Node(childState.first);
        childNode.parent = n;
        childNode.move = childState.second;
        children.push_back(add(childNode));
    }
    get(n).children = children;
}

string players::RumleMEALBot::name() const
{
    return "MEAL bot v1";
}

bool players::RumleMEALBot::preferToStart(const game::Board &board)
{
    return true;
}

game::Move players::RumleMEALBot::play(const game::Board &board)
{
    if (nm->empty())
    {
        root = nm->add(Node(State(board)));
    }
    else
    {
        auto sboard = SparseBoard(board);
        auto oldRoot = root;
        for (auto child : nm->get(root).children)
        {
            if (nm->get(child).state.board == sboard)
            {
                root = child;
                break;
            }
        }
        // Fixes problem when the instance is reused in statistics
        if (oldRoot == root)
        {
            nm = make_shared<NodeManager>();
            root = nm->add(Node(State(board)));
        }
    }
    auto uct = UCT(nm);

    clock_t begin = clock();

    unsigned int samples = 0;

    for (; (clock() - begin) * 1000 / CLOCKS_PER_SEC < MEAL_SAMPLE_TIME_MS && samples < MEAL_MAX_SAMPLES; ++samples)
    {
        auto leaf = uct.selectPromisingLeaf(root);
        if (nm->get(leaf).state.status() == State::Status::INPROGRESS)
        {
            nm->expandNode(leaf);
            for (auto child : nm->get(leaf).children)
            {
                ++samples;
                int playoutResult = nm->get(child).state.simulatePlayout();
                uct.backPropogate(child, playoutResult);
            }
        }
        else
        {
            ++samples;
            int playoutResult = nm->get(leaf).state.simulatePlayout();
            uct.backPropogate(leaf, playoutResult);
        }
    }
    root = uct.bestChild(root);
    auto move = nm->get(root).move;
    return move;
}
