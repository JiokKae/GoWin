#pragma once
#include "framework.h"

class GiboSGF {
	string m_aplication;
	int m_board_size = 19;
	string m_game_name;
	string m_game_comment;
	string m_event;
	string m_round;
	string m_date;
	string m_place;
	string m_black_player;
	string m_black_lank;
	string m_black_team;
	string m_white_player;
	string m_white_rank;
	string m_white_team;
	float m_komi = 6.5f;
	string m_handicap;
	string m_result;
	string m_user;
	string m_source;
	string m_copyright;
	string m_annotation;

	string* m_placement = NULL;
public:
	GiboSGF() {};
	//GiboSGF(char* address) { loadGibo(address); };
	~GiboSGF() { delete[] m_placement; };

	/*int sequence() const { return m_placement; }

	bool set_line_num(string lineNum);
	bool set_go_type(string goType);
	bool set_gongje(string gongje);
	bool set_compensation(string compensation);
	bool set_sequence(string sequence);

	bool loadGibo(char* address) {
		ifstream gibofile(address);
		if (!gibofile)
			return false;
		string temp;
		getline(gibofile, battle_type_);

		getline(gibofile, temp);
		if (!set_line_num(temp)) return false;

		getline(gibofile, temp);
		if (!white.setPlayer(temp)) return false;

		getline(gibofile, temp);
		if (!black.setPlayer(temp)) return false;

		getline(gibofile, link_);

		getline(gibofile, temp);
		if (!set_go_type(temp)) return false;

		getline(gibofile, temp);
		if (!set_gongje(temp)) return false;

		getline(gibofile, temp);
		if (!set_compensation(temp)) return false;

		getline(gibofile, m_date);

		getline(gibofile, base_time_);

		getline(gibofile, game_result_);

		getline(gibofile, temp);
		if (!set_sequence(temp))
			return false;
		if (placement_ != nullptr)
			delete[] placement_;
		placement_ = new string[m_sequence];
		for (int i = 0; i < m_sequence; i++)
			getline(gibofile, placement_[i]);

		gibofile.close();

		return true;
	}

	bool SaveGibo(char* address) {
		ofstream gibofile(address);
		if (!gibofile.is_open())
			return false;
		string temp;
		/*
		getline(gibo, battle_type_);

		getline(gibo, temp);
		if (!set_line_num(temp)) return false;

		getline(gibo, temp);
		if (!white.setPlayer(temp)) return false;

		getline(gibo, temp);
		if (!black.setPlayer(temp)) return false;

		getline(gibo, link_);

		getline(gibo, temp);
		if (!set_go_type(temp)) return false;

		getline(gibo, temp);
		if (!set_gongje(temp)) return false;

		getline(gibo, temp);
		if (!set_compensation(temp)) return false;

		getline(gibo, m_date);

		getline(gibo, base_time_);

		getline(gibo, game_result_);

		getline(gibo, temp);
		if (!set_sequence(temp))
		return false;
		if (placement_ != nullptr)
		delete[] placement_;
		placement_ = new string[sequence_];
		for (int i = 0; i < sequence_; i++)
		getline(gibo, placement_[i]);
		
		gibofile.close();
		return true;
		
	}
	

	string getPlacement(int sequence) { return placement_[sequence].substr(7, 2); }
	*/
};
