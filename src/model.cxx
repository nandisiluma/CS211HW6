#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height})
{
    compute_next_moves_();
}

Model::Rectangle Model::board() const
{
    return board_.all_positions();
}

Player Model::operator[](Position pos) const
{
    return board_[pos];
}

Move const* Model::find_move(Position pos) const
{
    auto i = next_moves_.find(pos);

    if (i == next_moves_.end())
        return nullptr;
    else
        return &*i;
}

void Model::play_move(Position pos)
{
    if (is_game_over())
        throw Client_logic_error("Model::play_move: game over");

    Move const* movep = find_move(pos);
    if (!movep)
        throw Client_logic_error("Model::play_move: no such move");
    else {
        really_play_move_(*movep);
    }
}

//
// BELOW ARE *OPTIONAL* HELPER FUNCTIONS
//

Position_set Model::find_flips_(Position current, Dimensions dir) const
{
    Position_set result = {};
    // Finding the max times we need to iterate
    // First initialized to the height of the board;
    // if width is >= height, then it is max is width
    int max_iteration = board_.dimensions().height;
    if (board_.dimensions().width >= board_.dimensions().height){
        max_iteration = board_.dimensions().width;
    }

    // loop through start + dir, start + 2 * dir...
    for (int i = 1; i < max_iteration; ++i) {
        Position square = current + i * dir;

        // Case 1: if we reach position that goes off the board or is
        // unoccupied, result is empty

        //1a: handling the cases where it goes off the board
        if ((square.x > board_.dimensions().width) || (square.x < 0)) {
            //if position x value exceeds width or less than 0
            return {};
        } else if ((square.y > board_.dimensions().height) || (square.y < 0)) {
            //if position y value exceeds width or less than 0
            return {};
        }
            //1b: handling case where it is unoccupied
        else if (board_[square] == Player::neither) {
            return {};
        }

        //Case 2: If position contains opposing player tile then add to
        // position set
        Player opposing_player = other_player(turn_);
        if (board_[square] == opposing_player) {
            result |= {square};
        }
        //Case 3: If position containing current player tile
        // then return result
        else if (board_[square] == turn_) {
            return result;
        }
    }
    return result; // CLion got angry at me for not returning anything
}

Position_set Model::evaluate_position_(Position pos) const
{
    // TODO
    Position_set result = {};
    return result;
}

//helper to check if the center is filled
bool Model::center_filled_()
{
    int i = 0;
    for (Position square: board_.center_positions()){
        if (board_[square] == Player::neither){
            ++i; //increment if not filled
        }
    }
    return (i == 0); //return true if all filled
}

void Model::compute_next_moves_()
{

    next_moves_.clear(); //clear next moves

    //Case 1: Not yet filled the center: Opening Phase
    if (!center_filled_()) {
        for (Position square: board_.center_positions()) {
            if (board_[square] == Player::neither) {
                next_moves_[square] = {square};
            }
        }
    }
    //Case 2: Center Filled: Main Phase
    else {
        // TODO
    }
}
}

bool Model::advance_turn_()
{
    turn_ = other_player(turn_);
    compute_next_moves_();
    return (!next_moves_.empty());
}

void Model::set_game_over_()
{
    turn_ = Player::neither;
    if (board_.count_player(Player::dark) > board_.count_player(Player::light)){
        winner_ = Player::dark;
    }
    else if (board_.count_player(Player::dark) == board_.count_player
            (Player::light)){
        winner_ = Player::neither;
    }
    else {
        winner_ = Player::light;
    }
}

void Model::really_play_move_(Move move)
{
    board_.set_all(move.second, turn());
    bool first_advance = advance_turn_();

    if (!first_advance){
        //if no move, try to advance again before setting over
        bool second_advance = advance_turn_();
        if(!second_advance){
            //if this one fails then set game over
            set_game_over_();
        }
    }
}


