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
#define STARTINGROOM 0
#define LOCKED -2

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
	bool _lit = false; //indicates if the object is lit
	int _weapon = 0; // value for weapon other than 0 indicates item is a weapon and its damage
	string _hiddenin = "";
public:
	Object(string description, string hiddendescription,
		bool liftable, bool lightable, bool visible, int weapon, string hiddenin);
	string description(void);
	string hiddendescription(void);
	bool liftable(void);
	bool lightable(void);
	bool visible(void);
	int weapon(void);
	void setVisible(bool visibility);
	bool lit(void);
	bool light(void);
	string hiddenin(void);
};
Object::Object(string description, string hiddendescription,
	bool liftable, bool lightable, bool visible, int weapon, string hiddenin = "")
{
	_description = description;
	_hiddendescription = hiddendescription;
	_liftable = liftable;
	_lightable = lightable;
	_visible = visible;
	_weapon = weapon;
	_hiddenin = hiddenin;
}
//----------------OBJECT GETTERS
string Object::description(void) { return _description; }
string Object::hiddendescription(void) { return _hiddendescription; }
bool Object::liftable(void) { return _liftable; }
bool Object::lightable(void) { return _lightable; }
bool Object::visible(void) { return _visible; }
int Object::weapon(void) { return _weapon; }
string Object::hiddenin(void) { return _hiddenin; };
bool Object::lit(void) { return _lit; };
bool Object::light(void)
{
	//returns true if object is lightable and has been lit	
	if (!_lightable)return false;
	_lit = true;
	return true;
}
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
	int go(void);
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
int Door::go(void)
{
	if (_locked)return LOCKED;
	return _leadsto;
}

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
	bool hasLight(void); //check to see if lit object in room
public:
	Room(string description, bool isdark);
	bool display(bool there_is_a_light); //will display both description and any objects in room 
	bool isDark(void);
	void addItem(Object NewObject);
	Object removeItem(string description); //REMOVES ITEM FROM ROOM AND RETURNS IT. A blank item is returned if not.
	bool search(string itemsearched);//search for itemsearched in room
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
bool Room::display(bool there_is_a_light)
{
	if (isdark && !there_is_a_light) { cout << "IT IS DARK! YOU SEE NOTHING!" << endl << endl; return false; }

	cout << description << endl;

	cout << endl << "Room contains:" << endl;

	if (Inventory.size() == 0) { cout << "Nothing!" << endl << endl; } // Nothing in the room
	else
	{
		for (int inventorycount = 0; inventorycount < int(Inventory.size()); inventorycount++) //got through inventory
		{
			if (Inventory[inventorycount].visible())
			{
				cout << Inventory[inventorycount].description() << endl;
			}
		}
	}
	cout << endl;
	return true;
}
bool Room::isDark(void)
{
	if (!isdark)return isdark; //room is light
	return !hasLight();//return false if lit object is present		
}
bool Room::hasLight(void)
{
	for (int inventorycount = 0; inventorycount < int(Inventory.size()); inventorycount++)//search through inventory
	{
		if (Inventory[inventorycount].lit()) return true;
		// there is a lit object in the room	
	}
	return false;
}
bool Room::search(string itemsearched)
{
	//this method attempts to search a specified item in the current room and not the entire room 

	bool founditemsearched = false; //rrecords if item being searched is in the room	
	bool itemsearchednotvisible = false; //records if player is trying to search an item that isn't visible

	cout << "YOU ATTEMPT TO SEARCH THE " << itemsearched << endl << endl;
	for (int inventorycount = 0; inventorycount < int(Inventory.size()); inventorycount++)//search through inventory
	{
		if (Inventory[inventorycount].hiddenin() == itemsearched)
		{
			Inventory[inventorycount].setVisible(true);
			cout << "YOU DISCOVER A " << Inventory[inventorycount].description() << "!" << endl << endl;
			return true;
		}

		if (Inventory[inventorycount].description() == itemsearched)
		{
			founditemsearched = true; //item searched is in room
			if (!Inventory[inventorycount].visible()) itemsearchednotvisible = true; //but it is not visible
		}
	}

	if (!founditemsearched || itemsearchednotvisible) cout << "YOU CANNOT FIND A " << itemsearched << " ANYWHERE!";//item being searched wasn't in the room	
	else cout << "YOU FIND NOTHING";

	cout << endl << endl;
	return false;
}
Object Room::removeItem(string description)
{
	Object BlankObject("", "", false, false, false, 0, "");

	for (int inventorycount = 0; inventorycount < int(Inventory.size()); inventorycount++)
	{
		if (Inventory[inventorycount].description() == description)
		{
			if (!Inventory[inventorycount].visible())break; //object not visible
			if (!Inventory[inventorycount].liftable())
			{
				cout << "YOU CANNOT LIFT THE " << description << ": IT IS TOO HEAVY!" << endl << endl;
				return BlankObject;
			}
			//object fount - remove from inventory 
			BlankObject = Inventory[inventorycount];
			Inventory.erase(Inventory.begin() + inventorycount);
			break;
		}
	}
	if (BlankObject.description() == "")cout << "THERE IS NO " << description << " HERE!";
	else cout << "YOU REMOVE THE " << description << " FROM THE ROOM!";
	
	cout << endl << endl;

	return BlankObject;
}

class Maze
{
private:
	vector<Room> Contents;
	vector<Connection> Connections;
	int currentroom = STARTINGROOM;
public:
	void displayAllRooms(void); //for testing purposes only
	void displayAllConnections(void); //for testing purposes only
	bool loadDescriptionsFromFile(void); //load in the room descriptions
	bool loadConnectionsFromFile(void); //load room connections
	bool loadObjectsFromFile(void); //load objects
	void goDirection(string direction); //go in indicated direction from current room
	void addItem(Object NewObject);//add item to inventory of current room
	Object removeItem(string description); //removes item from room and returns it. A blank item is returned if not
	void look(bool there_is_light);//look at current room 
	void unlockDoor(string direction, string key); //unlock indicated door in current room 
	bool search(string itemsearched); //search indicated item in current room
};
void Maze::goDirection(string direction)
{
	//this method sets the current room to the new room indicated by the supplied direction 
	//if there is no door available or the door is locked, the current room doesn't change
	int newroom;
	cout << "YOU ATTEMPT TO GO" << direction << endl << endl;

	for (int directioncount = 0; directioncount < int(directions.size()); directioncount++)
	{
		if (directions[directioncount] == direction)
		{
			newroom = Connections[currentroom].Direction[directioncount].go(); ////////////////////////TBADDED
			if (newroom == LOCKED)
			{
				cout << "The " << direction << " DOOR IS LOCKED! ";
				newroom = NODOOR; //Nodoor because door is locked 
			}
			break;
		}
	}

	if (newroom == NODOOR)cout << "YOU CANNOT GO IN THAT DIRECTION!" << endl;
	else
	{
		currentroom = newroom;
		cout << "You Go " << direction;
	}
	cout << endl << endl;
}
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
	int weapon = 0, roomnumber = 0;

	inputfile.open(ROOMOBJECTS); //*****OOOOO

	if (!inputfile)
	{
		cout << "Object file does not exist" << endl;
		return false;
	}

	while (inputfile >> roomnumber)
	{
		if (roomnumber < 0 || roomnumber >= int(Contents.size()))
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

	}
	inputfile.close(); //****CCCCC
	return true;
}
void Maze::displayAllConnections(void)
{
	for (int roomcount = 0; roomcount < int(Connections.size()); roomcount++)
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
void Maze::addItem(Object NewObject)
{
	Contents[currentroom].addItem(NewObject);
}
Object Maze::removeItem(string desription)
{
	return Contents[currentroom].removeItem(desription);
}
void Maze::look(bool there_is_a_light) //EDITING NEED DOING	
{
	bool exit = false, looksuccess = false; //records if there is an exit avaiable 
	looksuccess = Contents[currentroom].display(there_is_a_light); /////??????????????????????????????
	if (looksuccess)
	{
		cout << "YOU CAN GO:" << endl << endl;
		for (int directioncount = 0; directioncount < int(directions.size()); directioncount++)
		{
			if (Connections[currentroom].Direction[directioncount].go() != NODOOR)
			{
				cout << directions[directioncount] << " ";
				exit = true;
			}
		}
		if (!exit) cout << "NO WHERE";
		cout << endl << endl;
	}

}
void Maze::unlockDoor(string direction, string key)
{
	cout << "YOU ATTEMPT TO UNLOCK THE " << direction << " DOOR WITH THE " << key << endl << endl;

	for (int directioncount = 0; directioncount < int(directions.size()); directioncount++)
	{
		if (directions[directioncount] == direction)
		{
			Connections[currentroom].Direction[directioncount].unlockDoor(key);
			break;
		}
	}
}
bool Maze::search(string itemsearched) //More to be added elsewhere
{
	return Contents[currentroom].search(itemsearched);
}
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
	MyMaze.loadConnectionsFromFile();
	MyMaze.loadObjectsFromFile();
	MyMaze.displayAllRooms();
	MyMaze.displayAllConnections();

	return 0;
}