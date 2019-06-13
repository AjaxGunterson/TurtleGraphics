/*////////////////////////////////////////////////////////////////////////////////////////
//																						//
//	Programmer:	Nolan Ripplinger														//
//																						//
//	Description: This program simulates turtle graphics through either keyboard input	//
//				 or through an external text file, can output the graphic to console,	//
//				 text file, or both.													//
////////////////////////////////////////////////////////////////////////////////////////*/

#include <iostream>
#include <fstream>
using namespace std;

const int SIZE = 25;
const char EMPTY = ' ',
TURTLE_MARKING = '@',
TURTLE_LOC = 'O';

void splashScreen();
void commandHelp();
void initArray(char grid[][SIZE], int arratSize);
void printArray(char grid[][SIZE], int arraySize, int turtleXCoord, int turtleYCoord, int direction, bool turtleMarker, ofstream &outfile);
void turtleMovement(char grid[][SIZE], int &turtleX, int &turtleY, int &prevTurtleX, int &prevTurtleY,
	int distance, int direction, bool isMarkerDown);
void boundsHandler(int &turtleX);
void directionChanger(char input, int &direction);
void directionChecker(int direction);
void markerMaker(char grid[][SIZE], int turtleX, int turtleY, int prevTurtleX, int prevTurtleY);
void resetTurtle(int &turtleX, int&turtleY, int &direction, int &moveDistance, int &prevTurtleX,
	int &prevTurtleY, bool &isMarkerDown, bool &turtleMarker);

int main() {

	char input = 0,
		grid[SIZE][SIZE];
	int turtleX = 0, turtleY = 0, direction = 1, moveDistance = 0,
		prevTurtleX = 0, prevTurtleY = 0;
	bool isMarkerDown = false,
		turtleMarker = true;

	splashScreen();
	initArray(grid, SIZE);

	ofstream outfile;
	outfile.open("turtlePicture.txt");
	if (!outfile)
	{
		cout << "Error opening output file..." << endl;
		exit(1106);
	}

	ifstream infile;
	infile.open("turtleCommands.txt");
	if (!infile)
	{
		cout << "Error opening file..." << endl;
		exit(1106);
	}

	//while (input != 'q' && input != 'Q') //for keyboard input
	while (infile >> input)			//For text file input
	{
		cout << "Input a Command" << endl << endl; //(for keyboard input)
		//cin >> input; //(for file input)

		switch (input)
		{
		case 'H':
		case 'h': commandHelp();
			break;
		case 'Q':
		case 'q': cout << "Exiting" << endl << endl;
			break;
		case 'U':
		case 'u': cout << "The pen has been put up" << endl << endl;
			isMarkerDown = false;
			break;
		case 'D':
		case 'd': cout << "The pen has been put down" << endl << endl;
			isMarkerDown = true;
			break;
		case 'R':
		case 'r': directionChanger(input, direction);
			break;
		case 'L':
		case 'l': directionChanger(input, direction);
			break;
		case 'F':
		case 'f': //cin >> moveDistance;  //For keyboard input
				  infile >> moveDistance; //For text input
				  turtleMovement(grid, turtleX, turtleY, prevTurtleX, prevTurtleY, moveDistance, direction, isMarkerDown);
			break;
		case 'P':
		case 'p': printArray(grid, SIZE, turtleX, turtleY, direction, turtleMarker, outfile);
			break;
		case 'N':
		case 'n': cout << "Turtle marker enabled" << endl << endl;
			turtleMarker = true;
			break;
		case 'M':
		case 'm': cout << "Turtle marker disabled" << endl << endl;
			turtleMarker = false;
			break;
		case 'Z':
		case 'z': cout << "The turtle waddles back home to his favorite corner and cleans up his mess." << endl << endl;
			initArray(grid, SIZE);
			resetTurtle(turtleX, turtleY, direction, moveDistance, prevTurtleX, prevTurtleY, isMarkerDown, turtleMarker);
			break;
		default: cout << "Invalid input, type \"h\" for commands." << endl << endl;
			break;
		}//end switch
	}//end while

	infile.close();
	outfile.close();

	return 0;
}

/*Intro and starting prompt*/
void splashScreen()
{

	cout << "         _" << endl
		<< "     .-./*)" << endl
		<< "   _/___\\/" << endl
		<< "     U U" << endl;

	cout << "Your turtle will start at the top left and be facing east."
		<< endl
		<< "type \"h\" for commands."
		<< endl << endl;
}//end function

 /*Calls a list of handy helpful commands for keyboard users*/
void commandHelp()
{
	cout << "\nCommand \tMeaning\n"
		<< "_______\t\t________" << endl
		<< "  U \t\t Pen Up" << endl
		<< "  D \t\t Pen Down" << endl
		<< "  R \t\t Turn Right" << endl
		<< "  L \t\t Turn Left" << endl
		<< "  F n \t\t Move Forward n Spaces" << endl
		<< "  P \t\t Show the " << SIZE << " by " << SIZE << " Grid" << endl
		<< "  N \t\t Turns on Turtle Marker" << endl
		<< "  M \t\t Turns off Turtle Marker" << endl
		<< "  Q \t\t Exit the Program" << endl
		<< "  Z \t\t Reset the turtle" << endl
		<< endl;
}//end function

 /*Loads the array with the default "blank" values*/
void initArray(char grid[][SIZE], int SIZE)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			grid[i][j] = EMPTY;
		}//end for
	}//end for
}//end function

 /*Displays the contents of the array*/
void printArray(char grid[][SIZE], int SIZE, int turtleX, int turtleY, int direction, bool turtleMarker, ofstream &outfile)
{
	for (int i = 0; i < SIZE; i++)
	{
		cout << endl;
		outfile << endl;
		for (int j = 0; j < SIZE; j++)
		{
			if (i == turtleY && j == turtleX && turtleMarker) //Overlays turtle location on grid if enabled
				cout << TURTLE_LOC << " ";
			else
				cout << grid[i][j] << " ";
				outfile << grid[i][j] << " ";
		}//end for
	}//end for

	cout << "\nTurtle's current coords are: "
		<< "(" << turtleX << "," << turtleY << ")"
		<< endl
		<< "The turtle is facing ";
	if (direction == 0)
		cout << "north" << endl << endl;
	else if (direction == 1)
		cout << "east" << endl << endl;
	else if (direction == 2)
		cout << "south" << endl << endl;
	else if (direction == 3)
		cout << "west" << endl << endl;

}//end function

 /*Changes the location of the turtle, dependant on which
 direction it is facing.*/
void turtleMovement(char grid[][SIZE], int &turtleX, int &turtleY, int &prevTurtleX, int &prevTurtleY,
	int distance, int direction, bool isMarkerDown)
{
	//0 = north, 1 = east, 2 = south, 3 = west

	cout << "The turtle moves "
		<< distance << " spaces to the ";
	prevTurtleY = turtleY;
	prevTurtleX = turtleX;

	if (direction == 0)//north
	{
		cout << "north" << endl << endl;
		boundsHandler(turtleY -= distance);
	}//end if
	else if (direction == 1)//east
	{
		cout << "east" << endl << endl;
		boundsHandler(turtleX += distance);
	}//end if
	else if (direction == 2)//south
	{
		cout << "south" << endl << endl;
		boundsHandler(turtleY += distance);
	}//end if
	else if (direction == 3)//west
	{
		cout << "west" << endl << endl;
		boundsHandler(turtleX -= distance);
	}//end if
	if (isMarkerDown)
		markerMaker(grid, turtleX, turtleY, prevTurtleX, prevTurtleY);
}//end function

 /*Exception to make sure turtle will never escape its bounds.
 Can be used for both x and y chords*/
void boundsHandler(int &turtle)
{
	if (turtle < 0)
		turtle = 0;
	else if (turtle >= SIZE)
		turtle = SIZE - 1;
}//end function

 /*Changes the direction of your turtle*/
void directionChanger(char input, int &direction)
{
	//0 = north, 1 = east, 2 = south, 3 = west
	if (input == 'r' || input == 'R')
	{
		if (direction == 3)
			direction = 0;
		else
			direction++;
		cout << "You turn the turtle right" << endl;
	}//end if
	else if (input == 'l' || input == 'L')
	{
		if (direction == 0)
			direction = 3;
		else
			direction--;
		cout << "You turn the turtle left" << endl;
	}//end if
	directionChecker(direction); // calls current direction
}//end function

 /*Outputs which direction the turtle is facing*/
void directionChecker(int direction)
{
	//0 = north, 1 = east, 2 = south, 3 = west

	if (direction == 0)
		cout << "The turtle is facing north" << endl << endl;
	else if (direction == 1)
		cout << "The turtle is facing east" << endl << endl;
	else if (direction == 2)
		cout << "The turtle is facing south" << endl << endl;
	else if (direction == 3)
		cout << "The turtle is facing west" << endl << endl;
}//end function

 /*Uses the difference in present and past coord values to
 create turtle drawings*/
void markerMaker(char grid[][SIZE], int turtleX, int turtleY, int prevTurtleX, int prevTurtleY)
{
	int xDifference = turtleX - prevTurtleX,
		yDifference = turtleY - prevTurtleY;

	if (xDifference > 0)
	{
		for (int i = 0; i <= xDifference; i++)
		{
			grid[turtleY][turtleX - i] = TURTLE_MARKING;
		}//end for
	}//end if
	else if (xDifference < 0)
	{
		for (int i = 0; i >= xDifference; i--)
		{
			grid[turtleY][turtleX - i] = TURTLE_MARKING;
		}//end for
	}//end if
	if (yDifference > 0)
	{
		for (int i = 0; i <= yDifference; i++)
		{
			grid[turtleY - i][turtleX] = TURTLE_MARKING;
		}//end for
	}//end if
	else if (yDifference < 0)
	{
		for (int i = 0; i >= yDifference; i--)
		{
			grid[turtleY - i][turtleX] = TURTLE_MARKING;
		}//end for
	}//end if
}

/*Resets all important values to default*/
void resetTurtle(int &turtleX, int &turtleY, int &direction, int &moveDistance,
	int &prevTurtleX, int &prevTurtleY, bool &isMarkerDown, bool &turtleMarker)
{
	turtleX = 0, turtleY = 0,
		direction = 1, moveDistance = 0,
		prevTurtleX = 0, prevTurtleY = 0,
		isMarkerDown = 0;
}//end function
