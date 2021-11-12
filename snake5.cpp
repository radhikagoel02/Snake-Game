#include <bits/stdc++.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
#define M 20
#define N 50
using namespace std;
struct Node {
  char value;
  int X;
  int Y;
  struct Node *next;
  Node() { next = NULL; }
};
struct Snake {
  struct Node *head;
  struct Node *tail;
  char direction;
  int score;
  map<pair<int, int>, int> map;
  Snake() {
    head = NULL;
    tail = NULL;
    score = 0;
  }
};
char boundary[M][N];
struct Snake *snake = NULL;
struct Node *food = NULL;
int foodCreated = 0;
map<char, string> valid_input;
enum colors {
  BLACK,
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  BROWN,
  LIGHTGRAY,
  DARKGRAY,
  LIGHTBLUE,
  LIGHTGREEN,
  LIGHTCYAN,
  LIGHTRED,
  LIGHTMAGENTA,
  YELLOW,
  WHITE
};
void gotoxy(int x, int y) {
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void font() {
  CONSOLE_FONT_INFOEX cfi;
  cfi.cbSize = sizeof(cfi);
  cfi.nFont = 0;
  cfi.dwFontSize.X = 20; // Width of each character in the font
  cfi.dwFontSize.Y = 28; // Height
  cfi.FontFamily = FF_DONTCARE;
  cfi.FontWeight = FW_NORMAL;
  wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
  SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}
void setColor(int color) {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, color);
}
char startGame() {
  fstream fin;
  int i = 1;
  char n;
  fin.open("savedGame.txt", ios::in);
  setColor(LIGHTGREEN);
  cout << "SNAKE GAME!" << endl;
  setColor(WHITE);
  cout << i << ". NEW GAME" << endl;
  i++;
  if (fin) {
    cout << i << ". LOAD GAME" << endl;
    i++;
  }
  cout << i << ". EXIT GAME" << endl;
  i++;
  n = getch();
  setColor(LIGHTCYAN);
  while (n != '1' && n != '2' && n != '3') {
    cout << "Enter Valid Choice";
    n = getch();
    gotoxy(0, i);
  }
  fin.close();
  system("cls");
  return n;
}
void setup() {
  valid_input['w'] = "up";
  valid_input['W'] = "up";
  valid_input['s'] = "down";
  valid_input['S'] = "down";
  valid_input['a'] = "left";
  valid_input['A'] = "left";
  valid_input['d'] = "right";
  valid_input['D'] = "right";
  snake = (struct Snake *)new Snake();
  snake->head = (struct Node *)new Node();
  snake->head->value = 'Q';
  snake->head->X = 2;
  snake->head->Y = 2;
  snake->map[make_pair(snake->head->X, snake->head->Y)] = 1;
  struct Node *node = (struct Node *)new Node();
  node->X = 3;
  node->Y = 2;
  node->value = '#';
  snake->head->next = node;
  snake->map[make_pair(node->X, node->Y)] = 1;
  snake->tail = (struct Node *)new Node();
  snake->tail->value = '*';
  snake->tail->X = 4;
  snake->tail->Y = 2;
  snake->tail->next = NULL;
  node->next = snake->tail;
  snake->map[make_pair(snake->tail->X, snake->tail->Y)] = 1;
  snake->direction = 'w';
  snake->score = 0;
  food = (struct Node *)new Node();
  food->value = 'O';
  food->next = NULL;
  food->X = rand() % M;
  food->Y = rand() % N;
}
void printGame() {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      boundary[i][j] = ' ';
    }
  }
  // asigning values
  boundary[food->X][food->Y] = food->value;
  struct Node *temp = snake->head;
  while (temp != NULL) {
    boundary[temp->X][temp->Y] = temp->value;
    temp = temp->next;
  }
  // printing
  setColor(BLUE);
  cout << "SCORE = " << snake->score << endl;
  setColor(WHITE);
  for (int i = 0; i < N + 2; i++) {
    cout << "!";
  }
  cout << "\n";
  for (int i = 0; i < M; i++) {
    cout << "!";
    for (int j = 0; j < N; j++) {
      setColor(GREEN);
      if (boundary[i][j] == 'O')
        setColor(YELLOW);
      cout << boundary[i][j];
    }
    setColor(WHITE);
    cout << "!";
    cout << "\n";
  }
  for (int i = 0; i < N + 2; i++) {
    cout << "!";
  }
  cout << "\n";
}
void saveGame() {
  printf("Do you want to save the game?(Y/N)\n");
  char isGameSaved = getch();
  if (isGameSaved == 'y' || isGameSaved == 'Y') {
    fstream fptr;
    fptr.open("savedGame.txt", ios::out);
    if (fptr) {
      struct Node *temp = snake->head;
      while (temp != NULL) {
        fptr << "S" << temp->X << "," << temp->Y << "," << temp->value << " ";
        temp = temp->next;
      }
      fptr << "+" << snake->score << " "
           << ">" << snake->direction << " ";
      fptr << "F" << food->X << "," << food->Y << "," << food->value;
    }
    fptr.close();
    setColor(GREEN);
    cout << "Saved succesfully";
  }
  exit(0);
}
void loadGame() {
  fstream fin;
  fin.open("savedGame.txt", ios::in);
  string str = "";
  getline(fin, str);
  snake = (struct Snake *)new Snake();
  food = (struct Node *)new Node();
  int i = 0;
  struct Node *prev = (struct Node *)new Node();
  prev = NULL;
  while (i < str.length()) {
    if (str[i] == 'S') {
      struct Node *node = (struct Node *)new Node();
      i++;
      string x = "";
      while (str[i] != ',') {
        x += str[i];
        i++;
      }
      node->X = atoi(x.c_str());
      i++;
      string y = "";
      while (str[i] != ',') {
        y += str[i];
        i++;
      }
      node->Y = atoi(y.c_str());
      node->value = str[++i];

      snake->map[make_pair(node->X, node->Y)] = 1;
      if (snake->head == NULL && prev == NULL) {
        snake->head = node;
        prev = node;
      }

      else {
        prev->next = node;
        prev = node;
      }
      if (node->value == '*')
        snake->tail = node;
      i++;
    } else if (str[i] == '+') {
      string score = "";
      while (str[i] != ' ') {
        score += str[i];
        i++;
      }
      snake->score = atoi(score.c_str());
    } else if (str[i] == '>')
      snake->direction = str[++i];
    else if (str[i] == 'F') {
      i++;
      string x = "";
      while (str[i] != ',') {
        x += str[i];
        i++;
      }
      food->X = atoi(x.c_str());
      i++;
      string y = "";
      while (str[i] != ',') {
        y += str[i];
        i++;
      }
      food->Y = atoi(y.c_str());
      food->value = str[++i];
    }
    i++;
  }
}
bool delay() {
  clock_t t = clock();
  while (clock() < t + 4 * CLOCKS_PER_SEC) {
    if (kbhit() != 0) {
      return true;
    }
  }
  return false;
}
bool validMove(char dir) {
  if (valid_input[dir] == "" ||
      valid_input[snake->direction] == "up" && valid_input[dir] == "down" ||
      valid_input[snake->direction] == "down" && valid_input[dir] == "up" ||
      valid_input[snake->direction] == "left" && valid_input[dir] == "right" ||
      valid_input[snake->direction] == "right" && valid_input[dir] == "left")
    return false;
  return true;
}
char input() {
  char ch;

  bool deleteText = false;
  do {
    if (delay() == false)
      return snake->direction;
    ch = getch();
    if (ch == -32) {
      ch = getch();
      if (ch == 72) {
        ch = 'w';
      } else if (ch == 80) {
        ch = 's';
      } else if (ch == 75) {
        ch = 'a';
      } else if (ch == 77) {
        ch = 'd';
      }
    } else if (ch == 27) {
      saveGame();
    }
    if (!validMove(ch)) {
      if (!deleteText) {
        setColor(LIGHTRED);
        gotoxy(0, M + 3);
        cout << "INVALID INPUT";
        deleteText = true;
      }
      ch = '0';
    }
  } while (ch == '0');
  if (deleteText) {
    gotoxy(0, M + 3);
    cout << "              ";
  }
  return ch;
}
void shift(int *x, int *y, char newDirection) {
  struct Node *temp = snake->head;
  setColor(GREEN);
  gotoxy(*y + 1, *x + 2);
  cout << "Q";
  gotoxy(temp->Y + 1, temp->X + 2);
  cout << "#";
  while (temp != NULL) {
    snake->map[make_pair(*x, *y)]++;
    snake->map[make_pair(temp->X, temp->Y)]--;
    swap(temp->X, *x);
    swap(temp->Y, *y);
    temp = temp->next;
  }
  gotoxy(snake->tail->Y + 1, snake->tail->X + 2);
  cout << "*";
  gotoxy(*y + 1, *x + 2);
  cout << " ";
  snake->direction = newDirection;
}
int eatingFood(int x, int y) {
  int foodCreated = 1;
  if (snake->map[make_pair(food->X, food->Y)] != 0) {
    snake->tail->value = '#';
    gotoxy(snake->tail->Y + 1, snake->tail->X + 2);
    cout << '#';
    struct Node *newNode = (struct Node *)new Node();
    newNode->X = x;
    newNode->Y = y;
    newNode->value = '*';
    newNode->next = NULL;
    gotoxy(newNode->Y + 1, newNode->X + 2);
    cout << '*';
    snake->tail->next = newNode;
    snake->tail = snake->tail->next;
    snake->map[make_pair(newNode->X, newNode->Y)] = 1;
    foodCreated = 0;
    snake->score++;
    gotoxy(8, 0);
    cout << snake->score;
  }
  return foodCreated;
}
void ifCollides() {
  if (snake->map[make_pair(snake->head->X, snake->head->Y)] > 1) {
    setColor(RED);
    system("cls");
    cout << "GAME OVER!!";
    setColor(WHITE);
    exit(0);
  }
}
void move() {
  char newDirection = input();
  int x = snake->head->X, y = snake->head->Y;
  if (newDirection == 'w') {
    x -= 1;
    if (x < 0) {
      x = M - 1;
    }
  } else if (newDirection == 's') {
    x += +1;
    if (x == M)
      x = 0;
  } else if (newDirection == 'a') {
    y -= 1;
    if (y < 0)
      y = N - 1;
  } else if (newDirection == 'd') {
    y += 1;
    if (y == N)
      y = 0;
  }
  shift(&x, &y, newDirection);
  foodCreated = eatingFood(x, y);
  ifCollides();
  if (foodCreated == 0) {
    food->X = rand() % M;
    food->Y = rand() % N;
    while (snake->map[make_pair(food->X, food->Y)] != 0) {
      food->X = rand() % M;
      food->Y = rand() % N;
    }
    foodCreated = 1;
    setColor(YELLOW);
    gotoxy(food->Y + 1, food->X + 2);
    cout << 'O';
  }
}
int main() {

  font();
  char start = startGame();
  if (start == '1')
    setup();
  else if (start == '2') {
    loadGame();
  } else {
    setColor(WHITE);
    exit(0);
  }
  printGame();
  while (1) {
    move();
  }
}
