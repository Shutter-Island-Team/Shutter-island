/**
 * @file MapParser.cpp
 *
 * @see MapParser.hpp
 */

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

#include "../../include/terrain/MapParser.hpp"

MapParser::MapParser(const char* fileToParse, int bufferSize)
	:	m_inputFile(fileToParse),
		m_bufferSize(bufferSize)
{
	if (!m_inputFile.is_open()) {
		std::cerr << "MapParser - Could not open the file to parse.";
		std::cerr << std::endl;
		exit(EXIT_FAILURE);
	}
}

MapParser::~MapParser()
{
	m_inputFile.close();
}

bool MapParser::hasReachedEOF()
{
	return m_inputFile.eof();
}

unsigned int MapParser::getCurrentLine()
{
	return m_currentLine;
}

void MapParser::skipWhiteCharacters()
{
	/*
		Consumming all the white-space characters thanks to a loop.
	*/
	char currentChar = m_inputFile.get();
	while	(
				(currentChar == ' ')		||
				(currentChar == ASCII_TAB)	||
				(currentChar == ASCII_LF)	||
				(currentChar == ASCII_CR)
			)
	{
		if (currentChar == ASCII_LF) {
			m_currentLine++;
		}
		currentChar = m_inputFile.get();
	}

	/*
		Once the loop is over, either the end of file has been reached and the 
		current character can be discarded, or the current character
		has to be put back into the input stream.
	*/
	if (!m_inputFile.eof()) {
		m_inputFile.putback(currentChar);
	}
}

bool MapParser::isComment(char commentChar, bool skip)
{
	/*
		Testing if the next character is the comment marker.
	*/
	bool isComment = (m_inputFile.peek() == commentChar);

	/*
		If "skip" is set to "true", skipping the comment. In other words,
		consuming every character until the end of the line, or the end of the
		file.
	*/
	if (isComment && skip) {
		char* buffer = new char[m_bufferSize];
		do {
			m_inputFile.getline(buffer, m_bufferSize);
		} while(m_inputFile.fail()&& !m_inputFile.bad()&& !m_inputFile.eof());
		delete[] buffer;

		m_currentLine++;
	}

	return isComment;
}

int MapParser::parseInt()
{
	this->skipWhiteCharacters();
	
	/*
		The first step consists in determining the number of character
		corresponding to the next integer.
		Then, those characters are read and converted to an integer.
		If the next characters to read do not match the integer format,
		debug information are printed to the error output stream and the
		program exits.
	*/
	int initialIndex = m_inputFile.tellg();
	int currentIndex = initialIndex;
	
	/*
		The parsed integer may begin with a minus sign.
	*/
	char nextChar = m_inputFile.peek();
	if ((nextChar == '-') || isdigit(nextChar)) {
		currentIndex++;
		m_inputFile.seekg(currentIndex);
		nextChar = m_inputFile.peek();
		
		while (isdigit(nextChar)) {
			currentIndex++;
			m_inputFile.seekg(currentIndex);
			nextChar = m_inputFile.peek();
		}

		/*
			Checking that the parsed integer is separated from the next "token" 
			by a white-space character.
			If so, converting the parsed "string-integer" to an integer and 
			returning it.
		*/
		if	(
				(nextChar == ' ')		|| 
				(nextChar == ASCII_TAB)	||
				(nextChar == ASCII_LF)	||
				(nextChar == ASCII_CR)	||
				(m_inputFile.eof())
			)
		{
			m_inputFile.seekg(initialIndex);
			
			int intSize = currentIndex - initialIndex;
			char* intString = new char[intSize + 1];
			m_inputFile.read(intString, intSize);
			intString[intSize] = '\0';

			int parsedInt = std::stoi(intString, NULL);
			delete[] intString;

			return parsedInt;
		}
	}

	/*
		If this point is reached, the next characters to be parsed where not
		representing an integer.
	*/
	std::cerr << "parseInt[Line " << m_currentLine << "]" << " - ";
	std::cerr << "The parsed characters do not match an integer." << std::endl;
	exit(EXIT_FAILURE);
}

float MapParser::parseFloat()
{
	this->skipWhiteCharacters();

	/*
	The first step consists in determining the number of character
	corresponding to the next float.
	Then, those characters are read and converted to aa float.
	If the next characters to read do not match the float format,
	debug information are printed to the error output stream and the
	program exits.
	*/
	bool decimalSeparatorParsed = false;
	bool exponentialSeparatorParsed = false;
	bool hasDigits = false;

	int initialIndex = m_inputFile.tellg();
	int currentIndex = initialIndex;

	/*
		Testing if there is a leading minus sign.
	*/
	char nextChar = m_inputFile.peek();
	if (nextChar == '-') {
		currentIndex++;
		m_inputFile.seekg(currentIndex);
		nextChar = m_inputFile.peek();
	}
	
	/*
		Except the potential minus sign, the first character of the float must
		be either a decimal separator or a digit.
	*/
	if (isdigit(nextChar) || (nextChar == '.')) {
		if (nextChar == '.') {
			decimalSeparatorParsed = true;
		} else {
			hasDigits = true;
		}
		currentIndex++;
		m_inputFile.seekg(currentIndex);
		nextChar = m_inputFile.peek();
	} else {
		std::cerr << "parseFloat[Line " << m_currentLine << "]" << " - ";
		std::cerr << "The parsed characters do not match a float.";
		std::cerr << std::endl;
		exit(EXIT_FAILURE);
	}

	/*
		Then, the float may contain digits, a (and only one) decimal separator
		and an (and only one) exponential separator (potentially accompagned
		with a sign).
	*/
	while (isdigit(nextChar) || (nextChar == '.') || (nextChar == 'e')) {
		if (nextChar == '.') {
			if (decimalSeparatorParsed || exponentialSeparatorParsed) {
				std::cerr << "parseFloat[Line " << m_currentLine << "]";
				std::cerr << " - The parsed characters do not match a float.";
				std::cerr << std::endl;
				exit(EXIT_FAILURE);
			} else {
				decimalSeparatorParsed = true;
			}
		} else if (nextChar == 'e') {
			/*
				A digit must have been parsed before parsing the exponential
				separator.
			*/
			if (!hasDigits || exponentialSeparatorParsed) {
				std::cerr << "parseFloat[Line " << m_currentLine << "]";
				std::cerr << " - The parsed characters do not match a float.";
				std::cerr << std::endl;
				exit(EXIT_FAILURE);
			} else {
				exponentialSeparatorParsed = true;
			}
			currentIndex++;
			m_inputFile.seekg(currentIndex);
			nextChar = m_inputFile.peek();
			/*
				After an exponential separator, the character must be either
				a digit or a sign.
			*/
			if (!(isdigit(nextChar) || nextChar == '-' || nextChar == '+')) {
				std::cerr << "parseFloat[Line " << m_currentLine << "]";
				std::cerr << " - The parsed characters do not match a float.";
				std::cerr << std::endl;
				exit(EXIT_FAILURE);
			}
		} else {
			hasDigits = true;
		}
		currentIndex++;
		m_inputFile.seekg(currentIndex);
		nextChar = m_inputFile.peek();
	}

	/*
		Checking that the parsed float is separated from the next "token" by a
		white-space character.
		If so, converting the parsed "string-float" to a float and returning
		it.
	*/
	if	(
			(nextChar == ' ')		||
			(nextChar == ASCII_TAB) ||
			(nextChar == ASCII_LF)	||
			(nextChar == ASCII_CR)	||
			(m_inputFile.eof())
		) 
	{
		m_inputFile.seekg(initialIndex);

		int floatSize = currentIndex - initialIndex;
		char* floatString = new char[floatSize + 1];
		m_inputFile.read(floatString, floatSize);
		floatString[floatSize] = '\0';

		float parsedFloat = std::stof(floatString, NULL);
		delete[] floatString;

		return parsedFloat;
	} else {
		std::cerr << "parseFloat[Line " << m_currentLine << "]";
		std::cerr << " - The parsed characters do not match a float.";
		std::cerr << std::endl;
		exit(EXIT_FAILURE);
	}
}

void MapParser::parseString(std::string& parsedString)
{
	this->skipWhiteCharacters();

	/*
		The first step consists in determining the number of character
		corresponding to the next string.
		Then, those characters are read and converted to a string.
	*/
	int initialIndex = m_inputFile.tellg();
	int currentIndex = initialIndex;

	/*
		Parsing while the parsed character is different from a white-space
		character.
	*/
	char nextChar = m_inputFile.peek();
	while	(
				!(
					(nextChar == ' ') ||
					(nextChar == ASCII_TAB) ||
					(nextChar == ASCII_LF) ||
					(nextChar == ASCII_CR) ||
					(m_inputFile.eof())
				)
			) 
	{
		currentIndex++;
		m_inputFile.seekg(currentIndex);
		nextChar = m_inputFile.peek();
	}
	
	m_inputFile.seekg(initialIndex);

		int stringSize = currentIndex - initialIndex;
		char* readString = new char[stringSize + 1];
		m_inputFile.read(readString, stringSize);
		readString[stringSize] = '\0';

		parsedString.assign(readString);

		delete[] readString;
}

void MapParser::parseSeed(Seed& parsedSeed)
{
	/*
		Parsing the abscissa of the Seed.
	*/
	parsedSeed.setX(parseFloat());
	
	/*
		Parsing the ordinate of the Seed.
	*/
	parsedSeed.setY(parseFloat());

	/*
		Parsing the biome type of the Seed.
	*/
	std::string parsingString;
	parseString(parsingString);

	parsedSeed.setBiome(stringToBiome(parsingString.c_str()));
}