#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

#define MAZEDESCRIPTIONS "maze_descriptions.txt"
#define MAZECONNECTIONS "maze_connections.txt"
#define NODOOR -1
#define NUMBEROFDOORS 5
#define ROOMOBJECTS "room_objects.txt"

enum direction { NORTH, SOUTH, EAST, WEST, OUT };
const vector<string> directions = { "NORTH", "SOUTH", "EAST", "WEST", "OUT" };
const vector<string> shorthandDirections = { "N", "S", "E", "W", "O" };

class Object
{
private:
	string _description = "";
	string _hiddendescription = "";
	bool _liftable = false; // true = can be picked up
	bool _lightable = false; // true = can be lit
	bool _visible = false; // true can be seen 
	int _weapon = 0; // value for weapon other than 0 indicates item is a weapon and its damage
public:
	Object(string description, string hiddendescription,
		bool liftable, bool lightable, bool visible, int weapon);
	string description(void);
	string hiddendescription(void);
	bool liftable(void);
	bool lightable(void);
	bool visible(void);
	int weapon(void);
	void setVisible(bool visibility);
};
Object::Object(string description, string hiddendescription,
	bool liftable, bool lightable, bool visible, int weapon)
{
	_description = description;
	_hiddendescription = hiddendescription;
	_liftable = liftable;
	_lightable = lightable;
	_visible = visible;
	_weapon = weapon;
}

//----------------OBJECT GETTERS
string Object::description(void) { return _description; }
string Object::hiddendescription(void) { return _hiddendescription; }
bool Object::liftable(void) { return _liftable; }
bool Object::lightable(void) { return _lightable; }
bool Object::visible(void) { return _visible; }
int Object::weapon(void) { return _weapon; }
//---------------------
//---------------------OBJECT SETTERS
void Object::setVisible(bool visibility)
{
	_visible = visibility;
}
//-----------------------

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
class Connection
{
public:
	vector<Door> Direction;
	//least knowledge does not apply because connection only being used to organise 2d vector 
};
class Room
{
private:
	string description = "";
	bool isdark = false;
	vector<Object> Inventory;
public:
	Room(string description, bool isdark);
	void display(bool there_is_a_light); //will display both description and any objects in room 
	bool isDark(void);
	void addItem(Object NewObject);
	Object removeItem(string description); //REMOVES ITEM FROM ROOM AND RETURNS IT. A blank item is returned if not.
};
Room::Room(string description, bool isdark) //Constructor for Room
{
	this->description = description;
	this->isdark = isdark;
}
void Room::addItem(Object NewObject)
{
	Inventory.push_back(NewObject);
}
void Room::display(bool there_is_a_light)
{
	if (isdark && !there_is_a_light) { cout << "IT IS DARK! YOU SEE NOTHING!" << endl << endl; return; }

	cout << description << endl;

	cout << endl << "Room contains:" << endl;

	if (Inventory.size() == 0) { cout << "Nothing!" << endl << endl; }
	else
	{
		for (int inventorycount = 0; inventorycount < int(Inventory.size()-1); inventorycount++)
		{
			if (Inventory[inventorycount].visible())
			{
				cout << Inventory[inventorycount].description() << endl;
			}
		}
	}
	cout << endl;
}
bool Room::isDark(void)
{
	return isdark;
}
Object Room::removeItem(string description)
{
	Object BlankObject("", "", false, false, false, 0);

	for (int inventorycount = 0; inventorycount < int(Inventory.size()-1); inventorycount++)
	{
		if (Inventory[inventorycount].description() == description)
		{
			BlankObject = Inventory[inventorycount];
			Inventory.erase(Inventory.begin() + inventorycount);
			break;
		}
	}
	return BlankObject;
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
	bool loadObjectsFromFile(void); //load objects
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
bool Maze::loadObjectsFromFile(void)
{
	ifstream inputfile;
	string text, metacharacter, description = "", hiddendescription = "";
	bool liftable = false, lightable = false, visible = false;
	int weapon = 0, roomnumber =0;

	inputfile.open(ROOMOBJECTS); //*****OOOOO

	if (!inputfile)
	{
		cout << "Object file does not exist" << endl;
		return false;
	}

	while (inputfile >> roomnumber)
	{
		if (roomnumber < 0 || roomnumber > int(Contents.size())-1)
		{
			cout << "Illegal room number in object file" << endl;
			return false;
		}
		getline(inputfile, metacharacter); //LF
		getline(inputfile, description);
		getline(inputfile, hiddendescription);

		inputfile >> text;
		if (text == "TRUE") liftable = true;
		else liftable = false;

		inputfile >> text;
		if (text == "TRUE") lightable = true;
		else lightable = false;

		inputfile >> text;
		if (text == "TRUE") visible = true;
		else visible = false;

		inputfile >> weapon;

		Contents[roomnumber].addItem(Object(description, hiddendescription, liftable, lightable, visible, weapon));

		inputfile.close(); //****CCCCC

	}
}
void Maze::displayAllConnections(void)
{
	for (int roomcount = 0; roomcount < int(Connections.size())-1; roomcount++)
	{
		cout << "Room number " << roomcount << " is connected to rooms:" << endl;
		for (int direction = 0; direction < NUMBEROFDOORS; direction++)
		{
			cout << directions[direction] << ": " << Connections[roomcount].Direction[direction].leadsto() << endl;
		}
	}
}
void Maze::displayAllRooms(void)//Testing purposes
{
	for (int roomcount = 0; roomcount < int(Contents.size()); roomcount++) { Contents[roomcount].display(true); }
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
		if (direction == 0) { Connections.push_back(Connection()); }//new connection to record
		inputfile >> locked;
		inputfile.ignore(); //clear metacharacter
		getline(inputfile, keyrequired);//could be 
		if (locked == "TRUE") { Connections[roomnumber].Direction.push_back(Door(leadsto, true, keyrequired)); }
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

int main(void)
{
	Maze MyMaze;

	MyMaze.loadObjectsFromFile();
	MyMaze.loadDescriptionsFromFile();
	MyMaze.loadConnectionsFromFile();
	MyMaze.displayAllRooms();
	MyMaze.displayAllConnections();
	

	return 0;
}