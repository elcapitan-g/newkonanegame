#include "Board.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <random>
#include <limits>
#ifdef _WIN32
#include <windows.h>
#endif

using ms = std::chrono::milliseconds;
inline void sleep_ms(int t){ std::this_thread::sleep_for(ms(t)); }

// ---------------- Helper functions ----------------
void slowPrint(const std::string &s, int delay=6){
    for(char c : s){
        std::cout << c << std::flush;
        sleep_ms(delay);
    }
}

void instantPrint(const std::string &s){ std::cout << s << std::flush; }

void cls(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void flash(int repeats=2, int msDelay=60){
    for(int i=0;i<repeats;i++){
        std::cout << "\033[107m" << std::string(80,' ') << "\033[0m\n" << std::flush;
        sleep_ms(msDelay);
    }
}

void emberField(int lines=12, int count=40){
    std::default_random_engine rng((unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> x(0, 46), ycol(0,2), delay(10,40);
    const char* cols[] = {"\033[38;2;255;140;30m", "\033[38;2;255;90;20m", "\033[38;2;250;200;50m"};
    for (int i=0;i<count;i++){
        int xpos = x(rng);
        int c = ycol(rng);
        std::cout << std::string(xpos, ' ') << cols[c] << "*" << "\033[0m\n";
        sleep_ms(delay(rng));
    }
}

void waveReveal(const std::string &text, int amplitude=6, int stepDelay=6){
    for (size_t i=0;i<text.size();++i){
        int offset = (int)(std::sin((double)i * 0.28) * amplitude) + amplitude;
        std::cout << std::string(offset, ' ') << text[i] << '\n';
        sleep_ms(stepDelay);
    }
}

void runeSpiral(int size = 9){
    int n = size | 1;
    std::vector<std::string> grid(n, std::string(n, ' '));
    int x = n/2, y = n/2;
    grid[y][x] = '@';
    int dir = 0;
    int dx[4] = {1,0,-1,0};
    int dy[4] = {0,1,0,-1};
    for (int len = 1; len < n* n; ++len){
        for (int r=0; r< (len/((dir+1)/2))+1; ++r){
            x += dx[dir%4];
            y += dy[dir%4];
            if (y>=0 && y<n && x>=0 && x<n) grid[y][x] = '*';
        }
        dir++;
    }
    const char* glow = "\033[38;2;180;220;255m";
    const char* gold = "\033[38;2;255;215;0m";
    for (int r=0;r<n;r++){
        for (int c=0;c<n;c++){
            if (grid[r][c] == '@') std::cout << gold << 'O' << "\033[0m";
            else if (grid[r][c] == '*') std::cout << glow << '*' << "\033[0m";
            else std::cout << ' ';
            std::cout << ' ';
        }
        std::cout << '\n';
        sleep_ms(35);
    }
}

void tone(int freq, int duration_ms) {
#ifdef _WIN32
    Beep(freq, duration_ms);
#else
    int repeats = std::max(1, duration_ms / 60);
    for (int i=0;i<repeats;i++){
        std::cout << '\a' << std::flush;
        sleep_ms(60);
    }
#endif
}

// ---------------- Title Screen ----------------
void divineTitleScreen(){
    cls();
    const char* gold = "\033[38;2;255;215;0m";
    const char* cyan = "\033[38;2;120;200;255m";
    const char* mag = "\033[38;2;220;80;190m";
    const char* reset = "\033[0m";

    slowPrint(" E ho mai ka mana... Grant us strength...\n\n", 40);
    tone(330, 180);
    sleep_ms(300);

    waveReveal(std::string(cyan) + "The ocean sighs...Ancestors carry memory of the aina..." + std::string(reset), 8, 10);
    tone(370, 120);
    sleep_ms(250);

    flash(3, 55);
    for (int i=0;i<5;i++){
        tone(120 + i*10, 160);
        sleep_ms(80);
    }
    sleep_ms(200);

    emberField(12, 30);
    flash(5,40);
    tone(1200, 180);
    sleep_ms(250);

    cls();
    slowPrint(gold);

   slowPrint("   :::     .        ...         :::.    :::.      :::.         :::.   :::.      .,::::::\n",1);  
   slowPrint("   ;;; .;;,.     .;;;;;;;.      `;;;;,  `;;;      ;;`;;        `;;;;,  `;;;     ;;;;''''  \n",1);
   slowPrint("   [[[[[/'      ,[[      [[,      [[[[[. '[[     ,[[ '[[,        [[[[[. '[[     [[cccc   \n",1);
   slowPrint("   $$$$,        $$$,     $$$      $$$  Y$c$$    c$$$cc$$$c       $$$  Y$c$$     $$''''   \n",1);
   slowPrint("   888 88o,     '888,_ _,88P      888    Y88     888   888,      888    Y88     888oo,__ \n",1);
   slowPrint("   MMM  'MMP'      'YMMMMMP'       MMM     YM     YMM   ""`       MMM     YM     ''''YUMM \n",1);
   slowPrint("\n",1);
    instantPrint(reset);

    slowPrint(mag);
    slowPrint("                   Ancient Hawaiian Strategy - K O N A N E  \n\n", 1);
    instantPrint(reset);
    sleep_ms(200);

    tone(523,120); sleep_ms(80);
    tone(659,120); sleep_ms(80);
    tone(784,220);
    sleep_ms(320);

    std::cout << "                   >>  PRESS ENTER TO BEGIN YOUR CHALLENGE  <<\n";
    std::cin.clear();
    std::cin.get();
    cls();
}

// ---------------- Main Game ----------------
int main() {
    divineTitleScreen();

    Board* board = new Board(8);

    // Remove the center piece for standard Kōnane start
    board->removePiece(3, 3);  // assuming 0-indexed, center of 8x8 board

    char player = 'B';

while (true) {
    board->print(); // always show current board
    if (!board->hasMoves(player)) {
        std::cout << "Player " << player << " has no moves. ";
        std::cout << (player == 'B' ? "White" : "Black") << " wins!\n";
        break;
    }

    std::cout << "Player " << player << "'s turn. Enter move (r1 c1 r2 c2), or 'save', 'load', 'exit': ";
    std::string input;
    std::getline(std::cin, input);

    if (input == "exit") break;

    if (input.rfind("save", 0) == 0) {
        std::string name = input.substr(5);
        board->saveToFile(name);
        continue;
    }

    if (input.rfind("load", 0) == 0) {
        std::string name = input.substr(5);
        board->loadFromFile(name);
        continue;
    }

    int r1, c1, r2, c2;
    if (sscanf(input.c_str(), "%d %d %d %d", &r1, &c1, &r2, &c2) != 4) {
        std::cout << "Invalid input! Try again.\n";
        continue;
    }

    if (board->makeMove(r1, c1, r2, c2, player)) {
        tone(600, 100); // beep after valid move
        player = (player == 'B' ? 'W' : 'B'); // switch player
    } else {
        std::cout << "Invalid move! Try again.\n";
    }
}

    delete board;

    // GAME OVER - waits for ENTER
    std::cout << "Game over. Press ENTER to exit...";
    std::cin.get();

    return 0;
}