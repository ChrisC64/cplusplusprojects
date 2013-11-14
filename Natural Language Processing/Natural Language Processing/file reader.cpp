#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include<queue>
#include "file reader.h"
using namespace std;

// Create const array location of uni/bi/tri gram files-make writeable
ofstream BigramObj("ParseFiles\\bigram.txt", ios::trunc);
ofstream UnigramObj("ParseFiles\\unigram.txt", ios::trunc);
ofstream TrigramObj("ParseFiles\\trigram.txt", ios::trunc);
// File location of our corpus
ifstream CorpusObj;

// Constant for file name
const char UniFile[] = "ParseFiles\\uniram.txt";
const char BiFile[] = "ParseFiles\\bigram.txt";
const char TriFile[] = "ParseFiles\\trigram.txt";
const char CorpusFile[] = "ParseFiles\\The-Great-Gatsby.txt";

// Stack for storing data
//queue<string> nGramQueue;			// Used for bi and tri grams
queue<string> stringLineQueue;		// Used for unigram
double wordCount = 0;				// Total words in document
double percent = 0.0;				// Percentage of word appearance
// Map used for storing values of words
map<string, int> wordMap;
map<string, int> biWordMap;
map<string,int> triWordMap;

// Depth First Search (DFS) variables
queue<string> sQueue;

//parse functions
//string *unigramParse(string);
//string *bigramParse(string);
//string *trigramParse(string);
//string *parseString(string);
//parse the file, search through the text, and retrieve the n-gram of the file
void parseFile()
{
	CorpusObj.open("ParseFiles\\The-Great-Gatsby.txt", ios::in);
	//check if file is open.
	CorpusObj.seekg(ios::beg);
	if(CorpusObj.is_open())
	{
		do//read through file, and only save 'a-z', and '. ! ?' notations.
		{
			string lineString; 
			getline(CorpusObj, lineString);
			//store n-gram in string
			for(unsigned int i = 0; i < lineString.size(); i++)
			{
				lineString[i] = tolower(lineString[i]);//lower n-gram
			}
			//place in queue
			unigramParse(lineString);//this function will queue up the words, and then put them into the map
		}
		while(!CorpusObj.eof());
		//place map into file
		transferMap();
	}
	else
	{
		cout << "Error: Cannot Open File!\n";
	}
	CorpusObj.close();
}
//bigram parse
void biparseFile()
{
	CorpusObj.open("ParseFiles\\The-Great-Gatsby.txt", ios::in);
	CorpusObj.seekg(ios::beg);
	if(CorpusObj.is_open())
	{
		do//read through file, and only save 'a-z', and '. ! ?' notations.
		{
			string lineString;
			getline(CorpusObj, lineString);
			//store n-gram in string
			for(unsigned int i = 0; i < lineString.size(); i++)
			{
				lineString[i] = tolower(lineString[i]);
			}
			bigramParse(lineString);//this function will queue up the words, and then put them into the map
		}
		while(!CorpusObj.eof());
		//place map into file
		BiTransferMap();
	}
	else
	{
		cout << "Error: Cannot Open File!\n";
	}
	CorpusObj.close();
}
//trigram parse
void triparseFile()
{
	CorpusObj.open("ParseFiles\\The-Great-Gatsby.txt", ios::in);
	//check if file is open.
	CorpusObj.seekg(ios::beg);
	if(CorpusObj.is_open())
	{
		do
		{
		//read through file, and only save 'a-z', and '. ! ?' notations. 
			string lineString;
			getline(CorpusObj, lineString);
		//store n-gram in string
			for(unsigned int i = 0; i < lineString.size(); i++)
			{
		//lower n-gram
				lineString[i] = tolower(lineString[i]);
			}
		//place in queue
			/*string *pString;
			pString = &lineString;*/
			trigramParse(lineString);//this function will queue up the words, and then put them into the map
		}
		while(!CorpusObj.eof());
		//place map into file
		TriTransferMap();
	}
	else
	{
		cout << "Error: Cannot Open File!\n";
	}
	CorpusObj.close();
}
//add the string from parameter into the queue
string *unigramParse(string theLine)
{
	string lineWord;
	pair<map<string, int>::iterator,bool> ret;
	for(unsigned int i = 0; i < theLine.size(); i++)
	{
		if(theLine[i] >= 'a' && theLine[i] <= 'z' )
		{
			//save theLine into a char queue
			lineWord += theLine[i];
		}
		else if(theLine[i] == '.' || theLine[i] == '!' || theLine[i] == '?')
		{
			if(!lineWord.empty())//If lineWord isn't empty (due to double space), then store the word in lineWord
			{
				stringLineQueue.push(lineWord);
				lineWord.clear();
				wordCount++;
			}
			else//if lineWord is empty, due to a double spacing, do nothing and move on. 
			{

			}
		}
		else if(theLine[i] == ' ')
		{
			if(!lineWord.empty())//If lineWord isn't empty (due to double space), then store the word in lineWord
			{
				stringLineQueue.push(lineWord);
				lineWord.clear();
				wordCount++;
			}
			else//if lineWord is empty, due to a double spacing, do nothing and move on. 
			{

			}
		}
		//else
		//{
		//	//when the word has ended, store it into the a string queue
		//	stringLineQueue.push(lineWord);
		//	lineWord.clear();
		//	wordCount++;
		//}
	}
	
	//Perform these actions while the stringLineQueue isn't empty: Take word from front of Queue and add to lineWord, insert word into map,
	//if map contains key, increase value, else set value to 1, then pop out item from stringLineQueue. 
	while(!stringLineQueue.empty())
	{
		lineWord = stringLineQueue.front();
	//else store the key and value = 1
		ret = wordMap.insert(pair<string,int>(lineWord, 1));//if key is inserted for the first time, ret == true, if key already exists ret == false
		if(ret.second == false)
		{
			wordMap[lineWord]++;//increment value of key if already in use
		}
		stringLineQueue.pop();//pop out from queue
	}
	string *pString;
	pString = &lineWord;
	return pString;
}

//parse the file for bigram
string *bigramParse(string theLine)
{
	string lineWord;
	pair<map<string, int>::iterator,bool> ret;
	for(unsigned int i = 0; i < theLine.size(); i++)
	{
		if(theLine[i] >= 'a' && theLine[i] <= 'z' )
		{
			//save theLine into a char queue
			lineWord += theLine[i];
		}
		else if(theLine[i] == '.' || theLine[i] == '!' || theLine[i] == '?')
		{
			//if the line is a ., !, ?, or \n then save
			stringLineQueue.push(lineWord);
			if(stringLineQueue.size() == 2)
			{
				lineWord = stringLineQueue.front();
				stringLineQueue.pop();
				lineWord += " ";
				lineWord += stringLineQueue.front();
				//when stringLineQueue is filled with 2 elements, place into map. 
				ret = biWordMap.insert(pair<string,int>(lineWord, 1));//if key is inserted for the first time, ret == true, if key already exists ret == false
				if(ret.second == false)
				{
					biWordMap[lineWord]++;
				}
				lineWord.clear();
				//stringLineQueue.pop();
			}
			lineWord = theLine[i];
			stringLineQueue.push(lineWord);
			lineWord.clear();
			wordCount++;
			//if it is a space, then do not save and do nothing
		}
		else if(theLine[i] == ' ')
		{
			if(!lineWord.empty())//If lineWord isn't empty (due to double space), then store the word in lineWord
			{
				stringLineQueue.push(lineWord);
				lineWord.clear();
				wordCount++;
			}
			else//if lineWord is empty, due to a double spacing, do nothing and move on. 
			{

			}
		}
		//else
		//{
		//	//when the word has ended, store it into the a string queue
		//	stringLineQueue.push(lineWord);
		//	lineWord.clear();
		//	wordCount++;
		//}
		if(stringLineQueue.size() == 2)
		{
			lineWord = stringLineQueue.front();
			stringLineQueue.pop();
			lineWord += " ";
			lineWord += stringLineQueue.front();
			//when stringLineQueue is filled with 2 elements, place into map. 
			ret = biWordMap.insert(pair<string,int>(lineWord, 1));//if key is inserted for the first time, ret == true, if key already exists ret == false
			if(ret.second == false)
			{
				biWordMap[lineWord]++;
			}
			lineWord.clear();
			//stringLineQueue.pop();
		}
	}
	
	//if the map contains the key, increment the value of that key
	//while(stringLineQueue.size() > 2)
	//{
	//	
	//	lineWord = stringLineQueue.front();
	////else store the key and value = 1
	//	ret = wordMap.insert(pair<string,int>(lineWord, 1));//if key is inserted for the first time, ret == true, if key already exists ret == false
	//	if(ret.second == false)
	//	{
	//		wordMap[lineWord]++;
	//	}
	//	stringLineQueue.pop();
	//}
	string *pString;
	pString = &lineWord;
	return pString;
}
//parse the file for a trigram
string *trigramParse(string theLine)
{
	string lineWord;
	pair<map<string, int>::iterator,bool> ret;
	for(unsigned int i = 0; i < theLine.size(); i++)
	{
		if(theLine[i] >= 'a' && theLine[i] <= 'z' )
		{
			//save theLine into a char queue
			lineWord += theLine[i];
		}
		else if(theLine[i] == '.' || theLine[i] == '!' || theLine[i] == '?')
		{
			//if the line is a ., !, ?, or \n then save
			stringLineQueue.push(lineWord);
			if(stringLineQueue.size() == 3)
			{
				lineWord = stringLineQueue.front();//load the first element into the string
				stringLineQueue.pop();//remove the first element
				lineWord += " ";
				lineWord += stringLineQueue.front();//load the next element
				lineWord += " ";
				lineWord += stringLineQueue.back();
				//when stringLineQueue is filled with 2 elements, place into map. 
				ret = triWordMap.insert(pair<string,int>(lineWord, 1));//if key is inserted for the first time, ret == true, if key already exists ret == false
				if(ret.second == false)
				{
					triWordMap[lineWord]++;//increment value if key is already in use
				}
				lineWord.clear();
			}
			lineWord = theLine[i];
			stringLineQueue.push(lineWord);
			lineWord.clear();
			wordCount++;
			//if it is a space, then do not save and do nothing
		}
		else if(theLine[i] == ' ')
		{
			if(!lineWord.empty())//If a space is detected, load the word, then clear the string
			{
				stringLineQueue.push(lineWord);
				lineWord.clear();
				wordCount++;
			}
			else
			{
				//if string is empty, do nothing and continue actions. 
				//String should be empty if we previously encountered a space, and cleared the string from previous check,
				//and that next element in theLine[index] was another space (double space)
			}
		}
		if(stringLineQueue.size() == 3)
		{
			lineWord = stringLineQueue.front();//load the first element into the string
			stringLineQueue.pop();//remove the first element
			lineWord += " ";
			lineWord += stringLineQueue.front();//load the next element
			lineWord += " ";
			lineWord += stringLineQueue.back();
			//when stringLineQueue is filled with 2 elements, place into map. 
			ret = triWordMap.insert(pair<string,int>(lineWord, 1));//if key is inserted for the first time, ret == true, if key already exists ret == false
			if(ret.second == false)
			{
					triWordMap[lineWord]++;//increment value if key is already in use
			}
			lineWord.clear();
		}
		//else
		//{
		//	//when the word has ended, store it into the a string queue
		//	stringLineQueue.push(lineWord);
		//	lineWord.clear();
		//	wordCount++;
		//}
	}
	
	//if the map contains the key, increment the value of that key
	//while(!stringLineQueue.empty())
	//{
	//	lineWord = stringLineQueue.front();
	////else store the key and value = 1
	//	ret = wordMap.insert(pair<string,int>(lineWord, 1));//if key is inserted for the first time, ret == true, if key already exists ret == false
	//	if(ret.second == false)
	//	{
	//		wordMap[lineWord]++;
	//	}
	//	stringLineQueue.pop();
	//}
	string *pString;
	pString = &lineWord;
	return pString;
}

//this Function will place the wordMap into the file
void transferMap()
{
	map<string, int>::iterator it;
	for(it = wordMap.begin(); it != wordMap.end(); it++)
	{
		double value = (*it).second;
		percent = value/wordCount;
		//print out first and second of map
		UnigramObj << (*it).first << "  " << (*it).second << "  " << fixed << percent << endl;
	}
	//wordMap.clear();//clear map after use, before using for next grams
	return;
}
//transfer function for bigram
void BiTransferMap()
{
	map<string, int>::iterator it;
	for(it = biWordMap.begin(); it != biWordMap.end(); it++)
	{
		double value = (*it).second;
		percent = value/wordCount;
		//print out first and second of map
		BigramObj << (*it).first << "  " << (*it).second << "  " << fixed << percent << endl;
	}
	biWordMap.clear();//clear map after use, before using for next grams
	return;
}
//transfer function for trigram
void TriTransferMap()
{
	map<string, int>::iterator it;
	for(it = triWordMap.begin(); it != triWordMap.end(); it++)
	{
		double value = (*it).second;
		percent = value/wordCount;
		//print out first and second of map
		TrigramObj << (*it).first << "  " << (*it).second << "  " << fixed << percent << endl;
	}
	triWordMap.clear();//clear map after use, before using for next grams
	return;
}

// This function will parse the string from the user and allow us parse the string sentence; should use a timer to time out if it takes too long.
// NOTE: The 4's we use are number of "tries" but this shouldn't be hard coded. 
void parseString(string userLine)
{
	string lineString;
	string parseString;
	string userString = userLine;
	unsigned int count = 0;
	bool bSearch = false;
	int zeroCount = 0;
	while(count < 100 && !bSearch)
	{
		// As the letters are added into this new string, check that string again against the map
		parseString += userString[count];
		// If map.count(string) == 1, continue operation, 
		if(wordMap.count(parseString) == 1)  
		{
			// If word is found with added letter, add to count
			count++;    // Increment count
			zeroCount = 0; // Reset to 0 everytime we find a word
		}
		else if (wordMap.count(parseString) == 0) // Else if false, remove added letter and decrement count, then add the word
		{
			zeroCount++; // This counter will see how many times map.count(string) == 0. If it is false twice, we will begin operations below
			if(zeroCount >= 2 && zeroCount <= 4 || count >= userString.size())
			{
				if(zeroCount < 4)
				{
					//count++;
					//zeroCount = 0;//reset counter
				}
				else if(zeroCount == 4)
				{
					// Remove the word we loaded into the parse string after 4 tries 
					for(unsigned int i = 0; i < 4; i++)
					{
					parseString.erase(parseString.end()-1);
					count--;
					}
					zeroCount = 0;

					// Push the word we believe could be in our word bank and store for display later
					sQueue.push(parseString);
					parseString.clear();
					//count++;
				}
				else if (count >= userString.size())
				{
					sQueue.push(parseString);
					parseString.clear();
				}
				count++;
			}
			else
			{
				//do nothing
				count++;
			}
			
		}
		if(count >= userString.size())//when at the final location of the word, enter word into queue
		{
			if(!sQueue.empty())
			{
				sQueue.push(parseString);
			}
			bSearch = true;//notify that you've reached the end of your search
		}
	}
		cout << "\nI believe this is your sentence: \n";
		while(!sQueue.empty())
		{
			cout << sQueue.front();
			cout << " ";
			sQueue.pop();
		}
	//return &userString;
}

//void IDDFS(string userString)
//{
//	int maxDepth = 0;
//	bool bSearch = false;
//	int count = 0;
//	string resultString;
//	while(!bSearch && maxDepth < 1000)
//	{
//		resultString = DFS(maxDepth, userString, count, "");//enter DFS
//		//increment depth if resultString is empty
//		if(resultString.empty())
//		{
//			maxDepth++;
//			bSearch = false;
//			while(!sQueue.empty())
//			{
//				sQueue.pop();
//			}
//		}
//		else if(!resultString.empty())
//		{
//			bSearch = true;
//		}
//	}
//	cout << "We may have parsed your sentence: " << endl;
//	cout << resultString << endl;
//}
//DFS search
//string DFS(int depth, string line, int count, string currentParse)
//{
//	//check the string, and then find a value for each letter. If the letter returns no check, then save the letter, and move on. 
//	//as the letters are added into this new string, check that string again against the map
//	string parse = currentParse;//this will be the string we parse our lineParseString into
//	string lineParseString = line;//this will be the line passed by the user
//	parse += lineParseString[count];//the element at index [count] will be placed into the parsed string for checking
//	string retString;//this string is the one returned at the end when we find our parsed string
//	//if the value is defined in the map, add the string to our new line string, (parser) and add a space. 
//		if(wordMap.count(parse) == 1 && depth > 0)//if key is found, go deeper
//		{
//			//continue search
//			count++;
//			retString = DFS(depth - 1, lineParseString, count, parse);
//			if(retString.empty())
//			{
//				return retString;
//			}
//			else
//			{
//				return retString; //if not empty, return retString
//			}
//		}
//		else if(wordMap.count(parse) == 1 && depth <= 0)//this check should happen when the last word is reached
//		{
//			//check if at end of string
//			if(count < lineParseString.size()-1)
//			{
//				count++;//incrememnt count
//				parse += lineParseString[count];
//				if(wordMap.count(parse) == 1)
//				{
//					sQueue.push(parse);//load the queue
//					retString.clear();
//					return retString;
//				}
//				else if(wordMap.count(parse) == 0)
//				{
//					parse.erase(parse.end()-1);
//					sQueue.push(parse);
//					//parse.clear();
//					//parse += lineParseString[count];
//					count--;
//					retString.clear();
//					return retString;
//				}
//			}
//			else if(count > lineParseString.size()-1)
//			{
//				//if at end of userString, save final word, then return true
//				sQueue.push(parse);
//				while(!sQueue.empty())
//				{
//					retString += sQueue.front();
//					sQueue.pop();
//					retString += " ";
//				}
//				return retString;
//			}
//		}
//		else if(wordMap.count(parse) == 0 && depth > 0)
//		{
//			//if key does not exist, check if removing last value from string will == 1, if so, add word, if == 0, then continue search
//			count++;
//			retString = DFS(depth - 1, lineParseString, count, parse);
//			//return retString;
//			parse.erase(parse.end()-1);
//			if(wordMap.count(parse) == 1)
//			{
//				//add word to queue, add current letter again into parse, and send current parse into dfs
//				//count++;
//				//retString = DFS(depth-1, lineParseString, count, parse);
//				sQueue.push(parse);
//				parse.clear();//clear parse after push
//				parse += lineParseString[count];//begin parse with new letter in index				
//				if(!retString.empty() && wordMap.count(retString) == 1)
//				{
//					parse = retString;
//					return retString;
//					//sQueue.push(parse);//push into queue
//				}
//				else
//				{
//					retString.clear();
//					return retString;
//				}
//			}
//			else if (wordMap.count(parse) == 0)
//			{
//				//continue into dfs
//				//parse += lineParseString[count];//add back the removed element
//				//count++;
//				retString.clear();
//				return retString;
//			}
//		}
//		else if(wordMap.count(parse) == 0 && depth <= 0)
//		{
//			//return false, increment depth
//			retString.clear();
//			return retString;
//		}
//		else
//		{
//			retString.clear();
//			return retString;
//		}
//	//cout << "This is your string parsed: " << endl;
//	//cout << lineString << endl;
//}