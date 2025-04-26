#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

// Sleep helper for cross-platform
void Sleep(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

// Player info
string name, race, sex, playerClass;
int level = 1, Exp = 0, health = 100, maxHealth = 100, toLevel = 100;
vector<string> inventory = { "Small Health Potion", "Small Mana Potion" };
bool gameRunning = true;
bool questAccepted = false;

// Monster info
string monsterName = "";
int monsterHP = 0, monsterLevel = 0, monsterExp = 0;

// Map and location
int playerX = 2, playerY = 2;
char map[5][5] = {
    {'E','E','M','B','E'},
    {'N','M','E','M','E'},
    {'M','M','E','N','E'},
    {'N','P','M','E','E'},
    {'M','E','E','E','M'}
};

// Function prototypes
void MainMenu();
void NewGame();
void LoadGame();
void SaveGame();
void QuitGame();
void HUD();
void Move();
void Explore();
void CreateMonster();
void Combat();
void FightMenu();
void UseSkill();
void UseItem();
void InventoryMenu();
void TalkToNPC();
void StoryIntro();
void StoryMiddle();
void StoryEnd();

int main() {
    srand(time(0)); // Random seed
    MainMenu();
    return 0;
}

// Main Menu
void MainMenu() {
    while (gameRunning) {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. New Game\n";
        cout << "2. Load Game\n";
        cout << "3. Quit\n";
        cout << "Choose: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: NewGame(); break;
            case 2: LoadGame(); break;
            case 3: QuitGame(); break;
            default: cout << "Invalid option.\n"; break;
        }
    }
}

// New Game
void NewGame() {
    cout << "\n=== CHARACTER CREATION ===\n";
    cout << "Name: ";
    cin >> name;
    cout << "Race (Human, Elf, Dwarf): ";
    cin >> race;
    cout << "Gender (M/F): ";
    cin >> sex;

    cout << "\nChoose your Class:\n";
    cout << "1. Warrior (High HP)\n";
    cout << "2. Mage (Magic User)\n";
    cout << "3. Rogue (Fast Attacker)\n";
    cout << "Class: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        playerClass = "Warrior";
        health = maxHealth = 150;
    } else if (choice == 2) {
        playerClass = "Mage";
        health = maxHealth = 100;
    } else if (choice == 3) {
        playerClass = "Rogue";
        health = maxHealth = 120;
    } else {
        cout << "Invalid class. Defaulting to Warrior.\n";
        playerClass = "Warrior";
        health = maxHealth = 150;
    }

    StoryIntro();
    HUD();
    Move();
}

// HUD display
void HUD() {
    cout << "\n=== STATUS ===\n";
    cout << "Name: " << name << " | Race: " << race << " | Sex: " << sex << " | Class: " << playerClass << endl;
    cout << "Level: " << level << " | EXP: " << Exp << "/" << toLevel << " | HP: " << health << "/" << maxHealth << endl;
}

// Save game
void SaveGame() {
    ofstream file("save.txt");
    file << name << " " << race << " " << sex << " " << playerClass << " "
         << level << " " << Exp << " " << health << " " << maxHealth << " " << toLevel << endl;
    file.close();
    cout << "Game Saved!\n";
}

// Load game
void LoadGame() {
    ifstream file("save.txt");
    if (file) {
        file >> name >> race >> sex >> playerClass >> level >> Exp >> health >> maxHealth >> toLevel;
        cout << "Game Loaded!\n";
        HUD();
        Move();
    } else {
        cout << "No save file found.\n";
        MainMenu();
    }
}

// Quit game
void QuitGame() {
    cout << "Exiting game. Goodbye!\n";
    gameRunning = false;
}

// Movement
void Move() {
    while (true) {
        HUD();
        cout << "\n=== MOVE MENU ===\n";
        cout << "1. North\n2. South\n3. East\n4. West\n5. Inventory\n6. Save Game\n7. Quit\nMove: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: if (playerY > 0) playerY--; break;
            case 2: if (playerY < 4) playerY++; break;
            case 3: if (playerX < 4) playerX++; break;
            case 4: if (playerX > 0) playerX--; break;
            case 5: InventoryMenu(); continue;
            case 6: SaveGame(); continue;
            case 7: QuitGame(); return;
            default: cout << "Invalid move.\n"; continue;
        }

        Explore();
    }
}

// Explore location
void Explore() {
    char tile = map[playerY][playerX];
    switch (tile) {
        case 'M': CreateMonster(); Combat(); break;
        case 'N': TalkToNPC(); break;
        case 'P': StoryMiddle(); break;
        case 'B': StoryEnd(); break;
        case 'E': cout << "Nothing here. Move on.\n"; break;
    }
}

// Create monster
void CreateMonster() {
    string monsters[] = {"Goblin", "Orc", "Skeleton", "Witch"};
    monsterName = monsters[rand() % 4];
    monsterLevel = rand() % 3 + level;
    monsterHP = monsterLevel * 30;
    monsterExp = monsterHP / 2;
    cout << "\nA wild " << monsterName << " appears!\n";
}

// Combat
void Combat() {
    while (health > 0 && monsterHP > 0) {
        FightMenu();
    }

    if (health <= 0) {
        cout << "You have been defeated...\n";
        QuitGame();
    }

    if (monsterHP <= 0) {
        cout << "You defeated the " << monsterName << "!\n";
        Exp += monsterExp;
        if (Exp >= toLevel) {
            level++;
            Exp = 0;
            toLevel += 100;
            health = maxHealth;
            cout << "LEVEL UP! Now Level " << level << "!\n";
        }
    }
}

// Fight menu
void FightMenu() {
    cout << "\n=== COMBAT MENU ===\n";
    cout << "1. Attack\n2. Use Skill\n3. Use Item\n4. Flee\nAction: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        int damage = 10 + rand() % 10 + level;
        cout << "You hit the " << monsterName << " for " << damage << " damage!\n";
        monsterHP -= damage;
    }
    else if (choice == 2) {
        UseSkill();
    }
    else if (choice == 3) {
        UseItem();
    }
    else if (choice == 4) {
        if (rand() % 2 == 0) {
            cout << "You fled successfully!\n";
            return;
        } else {
            cout << "Failed to flee!\n";
        }
    }
    else {
        cout << "Invalid choice.\n";
    }

    if (monsterHP > 0) {
        int monsterDamage = monsterLevel * (5 + rand() % 5);
        cout << "The " << monsterName << " hits you for " << monsterDamage << " damage!\n";
        health -= monsterDamage;
    }
}

// Use skill
void UseSkill() {
    int skillDamage = 20 + rand() % 15 + level;
    cout << "You cast a powerful skill and deal " << skillDamage << " damage!\n";
    monsterHP -= skillDamage;
}

// Use item
void UseItem() {
    if (inventory.empty()) {
        cout << "You have no items!\n";
        return;
    }

    cout << "Inventory:\n";
    for (int i = 0; i < inventory.size(); i++) {
        cout << i + 1 << ". " << inventory[i] << endl;
    }
    cout << "Choose item: ";
    int choice;
    cin >> choice;

    if (choice >= 1 && choice <= inventory.size()) {
        if (inventory[choice-1] == "Small Health Potion") {
            health += 30;
            if (health > maxHealth) health = maxHealth;
            cout << "You recovered 30 HP!\n";
        }
        else if (inventory[choice-1] == "Small Mana Potion") {
            cout << "You recovered some magic power (flavor only).\n";
        }
        inventory.erase(inventory.begin() + (choice-1));
    } else {
        cout << "Invalid choice.\n";
    }
}

// Inventory
void InventoryMenu() {
    cout << "\n=== INVENTORY ===\n";
    for (int i = 0; i < inventory.size(); i++) {
        cout << i + 1 << ". " << inventory[i] << endl;
    }
}

// Talk to NPC
void TalkToNPC() {
    if (!questAccepted) {
        cout << "\nYou meet a villager.\n";
        cout << "Quest: Bring back a Monster Fang.\nAccept quest? (1=Yes, 2=No): ";
        int choice;
        cin >> choice;
        if (choice == 1) {
            questAccepted = true;
            cout << "Quest accepted!\n";
        } else {
            cout << "Maybe later.\n";
        }
    } else {
        cout << "You already accepted the quest.\n";
    }
}

// Story parts
void StoryIntro() {
    cout << "\nYou awaken in a dark forest, with no memory of how you arrived.\n";
}

void StoryMiddle() {
    cout << "\nYou find a strange shrine glowing with magical energy...\n";
}

void StoryEnd() {
    cout << "\nYou reach the final gate and escape the cursed forest. You Win!\n";
    QuitGame();
}
