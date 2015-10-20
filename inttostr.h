/*
 inttostr.h
	int <-> string conversion library
	(c) marqueewinq, 2011
*/
#pragma once
#ifndef INTTOSTR_H_
	#define INTTOSTR_H_
#endif

#include <string>
#include <sstream>

/* function prototypes */
std::string inttostr(const int);
int strotint(const std::string);

/* implementation */
std::string inttostr(int n)
{
	std::ostringstream ss;
	ss << n;
	return ss.str();
}

int strtoint(std::string s)
{
	std::istringstream ss;
	ss.str(s);
	int result;
	ss >> result;
	return result;
}

double strtoflt(std::string s)
{
	std::istringstream ss;
	ss.str(s);
	double result;
	ss >> result;
	return result;
}

double strtoflt(char* s)
{
	return strtoflt(string(s));
}
