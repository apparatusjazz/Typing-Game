#include <iostream>
#include <chrono>
#include <string>
#include <cctype>
#include <ctime>
#include <cstdlib>
#include <cmath>
using namespace std;

/*Typing game - generates string of length 5
40% chance for lowercase alphabetic
40% chance for uppercase alphabetic
10% chance for number digit [0-9] ; wildcard
10% chance for other symbols [%-!]; wildcard
2000 points initially, ends when at 0 or at 5000
+ 500 points when typed correctly within 10 sec or 10,000 ms time
If over time, points lost is number of ms after 10,000
If mispelled, points lost is the difference of ASCII code from typed character to generated character, if length is different, pad with space
If mispelled and over time, ASCII difference is double and time ms penalty added*/

string generate(const int length); //Generates random 5 character strings
void convert(string& type_it, string& userInput);//Converts to allow any digit / symbol type from user input
void changeDisplay(string& display_it); //Replaces '0' and '*' characters with [0-9] and [%-!]
void padWithSpace(string& userInput); //Pads strings with length < 5 with spaces
int numoffset(const string& type_it, const string& userInput); //Calculates ASCII absolute value difference


int main() {

  int points = 2000, penalty = 0, offset = 0, standardmsec = 10000;
  const int length = 5;
  string userInput, type_it, display_it;

  while(points < 5000 && points > 0){

    type_it = generate(length);
    display_it = type_it;
    changeDisplay(display_it);

    cout << "Your current points " << points << ", just type -> " << display_it << ": ";
    auto start = chrono::steady_clock::now(); //Starts clock to measure time
    cin >> userInput;

    auto end = chrono::steady_clock::now();
    int mseconds = chrono::duration_cast<chrono::milliseconds>(end - start).count(); //get milliseconds

    cout << mseconds << " milliseconds, you ";
    if(mseconds > standardmsec) cout << "*failed*";
    else cout << "made";
    cout << " it within the interval of 10000...\n";

    padWithSpace(userInput);
    convert(type_it, userInput);
    offset = numoffset(type_it, userInput);
    penalty = offset;

    if(offset > 0 && mseconds > standardmsec){
      points = points - 2*offset - mseconds + standardmsec;
      penalty = 2*offset - standardmsec + mseconds;
    }
    else if(offset > 0){
      points = points - offset;
    }
    else if(mseconds > standardmsec){
      points = points - mseconds + standardmsec;
    }
    else
      points += 500;

    if(offset > 0){
      cout << "String offset is " << offset << " your total penalty is " << penalty << "...\n";
    }

  }
  cout << "Bye..." << endl;
  return 0;
}

/*
Random string generated with string generate function below
*/
string generate(const int length){ 
  srand(time(0));
  string type_it;

  for(int i = 0; i < length; i++){
    int a = (rand() % 100) + 1;
    if(a <= 10){ //numbers
      type_it.push_back('0');
    }
    else if(a <= 20){ //symbols
      type_it.push_back('*');
    }
    else if(a <= 60){ //lowercase alphabet
      int b = (rand() % 26) + 97;
      type_it.push_back(b);
    }
    else{ //uppercase alphabet
      int b = (rand() % 26) + 65; 
      type_it.push_back(b);
    }
  }
  return type_it;
}

void convert(string& type_it, string& userInput){
  for(int i = 0; i < type_it.length(); i++){
    if(isdigit(userInput[i]) && isdigit(type_it[i]))
      type_it[i] = userInput[i];
    else if(!(isalnum(userInput[i]) || isalnum(type_it[i]) || userInput[i] == 32))
      type_it[i] = userInput[i];
  }
}

void changeDisplay(string& display_it){
  const string numeric = "[0-9]", symbol = "[%-!]";
  
  for(int i = 0; i < display_it.length(); i++){
      if(isdigit(display_it[i])){
        display_it.replace(i, 1, numeric);
        i += 4;
      }
      else if(!(isalnum(display_it[i]))){
        display_it.replace(i, 1, symbol);
        i += 4;
      }
    }
}

void padWithSpace(string& userInput){
  if(userInput.length() < 5){
      for(int i = userInput.length(); i <= 4; i++){
        userInput.push_back(' ');
      }
    }
}

int numoffset(const string& type_it, const string& userInput){
  int offset = 0;
  for(int i = 0; i < type_it.length(); i++){
    if(userInput[i] != type_it[i])
      offset += abs(userInput[i] - type_it[i]);
  }
  return offset;
}
