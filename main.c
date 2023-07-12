#include "web_crawler.h"  // gets our web_crawler struct and functions
#include "queue.h"  // gets our queue struct and functions
<<<<<<< Updated upstream

=======
#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <termios.h>

//Structure for original terminal settings
struct termios orig_terminal;

//Turn off Flag that looks for \n in lines -- Immediate input interpretation
void set_mode_nb(){
    struct termios terminal_stateAlt;
    tcgetattr(STDIN_FILENO, &terminal_stateAlt);
    terminal_stateAlt.c_lflag &= ~(ICANON | ECHO);
    terminal_stateAlt.c_cc[VMIN] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &terminal_stateAlt);
}
>>>>>>> Stashed changes

int main() {
    web_crawler *crawler = web_crawler_create("http://example.com", 10);
    if (!crawler) {
        fprintf(stderr, "Failed to create web crawler\n");
        return 1;
    }

    web_crawler_run(crawler);

<<<<<<< Updated upstream
=======
        //Check if 'q' is pressed - Exit loop if pressed
        char r;
        if(read(STDIN_FILENO, &r, 1) == 1 && r == 'q'){
            break;
        }

        usleep(10000);
    }
    
    //Reset Terminal Settings - Turns flag back on
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_terminal);

>>>>>>> Stashed changes
    web_crawler_destroy(crawler);

    return 0;
}
