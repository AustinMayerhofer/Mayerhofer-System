#include "Team.h"
#include "Game.h"
#include <iomanip>
#include <math.h>
#include <queue>

Team::Team(std::string team_name, char conf_type) : name(team_name), conf(conf_type), wins(0), losses(0), ranking_points(0) {

}

void Team::play_game(Game* game) {
	games.push_back(game);

	if (game->get_winner()->get_name() == name) {
		wins++;
	}
	else {
		losses++;
	}
}

std::string Team::get_name() {
	return name;
}

char Team::get_conf_type() {
	return conf;
}

int Team::get_num_wins() {
	return wins;
}

int Team::get_num_losses() {
	return losses;
}

double Team::get_win_percentage() {
	double win_pct = static_cast<double>(wins) / (wins + losses);
	return win_pct;
}

double Team::calculate_ranking_points() {
	double ranking_points = 0;
	std::priority_queue<int> best_wins;
	int num_priority_games = 3; // changeable: number of "big win" boosts a team can get
	double difference_in_importance_first_vs_last_game = 0.1; // changeable: percentage difference in importance between first and last game
	// Add points for games
	/*
	for (Game* game_ptr : games) {
		ranking_points += ranking_points_for_game(game_ptr);
	}
	*/
	double game_multiplier = 1;
	for (int i = games.size() - 1; i >= 0; i--) {
		ranking_points += game_multiplier * ranking_points_for_game(games[i]);
		if (games.size() != 1) { game_multiplier -= difference_in_importance_first_vs_last_game / (games.size() - 1.0); }
		
	}
	// Add points for top 3 wins (without scoring margin)
	for (Game* game_ptr : games) {
		if (game_ptr->get_winner()->get_name() == name) {
			int points_from_game = convert_win_pct_to_ranking_points_for_win(game_ptr->get_loser()->get_win_percentage(), game_ptr->get_loser()->get_conf_type());
			best_wins.push(points_from_game);
		}
	}
	for (int i = 0; i < num_priority_games && !best_wins.empty(); i++) {
		ranking_points += best_wins.top();
		best_wins.pop();
	}

	//ranking_points = ranking_points;  // does not account for number of games played
	/*
	ranking_points = ranking_points / games.size(); // accounts for number of games played
	ranking_points /= 10;  // just to look nice when adjusting for games played
	*/
	
	//ranking_points = ranking_points / get_num_wins(); // accounts for number of wins

	ranking_points = ranking_points / log2(games.size() + 2); // accounts for log(games played)
	
	return ranking_points;
}

void Team::print() {
	std::cout << name << ": " << wins << "-" << losses << " (" << calculate_ranking_points() << ")";
}

double Team::ranking_points_for_game(Game* game) {
	double r_points = 0;
	if (game->get_winner()->get_name() == name) {  // if team is the winner
		r_points += static_cast<double>(convert_win_pct_to_ranking_points_for_win(game->get_loser()->get_win_percentage(), game->get_loser()->get_conf_type()));
	}
	else {  // if team is the loser
		r_points += static_cast<double>(convert_win_pct_to_ranking_points_for_loss(game->get_winner()->get_win_percentage(), game->get_winner()->get_conf_type()));
	}

	r_points += add_rank_points_for_score_margin(r_points, game->get_winner_score(), game->get_loser_score());
	
	return r_points;


}

int Team::convert_win_pct_to_ranking_points_for_win(double win_pct, char opp_conf_type) {
	if ((opp_conf_type == 'F') || 
		(opp_conf_type == 'G' && win_pct <= .4)) {
		return 2;
	}
	else if ((opp_conf_type == 'G' && win_pct > .4 && win_pct <= .6) || 
		(opp_conf_type == 'P' && win_pct < .4)) {
		return 3;
	}
	else if (opp_conf_type == 'P' && win_pct >= .4 && win_pct < .5) {
		return 4;
	}
	else if ((opp_conf_type == 'G' && win_pct > .6 && win_pct <= .75) || 
		(opp_conf_type == 'P' && win_pct >= .5 && win_pct < .6)) {
		return 5;
	}
	else if ((opp_conf_type == 'G' && win_pct > .75 && win_pct < .834) ||
		(opp_conf_type == 'P' && win_pct >= .6 && win_pct < .667)) {
		return 6;
	}
	else if ((opp_conf_type == 'G' && win_pct >= .834 && win_pct <= .916) || 
		(opp_conf_type == 'P' && win_pct >= .667 && win_pct <= .75)) {
		return 7;
	}
	else if ((opp_conf_type == 'G' && win_pct > .916) || 
		(opp_conf_type == 'P' && win_pct > .75 && win_pct < .917)) {
		return 8;
	}
	else if (opp_conf_type == 'P' && win_pct >= .917) {
		return 9;
	}
	else {
		std::cerr << "Error: in convert_win_pct_to_ranking_points_for_win in Team.cpp, function did not return properly" << std::endl;
		exit(1);
	}
}

int Team::convert_win_pct_to_ranking_points_for_loss(double win_pct, char opp_conf_type) {
	return convert_win_pct_to_ranking_points_for_win(win_pct, opp_conf_type) - 11;
}

double Team::add_rank_points_for_score_margin(double r_points, int winner_score, int loser_score) {
	//r_points += r_points * static_cast<double>(winner_score - loser_score) / 30.0; // linear function
	r_points += r_points * log2((winner_score - loser_score + 30.0) / 30.0); // logarithmic function
	return r_points;
}
