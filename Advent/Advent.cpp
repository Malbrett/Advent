#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include <Windows.h>

using namespace std;
void spacing(int lengthy)
{
	switch (lengthy) {
	default:
	case 0:
		cout << endl;
		Sleep(100);
		break;
	case 1:
		cout << "---" << endl;
		Sleep(500);
		break;
	case 2:
		cout << endl << "---" << endl << endl;
		Sleep(1000);
		break;
	}
}
void typeText( const char * p) 
{
	if (NULL == p) {
		return;
	}
	while (*p) {
		printf("%c", *p++);
		Sleep(30);
	}
	Sleep(300);
}
void death() 
{

}

//Global Variables
int incorrectAction = 1;
string enemyNames[5] = { "door", "goblin", "skeleton", "ogre", "human" };
int enemyPatterns[5][6] = { { 100, 101, 101, 101, 101, 101}, { 0, 5, 20, 60, 75, 100 }, { 0, 15, 25, 45, 75, 100 }, { 0, 40, 55, 65, 95, 100 }, { 0, 20, 40, 60, 80, 100 } };
string weaponNames[5] = { "fists", "shiv" };
int weaponStats[5][5] = {{ 10, 0, 0 }, { 15, 0, 0 }};

//Action Keywords
string actionReq;
const int numberOfActions = 13;
string possibleActions[numberOfActions] = { "help", "inspect", "take", "use", "open", "move", "talk", "attack", "stats", "inventory", "equip", "quit", "mega" };
string DEVcomm[1] = { "var" };
string combatActions[8] = { "nothing", "heavy", "parry", "quick", "feint", "dodge", "random", "retreat" };

//Classes
class Character {			//This is used to keep track of all stats and conditions of characters,
public:						//player or otherwise.
	int type = 0;
	bool aggro = false;
	bool dead = false;
	int attackChoice = 0;
	string name;
	int level = 1;
	int exp = 0;
	int expCap = 100;
	int health = 100;
	int healthMax = 100;
	int perkPoints = 0;

	string currentWeapon = "fists";
	int damage = 10;
	//int critChance = 0;
	//int critDamage = 200;

	string currentArmor = "rags";
	float defense = 0;

	string inventory[10];
	string invDesc[10][5];
	int itemUnlock[10][5] = { 0 };

	void displayStats() {
		if (type == -1) {
			cout << "Name: " << name << endl;
			cout << "Level " << level << endl;
			cout << exp << "/" << expCap << " EXP" << endl;
			cout << health << "/" << healthMax << " Health" << endl;
			spacing(0);
			cout << "Wielding " << currentWeapon << endl;
			cout << damage << " damage" << endl;
			//cout << critChance << "% chance to deal " << critDamage << "% damage" << endl;
			spacing(0);
			cout << "Wearing " << currentArmor << endl;
			cout << (defense * 100) << "% damage resist" << endl;
		}
		else {
			cout << "Enemy name: " << name << endl;
			cout << "Level " << level << endl;
			cout << health << "/" << healthMax << " Health" << endl;
			spacing(0);
			cout << "Wielding " << currentWeapon << endl;
			cout << damage << " damage" << endl;
			spacing(0);
			cout << "Wearing " << currentArmor << endl;
			cout << (defense * 100) << "% damage resist" << endl;
		}
	}

	void displayInventory() {
		for (int i = 0; i < 10; i++) {
			if (inventory[i].length() > 0) {
				cout << " - " << inventory[i] << endl;
			}
			else if (i == 0) {
				cout << "Empty." << endl;
				return;
			}
		}
	}

	void equip() {
		for (int ix = 0; ix < 10; ix++) {
			if (actionReq.find(inventory[ix]) != string::npos && inventory[ix].length() > 0) {
				for (int iy = 0; iy < 5; iy++) {
					if (weaponNames[iy].find(inventory[ix]) != string::npos && weaponNames[iy].length() > 0) {
						currentWeapon = weaponNames[iy];
						damage = weaponStats[iy][0];
						//critChance = weaponStats[iy][1];
						//critDamage = weaponStats[iy][2];
						cout << weaponNames[iy] << " has been equipped" << endl;
						return;
					}
				}
			}
		}
		cout << "That is not a valid command" << endl;
	}
	
	void gainExp(int quantity) {
		bool increasedLevel = false;
		spacing(1);
		cout << "You gained " << quantity << " EXP" << endl;
		spacing(1);
		exp = exp + quantity;
		levelCheck:
		if (exp >= expCap) {
			level++;
			perkPoints++;
			healthMax = healthMax + (10 * level);
			health = healthMax;
			expCap = 2 * expCap + (10 * 1 ^ level);
			increasedLevel = true;
			goto levelCheck;
		}
		if (increasedLevel) {
			cout << "You increased to Level " << level << endl;
		}
	}

	void takeDamage(int quantity) {
		int healthBeforeDamage = health;
		health = health - (quantity * (1 - defense));
		if (type == -1) {
			cout << "You took " << (healthBeforeDamage - health) << " damage." << endl;
		}
		else {
			cout << "The " << name << " took " << (healthBeforeDamage - health) << " damage." << endl;
		}
		if (health <= 0) {
			health = 0;
			if (type == -1) {
				dead = true;
				cout << "You have died." << endl;
				death();
			}
			else {
				dead = true;
				aggro = false;
				cout << "The " << name << " died." << endl;
				return;
			}
		}
	}

	void determineAttack() {
		int percentage = rand() % 100 + 1;
		if (type == -1) {
			for (int i = 0; i < 6; i++) {
				if (percentage <= enemyPatterns[4][i]) {		//Player attack patterns
					attackChoice = i;
					return;
				}
			}
		}
		else {
			for (int i = 0; i < 6; i++) {						//Enemy attack patterns
				if (percentage <= enemyPatterns[type][i]) {
					attackChoice = i;
					return;
				}
			}
		}
	}
};
class Room {
public:
	string name;
	string intro[2] = { "You enter the room" }; //This is the default message. The second value is custom per room.
	string description[5];
	string doorLeadsTo[10];
	string objects[10];
	string objectDesc[10][5];
	int takeUnlock[10][5] = { 0 };
	int inspectUnlock[10][5] = { 0 };
	int itemLock[10][5] = { 0 };
	int itemUnlock[10][5] = { 0 };
	bool roomInspected = false;
	bool equippable[10] = { false, false, false, false, false, false, false, false, false, false };
	bool inspectable[10] = { false, false, false, false, false, false, false, false, false, false };
	bool previouslyEntered = false;

	void enterRoom() {
		if (!previouslyEntered) {
			cout << intro[1] << endl;
			previouslyEntered = true;
		}
		else {
			cout << intro[0] << endl;
		}
	}
};
class Music {
public:
	const string note[12] = { "c", "c#", "d", "d#", "e", "f", "f#", "g", "g#", "a", "a#", "b" };
	const int baseHz[12] = { 32, 34, 36, 38, 41, 43, 46, 49, 52, 55, 58, 61 };

	string tracklist[10] = { "Mega" };
	int mega[16] = { 293, 293, 587, 0, 440, 0, 0, 415, 0, 392, 0, 349, 0, 293, 349, 392 };
	string mega21[31] = { "d", "d", "d", "x", "a", "x", "x", "g#", "x", "g", "x", "f", "x", "d", "f", "g", "c", "c", "d", "x", "a", "x", "x", "g#", "x", "g", "x", "f", "d", "f", "g" };
	int mega22[31] = { 4, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };

	void playTone(int hz) {
		if (hz >> 0) {
			Beep(hz, 100);
		}
		else {
			Sleep(50);
		}
	}

	void playNote(string noteIn, int hz, int octave) {
		if (noteIn.find("x") != string::npos) {
			playTone(0); //Skip a beat
			return;
		}
		if (noteIn.find("tone") != string::npos) {
			playTone(hz); //Microtones!
			return;
		}
		for (int ix = 11; ix >= 0; ix--) { //If this didn't go backwards it would never play sharps
			if (noteIn.find(note[ix]) != string::npos) {
				hz = baseHz[ix] * ( 2 ^ octave );
				playTone(hz);
				return;
			}
		}
	}

	void playSong(string songTitle) {
		for (int ix = 0; ix < 10; ix++) {
			if (songTitle.find(tracklist[ix]) != string::npos) {
				if (tracklist[ix].length() > 0) {
					switch (ix)
					{
					case 0:
						for (int iy = 0; iy < 31; iy++) {
							playNote(mega21[iy], 0, mega22[iy]);
						}
						for (int iy = 0; iy < 16; iy++) {
							playNote("t", mega[iy], 0);
						}
						break;
					default:
						break;
					}
					return;
				}
			}
		}
	}
};

//Class Initialization
Character Player;
Character Enemy;
Room room;
Music music;

void title()
{
	cout << endl;
	Sleep(1000);
	cout << R"(   ___ ______ _   _ _____ _   _ _____ )" << endl; Sleep(500); music.playNote("c", 0, 4);
	cout << R"(  / _ \|  _  \ | | |  ___| \ | |_   _|)" << endl; Sleep(500); music.playNote("d", 0, 4);
	cout << R"( / /_\ \ | | | | | | |__ |  \| | | |  )" << endl; Sleep(500); music.playNote("e", 0, 4);
	cout << R"( |  _  | | | | | | |  __|| . ` | | |  )" << endl; Sleep(500); music.playNote("f", 0, 4);
	cout << R"( | | | | |/ /\ \_/ / |___| |\  | | |  )" << endl; Sleep(500); music.playNote("a", 0, 4);
	cout << R"( \_| |_/___/  \___/\____/\_| \_/ \_/  )" << endl; Sleep(500); music.playNote("b", 0, 4);
}

void inspect()
{
	if (actionReq.find(room.name) != string::npos || actionReq.find("room") != string::npos) {		//if the request includes the name of the current room...
		for (int i = 0; i < 5; i++) {						//for each line of the description...
			if (room.description[i].length() > 0) {			//except blank lines...
				cout << room.description[i] << endl;		//output the description of the room.
			}
		}
		if (!room.roomInspected) {
			room.roomInspected = true;
			Player.gainExp(Player.level * 20);
		}
		return;
	}

	if (!room.roomInspected) {
		cout << "That is not a valid command" << endl;
		return;
	}

	for (int ix = 0; ix < 10; ix++) {
		if (actionReq.find(Player.inventory[ix]) != string::npos) {
			if (Player.inventory[ix].length() > 0) {
				for (int iy = 0; iy < 5; iy++) {					//for each line of the description...
					if (Player.invDesc[ix][iy].length() > 0) {		//except blank lines...
						cout << Player.invDesc[ix][iy] << endl;		//output the description of the item.
					}
				}
				return;
			}
		}
	}

	for (int ix = 0; ix < 10; ix++) {
		if (room.objects[ix].length() == 0) {
			continue;
		}
		if (actionReq.find(room.objects[ix]) != string::npos) {			//if the request includes the name of one of the currently defined objects...
			if (!room.inspectable[ix]) {
				cout << "That is not a valid command" << endl;
				return;
			}
			for (int iy = 0; iy < 5; iy++) {								//for each line of the general description...
				if (room.objectDesc[ix][iy].length() > 0) {						//except blank lines...
					cout << room.objectDesc[ix][iy] << endl;						//output the description.
				}
				else if (iy == 0) {
					cout << "That is not a valid command" << endl;
					return;
				}
			}
			for (int i = 0; i < 5; i++) {
				if (room.takeUnlock[ix][i] > 0) {
					room.equippable[room.inspectUnlock[ix][i]] = true;
				}
				if (room.inspectUnlock[ix][i] > 0) {
					room.inspectable[room.inspectUnlock[ix][i]] = true;
					Player.gainExp(Player.level * 10);
				}
			}
			return;
		}
	}
	cout << "That is not a valid command" << endl;
	return;
}

void take() 
{
	for (int ia = 0; ia < 10; ia++) {
		if (room.objects[ia].length() == 0) {
			continue;
		}
		if (actionReq.find(room.objects[ia]) != string::npos) {			//if the request includes the name of one of the currently defined objects...
			if (room.equippable[ia]) {
				for (int ib = 0; ib < 10; ib++) {
					if (Player.inventory[ib].length() == 0) {
						Player.inventory[ib] = room.objects[ia];
						for (int i = 0; i < 5; i++) {
							if (room.objectDesc[ia][i].length() > 0) {
								Player.invDesc[ib][i] = room.objectDesc[ia][i];
							}
							if (room.itemUnlock[ia][i] > 0) {
								Player.itemUnlock[ib][i] = room.itemUnlock[ia][i];
							}
						}
						cout << "The " << room.objects[ia] << " has been added to your inventory." << endl;
						break;
					}
				}
			}
			else {
				cout << "That is not a valid command" << endl;
			}
			return;
		}
	}
}

void use()
{
	
	cout << "That is not a valid command" << endl;
	return;
}

void open()
{

}

void move()
{

}

void attack()
{
	locale loc;
	if (Player.exp <= 50) {
		cout << R"(Combat in Advent works like advanced rock paper scissors, where one move is weak or strong against another.
Your options are to quick attack, heavy attack, feint attack, parry, or dodge.
Depending on which action you choose and which one your enemy chooses, you might deal, or take, bonus damage.
You can also type "random" if you want Jesus to take the wheel.
Or "nothing" if you just... don't want to do anything.
You can also attempt to "retreat.")";
	}
beginCombat:
	spacing(2);
	Enemy.determineAttack();
	switch (Enemy.attackChoice) { //This determines the flavor text that displays to warn of an attack.
	case 0:
		cout << "The " << Enemy.name << " stands before you." << endl;
		break;
	case 1: //Heavy
		if (rand() % 100 + 1 < 45) {
			cout << "The enemy strengthens their footing" << endl;
		}
		else {
			cout << "The enemy draws back their weapon" << endl;
		}
		break;
	case 2: //Parry
		if (rand() % 100 + 1 < 45) {
			cout << "The enemy strengthens their footing" << endl;
		}
		else {
			cout << "The enemy readies their weapon" << endl;
		}
		break;
	case 3: //Quick
		if (rand() % 100 + 1 < 35) {
			cout << "The enemy readies their weapon" << endl;
		}
		else {
			cout << "The enemy advances toward you" << endl;
		}
		break;
	case 4: //Feint
		if (rand() % 100 + 1 < 45) {
			cout << "The enemy advances toward you" << endl;
		}
		else {
			cout << "The enemy draws back their weapon" << endl;
		}
		break;
	case 5: //Dodge
		if (rand() % 100 + 1 < 65) {
			cout << "The enemy takes half a step back" << endl;
		}
		else {
			cout << "The enemy takes half a step back" << endl;
		}
		break;
	}
	spacing(0);
	cout << "What move do you make?" << endl;
blankInput:
	getline(cin, actionReq);
	if (actionReq.length() == 0) {
		goto blankInput;
	}
	spacing(1);
	int parsedAction = -1;
	for (unsigned int i = 0; i < actionReq.length(); ++i) {
		actionReq[i] = tolower(actionReq[i], loc);
	}
	for (int i = 0; i < 8; i++) {
		if (actionReq.find(combatActions[i]) != string::npos) {
			parsedAction = i;
		}
	}
makeAttack:
	int winner = 0;
	switch (parsedAction) {
	case 0:
		if (Enemy.attackChoice == 0) {
			cout << "Neither of you does anything. A complete standstill." << endl
				<< "Almost like you're fighting a door or something." << endl;
			winner = 0; //Tie
		}
		else {
			cout << "You stand unflinching as the " << Enemy.name << " hits you without any consideration for mercy." << endl;
			winner = 2; //Enemy win
		}
		break;
	case 1: //Heavy
		switch (Enemy.attackChoice) {
		case 0:
			cout << "The " << Enemy.name << " does nothing as you brutally strike it, without any thought for mercy." << endl;
			winner = 1; //Player win
			break;
		case 1: //Heavy
			cout << "You both throw your strength into powerful blows. Your weapons violently clash, but you are both unscathed." << endl;
			winner = 0; //Tie
			break;
		case 2: //Parry
			cout << "The " << Enemy.name << " stands their ground in an attempt to weather the storm of your fury," << endl;
			cout << "but your blow shatters their defence and the strike lands." << endl;
			winner = 1; //Player win
			break;
		case 3: //Quick
			cout << "As you draw back your mighty swing, the " << Enemy.name << " delivers a quick and decisive strike to your while your guard is lowered." << endl;
			winner = 2; //Enemy win
			break;
		case 4: //Feint
			cout << "You both press the advance, but the " << Enemy.name << " draws back their weapon, hoping to have misguided your defence." << endl;
			cout << "Having miscalculated your ferociousness, the " << Enemy.name << " suffers the weight of your strike." << endl;
			winner = 1; //Player win
			break;
		case 5: //Dodge
			cout << "As you send your weapon careening through the air toward the " << Enemy.name << ", they deftly sidestep your attack," << endl;
			cout << "putting you off balance from the weight of your own swing, and vulnerable to their counter-attack." << endl;
			winner = 2; //Enemy win
			break;
		}
		break;
	case 2: //Parry
		switch (Enemy.attackChoice) {
		case 0:
			cout << "The " << Enemy.name << " does nothing, and here you are trying to parry." << endl;
			winner = 0; //Tie
			break;
		case 1: //Heavy
			cout << "You stand your ground in an attempt to weather the storm of the " << Enemy.name << "'s fury," << endl;
			cout << "but their blow shatters your defence and the strike lands." << endl;
			winner = 2; //Enemy win
			break;
		case 2: //Parry
			cout << "You both lie in wait, ready to deflect the other's blow, but neither of you makes a move." << endl;
			winner = 0; //Tie
			break;
		case 3: //Quick
			cout << "The " << Enemy.name << " lashes out with a deft jab, which you deflect with even greater agility." << endl;
			cout << "Having made them vulnerable, you deliver a vicious riposte." << endl;
			winner = 1; //Player win
			break;
		case 4: //Feint
			cout << "The " << Enemy.name << " pushes forward with an attack, but as you move to deflect it, they pull back," << endl;
			cout << "altering the course of their swing and landing a strike through your defences." << endl;
			winner = 2; //Enemy win
			break;
		case 5: //Dodge
			cout << "You stand ready, awaiting an attack, but the " << Enemy.name << " just sidesteps for no reason." << endl;
			cout << "Observing their movements, you hit them with a calculated strike, ending their silly dance." << endl;
			winner = 1; //Player Win
			break;
		}
		break;
	case 3: //Quick
		switch (Enemy.attackChoice) {
		case 0:
			cout << "The " << Enemy.name << " does nothing as you rapidly strike it, without any thought for mercy." << endl;
			winner = 1; //Player win
			break;
		case 1: //Heavy
			cout << "As the " << Enemy.name << " draws back a mighty swing, you deliver a quick and decisive strike while their guard is lowered." << endl;
			winner = 1; //Player win
			break;
		case 2: //Parry
			cout << "You lash out with a deft jab, which the " << Enemy.name << " deflects with even greater agility." << endl;
			cout << "Having made you vulnerable, they deliver a vicious riposte." << endl;
			winner = 2; //Enemy win
			break;
		case 3: //Quick
			cout << "You both lash out with quick, furious strikes, but your weapons merely glance off each other." << endl;
			winner = 0; //Tie
			break;
		case 4: //Feint
			cout << "You both press the advance, but the " << Enemy.name << " draws back their weapon, hoping to have misguided your defence." << endl;
			cout << "Having miscalculated your cunning, you deliver a decisive strike on the " << Enemy.name << "." << endl;
			winner = 1; //Player win
			break;
		case 5: //Dodge
			cout << "You throw out a quick, uncalculated strike, which the " << Enemy.name << " nimbly dodges," << endl;
			cout << "retalliating with a deft strike of their own." << endl;
			winner = 2; //Enemy win
			break;
		}
		break;
	case 4: //Feint
		switch (Enemy.attackChoice) {
		case 0:
			cout << "Why are you trying fancy combat tactics on a helpless " << Enemy.name << ", you psychopath." << endl;
			winner = 0; //Tie
			break;
		case 1: //Heavy
			cout << "You both press the advance, but you draw back your weapon, hoping to have misguided their defence." << endl;
			cout << "Having miscalculated the " << Enemy.name << "'s ferociousness, you suffer the weight of their strike." << endl;
			winner = 2; //Enemy win
			break;
		case 2: //Parry
			cout << "You push forward with an attack, but as the " << Enemy.name << " moves to deflect it, you pull back," << endl;
			cout << "altering the course of your swing and landing a strike through their defences." << endl;
			winner = 1; //Player win
			break;
		case 3: //Quick
			cout << "You both press the advance, but you draw back your weapon, hoping to have misguided the " << Enemy.name << "'s defence." << endl;
			cout << "Having miscalculated their cunning, the " << Enemy.name << " delivers a decisive strike on you." << endl;
			winner = 2; //Enemy win
			break;
		case 4: //Feint
			cout << "You both swing, but redirect your strikes elsewhere at the last second. Despite the deception, your weapons meet." << endl;
			winner = 0; //Tie
			break;
		case 5: //Dodge
			cout << "You throw a calculated swing, expertly misdirecting the " << Enemy.name << " to dodge the wrong way," << endl;
			cout << "at which point you redirect your strike to land a decisive blow on them before they know what's happened." << endl;
			winner = 1; //Player win
			break;
		}
		break;
	case 5: //Dodge
		switch (Enemy.attackChoice) {
		case 0:
			cout << "You narrowly manage to dogde the " << Enemy.name << "'s colossally powerful strike, that surely would have decimated you if it had hit." << endl;
			winner = 0; //Tie
			break;
		case 1: //Heavy
			cout << "As the " << Enemy.name << " sends their weapon careening through the air toward you, you deftly sidestep their attack," << endl;
			cout << "putting them off balance from the weight of their own swing, and vulnerable to your counter-attack." << endl;
			winner = 1; //Player win
			break;
		case 2: //Parry
			cout << "The " << Enemy.name << " stands ready, awaiting an attack, but you just sidestep for no reason." << endl;
			cout << "Observing your movements, they hit you with a calculated strike, ending your silly dance." << endl;
			winner = 2; //Enemy win
			break;
		case 3: //Quick
			cout << "The " << Enemy.name << " throws out a quick, uncalculated strike, which you nimbly dodge," << endl;
			cout << "retalliating with a deft strike of your own." << endl;
			winner = 1; //Player win
			break;
		case 4: //Feint
			cout << "The " << Enemy.name << " throws a calculated swing, expertly misdirecting you to dodge the wrong way," << endl;
			cout << "at which point they redirect their strike to land a decisive blow on you before you know what's happened." << endl;
			winner = 2; //Enemy win
			break;
		case 5: //Dodge
			cout << "You both backstep, avoiding attacks that never came." << endl;
			winner = 0; //Tie
			break;
		}
		break;
	case 6: //Random
		Player.determineAttack();
		parsedAction = Player.attackChoice;
		goto makeAttack;
	case 7:
		if (!Enemy.aggro) {
			cout << "You lower your weapon, and the " << Enemy.name << " doesn't do anything to stop you." << endl;
			return;
		}
		else {
			cout << "This doesn't work for non-door enemies yet, sorry." << endl
				<< "I won't penalize you for it tho, just try again." << endl;
			winner = 0; //Tie
			break;
		}
		break;
	}
	spacing(0);
	switch (winner) {
	case 0:
		cout << "Neither of you take damage." << endl;
		break;
	case 1:
		Enemy.takeDamage(Player.damage);
		break;
	case 2:
		Player.takeDamage(Enemy.damage);
		break;
	}

	if (Enemy.dead) {
		cout << "You've won the combat" << endl;
		return;
	}
	else {
		goto beginCombat;
	}
}

void help()
{
	cout << "Inspect:	Takes a closer look at things." << endl
		<< "Use:		Interact with things, sometimes using other things." << endl
		<< "Take:		Picks up items." << endl
		<< "Move:		WIP" << endl
		<< "Talk:		WIP" << endl
		<< "Attack:		Start combat." << endl
		<< "Stats:		Display your stats, like health and level, as well as the enemy's stats." << endl
		<< "Inventory:	Displays your inventory." << endl
		<< "Equip:		Equip weapons or armor." << endl
		<< "Quit:		Quit the game." << endl;
}

void createRoom(int roomNum)
{
	room = Room();
	switch (roomNum) {
	case 1: //starting cell
		room.roomInspected = false;
		room.name = "cell";
		room.intro[0] = "You enter the cell";
		room.intro[1] = "You awaken in a jail cell, with no memory of what has happened";
		room.description[0] = "The cell's walls are heavily worn, stones crumbling under their own weight.";
		room.description[1] = "The door is rusted iron with a small window at eye level.";
		room.description[2] = "Pushed up against one of the walls is a rickety bed. There appears to be a note on the pillow.";

		room.objects[0] = "wall";
		room.objectDesc[0][0] = "Looking closer at one of the walls,";
		room.objectDesc[0][1] = "you see several bricks with scratches covering their surface";
		room.objectDesc[0][2] = "all surrounding a slightly loose brick.";
		room.inspectable[0] = true;
		room.inspectUnlock[0][0] = 1;

		room.objects[1] = "brick";
		room.objectDesc[1][0] = "Wiggling the loose brick out of position, you find an ornate key hidden behind";
		room.takeUnlock[1][0] = 2;
		room.inspectUnlock[1][0] = 2;

		room.objects[2] = "key";
		room.objectDesc[2][0] = "The key is small and ornate, with many intricate swirls and motifs.";
		room.objectDesc[2][1] = "The silver surface is tarnished, but it feels solidly constructed despite how it looks.";
		room.itemUnlock[2][0] = 13;

		room.objects[3] = "door";
		room.objectDesc[3][0] = "The door is heavy iron, but rusted through around the edges.";
		room.objectDesc[3][1] = "While imposing, it seems like the rust may have it near the end of its usefulness.";
		room.objectDesc[3][2] = "And obviously it's locked, you criminal.";
		room.inspectable[3] = true;
		room.itemLock[2][0] = 13;

		room.objects[4] = "bed";
		room.objectDesc[4][0] = "The bed is loosely constructed wooden cot.";
		room.objectDesc[4][1] = "The rotting, slightly moist planks would be lucky to support even a child.";
		room.objectDesc[4][3] = "Peering underneath, you see someone has stashed away a rough shiv. Dubious, even for the wielder.";
		room.inspectable[4] = true;
		room.takeUnlock[4][0] = 5;
		room.inspectUnlock[4][0] = 5;

		room.objects[5] = "shiv";
		room.objectDesc[5][0] = "The shiv is roughly shaped from a rusty piece of metal pulled off of the cell door.";
		room.objectDesc[5][1] = "A long shred of bedsheet wrapped around the base forms a handle that just barely protects your hand";

		room.objects[6] = "note";
		room.objectDesc[6][0] = "The note is written in terrible, scraggly handwriting, on slightly mildewed paper.";
		room.objectDesc[6][1] = "It says, 'Thanks for playing my game :) -Ben'";
		room.inspectable[6] = true;
		room.equippable[6] = true;

		Enemy.name = "Door";
		Enemy.type = 0;
		Enemy.health = 50;
		Enemy.healthMax = 100;
		Enemy.currentWeapon = "nothing";
		Enemy.damage = 1;
		Enemy.currentArmor = "surface";
		Enemy.defense = 0.25;
		break;
	default:
		break;
	};
	return;
}

void actionHandler()
{
	locale loc;															//Make sure we're using the right locale
	int parsedAction = -1;						//If none of the actions are recognized, this will stay this way

	/*if (actionReq.find("DEV") != string::npos) {						//Check to see if the action is a DEV command (all caps)
		parsedAction = -2;
		goto parseFinish;
	}*/

	for (unsigned int i = 0; i < actionReq.length(); ++i) {				//Convert the string to all lower case
		actionReq[i] = tolower(actionReq[i], loc);						//So it doesn't fuck up
	}
	for (int i = 0; i < numberOfActions; i++) {
		if (actionReq.find(possibleActions[i]) != string::npos) {		//but check the list of DEV commands?
			parsedAction = i;											//and parse it I guess??
			goto parseFinish;
		}
	}

parseFinish:
	spacing(1);
	switch (parsedAction) {
	case -2:
		cout << "This doesn't work yet" << endl;
		break;
	case 0:
		help();
		break;
	case 1:
		inspect();
		break;
	case 2:
		take();
		break;
	case 3:
		//use();
		break;
	case 4:
		//open();
	case 5:
		//move();
		break;
	case 6:
		//talk();
		break;
	case 7:
		attack();
		break;
	case 8:
		Player.displayStats();
		if (Enemy.aggro) {
			spacing(2);
			Enemy.displayStats();
		}
		break;
	case 9:
		Player.displayInventory();
		break;
	case 10:
		Player.equip();
		break;
	case 11:
		exit(0);
	case 12:
		music.playSong("Mega");
		break;
	default:
		cout << "That is not a valid command" << endl;
		incorrectAction++;
		break;
	}
	return;
}

int main() 
{
beginGame:
	srand(time(0));
	title();
	spacing(2);
	Player.type = -1;
	Player.level = 1;
	Player.exp = 0;
	Player.expCap = 100;
	Player.health = 100;
	Player.healthMax = 100;
	cout << "What is your name?\n";
	cin >> Player.name;
	spacing(1);
	cout << "Hello, " << Player.name << "!" << endl;
	cout << endl;

	createRoom(1);
	room.enterRoom();
	cout << "I know, very original..." << endl
		<< "Would you believe me if I said it was a placeholder?" << endl;

actionRequest:
	if (Enemy.aggro) {
		attack();
	}
	spacing(2);
	cout << "What do you want to do?" << endl;
	if (incorrectAction >= 1) {
		cout << "Type 'help' for a list of commands" << endl;
		incorrectAction = 0;
	}
blankInput:
	getline(cin, actionReq);
	if (actionReq.length() == 0) {
		goto blankInput;
	}
	actionHandler();
	goto actionRequest;

	return 0;
}