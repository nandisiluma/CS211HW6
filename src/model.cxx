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
    //loop through start + dir, start + 2*dir...

    //if we reach position that goes off the board or is unoccupied, result is
    //empty

    //if position contains opposing player tile then add to position set

    //if we reach a position containing the current player's tile then
    // return the result

}

Position_set Model::evaluate_position_(Position pos) const
{

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


