#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

#define MAZEDESCRIPTIONS "maze_descriptions.txt"

class Room
{
private:
	string description = "";
	bool isdark = false;
public:
	Room(string description,bool isdark);
	void display(bool there_is_a_light); //will display both description and any objects in room 
	bool isDark(void);
};
Room::Room(string description, bool isdark) //Constructor for Room
{
	this->description = description;
	this->isdark = isdark;
}
void Room::display(bool there_is_a_light)
{
	if (isdark && !there_is_a_light) cout << "IT IS DARK! YOU SEE NOTHING!" << endl;
	else cout << description << endl;
}
bool Room::isDark(void)
{
	return isdark;
}

class Maze
{
private:
	vector<Room> Contents;
public:
	void displayAllRooms(void); //for testing purposes only
	bool loadDescriptionsFromFile(void); //load in the room descriptions
};

bool Maze::loadDescriptionsFromFile(void)
{
	ifstream inputfile;
	int roomcount = 0;
	string roomdetails;
	string isdark;

	inputfile.open(MAZEDESCRIPTIONS);

	if (!inputfile)
	{
		cout << "Maze description file does not exist" << endl;
		return false;
	}
	Contents.clear(); //clear out any old data

	while (getline(inputfile, roomdetails))
	{
		getline(inputfile, isdark);
		if (isdark == "TRUE") { Contents.push_back(Room(roomdetails, true)); }
		else Contents.push_back(Room(roomdetails, false));

		roomcount++;
	}

	inputfile.close();

	return true;
}
void Maze::displayAllRooms(void)//Testing purposes
{
	for (int roomcount = 0; roomcount < int(Contents.size()); roomcount++){ Contents[roomcount].display(true); }
}
class Adventurer
{
private:

public:
};

class AdventureGame
{
private:
	Adventurer CurrentPlayer;
	Maze CurrentMaze;
public:
};
int main(void)
{
	Maze MyMaze;
	MyMaze.loadDescriptionsFromFile();
	MyMaze.displayAllRooms();
	return 0;
}