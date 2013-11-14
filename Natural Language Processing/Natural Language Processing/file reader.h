#ifndef FILE_READER_H
#define FILE_READER_H
#include <string>
using namespace std;
//#include <string>
//declare functions for reading and writing files
//parsing file function - parses file
//-The parse file function will search through and only count a-z, space, and ". ! ?" notations.
void parseFile();
void biparseFile();
void triparseFile();
//parse a user string
//string *parseString(string);
string *unigramParse(string);
string *bigramParse(string);
string *trigramParse(string);
void parseString(string);
//DFS search
string DFS(int, string, int, string);
void IDDFS(string);
//write map data to file
void transferMap();
void BiTransferMap();
void TriTransferMap();
//close the file
void close();
#endif FILE_READER_H