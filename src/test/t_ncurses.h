#include <ncurses.h>

#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void basic()
{
    int c = 0;
    // init ncurses mode
    initscr();
    // printw("Hello World !!!");
    // refresh();
    // getch();
    // hide cursor
    curs_set(0);
    while (c < 1000) {
        // print at row 0 col 0
        mvprintw(0, 0, "%d", c++);
        refresh();
        sleep(1);
    }
    // end ncursor mo
    endwin();
}

void initialization()
{
    int ch = 4;
    char str[30];

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    printw("Type any character to see it in bold\n");
    refresh();
    // ch = getch();
    scanf("%s", str);

    if (ch == KEY_F(1))
        printw("F1 key pressed");
    else {
        printw("The pressed key is ");
        attron(A_BOLD | A_UNDERLINE);
        // printw("%c", ch);
        // printw("%s", str);
        addstr(str);
        attroff(A_BOLD);
    }
    refresh();
    getch();
    endwin();
}

void printw_ex()
{
    char mesg[] = "Just a string";
    int row, col;

    initscr();
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2, (col - strlen(mesg)) / 2, "%s", mesg);
    mvprintw(row - 2, 0, "This screen has %d rows and %d columns\n", row, col);
    printw("Try resizing your window(if possible) and then run this program again");
    refresh();
    getch();
    endwin();
}

void scanw_ex()
{
    char mesg[] = "Enter a string: ";
    char str[80];
    int row, col;

    initscr();
    getmaxyx(stdscr, row, col);
    attron(A_BLINK);
    mvprintw(row / 2, (col - strlen(mesg)) / 2, "%s", mesg);
    attroff(A_BLINK);
    getstr(str);
    mvprintw(LINES - 2, 0, "You Entered: %s", str);
    getch();
    endwin();
}

void chgat_ex()
{
    initscr();
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    printw("A Big string which i didn;t care to type fully\n ");
//mvchgat(0, 0, -1, A_BLINK, 1, NULL);
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(2));
    printw("A Big string which i didn;t care to type fully\n ");
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(3));
    printw("A Big string which i didn;t care to type fully\n ");
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(4));
    printw("A Big string which i didn;t care to type fully\n ");
    attroff(COLOR_PAIR(4));

    attron(COLOR_PAIR(5));
    printw("A Big string which i didn;t care to type fully\n ");
    attroff(COLOR_PAIR(5));
    
    attron(COLOR_PAIR(6));
    printw("A Big string which i didn;t care to type fully\n ");
    attroff(COLOR_PAIR(6));

    attron(COLOR_PAIR(7));
    printw("A Big string which i didn;t care to type fully\n ");
    attroff(COLOR_PAIR(7));

    attron(COLOR_PAIR(8));
    printw("A Big string which i didn;t care to type fully\n ");
    attroff(COLOR_PAIR(8));
    refresh();
    getch();
    endwin();
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{   WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0 , 0);      /* 0, 0 gives default characters 
                     * for the vertical and horizontal
                     * lines            */
    wrefresh(local_win);        /* Show that box        */

    return local_win;
}

void destroy_win(WINDOW *local_win)
{   
    /* box(local_win, ' ', ' '); : This won't produce the desired
     * result of erasing the window. It will leave it's four corners 
     * and so an ugly remnant of window. 
     */
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    /* The parameters taken are 
     * 1. win: the window on which to operate
     * 2. ls: character to be used for the left side of the window 
     * 3. rs: character to be used for the right side of the window 
     * 4. ts: character to be used for the top side of the window 
     * 5. bs: character to be used for the bottom side of the window 
     * 6. tl: character to be used for the top left corner of the window 
     * 7. tr: character to be used for the top right corner of the window 
     * 8. bl: character to be used for the bottom left corner of the window 
     * 9. br: character to be used for the bottom right corner of the window
     */
    wrefresh(local_win);
    delwin(local_win);
}

void test_windows()
{
    WINDOW *my_win;
    int startx, starty, width, height;
    int ch;

    initscr();          /* Start curses mode        */
    cbreak();           /* Line buffering disabled, Pass on
                     * everty thing to me       */
    keypad(stdscr, TRUE);       /* I need that nifty F1     */

    height = 3;
    width = 10;
    starty = (LINES - height) / 2;  /* Calculating for a center placement */
    startx = (COLS - width) / 2;    /* of the window        */
    printw("Press F1 to exit");
    refresh();
    my_win = create_newwin(height, width, starty, startx);

    while((ch = getch()) != KEY_F(1))
    {   switch(ch)
        {   case KEY_LEFT:
                destroy_win(my_win);
                my_win = create_newwin(height, width, starty,--startx);
                break;
            case KEY_RIGHT:
                destroy_win(my_win);
                my_win = create_newwin(height, width, starty,++startx);
                break;
            case KEY_UP:
                destroy_win(my_win);
                my_win = create_newwin(height, width, --starty,startx);
                break;
            case KEY_DOWN:
                destroy_win(my_win);
                my_win = create_newwin(height, width, ++starty,startx);
                break;  
        }
    }
        
    endwin();           /* End curses mode        */
}

typedef struct _win_border_struct {
    chtype  ls, rs, ts, bs, 
        tl, tr, bl, br;
}WIN_BORDER;

typedef struct _WIN_struct {

    int startx, starty;
    int height, width;
    WIN_BORDER border;
}WIN;

void init_win_params(WIN *p_win)
{
    p_win->height = 3;
    p_win->width = 10;
    p_win->starty = (LINES - p_win->height)/2;  
    p_win->startx = (COLS - p_win->width)/2;

    p_win->border.ls = '|';
    p_win->border.rs = '|';
    p_win->border.ts = '-';
    p_win->border.bs = '-';
    p_win->border.tl = '+';
    p_win->border.tr = '+';
    p_win->border.bl = '+';
    p_win->border.br = '+';

}
void print_win_params(WIN *p_win)
{
    mvprintw(25, 0, "%d %d %d %d", p_win->startx, p_win->starty, 
                p_win->width, p_win->height);
    refresh();
}
void create_box(WIN *p_win, bool flag)
{   int i, j;
    int x, y, w, h;

    x = p_win->startx;
    y = p_win->starty;
    w = p_win->width;
    h = p_win->height;

    if(flag == TRUE)
    {   mvaddch(y, x, p_win->border.tl);
        mvaddch(y, x + w, p_win->border.tr);
        mvaddch(y + h, x, p_win->border.bl);
        mvaddch(y + h, x + w, p_win->border.br);
        mvhline(y, x + 1, p_win->border.ts, w - 1);
        mvhline(y + h, x + 1, p_win->border.bs, w - 1);
        mvvline(y + 1, x, p_win->border.ls, h - 1);
        mvvline(y + 1, x + w, p_win->border.rs, h - 1);

    }
    else
        for(j = y; j <= y + h; ++j)
            for(i = x; i <= x + w; ++i)
                mvaddch(j, i, ' ');
                
    refresh();

}
void test_windows_more()
{
    WIN win;
    int ch;

    initscr();          /* Start curses mode        */
    start_color();          /* Start the color functionality */
    cbreak();           /* Line buffering disabled, Pass on
                     * everty thing to me       */
    keypad(stdscr, TRUE);       /* I need that nifty F1     */
    noecho();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    /* Initialize the window parameters */
    init_win_params(&win);
    print_win_params(&win);

    attron(COLOR_PAIR(1));
    printw("Press F1 to exit");
    refresh();
    attroff(COLOR_PAIR(1));
    
    create_box(&win, TRUE);
    while((ch = getch()) != KEY_F(1))
    {   switch(ch)
        {   case KEY_LEFT:
                create_box(&win, FALSE);
                --win.startx;
                create_box(&win, TRUE);
                break;
            case KEY_RIGHT:
                create_box(&win, FALSE);
                ++win.startx;
                create_box(&win, TRUE);
                break;
            case KEY_UP:
                create_box(&win, FALSE);
                --win.starty;
                create_box(&win, TRUE);
                break;
            case KEY_DOWN:
                create_box(&win, FALSE);
                ++win.starty;
                create_box(&win, TRUE);
                break;  
        }
    }
    endwin();           /* End curses mode        */    
}


#define WIDTH 30
#define HEIGHT 10

const char *choices[] = { 
            "Choice 1",
            "Choice 2",
            "Choice 3",
            "Choice 4",
            "Exit",
};

int n_choices = sizeof(choices) / sizeof(char *);

void print_menu(WINDOW *menu_win, int highlight)
{
    int x, y, i;    

    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for(i = 0; i < n_choices; ++i)
    {   if(highlight == i + 1) /* High light the present choice */
        {   wattron(menu_win, A_REVERSE); 
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh(menu_win);
}

void interface_keyboard()
{
    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;

    initscr();
    clear();
    noecho();
    cbreak();   /* Line buffering disabled. pass on everything */
    int startx = (80 - WIDTH) / 2;
    int starty = (24 - HEIGHT) / 2;
        
    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_win, TRUE);
    mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
    refresh();
    print_menu(menu_win, highlight);
    while(1)
    {
        c = wgetch(menu_win);
        switch(c)
        {   case KEY_UP:
                if(highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if(highlight == n_choices)
                    highlight = 1;
                else 
                    ++highlight;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
                refresh();
                break;
        }
        print_menu(menu_win, highlight);
        if(choice != 0) /* User did a choice come out of the infinite loop */
            break;
    }   
    mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
    clrtoeol();
    refresh();
    endwin();
}

int startx = 0;
int starty = 0;
/* Report the choice according to mouse position */

void report_choice(int mouse_x, int mouse_y, int *p_choice)
{   
    int i,j, choice;

    i = startx + 2;
    j = starty + 3;
    
    for(choice = 0; choice < n_choices; ++choice)
        if(mouse_y == j + choice && mouse_x >= i && mouse_x <= int(i + strlen(choices[choice])))
        {   if(choice == n_choices - 1)
                *p_choice = -1;     
            else
                *p_choice = choice + 1; 
            break;
        }
}

void interface_mouse()
{
    int c, choice = 0;
    WINDOW *menu_win;
    MEVENT event;

    /* Initialize curses */
    initscr();
    clear();
    noecho();
    cbreak();   //Line buffering disabled. pass on everything

    /* Try to put the window in the middle of screen */
    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;
    
    attron(A_REVERSE);
    mvprintw(23, 1, "Click on Exit to quit (Works best in a virtual console)");
    refresh();
    attroff(A_REVERSE);

    /* Print the menu for the first time */
    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    print_menu(menu_win, 1);
    /* Get all the mouse events */
    mousemask(ALL_MOUSE_EVENTS, NULL);
    
    keypad(menu_win, TRUE);
    while(1)
    {   c = wgetch(menu_win);
        switch(c)
        {   
        case KEY_MOUSE: {
            if(getmouse(&event) == OK)
            {   /* When the user clicks left mouse button */
                if(event.bstate & BUTTON1_PRESSED)
                {   
                    report_choice(event.x + 1, event.y + 1, &choice);
                    if(choice == -1) //Exit chosen
                        goto end;
                    mvprintw(22, 1, "Choice made is : %d String Chosen is \"%10s\"", choice, choices[choice - 1]);
                    refresh(); 
                }
            }
            print_menu(menu_win, choice);
            }
            break;
        }
    }       
end:
    endwin();
}

void leave_curses()
{
    initscr();          /* Start curses mode          */
    printw("Hello World !!!\n");    /* Print Hello World          */
    refresh();          /* Print it on to the real screen */
    def_prog_mode();        /* Save the tty modes         */
    endwin();           /* End curses mode temporarily    */
    printf("In cooked mode");      /* Do whatever you like in cooked mode */
    reset_prog_mode();      /* Return to the previous tty mode*/
                    /* stored by def_prog_mode()      */
    refresh();          /* Do refresh() to restore the    */
                    /* Screen contents        */
    printw("Another String\n"); /* Back to curses use the full    */
    refresh();          /* capabilities of curses     */
    endwin();           /* End curses mode        */
}

void acs_variables()
{
        initscr();

        printw("Upper left corner           "); addch(ACS_ULCORNER); printw("\n"); 
        printw("Lower left corner           "); addch(ACS_LLCORNER); printw("\n");
        printw("Lower right corner          "); addch(ACS_LRCORNER); printw("\n");
        printw("Tee pointing right          "); addch(ACS_LTEE); printw("\n");
        printw("Tee pointing left           "); addch(ACS_RTEE); printw("\n");
        printw("Tee pointing up             "); addch(ACS_BTEE); printw("\n");
        printw("Tee pointing down           "); addch(ACS_TTEE); printw("\n");
        printw("Horizontal line             "); addch(ACS_HLINE); printw("\n");
        printw("Vertical line               "); addch(ACS_VLINE); printw("\n");
        printw("Large Plus or cross over    "); addch(ACS_PLUS); printw("\n");
        printw("Scan Line 1                 "); addch(ACS_S1); printw("\n");
        printw("Scan Line 3                 "); addch(ACS_S3); printw("\n");
        printw("Scan Line 7                 "); addch(ACS_S7); printw("\n");
        printw("Scan Line 9                 "); addch(ACS_S9); printw("\n");
        printw("Diamond                     "); addch(ACS_DIAMOND); printw("\n");
        printw("Checker board (stipple)     "); addch(ACS_CKBOARD); printw("\n");
        printw("Degree Symbol               "); addch(ACS_DEGREE); printw("\n");
        printw("Plus/Minus Symbol           "); addch(ACS_PLMINUS); printw("\n");
        printw("Bullet                      "); addch(ACS_BULLET); printw("\n");
        printw("Arrow Pointing Left         "); addch(ACS_LARROW); printw("\n");
        printw("Arrow Pointing Right        "); addch(ACS_RARROW); printw("\n");
        printw("Arrow Pointing Down         "); addch(ACS_DARROW); printw("\n");
        printw("Arrow Pointing Up           "); addch(ACS_UARROW); printw("\n");
        printw("Board of squares            "); addch(ACS_BOARD); printw("\n");
        printw("Lantern Symbol              "); addch(ACS_LANTERN); printw("\n");
        printw("Solid Square Block          "); addch(ACS_BLOCK); printw("\n");
        printw("Less/Equal sign             "); addch(ACS_LEQUAL); printw("\n");
        printw("Greater/Equal sign          "); addch(ACS_GEQUAL); printw("\n");
        printw("Pi                          "); addch(ACS_PI); printw("\n");
        printw("Not equal                   "); addch(ACS_NEQUAL); printw("\n");
        printw("UK pound sign               "); addch(ACS_STERLING); printw("\n");

        refresh();
        getch();
        endwin();
}

int test_ncurses()
{
    // initialization();
    // printw_ex();
    // scanw_ex();
    chgat_ex();
    // test_windows();
    // test_windows_more();
    // interface_keyboard();
    // interface_mouse();
    // leave_curses();
    // acs_variables();

    return 0;
}
