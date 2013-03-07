#ifndef _SCI_DATA_PARSER_H_INCLUDED_
#define _SCI_DATA_PARSER_H_INCLUDED_

#include <vector>
#include <ionScene.h>
#include <ionCore.h>


class SciDataManager;

class SciDataParser
{

public:

	SciDataManager * Manager;

	virtual void load(std::string const & FileName) = 0;

};

class SciDataParserCTD : public SciDataParser
{

public:

	void load(std::string const & FileName);
	void examine(std::string const & FileName);

};

class SciDataParserSimpleTXT : public SciDataParser
{

public:

	void load(std::string const & FileName);

};

class SciDataParserCSV : public SciDataParser
{

public:

	char FieldDelim;
	char ValueDelim;
	bool FieldNames;
	std::vector<std::string> Fields;

	void load(std::string const & FileName);
	void mergedLoad(std::string const & FileName, std::string const & FileName2, std::string const & MatchField);

	SciDataParserCSV()
		: FieldDelim(','), ValueDelim(','), FieldNames(true)
	{}

};

class SciDataParserGrid1 : public SciDataParser
{

public:

	void load(std::string const & FileName);

};

#endif
