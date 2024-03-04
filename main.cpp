#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;


class game_play;
class controller;
class game_menu;


void clearScreen()
{
    system("cls");
}

struct point
{
    int i;
    int j;

    point(int a,int b): i(a) , j(b) {};
};


bool operator==(const point& lhs,const point& rhs)
{
    return lhs.i == rhs.i && lhs.j == rhs.j;
}


enum class scene
{
    menuScene,
    playScene,
    overScene
};


class game_play
{
private:
    string map_name = "";
    vector<vector<char>> game_view;
    point player;

    int height = 0;
    int width = 0;

public:
    game_play(string file_name): map_name(file_name) , player(0,0){};//{} is needed too.
    ~game_play(){}; //{} is needed.for excecution.(NOT only declare)


    bool load_maze()
    {
        map_name += ".txt";

        ifstream maze(map_name);

        if(!maze.is_open())
        {
            cerr << "FAIL TO OPEN" << endl;
            return false;
        }

        string line = "";

        while(getline(maze,line))
        {
            vector<char> row(line.begin(),line.end());
            game_view.push_back(row);

            width = max(width,static_cast<int>(row.size()));
            height++;
        }

        maze.close();

        for(int i = 0;i < height;i++)
        {
            for(int j = 0;j < width;j++)
            {
                if(game_view[i][j] == 'p')
                {
                    player.i = i;
                    player.j = j;

                    return true;
                }
            }
        }

        return false;
    }


    void print_maze()
    {
        clearScreen();

        for(int i = 0;i < height;i++)
        {
            for(int j = 0;j < width;j++)
            {
                if(game_view[i][j] == '#')
                {
                    cout << "#";
                }
                else if(game_view[i][j] == '*')
                {
                    cout << " ";
                }
                else if(game_view[i][j] == 'p')
                {
                    cout << "&";
                }
                else if(game_view[i][j] == '!')
                {
                    cout << "!"; 
                }
            }
            cout << endl;
        }
    }


    point get_player()
    {
        return player;
    }


    void set_player(point pl_pos)//player position
    {
        game_view[player.i][player.j] = '*';//remove old cursor
        player = pl_pos;
        game_view[player.i][player.j] = 'p';//set new cursor
    }


    char collision(point obj_pos_ad)//object position, advanced
    {
        if(game_view[obj_pos_ad.i][obj_pos_ad.j] == '#')
        {
            return '#';
        }

        return ' ';
    }


    int play_now();
};


class controller
{
private:
    point player_now;
    point player_advance;
    game_play* maze_state;

public:
    controller(game_play* maze):maze_state(maze),player_now(0,0),player_advance(0,0){};
    ~controller(){};

    void move()
    {
        player_now = maze_state->get_player();
        player_advance = player_now;

        string command = "";

        cin >> command;

        int n = static_cast<int>(command.size());

        for(int i = 0;i < n;i++)
        {
            if(command[i] == 'a')
            {
                --player_advance.j;
            }
            else if(command[i] == 's')
            {
                ++player_advance.i;
            }
            else if(command[i] == 'd')
            {
                ++player_advance.j;
            }
            else if(command[i] == 'w')
            {
                --player_advance.i;
            }
            
            if(!(player_now == player_advance))//advanceing success!
            {
                if(maze_state->collision(player_advance) == ' ')
                {
                    player_now = player_advance;//collision check success!
                }
                else if(maze_state->collision(player_advance) == '#')
                {
                    player_advance = player_now;
                }
            }
        }

        maze_state->set_player(player_now);
    }
};


int game_play::play_now()
    {
        game_play maze1(map_name);
        controller contr(&maze1);

        if(!maze1.load_maze())
        {
            cerr << "Load Error!" << endl;
            return -1;
        };

        while(true)
        {
            maze1.print_maze();

            contr.move();
        }
    }


class game_menu
{
private:
    string maze_name;
    string answer;
public:
    game_menu():maze_name(""),answer(""){};
    ~game_menu(){};

    void main_menu(scene& current)
    {
        while(true)
        {
            clearScreen();

            cout << "Main menu. Please input Maze name here. :" << endl;
            cin >> maze_name;
            
            cout << "You want to play " << maze_name << "? (y/n)" << endl;
            cin >> answer;

            if(answer == "y")
            {
                current = scene::playScene;
                break;
            }
        }
    }

    string get_maze_name()
    {
        return maze_name;
    }
};



int main()
{
    scene current = scene::menuScene;

    game_menu menu;

    while(true)
    {
        switch(current)
        {
            case scene::menuScene:
                menu.main_menu(current);
                break;
            case scene::playScene:
                game_play play(menu.get_maze_name());

                play.play_now();

                break;
        }
    }


    return 0;
}


/*
        size_t dot = map_name.find_last_of(".");

        if(dot != string::npos)
        {
            if(map_name.substr(dot + 1) != "txt")
            {
                cerr << "TEXT FILE PLEASE!" << endl;
                return false;
            }
        }
        else if(dot == string::npos)
        {
            cerr << "FILE PLEASE!" << endl;
            return false;
        }
*/