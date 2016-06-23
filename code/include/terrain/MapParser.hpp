/**
 * @file MapParser.hpp
 * @brief
 * Parser dedicated to parse the import files containing the map's data 
 * (namely, the seeds' data and the heightmap's data).
 */

#ifndef MAPPARSER_HPP
#define MAPPARSER_HPP

#include <fstream>

#include "Biome.hpp"
#include "Seed.hpp"

/**
 * @brief
 * The ASCII integer value associated with the "Horizontal tabulation".
 */
#define ASCII_TAB 9

/**
 * @brief
 * The ASCII integer value associated with the "Line feed".
 */
#define ASCII_LF 10

/**
 * @brief
 * The ASCII integer value associated with the "Carriage return".
 */
#define ASCII_CR 13

/**
 * @brief
 * The default size of the buffer used to "skip" comments.
 */
#define DEFAULT_BUFFER_SIZE	256

/**
 * @brief
 * The class MapParser encapsulates the parser dedicated to parse data files
 * while importing the map's data.
 */
class MapParser
{
public:
	/**
	 * @brief
	 * Standard constructor.
	 *
	 * @param fileToParse A constant C-string containing the path to the file
	 *	to parse.
	 */
	MapParser(const char* fileToParse, int bufferSize = DEFAULT_BUFFER_SIZE);

	/**
	 * @brief
	 * Standard destructor.
	 */
	~MapParser();

	/**
	 * @brief
	 * Getter on m_hasReachedEOF.
	 *
	 * @return bool The value of m_hasReachedEOF.
	 */
	bool hasReachedEOF();

	/**
	* @brief
	* Getter on m_currentLine.
	*
	* @return unsigned int The value of m_currentLine.
	*/
	unsigned int getCurrentLine();

	/**
	 * @brief
	 * Skips all the "white-space characters" until the next "valuable"
	 * character.
	 */
	void skipWhiteCharacters();

	/**
	 * @brief
	 * Tests whether what follows is a comment (according to the fact that, 
	 * in the file format we use, a comment begins with a "special" 
	 * character, given in "commentCharacter").
	 * If it is a comment, skips it according to the given boolean value.
	 *
	 * @param commentCharacter The character which marks the beginning of a
	 *	comment.
	 * @param skip A boolean representing "Must the comment be skipped?".
	 * 
	 * @return bool A boolean representing "Is the line a comment?".
	 */
	bool isComment(char commentChar, bool skip);

	/**
	 * @brief
	 * Parses an integer from input file. If it fails, exits the program and
	 * prints debug information on the error output stream.
	 *
	 * @return int The parsed integer.
	 */
	int parseInt();

	/**
	 * @brief
	 * Parses a float from input file. If it fails, exits the program and
	 * prints debug information on the error output stream.
	 * NB: Null integer part can be omitted (in other words, "0.8" and ".8"
	 * represents the same float), and exponential notation is also authorized.
	 *
	 * @return float The parsed float.
	 */
	float parseFloat();

	/**
	 * @brief
	 * Parses a string, regardless of the characters it contains, delimited
	 * by white-space characters.
	 *
	 * @param parsedString A reference on a "string object" in which the parsed
	 * string has to be stored.
	 */
	void parseString(std::string& parsedString);

	/**
	* @brief
	* Parses a seed from input file. If it fails, exits the program and
	* prints debug information on the error output stream.
	*
	* @param parsedSeed A reference on the seed to fill with the parsed data.
	*/
	void parseSeed(Seed& parsedSeed);

private:
	/**
	 * @brief
	 * The stream on the input file to parse.
	 */
	std::ifstream m_inputFile;

	/**
	 * @brief
	 * The current considered line of the parsed file.
	 */
	unsigned int m_currentLine = 1;

	/**
	 * @brief
	 * The size of the buffer used to "skip" comments.
	 */
	int m_bufferSize;
};

#endif // MAPPARSER_HPP