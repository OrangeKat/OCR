#include <cairo.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
        cairo_surface_t *surface =
            cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 540, 540);
        cairo_t *cr =
            cairo_create (surface);


        FILE* fp = fopen(argv[1], "r"); //open inputed file in reading mode
        int board[81];                  //loads int array to contain sudoku
        int bi = 0;
        char buff[12];                  //buffer to read the lines of the file
        for (int l = 0; l < 9; l++)         //get the 9 lines
        {
        fgets(buff, 12, fp);    //reads a line and stores it in buff
        int id = 0;
        while (buff[id] != '\0') //convert and append string to board
        {
                if (buff[id] == ' ' || buff[id] == '\n') //ignore whitespaces and newlines
                {
                        id++;
                        continue;
                }
                if (buff[id] == '.') //replace periods by zeros
                {
                        board[bi] = 0;
                        bi++;
                        id++;
                }
                else
                {
                        board[bi] = buff[id] - '0'; //get numbers
                        bi++;
                        id++;
                }
        }
        fgets(buff, 12, fp); //this pick up the \n at the end of the line
        if (l == 2 || l == 5)
                fgets(buff, 12, fp); //pick up the \n between squares
        }
        fclose(fp);                     //close inputed file

	int board2[81];

	if (argc == 3)
	{
	        fp = fopen(argv[2], "r"); //open inputed file in reading mode
	        int bi = 0;
	        for (int l = 0; l < 9; l++)         //get the 9 lines
	        {
	        fgets(buff, 12, fp);    //reads a line and stores it in buff
	        int id = 0;
	        while (buff[id] != '\0') //convert and append string to board
	        {
	                if (buff[id] == ' ' || buff[id] == '\n') //ignore whitespaces and newlines
	                {
	                        id++;
	                        continue;
	                }

	                else
	                {
				if (buff[id] == '.') //is new?
	                        	board2[bi] = 0;
				else
					board2[bi] = 1;
	                        bi++;
	                        id++;
	                }
	        }
	        fgets(buff, 12, fp); //this pick up the \n at the end of the line
	        if (l == 2 || l == 5)
	                fgets(buff, 12, fp); //pick up the \n between squares
	        }
	        fclose(fp);                     //close inputed file
	}

        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        cairo_paint(cr);

        cairo_set_line_width(cr, 10.0);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_rectangle(cr, 20.0, 20.0, 500.0, 500.0);

        cairo_move_to(cr, 187, 20);
        cairo_line_to(cr, 187, 520);
        cairo_move_to(cr, 353, 20);
        cairo_line_to(cr, 353, 520);
        cairo_move_to(cr, 20, 187);
        cairo_line_to(cr, 520, 187);
        cairo_move_to(cr, 20, 353);
        cairo_line_to(cr, 520, 353);

        cairo_stroke(cr);

        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_line_width(cr, 5.0);
        for (int i = 0; i < 3; i++)
        {
                for (int j = 1; j < 3; j++)
                {
                        cairo_move_to(cr, 20 + i*167 + j*56, 20);
                        cairo_line_to(cr, 20 + i*167 + j*56, 520);
                        cairo_move_to(cr, 20, 20 + i*167 + j*56);
                        cairo_line_to(cr, 520, 20 + i*167 + j*56);
                }
        }
        cairo_stroke(cr);

        cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 32.0);

        for (int h = 0; h < 3; h++)
        {
                for (int i = 0; i < 3; i++)
                {
                        for (int j = 0; j < 3; j++)
                        {
                                for (int k = 0; k < 3; k++)
                                {
                                        cairo_move_to(cr, (37 + h*167 + j*56), (61 + i*167 + k*56));
                                        if (argc == 3)
					{
						if (board2[(i*3+k)*9 + (h*3+j)] == 0)
							cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
						else
							cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
					}
					switch(board[(i*3+k)*9 + (h*3+j)])
					{
						case 1 :
							cairo_show_text(cr, "1");
							break;
						case 2 :
							cairo_show_text(cr, "2");
							break;
						case 3 :
							cairo_show_text(cr, "3");
							break;
						case 4 :
							cairo_show_text(cr, "4");
							break;
						case 5 :
							cairo_show_text(cr, "5");
							break;
						case 6 :
							cairo_show_text(cr, "6");
							break;
						case 7 :
							cairo_show_text(cr, "7");
							break;
						case 8 :
							cairo_show_text(cr, "8");
							break;
						case 9 :
							cairo_show_text(cr, "9");
							break;
						default :
							break;
					}
                                }
                        }
                }
        }

        cairo_destroy(cr);
        cairo_surface_write_to_png(surface, "img.png");
        cairo_surface_destroy(surface);
        return 0;
}
