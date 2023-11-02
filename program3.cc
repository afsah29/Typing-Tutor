/* ***********************************
PROGRAM 3: Typing Tutor
CS 141, Spring 2023.
By; Afsah Ahmed
*********************************** */


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>
#include <iomanip>
using namespace std;

const int NMISP = 5;
const int RNG_RANGE = 100;

void
display_header(const int ndict, const int nmiss)
{
  cout<<"*** Typing Tutor"<<endl;
  cout<<"There are "<<ndict<<" words in the dictionary."<<endl<<"There are "<<nmiss<<" commonly misspelled words."<<endl;
}

void
display_menu_options()
{
    cout << endl << "*** Select a menu option:" << endl
         << "1. Spell commonly misspelled words" << endl
         << "2. Type random words from the full dictionary" << endl
         << "3. Display some words from one of the lists" << endl
         << "4. Use binary search to lookup a word in the dictionary" << endl
         << "5. Exit the program." << endl
         << "Your choice: ";
}

vector<int>
randn(const int n, const int max)
{
  // A vector that allows us to return random numbers
  vector<int>random_numbers;
  for(int i = 1; i <= n; i++){
    int random_number = rand();
    random_number= (random_number)%(max+1);
    random_numbers.push_back(random_number);
  }
  return random_numbers;
}

// Open file
ifstream
open(const string& src_file)
{
    ifstream src{src_file};
    if (!src.is_open()) {
        cout << "Could not find file " << src_file << "." << endl;
    }
    return src;
}

void
extract_dict(vector<string>& dst, const string& src_file)
{ 
  ifstream file = open(src_file);
  string line; 

  // To itterate over file so you can access each line and have them stored in a vector 
  while (getline(file,line)){
    dst.push_back(line);
  }
  file.close();
}

void
extract_misspelled(vector<string>& misspelled, vector<string>& correct,
                    const string& src_file)
{
  ifstream file(src_file);
  string wordpair_1;
  string wordpair_2;

  // Loops over the file and adds misspelled words and correctly spelled words into two different vectors
  while (file >> wordpair_1){
    file>>wordpair_2;
    misspelled.push_back(wordpair_1);
    correct.push_back(wordpair_2);
  }
  file.close();
}

void
display_elems(const vector<string>& src, const vector<int>& positions)
{
  for (int j = 0; j < positions.size(); j++){
    cout << src[positions[j]];
    if (j+1 != positions.size()){
      cout<<" ";
    }
  }
}

void
extract_spelling_answers(vector<string>& dst, const vector<string>& correct,
                            const vector<int> positions)
{
  for(int j = 0; j<positions.size(); j++){
    dst.push_back(correct[positions[j]]);
  }
}

void
extract_spelling_mistakes(vector<string>& dst, const string& to_grade,
                const vector<string>& correct)
{
  string answer;
  vector <string>words;

  stringstream word_input(to_grade);
  while (word_input >> answer){
    words.push_back(answer);
  }
  //displays correct spelling
  for (int i=0; i<correct.size();i++){
    if(words[i]!= correct[i]){
      dst.push_back(words[i] + " -> "+correct[i]);
    }
  }
  while (words.size() < correct.size()){
    words.push_back("NO INPUT");
  }
}

void
report_misspell_mistakes(const vector<string>& mistakes,
                            const int num_words)
{
  int correct_words;
  correct_words = num_words -mistakes.size();

  int score;
  score = (correct_words*3)-(mistakes.size()*6);

  int i;

  if (mistakes.size()==0){
    cout<<"No misspelled words!"<<endl;
  }
  else{
    cout<<"Misspelled words:"<<endl;
    for (i=0;i<mistakes.size();i++){
      cout<<"    "<<mistakes[i]<<endl;
    }
  }
//Score board
  cout<<correct_words*3<<" points: "<<correct_words<<" spelled correctly x 3 points for each."<<endl;
  cout<<mistakes.size()*6<<" point penalty: "<<mistakes.size()<<" spelled incorrectly x -6 points for each."<<endl;
  cout<<"Score: "<< score<<endl;
}

void
evaluate_user_misspells(const vector<string>& src, const vector<int>& positions)
{
  string user_input;
  getline(cin,user_input);

  vector<string>correct;
  vector<string>wrong;

  //retrieves correct spellings and stores in vector
  extract_spelling_answers(correct,src,positions);

  //retrieves mistakes and stores in vector
  extract_spelling_mistakes(wrong,user_input,correct);

  //displays score
  report_misspell_mistakes(wrong,correct.size());
  
}

void
misspelled(const vector<string>& mspelled, const vector<string>& correct)
{
  cout<<endl<<"*** Using commonly misspelled words."<<endl;
  vector<int> N;  
  N= randn(5,mspelled.size()-1);

  display_elems(mspelled, N);
  cout<<endl;
  cout<<"Type the words above with the correct spelling:"<<endl;

  evaluate_user_misspells(correct,N);

}

void
full_dict(const vector<string>& dict)
{
  cout<<endl;
  cout<<"*** Using full dictionary."<<endl;
  vector<int>N;
  N = randn(5,dict.size()-1);
  display_elems(dict,N);
  cout<<endl;
  cout<<"Correctly type the words above:"<<endl;
  evaluate_user_misspells(dict,N);
  
}

void
display_words_prompt()
{
  
    // This displays the data set choices for menu option 3
    cout << endl << "*** Displaying word list entries." << endl
         << "Which words do you want to display?" << endl
         << "A. Dictionary of all words" << endl
         << "B. Commonly misspelled words (wrongly spelled)" << endl
         << "C. Commonly misspelled words (correctly spelled)" << endl
         << "Your choice: ";
}

void
print_range(const vector<string>& data)
{
  int first;
  int last;
  int max;

  max=data.size()-1;
  cout<<"*** Enter the start and end indices between 0 and "<<max<<": ";
  
  cin>>first>>last;

  if (first <0 || data.size()<=first|| last<first || data.size()<= last){
    cout << "The end index must come after the start, and the indices must be in range."<<endl;
    return;
  }
  for (int j = first; j <= last; j++) {
    cout << j <<". "<<data[j]<<endl;
  }
}

void
display_in_range(const vector<string>& dict, const vector<string>& mspelled,
                const vector<string>& correct)
{
  //display option screen and then based on userinput show the next screen
  display_words_prompt();
  char user_input;
  cin>> user_input;

  if (user_input =='a' || user_input == 'A'){
    print_range(dict);
  }
  else if (user_input == 'b'|| user_input =='B'){
    print_range(mspelled);
  }
  else if (user_input =='c'|| user_input == 'C'){
    print_range(correct);
  }
}

void
bsearch(const vector<string>& data)
{
  string input;
  cout<<"*** Enter word to search: ";
  cin>> input;
  
  int total = 1;
  int start =0;
  int end;
  end = data.size()-1;
  int middle;

  //search for word
  while (end >= start){
    if ((start+end)%2 !=0) {
    middle = ((start+end)/2)+1;
    }
  else{
    middle = (start+end)/2;
  }

    if (total > 9) {
      cout<<"   "<<total<<". Comparing to: "<<data[middle]<<endl;
    }
    else{
      cout<<"    "<<total<<". Comparing to: "<<data[middle]<<endl;
    }

    if (data[middle]==input){
      cout<<input<<" was found."<<endl;
      return;
    }
    else if (data[middle]<input){
      start = middle+1;
    }
    else {
      end = middle-1;
    }
    total++;
  }
  cout << input<<" was NOT found."<<endl;
  
}

void
extract_all_data(vector<string>& dict, vector<string>& mspelled,
                    vector<string>& correct)
{
    extract_dict(dict, "dictionary.txt");
    extract_misspelled(mspelled, correct, "misspelled.txt");
}

int
main()
{
    srand(1);
    vector<string> dict, mspelled, correct;
    extract_all_data(dict, mspelled, correct);
    display_header(dict.size(), correct.size());
    unsigned int choice;
    do {
        display_menu_options();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (choice) {
        case 1:
            misspelled(mspelled, correct);
            break;
        case 2:
            full_dict(dict);
            break;
        case 3:
            display_in_range(dict, mspelled, correct);
            break;
        case 4:
            bsearch(dict);
            break;
        case 5:
            cout << "Exiting." << endl;
            break;
        default:
            cout << "No menu option " << choice << ". "
                 << "Please try again." << endl;
        }
    } while ((choice != 5) && !cin.eof());

    return 0;
}
