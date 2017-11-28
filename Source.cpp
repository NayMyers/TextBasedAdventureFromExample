#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

#define MAZEDESCRIPTIONS "maze_descriptions.txt"
#define MAZECONNECTIONS "maze_connections.txt"
#define NODOOR -1
#define NUMBEROFDOORS 5

enum direction {NORTH,SOUTH,EAST,WEST,OUT};
const vector<string> directions = { "NORTH", "SOUTH", "EAST", "WEST", "OUT" };
const vector<string> shorthandDirections = { "N", "S", "E", "W", "O" };

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
	vector<Connection> Connections;
public:
	void displayAllRooms(void); //for testing purposes only
	void displayAllConnections(void); //for testing purposes only
	bool loadDescriptionsFromFile(void); //load in the room descriptions
	bool loadConnectionsFromFile(void); //load room connections
};

bool Maze::loadDescriptionsFromFile(void)
{
	ifstream inputfile;
	int roomcount = 0;
	string roomdetails;
	string isdark;

	inputfile.open(MAZEDESCRIPTIONS);//*****OOOOO

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

	inputfile.close();//*****CCCCC

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
bool Maze::loadConnectionsFromFile(void)
{
	ifstream inputfile;
	int leadsto = NODOOR;
	string locked, keyrequired;
	int roomnumber = 0;
	int direction = 0;
	string metacharacter;

	inputfile.open(MAZECONNECTIONS); //*****OOOOO

	if (!inputfile)
	{
		cout << "Maze connections file does not exist" << endl;
		return false;
	}
	Connections.clear(); //clear out old data 

	while (inputfile >> leadsto)
	{
		if (direction == 0){ Connections.push_back(Connection()); }//new connection to record
		inputfile >> locked;
		inputfile.ignore(); //clear metacharacter
		getline(inputfile, keyrequired);//could be 
		if (locked == "TRUE"){ Connections[roomnumber].Direction.push_back(Door(leadsto, true, keyrequired)); }
		else { Connections[roomnumber].Direction.push_back(Door(leadsto, false, keyrequired)); }
		direction++;
		if (direction >= NUMBEROFDOORS)
		{
			roomnumber++;
			direction = 0;
		}
	}
	
	inputfile.close(); //*****CCCCC
	return true;
}

class Door
{
private:
	int _leadsto = NODOOR; //an int value indicating room number door leads to 
	bool _locked = false; //indicates whether door is locked 
	string _keyrequired = ""; //name of key required to open door if locked 
public:
	Door(int leadsto, bool locked, string keyrequired);
	int leadsto(void);
	bool locked(void);
	void unlockDoor(string key);
};
Door::Door(int leadsto, bool locked, string keyrequired)
{
	_leadsto = leadsto;
	_locked = locked;
	_keyrequired = keyrequired;
}

int Door::leadsto(void)
{
	return _leadsto;
}
bool Door::locked(void)
{
	return _locked;
}
void Door::unlockDoor(string key)
{
	if (!_locked)
	{
		cout << "Door is not locked!" << endl;
		return;
	}
	if (key == _keyrequired)
	{
		cout << "You have the right key! The door unlocks" << endl;
		_locked = false;
		return;
	}

	cout << "you do not have the right key! The door remains locked!" << endl;
}
class Connection
{
public:
	vector<Door> Direction;
	//least knowledge does not apply because connection only being used to organise 2d vector 
};

int main(void)
{
	Maze MyMaze;
	MyMaze.loadDescriptionsFromFile();
	MyMaze.displayAllRooms();
	return 0;
}