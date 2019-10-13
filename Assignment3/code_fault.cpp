#include<iostream>
#include<fstream>
#include<string>

using namespace std;

//string readF = "HungerGames_edit.txt";
//string ignoreF = "ignoreWords.txt";
bool DEBUG = false;

//a struct containing string word and int count for the occurrances of the word
struct wordItem{
	string word;
	int count;

	//default constructor
	wordItem(){
		count = 0;
		word = "";
	}

	//constructor when a word is passed in, count becomes 1
	wordItem(string wrd){
		count = 1;
		word = wrd;
	}

	//returns the string word of the struct
	string getWord(){
		return word;
	}

	//returns the int count of the occurances of the word
	int getCount(){
		return count;
	}

	//when called, count is incremented by 1
	void occurrance(){
		++count;
	}
};

//class containing arrays of the words from the passed in file and methods to manipulate the data
class WordAnalysis{
	//private class members
	private:
	int totalWords = 0;
	int wordsCount = 0; //amount of words stored in the array *words
	int wordsSize = 100; //size of the array *words
	int doubles = 0; //amount of times the array size was doubled
	wordItem *words = new wordItem[wordsSize]; //array containing struct wordItems
	string stopWords[50]; //array containing stopWords
	public:
	//default constructor for the class
	WordAnalysis(){
	}

	//Fill array stopWords
	void fillStopWords(ifstream &ifs, string ignoreF){
		//open the ignore words file
		ifs.open(ignoreF);
		string wrd;
		//loop through each index in the array and set each index to a new stop word
		for(int i = 0; i < 50 && ifs.good(); i++){
			ifs >> wrd;
			if(DEBUG)cout<<"Adding ignore word: "<<wrd<<":"<<endl;
			stopWords[i] = wrd;
		}
		//deallocate the file stream
		ifs.close();
	}

	//Double the size of the array *words
	void doubleArray(){
		if(DEBUG) cout<<"Doubling Array Size from "<<wordsSize<<" to "<<wordsSize * 2<<endl;
		//create a temporary array twice the size of the original
		wordItem *temp = new wordItem[wordsSize * 2];
		//fill the temporary array with the original array
		for(int i = 0; i < wordsSize; i++){
			temp[i] = words[i];
		}
		//delete the original array
		delete[]words;
		//point the original array to the temp array, which is twice the size
		words = temp;
		//update wordsSize to the new array size
		wordsSize *= 2;
		//increment the amount of double operations performed
		++doubles;
	}

	//Check if the word is a stopWord
	bool isStopWord(string wrd){
		//loop through the stopWords array to see if the passed in word matches an element
		for(int i = 0; i < 50; i++){
			//if(DEBUG)cout<<"Comparing "<<wrd<<" to "<<stopWords[i]<<":"<<endl;
			if(wrd == stopWords[i]){
				if(DEBUG) cout<<"Stop word: "<<wrd<<endl;
				//if a match is found return true
				return true;
			}
		}
		//if a match is not found, return false
		return false;
	}

	//Construct the array containing all words
	void buildWords(ifstream &ifs, string readF){
		//open the readF file with the ifstream
		ifs.open(readF);
		string wrd;
		//read each line
		while(ifs.good()){
			//pass each word into string wrd
			ifs >> wrd;
			//check if the word is a stopword
			if(!isStopWord(wrd)){
				//check if array size has to be increased
				if(wordsCount >= wordsSize){
					if(DEBUG)cout<<"Calling doubleArray"<<endl;
					this->doubleArray();	
				}
				//add the word
				addWord(wrd);
			}
		}
	}

	//Add a word to *words considering order
	void addWord(string wrd){
		if(DEBUG)cout<<"Array size: "<<wordsSize<<", count: "<<wordsCount<<endl;
		//boolean to see if the word already exists in the array
		bool exists = false;
		//loop through the existing array
		for(int i = 0; i < wordsCount; i++){
			//check if a match is found with the addword and an element in the array
			if(words[i].getWord() == wrd){
				//match is found so exists is true
				exists = true;
				//add 1 to the total words count
				++totalWords;
				if(DEBUG)cout<<"Incrementing word: '"<<wrd<<"' at index "<<i<<endl;
				//add 1 to the count of the word, since it exists
				words[i].occurrance();
				//Move word up the list until the wordItem count is equal to the one above it
				while(i != 0 && words[i].getCount() > words[i-1].getCount()){
					if(DEBUG)cout<<"index "<<i-1<<" w/ count "<<words[i-1].getCount()<<" is less than index "<<i<<" w/ count "<<words[i].getCount()<<endl;
					//swap the two elements if the item above has a lesser count than the item below
					wordItem temp = words[i];
					words[i] = words[i-1];
					words[i-1] = temp;
					--i;
				}
				//end the loop to search for a match
				break;
			}
		}
		//if the word was not found as existing
		if(!exists){
			//add the element to the array
			if(DEBUG)cout<<"Adding word: '"<<wrd<<"' to index "<<wordsCount<<endl;
			wordItem wrdStruct(wrd);
			words[wordsCount] = wrdStruct;
			//add 1 to the word count
			++wordsCount;
			//add 1 to the total word count
			++totalWords;
		}
	}

	//print printN of the top of the array *words
	void printTop(int printN){
		for(int i = 0; i < printN; i++){
			cout<<words[i].getCount()<<" - "<<words[i].getWord()<<endl;
		}	
	}

	//print printN of the bottom of the array *words
	void printBottom(int printN){
		for(int i = wordsCount - 1; i >= wordsCount - printN; i--){
			cout<<words[i].getCount()<<" - "<<words[i].getWord()<<endl;
		}	
	}

	//print the entire array *words
	void printWords(){
		for(int i = 0; i < wordsCount; i++){
			cout<<words[i].getCount()<<" - "<<words[i].getWord()<<endl;
		}
	}

	//print singular element of the *words array
	void printWord(string word){
		for(int i = 0; i < wordsCount; i++){
			if(words[i].getWord() == word) cout<<words[i].getWord()<<" - "<<words[i].getCount()<<endl;
		}	
	}

	//get the amount of times the *words array doubled
	int getDoubles(){
		return doubles;
	}
	
	//get the number of unique non-common words (entries in *words)
	int getWords(){
		return wordsCount;
	}

	//get the number of non-common words (how many times addWord was called)
	int getWordsTotal(){
		return totalWords;
	}

	//search for word from command line arguments in *words
	void searchCount(string arg){
		//delimete
		char del = ',';
		//final and beginning delimeter, if any
		int end = arg.rfind(del);
		int index = arg.find(del);

		//check if delimeter exists in the string
		if(end == string::npos){
			//if no delimeter, print word if string exists
			if(sizeof(arg) > 0){
				printWord(arg);
			}else{
				//string is empty, don't check for word
				cout<<"no argument"<<endl;
			}
		} else {
			//print word up to comma
			printWord(arg.substr(0,arg.find(del)));
			//keep printing words so long as there are commas
			while(index != end){
				//print word from previous comma +1 up to the next comma
				printWord(arg.substr(index+1,arg.find(del,index+1)-index-1));
				//update comma marker
				index = arg.find(del, index+1);
			}
			//print final word past last comma
			printWord(arg.substr(end+1));
		}
	}

};

int main(int argc, char *argv[]){
	ifstream ifs; //creation of the filestream
	
	//create the WordAnalysis class
	WordAnalysis wordAnalysis;
	wordAnalysis.fillStopWords(ifs, argv[3]);

	//outputs
	wordAnalysis.printTop(stoi(argv[1]));
	cout<<"#"<<endl;
	wordAnalysis.printBottom(stoi(argv[1]));
	cout<<"#"<<endl;
	cout<<"Array doubled: "<<wordAnalysis.getDoubles()<<endl;
	cout<<"#"<<endl;
	cout<<"Unique non-common words: "<<wordAnalysis.getWords()<<endl;
	cout<<"#"<<endl;
	cout<<"Unique number of non-common words: "<<wordAnalysis.getWordsTotal()<<endl;
	cout<<"#"<<endl;
	wordAnalysis.searchCount(argv[4]);
}
