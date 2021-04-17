#include <iostream>
#include <string>
#include <locale>
using namespace std;

string playerName;
string enemyNames[5] = {"Slime", "Goblin", "Skeleton", "Ogre", "Lich"}

class Character {			//This is used to keep track of all stats and conditions of characters,
public:						//player or otherwise.
	string name;
	int level = 1;
	int exp = 0;
	int expCap = 100;
	int health = 100;
	int perkPoints = 0;

	void levelCheck() {			//Level up, basically.
		if (exp >= expCap) {
			level++;
			perkPoints++;
			health = health + (10 * level);
			exp = exp - expCap;
			expCap = expCap + (10 * 1 ^ level);
		}
	}
};

int main()
{
	cin >> playerName;
	Character player; 

}