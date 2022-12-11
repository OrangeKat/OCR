#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

void safelist(int board[], int cell, int v[]) //build a list of valid numbers for the cell
{
	int p = (cell / 9) * 9; //start position
	for (int i = p; i < p + 9; i++) //check line
	{
		if (i == cell || board[i] == 0) //is cell empty?
			continue;
		v[board[i] - 1] = 1; //set equivalent position in v to 1, ie: found 5 => v[4] = 1
	}

	p = cell % 9;
	for (int i = p; i < p + 73; i += 9) //check colum
	{
		if (i == cell || board[i] == 0) 
			continue;
		v[board[i] - 1] = 1;
	}

	p = ((cell / 27) * 27)  + (((cell % 9) / 3) * 3); //upper left corner of square
	for (int i = 0; i < 3; i++) //line loop
	{
		for (int j = 0; j < 3; j++) //colum loop
		{
			if (p + j == cell || board[p+j] == 0)
				continue;
			v[board[p+j]-1] = 1;
		}
		p += 9; //next line
	}
	return;
}

int rsolve(int board[], int cell) //recursion for each cell
{
	if (cell == 81) //has the end of the board been reached? ie: is the board solved?
		return 1;

	if (board[cell] == 0) //is the cell empty?
	{
		int c = 0;                             //bool check
		int v[] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; //initialize valid numbers list
		safelist(board, cell, v);              //fill valid numbers list

		for (int i = 0; i < 9; i++)            //for every available value for the cell
		{
			if (v[i] == 1)                 //is number available?
				continue;
			board[cell] = i+1;             //put number in cell
			c = rsolve(board, cell+1);     //move to next cell
			if (c)                         //has the board been solved?
				break;
		}

		if (c)                                 //has the board been solved?
			return 1;                      //report solved board to previous recursion
		else
		{
			board[cell] = 0;               //reset cell to zero
			return 0;                      //backtrack to previous recursion
		}
	}

	else //the cell is already filled and shouldn't be changed
		return rsolve(board, cell+1);
}

void solve(int board[]) //this solving method uses backtracking
{
	int c = 0; //check if sudoku is already solved
	for (int i = 0; i < 81; i++)
	{
		if (board[i] == 0)
		{
			c = 1;
			break;
		}
	}
	if (c == 0) //if already solved, do nothing
		return;

	rsolve(board, 0); //lauch recursion for backtracking
	//if the board is unsolvable, it will have been backtracked to its initial values
	return;
}

int main(int argc, char** argv)
{
	if (argc != 2)
		errx(EXIT_FAILURE, "Usage: solver {text file adress}");;

	FILE* fp = fopen(argv[1], "r"); //open inputed file in reading mode
	int board[81];                  //loads int array to contain sudoku
	int bi = 0;
	char buff[12];                  //buffer to read the lines of the file
	for (int l = 0; l < 9; l++)         //get the 9 lines
	{
		fgets(buff, 12, fp);    //reads a line and stores it in buff
		int i = 0;
		while (buff[i] != '\0') //convert and append string to board
		{
			if (buff[i] == ' ' || buff[i] == '\n') //ignore whitespaces and newlines
			{
				i++;
				continue;
			}
			if (buff[i] == '.') //replace periods by zeros
			{
				board[bi] = 0;
				bi++;
				i++;
			}
			else
			{
				board[bi] = buff[i] - '0'; //get numbers
				bi++;
				i++;
			}
		}
		fgets(buff, 12, fp); //this pick up the \n at the end of the line
		if (l == 2 || l == 5)
			fgets(buff, 12, fp); //pick up the \n between squares
	}
	fclose(fp);                     //close inputed file

	solve(board);

	fp = fopen(strcat(argv[1], ".result"), "w+"); //create and open result file

	bi = 0;
	for (int k = 0; k < 9; k++) //prints the board in the result file
	{
		for (int j = 0; j < 9; j++)
		{
			fputc(board[bi] + '0', fp);
			bi++;
			if (j == 2 || j == 5)
				fputc(' ', fp);
		}
		fputc('\n', fp);
		if (k == 2 || k == 5)
			fputc('\n', fp);
	}

	fclose(fp);                    //close result file

	return EXIT_SUCCESS;
}
