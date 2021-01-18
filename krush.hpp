#define WINWIDTH 800
#define WINHEIGHT 800
#define en_width 60
#define en_height 60
#define tower_size 85

#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<vector>
#include<cmath>
#include"rsdl.hpp"

using namespace std;

void get_level_data(string& mapdir,string& spts,string& path,string& spt,string& defpt,string& wavdir);

void get_waves_data(string& wavdir, vector<int>& arrival_sec,vector<string>& enemy_type, vector<bool>& enemy_active);

void initialize_towers(vector<int>& tower_x,vector<int>& tower_y,vector<int>& tower_type,string& spt);

string catch_enemy_x_y_dir(int& e_x, int& e_y, const int frame, const int k ,int arrival_sec, const string& enemy_type , string& path);

void update_enemies(Window& win,string& mapdir,string& path,string& defpt, vector<bool>& enemy_active, vector<string>& enemy_type,vector<int>& arrival_sec, int& frame, int& enemy_i, int& life, int& coins);

void take_lives(int& life,string enemy_type);

int spt_i(string& spt, string& spts,int m_x,int m_y );

void set_tower(vector<int>& tower_type,int tower_i ,char p_key ,int& coins );

void update_towers(Window& win,vector<int>& tower_x,vector<int>& tower_y,vector<int>& tower_type);

void print_tower(Window& win,int tower_x,int tower_y,int tower_type,int spt_width, int spt_height);
