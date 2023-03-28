#ifndef MOVIE_H
#define MOVIE_H
#include <string>

using namespace std;

class MyClassMovie
{
private:
	string name;
	string genre;
	int year;

public:
	MyClassMovie();
	MyClassMovie(string n, int y, string g);
	//~movie();
	void setName(string n);
	string getName() const;
	void setGenre(string g);
	string getGenre() const;
	void setYear(int y);
	int getYear() const;
	void print();
};

#endif