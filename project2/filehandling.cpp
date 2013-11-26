#include "filehandling.h"
#include <boost/regex.hpp>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <string.h>


using namespace std;

typedef struct 
{
	int xlow;
	int ylow;
	int xhigh;
	int yhigh;
}highlow;

typedef struct
{
	char alive;
	char dead;
} life_chars;
typedef struct
{
	char empty;
	char head;
	char tail;
	char wire;
} wire_chars;


string getName(string fileInfo);
highlow* getTerrain(string fileInfo);
life_chars* getLifeChars(string fileInfo);
void getWindow(string fileInfo, board* gameBoard);
void getLifeColors(string fileInfo, life_board* gameBoard);
void initLife(string fileInfo, life_board* gameBoard);
wire_chars* getWireChars(string fileInfo);
life_chars* getEleChars(string fileInfo);
void getWireColors(string fileInfo, wire_board* gameBoard);
void initWire(string fileInfo, wire_board* gameBoard);

board* stringToBoard(string fileInfo){

	bool isLife = false;
	bool isWire = false;
	bool isEle = false;

	fileInfo = compress(fileInfo);
	int i = 0;
	for(i = 0; i < fileInfo.length() && fileInfo[i] != '='; i++);

	string type = fileInfo.substr(0, i);

	if(type == "Life"){
		isLife = true;
	}
	else if(type == "WireWorld"){
		isWire = true;
	}
	else if(type == "Elementary"){
		isEle = true;
	}else{
		cerr << "File was not in the correct format\n";
		throw -1;
	}

	string name = getName(fileInfo);

	highlow * ter = getTerrain(fileInfo);

	life_chars* l_chars;
	wire_chars* w_chars;

	if(isLife)
		l_chars = getLifeChars(fileInfo);
	if(isWire)
		w_chars = getWireChars(fileInfo);
	if(isEle)
		l_chars = getEleChars(fileInfo);

	board * gameBoard;

	if(isLife){
		gameBoard = new life_board(name, ter->xhigh, ter->xlow, ter->yhigh, ter->ylow, l_chars->alive, l_chars->dead);
		delete l_chars;
		getLifeColors(fileInfo, (life_board*) gameBoard);
		initLife(fileInfo, (life_board*) gameBoard);
	}
	else if(isWire){
		gameBoard = new wire_board(name, ter->xhigh, ter->xlow, ter->yhigh, ter->ylow, w_chars->empty, w_chars->head, w_chars->tail, w_chars->wire);
		delete w_chars;
		getWireColors(fileInfo, (wire_board*) gameBoard);
		initWire(fileInfo, (wire_board*) gameBoard);
	}

	getWindow(fileInfo, gameBoard);

	delete ter;

	return gameBoard;
}
void initWire(string fileInfo, wire_board* gameBoard){

}
void getWireColors(string fileInfo, wire_board* gameBoard){
	boost::smatch headMatch;
	boost::smatch emptyMatch;
	boost::smatch tailMatch;
	boost::smatch wireMatch;
	boost::smatch container;
	boost::regex reHead("Head[^;]+;");
	boost::regex reEmpty("Empty[^;]+;");
	boost::regex reWire("Wire[^;]+;");
	boost::regex reTail("Tail[^;]+;");
	boost::regex reParens("\\([^\\)]+\\)");
	boost::regex reColors("Colors[^}]+\\}");

	string headColor;
	string emptyColor;
	string tailColor;
	string wireColor;

	//Head color
	int r, g, b;

	boost::regex_search(fileInfo, container, reColors);

	if(container.size() <= 0 || container[0].str() == ""){
		cerr << "Color portions not found default values will be used." << '\n';
		gameBoard->setHeadColor(255, 64, 64);
		gameBoard->setTailColor(255, 64, 255);
		gameBoard->setEmptyColor(64, 64, 64);
		gameBoard->setWireColor(64, 64, 255);
		return;
	}
	std::cout << container[0].str() << '\n';

	boost::regex_search(container[0].str(), headMatch, reHead);
	if(headMatch.size() == 1){
		boost::regex_search(headMatch[0].str(), headMatch, reParens);
		if(headMatch.size() != 1){ 
			cerr << "Error attempting to read Head color value from Colors portion, default will be used" << '\n';
			gameBoard->setHeadColor(255, 64, 64);
		}
		headColor = headMatch.str();
		int r, b ,g;
		if((sscanf(headColor.c_str(), "(%d,%d,%d)", &r, &g, &b)) != 3){
			cerr << "Head in Colors portion was not valid, default will be used" << '\n';
			gameBoard->setHeadColor(255, 64, 64);
		}
		else if(r < 0 || r > 255 || b < 0 || b > 255 || g < 0 || g > 255){
			cerr << "RBG value for Head in Colors portion was not valid, default will be used" << '\n';
			gameBoard->setHeadColor(255, 64, 64);
		}else{
			std::cout << headColor << '\n';
			gameBoard->setHeadColor(r, g, b);
		}	
	}else{
		cerr << "Head portion not found, default color will be used\n";
		gameBoard->setHeadColor(255, 64, 64);
	}

	//Empty color
	boost::regex_search(container[0].str(), emptyMatch, reEmpty);
	if(emptyMatch.size() == 1){
		boost::regex_search(emptyMatch[0].str(), emptyMatch, reParens);
		if(emptyMatch.size() != 1){
			cerr << "Error attempting to read Empty color value from Colors portion, default will be used" << '\n';
			gameBoard->setEmptyColor(64, 64, 64);
		}
		emptyColor = emptyMatch.str();
		if((sscanf(emptyColor.c_str(), "(%d,%d,%d)", &r, &g, &b)) != 3){
			cerr << "RBG value for Empty in Colors portion was not valid, default will be used." << '\n';
			gameBoard->setEmptyColor(64 , 64, 64);
		}
		else if(r < 0 || r > 255 || b < 0 || b > 255 || g < 0 || g > 255){
			cerr << "RBG value for Empty in Colors portion was not valid" << '\n';
			gameBoard->setEmptyColor(64 , 64, 64);
		}else{
			std::cout << emptyColor << '\n';
			gameBoard->setEmptyColor(r, g, b);
		}
	}else{
		cerr << "Empty color not found default will be used\n";
		gameBoard->setEmptyColor(0 , 0, 0);
	}
}

wire_chars* getWireChars(string fileInfo){

	boost::smatch container;
	boost::regex reChars("Chars[^}]+\\}");

	boost::regex_search(fileInfo, container, reChars);
	if(container.size() != 1){
		cerr << "Error with the Chars portion" << '\n';
		throw -1;
	}
	int empty = 32, tail = 43, head = 35, wire = 46;

	const char * emptyLoc = strstr(container.str().c_str(), "Empty=");
	if(emptyLoc){
		if((sscanf(emptyLoc, "Empty=%d", &empty)) != 1){
			cerr << "Error reading Empty value from Chars portion, default will be used" << '\n';
			empty = 32;
		}
	}else{
		cerr << "Could not find location of Empty value, default will be used" << '\n';
		empty = 32;
	}
	const char * tailLoc = strstr(container.str().c_str(), "Tail=");
	if(tailLoc){
		if((sscanf(tailLoc, "Tail=%d", &tail)) != 1){
			cerr << "Error reading Tail value from Chars portion. default will be used" << '\n';
			tail = 126;
		}
	}else{
		cerr << "Could not find location of Tail value, default will be used" << '\n';
		tail = 126;
	}
	const char * headLoc = strstr(container.str().c_str(), "Head=");
	if(headLoc){
		if((sscanf(headLoc, "Head=%d", &head)) != 1){
			cerr << "Error reading Head value from Chars portion. default will be used" << '\n';
			head = 126;
		}
	}else{
		cerr << "Could not find location of Head value, default will be used" << '\n';
		head = 126;
	}
	const char * wireLoc = strstr(container.str().c_str(), "Wire=");
	if(wireLoc){
		if((sscanf(wireLoc, "Wire=%d", &wire)) != 1){
			cerr << "Error reading Wire value from Chars portion. default will be used" << '\n';
			wire = 126;
		}
	}else{
		cerr << "Could not find location of Wire value, default will be used" << '\n';
		wire = 126;
	}

	wire_chars * chars = new wire_chars;
	chars->empty = (char) empty;
	chars->tail = (char) tail;
	chars->head = (char) head;
	chars->wire = (char) wire;

	return chars;
}
life_chars* getEleChars(string fileInfo){

	boost::smatch container;
	boost::regex reChars("Chars[^}]+\\}");

	boost::regex_search(fileInfo, container, reChars);
	if(container.size() != 1){
		cerr << "Error with the Chars portion" << '\n';
		throw -1;
	}
	int one = 35, zero = 46;

	const char * oneLoc = strstr(container.str().c_str(), "One=");
	if(oneLoc){
		if((sscanf(oneLoc, "One=%d", &one)) != 1){
			cerr << "Error reading One value from Chars portion, default will be used" << '\n';
			one = 35;
		}
	}else{
		cerr << "Could not find location of One value, default will be used" << '\n';
		one = 35;
	}
	const char * zeroLoc = strstr(container.str().c_str(), "Zero=");
	if(zeroLoc){
		if((sscanf(zeroLoc, "Zero=%d", &zero)) != 1){
			cerr << "Error reading Zero value from Chars portion. default will be used" << '\n';
			zero = 46;
		}
	}else{
		cerr << "Could not find location of Zero value, default will be used" << '\n';
		zero = 46;
	}
	life_chars * chars = new life_chars;
	chars->alive = (char) one;
	chars->dead = (char) zero;

	cout << one << '\n';
	cout << zero << '\n';

	return chars;
}

//Handles retrieval of Initial information from the string containing the file
void initLife(string fileInfo, life_board* gameBoard){
	boost::smatch container;

	boost::regex reInitial("Initial[^}]+\\}");
	boost::regex reParens("\\([^\\)]+\\)");

	boost::regex_search(fileInfo, container, reInitial);

	if(container.size() != 1){
		cerr << "Error with Initial portion\n";
		throw -1;
	}
	string temp = container.str();
	boost::regex_iterator<string::iterator> regit (temp.begin(), temp.end(), reParens);
	boost::regex_iterator<string::iterator> end;

	while(regit != end){
		int xc;
		int yc;
		if((sscanf(regit->str().c_str(), "(%d, %d)", &xc, &yc)) != 2){
			cerr << "Error reading X, Y coordinate" << '\n';
			regit++;
			continue;
		}
		if(xc < gameBoard->getXMin() || xc > gameBoard->getXMax() || yc < gameBoard->getYMin() || yc > gameBoard->getYMax()){
			cerr << "Coordinate in " << regit->str() << " was not in range of the terrain." << '\n';
			regit++;
			continue;
		}
		gameBoard->setAlive(xc, yc);
		regit++;
	}
}

void getLifeColors(string fileInfo, life_board* gameBoard){

	boost::smatch aliveMatch;
	boost::smatch deadMatch;
	boost::smatch container;
	boost::regex reAlive("Alive[^;]+;");
	boost::regex reDead("Dead[^;]+;");
	boost::regex reParens("\\([^\\)]+\\)");
	boost::regex reColors("Colors[^}]+\\}");

	string aliveColor;
	string deadColor;
	//Alive color
	int r, g, b;

	boost::regex_search(fileInfo, container, reColors);

	if(container.size() <= 0 || container[0].str() == ""){
		cerr << "Color portions not found default values will be used." << '\n';
		gameBoard->setAliveColor(255, 255, 255);
		gameBoard->setAliveColor(255, 255, 255);
		gameBoard->setDeadColor(64 , 64, 64);
		return;
	}
	boost::regex_search(container[0].str(), aliveMatch, reAlive);
	if(aliveMatch.size() == 1){
		boost::regex_search(aliveMatch[0].str(), aliveMatch, reParens);
		if(aliveMatch.size() != 1){ 
			cerr << "Error attempting to read Alive color value from Colors portion, default will be used" << '\n';
			gameBoard->setAliveColor(255, 255, 255);
		}
		aliveColor = aliveMatch.str();
		int r, b ,g;
		if((sscanf(aliveColor.c_str(), "(%d,%d,%d)", &r, &g, &b)) != 3){
			cerr << "RBG value for Alive in Colors portion was not valid, default will be used" << '\n';
			gameBoard->setAliveColor(255, 255, 255);
		}
		if(r < 0 || r > 255 || b < 0 || b > 255 || g < 0 || g > 255){
			cerr << "RBG value for Alive in Colors portion was not valid, default will be used" << '\n';
			gameBoard->setAliveColor(255, 255, 255);
		}else{
			gameBoard->setAliveColor(r, g, b);
		}
		
	}else{
		cerr << "Alive portion not found, default color will be used\n";
		gameBoard->setAliveColor(255, 255, 255);
	}

	//Dead color
	boost::regex_search(container[0].str(), deadMatch, reDead);
	if(deadMatch.size() == 1){
		boost::regex_search(deadMatch[0].str(), deadMatch, reParens);
		if(deadMatch.size() != 1){
			cerr << "Error attempting to read Dead color value from Colors portion, default will be used" << '\n';
			gameBoard->setDeadColor(64 , 64, 64);
		}
		deadColor = deadMatch.str();
		if((sscanf(deadColor.c_str(), "(%d,%d,%d)", &r, &g, &b)) != 3){
			cerr << "RBG value for Dead in Colors portion was not valid, default will be used." << '\n';
			gameBoard->setDeadColor(64 , 64, 64);
		}
		else if(r < 0 || r > 255 || b < 0 || b > 255 || g < 0 || g > 255){
			cerr << "RBG value for Dead in Colors portion was not valid" << '\n';
			gameBoard->setDeadColor(64 , 64, 64);
		}else{
			gameBoard->setDeadColor(r, g, b);
		}
	}else{
		cerr << "Dead color not found default will be used\n";
		gameBoard->setDeadColor(0 , 0, 0);
	}
}

//Handles retrieval of name from the string containing the file
string getName(string fileInfo){
	boost::regex reName("Name[^;]+");
	boost::regex reQuotes("\"[^\"]+\"");
	boost::smatch container;
	string name = "";
	regex_search(fileInfo, container, reName);
	if(container.size() == 1){
		boost::regex_search(container[0].str(), container, reQuotes);
		if(container.size() == 1){
			name = trimQuotes(container[0].str());
		}
	}
	return name;
}
highlow* getTerrain(string fileInfo){
	boost::regex reTerrain("Terrain[^}]+\\}");
	boost::regex reXrange("Xrange[^;]+\\;");
	boost::regex reYrange("Yrange[^;]+\\;");
	boost::regex reLowHigh("-?[0-9]+\\.\\.[0-9]+");
	boost::smatch container;

	//Handles retrieval of Terrain information from the string containing the file
	boost::regex_search(fileInfo, container, reTerrain);
	if(container.size() != 1){
		cerr << "Error with the terrain portion" << '\n';
		throw -1;
	}
	boost::smatch xrange;
	boost::regex_search(container[0].str(), xrange, reXrange);
	if(xrange.size() != 1){
		cerr << "Error with the Xrange portion of terrain" << '\n';
		throw -1;
	}
	boost::regex_search(xrange[0].str(), xrange, reLowHigh);
	if(xrange.size() != 1){
		cerr << "Error with the Xrange portion of terrain" << '\n';
		throw -1;
	}
	string x = xrange.str();

	boost::smatch yrange;
	boost::regex_search(container[0].str(), yrange, reYrange);
	if(xrange.size() != 1){
		cerr << "Error with the Yrange portion of terrain" << '\n';
		throw -1;
	}
	boost::regex_search(yrange[0].str(), yrange, reLowHigh);
	if(xrange.size() != 1){
		cerr << "Error with the Yrange portion of terrain" << '\n';
		throw -1;
	}
	string y = yrange.str();

	int xlow;
	int xhigh;
	int ylow; 
	int yhigh;
	if((sscanf(x.c_str(), "%d..%d", &xlow, &xhigh)) != 2){
		cerr << "Error reading range of x values" << '\n';
		throw -1;
	}
	if((sscanf(y.c_str(), "%d..%d", &ylow, &yhigh)) != 2){
		cerr << "Error reading range of y values" << '\n';
		throw -1;
	}
	highlow * coor = new highlow;
	coor->xlow = xlow;
	coor->ylow = ylow;
	coor->xhigh = xhigh;
	coor->yhigh = yhigh;
	return coor;
}
//Handles retrieval of Chars information from the string containing the file
life_chars* getLifeChars(string fileInfo){

	boost::smatch container;
	boost::regex reChars("Chars[^}]+\\}");

	boost::regex_search(fileInfo, container, reChars);
	if(container.size() != 1){
		cerr << "Error with the Chars portion" << '\n';
		throw -1;
	}
	int alive = 64, dead = 126;

	const char * aliveLoc = strstr(container.str().c_str(), "Alive=");
	if(aliveLoc){
		if((sscanf(aliveLoc, "Alive=%d", &alive)) != 1){
			cerr << "Error reading Alive value from Chars portion, default will be used" << '\n';
			alive = 64;
		}
	}else{
		cerr << "Could not find location of Alive value, default will be used" << '\n';
		alive = 64;
	}
	const char * deadLoc = strstr(container.str().c_str(), "Dead=");
	if(deadLoc){
		if((sscanf(deadLoc, "Dead=%d", &dead)) != 1){
			cerr << "Error reading Dead value from Chars portion. default will be used" << '\n';
			dead = 126;
		}
	}else{
		cerr << "Could not find location of Dead value, default will be used" << '\n';
		dead = 126;
	}
	life_chars * chars = new life_chars;
	chars->alive = (char) alive;
	chars->dead = (char) dead;
	return chars;
}
void getWindow(string fileInfo, board* gameBoard){

	boost::regex reLowHigh("-?[0-9]+\\.\\.[0-9]+");
	boost::regex reXrange("Xrange[^;]+\\;");
	boost::regex reYrange("Yrange[^;]+\\;");
	boost::regex reWindow("Window[^}]+\\}");

	//Handles retrieval of Window information from the string containing the file
	boost::smatch newCont;
	boost::regex_search(fileInfo, newCont, reWindow);
	if(newCont.size() == 1 && newCont[0].matched){
		boost::smatch xrange;
		boost::regex_search(newCont[0].str(), xrange, reXrange);
		if(xrange.size() != 1){
			cerr << "Error with the Xrange portion of terrain" << '\n';
			throw -1;
		}
		boost::regex_search(xrange[0].str(), xrange, reLowHigh);
		if(xrange.size() != 1){
			cerr << "Error with the Xrange portion of terrain" << '\n';
			throw -1;
		}
		string x = xrange.str();

		boost::smatch yrange;
		boost::regex_search(newCont[0].str(), yrange, reYrange);
		if(xrange.size() != 1){
			cerr << "Error with the Yrange portion of terrain" << '\n';
			throw -1;
		}
		boost::regex_search(yrange[0].str(), yrange, reLowHigh);
		if(xrange.size() != 1){
			cerr << "Error with the Yrange portion of terrain" << '\n';
			throw -1;
		}
		string y = yrange.str();

		int xlow = gameBoard->getXMin();
		int xhigh = gameBoard->getXMax();
		int ylow = gameBoard->getYMin();
		int yhigh = gameBoard->getYMax();
		if((sscanf(x.c_str(), "%d..%d", &xlow, &xhigh)) != 2){
			cerr << "Error reading range of x values, default will be used" << '\n';
			xlow = gameBoard->getXMin();
			xhigh = gameBoard->getXMax();
		}
		if(xlow < gameBoard->getXMin()){
			cerr << "Window X-Low was out of range of the terrain, default will be used\n";
			xlow = gameBoard->getXMin();
		}
		if(xhigh > gameBoard->getXMax()){
			cerr << "Window X-High was out of range of the terrain, default will be used\n";
			xhigh = gameBoard->getXMax();
		}
		if((sscanf(y.c_str(), "%d..%d", &ylow, &yhigh)) != 2){
			cerr << "Error reading range of y values" << '\n';
			throw -1;
		}
		if(ylow < gameBoard->getYMin()){
			cerr << "Window Y-Low was out of range of the terrain, default will be used\n";
			ylow = gameBoard->getYMin();
		}
		if(yhigh > gameBoard->getYMax()){
			cerr << "Window Y-High was out of range of the terrain, default will be used\n";
			yhigh = gameBoard->getYMax();
		}

		gameBoard->setWinWidth(xhigh, xlow);
		gameBoard->setWinHeight(yhigh, ylow);
	}
}

string getStringFromFile(const char *filename){
	ifstream inf(filename);
	if(inf){
		string fileData;
		inf.seekg(0, ios::end);
		fileData.resize(inf.tellg());
		inf.seekg(0, ios::beg);
		inf.read(&fileData[0], fileData.size());
		inf.close();
		return fileData;
	}else{
		cerr << "Error reading file: " << filename << '\n';
		throw -1;
	}
}
string getStringFromStdIn(){
	try{
		string fileData = "";
		char c;
		cin.get(c);
		while(!cin.eof()){
			fileData += c;
			cin.get(c);
		}
		return fileData;
	}catch(int e){
		cerr << "Error getting string from stdin\n";
		throw -1;
	}
}
string trimQuotes(string toTrim){
	string finalString;
	finalString.resize(toTrim.size() - 2);
	int j = 0;
	for(unsigned int i = 0; i < toTrim.size(); i++){
		if('\"' != toTrim[i]){
			finalString[j] = toTrim[i];
			j++;
		}
	}
	return finalString;
}
string compress(string data){
	string noComm;
	noComm.resize(data.size());
	bool isComment = false;
	bool inQoutes = false;
	int j = 0;
	for(unsigned int i = 0; i < data.size(); i++){
		if('#' == data[i] && !inQoutes){
			isComment = true;
		}
		else if(isComment){
			if('\n' == data[i]){
				isComment = false;
			}
		}
		else{
			if((data[i] == ' ' || data[i] == '\t' || data[i] == '\n') && !inQoutes)
				continue;
			noComm[j] = data[i];
			j++;
			if(data[i] == '\"' && !inQoutes){
				inQoutes = true;
			}
			else if(data[i] == '\"' && inQoutes){
				inQoutes = false;
			}
		}
	}
	noComm[j] = '\0';
	return noComm;
}
void updateBoardWithCommands(board * gameBoard, int argc, char *argv[]){
	int generations = 0;
	int txIndex = -1, tyIndex = -1, wyIndex = -1, wxIndex = -1;
	int updateTer = 0, updateWin = 0;

	int yh, yl, err, xl, xh;
	for (int i = 1; i < argc; ++i)
	{
		if('-' == argv[i][0]){
			int len = strlen(argv[i]);
			for (int j = 1; j < len; ++j)
			{
				if('g' == argv[i][j]){
					if(i + 1 < argc){
						++i;
						if(!isdigit(argv[i][0])){
							cerr << "Not a valid digit after -g for generations. Default of 0 will be used.\n";
						}else{
							generations = atoi(argv[i]);
						}
					}else{
						cerr << "Value needs to be specified after -g.\n";
					}
					break;
				}
				else if('t' == argv[i][j]){

					if(j + 1 < len && 'y' == argv[i][j + 1]){
						tyIndex = ++i;
						updateTer = 1;
					}
					else if(j + 1 < len && 'x' == argv[i][j + 1]){
						txIndex = ++i;
						updateTer = 1;
					}
					break;
				}
				else if('w' == argv[i][j]){
					if(j + 1 < len && 'y' == argv[i][j + 1]){
						wyIndex = ++i;
						updateWin = 1;
					}
					else if(j + 1 < len && 'x' == argv[i][j + 1]){
						wxIndex = ++i;
						updateWin = 1;
					}
					break;
				}
			}
		}
	}
	if(updateTer){
		xl = gameBoard->getXMin();
		xh = gameBoard->getXMax();
		yl = gameBoard->getYMin();
		yh = gameBoard->getYMax();	
		if(-1 != txIndex){
			err = sscanf(argv[txIndex], "%d..%d", &xl, &xh);
			if(2 != err || xl > xh){
				cerr << "Values not correct after -tx will use terrian dimensions from file\n";
				xl = gameBoard->getXMin();
				xh = gameBoard->getXMax();
			}
		}			
		if(-1 != tyIndex){
			err = sscanf(argv[tyIndex], "%d..%d", &yl, &yh);				
			if(2 != err || yl > yh){
				cerr << "Values not correct after -ty will use terrian dimensions from file\n";
				yl = gameBoard->getYMin();
				yh = gameBoard->getYMax();
			}
		}
		gameBoard->updateTerrain(xh, xl, yh, yl);
	}
	if(updateWin){

		if(-1 != wxIndex){
			err = sscanf(argv[wxIndex], "%d..%d", &xl, &xh);
			if(2 != err || xl > xh){
				cerr << "Values not correct after -wx will use window dimensions from file\n";
			}else{
				gameBoard->setWinWidth(xh, xl);
			}
		}
		if(-1 != wyIndex){
			err = sscanf(argv[wyIndex], "%d..%d", &yl, &yh);
			if(2 != err || yl > yh){
				cerr << "Values not correct after -wy will use window dimensions from file\n";
			}else{
				gameBoard->setWinHeight(yh, yl);
			}
		}
	}
	gameBoard->updateN(generations);
}