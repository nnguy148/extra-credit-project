#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Player Information
string playerName;
string playerRace;
char playerGender;
string playerClass;
int playerLevel = 1;
int playerExp = 0;
int playerMaxExp = 100;
int playerHp = 100;
int playerMaxHp = 100;
int playerAttack = 10;
int playerDefense = 5;
vector<string> inventory = { "Small Health Potion", "Small Mana Potion" };

// Function prototypes
void showIntro();
void createCharacter();
void movePlayer();
void explore();
void battle();
void showInventory();

int main() {
    srand(time(0)); // Random seed for encounters

    showIntro();
    createCharacter();

    while (true) {
        cout << "\n=== PLAYER STATUS ===\n";
        cout << "Name: " << playerName << " | Race: " << playerRace << " | Class: " << playerClass << endl;
        cout << "Level: " << playerLevel << " | EXP: " << playerExp << "/" << playerMaxExp << " | HP: " << playerHp << "/" << playerMaxHp << endl;

        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Move\n2. Check Inventory\n3. Save Game (Coming Soon)\n4. Quit Game\n";
        cout << "Choice: ";

        int menuChoice;
        cin >> menuChoice;

        if (menuChoice == 1) {
            movePlayer();
        } else if (menuChoice == 2) {
            showInventory();
        } else if (menuChoice == 3) {
            cout << "Save feature coming soon!\n";
        } else if (menuChoice == 4) {
            cout << "Thanks for playing! See you next time!\n";
            break;
        } else {
            cout << "Invalid input. Try again.\n";
        }

        if (playerHp <= 0) {
            cout << "You have fallen in battle...\nGame Over.\n";
            break;
        }
    }

    return 0;
}

// Introduction story
void showIntro() {
    cout << "You awaken in darkness...\n";
    cout << "The cold wind cuts through your skin like broken glass...\n";
    cout << "You can't remember anything...\n\n";
    cout << "[Press Enter to continue...]\n";
    cin.ignore();
}

// Character creation
void createCharacter() {
    cout << "\n=== CHARACTER CREATION ===\n";
    
    cout << "Enter your character's name: ";
    getline(cin, playerName);

    cout << "Choose your Race:\n1. Human\n2. Elf\n3. Orc\n4. Dwarf\n";
    cout << "Race: ";
    int raceChoice;
    cin >> raceChoice;
    if (raceChoice == 1) playerRace = "Human";
    else if (raceChoice == 2) playerRace = "Elf";
    else if (raceChoice == 3) playerRace = "Orc";
    else playerRace = "Dwarf";

    cout << "Choose your Gender (M/F): ";
    cin >> playerGender;

    cout << "Choose your Class:\n1. Warrior\n2. Mage\n3. Rogue\n";
    cout << "Class: ";
    int classChoice;
    cin >> classChoice;

    if (classChoice == 1) {
        playerClass = "Warrior";
        playerAttack += 5;
        playerHp += 30;
    } else if (classChoice == 2) {
        playerClass = "Mage";
        playerAttack += 10;
    } else {
        playerClass = "Rogue";
        playerAttack += 7;
        playerDefense += 2;
    }

    cout << "\nWelcome, " << playerName << " the " << playerRace << " " << playerClass << "!\n";
    cout << "Starting HP: " << playerHp << "\nStarting Attack: " << playerAttack << "\nStarting Defense: " << playerDefense << "\n";
}

// Player movement
void movePlayer() {
    cout << "\n=== MOVE MENU ===\n";
    cout << "1. North\n2. South\n3. East\n4. West\n";
    cout << "Move: ";

    int moveChoice;
    cin >> moveChoice;

    explore();
}

// Exploring after moving
void explore() {
    int encounter = rand() % 3; // 0 = Nothing, 1 = Monster, 2 = NPC

    if (encounter == 0) {
        cout << "You move carefully but find nothing...\n";
    } else if (encounter == 1) {
        battle();
    } else {
        cout << "You meet a traveling merchant. He wishes you luck on your journey.\n";
    }
}

// Battle system
void battle() {
    int monsterHp = 50;
    cout << "\nA wild Goblin appears!\n";

    while (monsterHp > 0 && playerHp > 0) {
        cout << "\n=== COMBAT MENU ===\n";
        cout << "1. Attack\n2. Use Skill\n3. Use Item\n4. Flee\n";
        cout << "Action: ";

        int combatChoice;
        cin >> combatChoice;

        if (combatChoice == 1) {
            int damage = playerAttack - 5 + (rand() % 10);
            cout << "You attack and deal " << damage << " damage!\n";
            monsterHp -= damage;
        } else if (combatChoice == 2) {
            int skillDamage = playerAttack + 10;
            cout << "You use a powerful skill and deal " << skillDamage << " damage!\n";
            monsterHp -= skillDamage;
        } else if (combatChoice == 3) {
            if (!inventory.empty()) {
                cout << "You use a " << inventory.back() << "!\n";
                playerHp += 20;
                if (playerHp > playerMaxHp) playerHp = playerMaxHp;
                inventory.pop_back();
            } else {
                cout << "No items left in your inventory!\n";
            }
        } else if (combatChoice == 4) {
            cout << "You fled from the battle safely.\n";
            return;
        } else {
            cout << "Invalid choice, try again.\n";
            continue;
        }

        if (monsterHp > 0) {
            int monsterAttack = 10 + (rand() % 5);
            cout << "The Goblin hits you for " << monsterAttack << " damage!\n";
            playerHp -= monsterAttack;
        }
    }

    if (playerHp <= 0) {
        cout << "You have been defeated by the Goblin...\n";
    } else {
        cout << "You defeated the Goblin!\n";
        int expGain = 30;
        cout << "You gained " << expGain << " EXP!\n";

        playerExp += expGain;
        if (playerExp >= playerMaxExp) {
            playerLevel++;
            playerExp = 0;
            playerMaxExp += 50;
            playerHp = playerMaxHp;
            cout << "Congratulations! You leveled up to Level " << playerLevel << "!\n";
        }
    }
}

// Show Inventory
void showInventory() {
    cout << "\n=== INVENTORY ===\n";
    if (inventory.empty()) {
        cout << "You have no items.\n";
    } else {
        for (const auto& item : inventory) {
            cout << "- " << item << "\n";
        }
    }
}

