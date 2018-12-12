#include <gtk/gtk.h>
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PINS 19

// pin definitions
#define P_ORANGE 14
#define P_RED 15
#define P_BROWN 17
#define P_BLACK 27
#define P_WHITE 22
#define P_GREY 10
#define P_PURPLE 9
#define P_BLUE 11
#define P_GREEN 13
#define P_YELLOW 19
#define P_ORANGE2 26
#define P_A 18
#define P_B 23
#define P_C 24
#define P_D 25
#define P_E 8
#define P_F 12
#define P_G 16
#define P_H 20

#define NUM_TOP_PINS 11
char top_pins[NUM_TOP_PINS] = {P_ORANGE, P_RED, P_BROWN, P_BLACK, P_WHITE, P_GREY, P_PURPLE, P_BLUE, P_GREEN, P_YELLOW, P_ORANGE2};

#define NUM_BOTTOM_PINS 8
char bottom_pins[NUM_BOTTOM_PINS] = {P_A, P_B, P_C, P_D, P_E, P_F, P_G, P_H};

#define L_GREEN 2
#define L_BLUE 3
#define L_RED 4

int dart = 1;
int players = 1;
void num_of_players_1(){players = 1;}
void num_of_players_2(){players = 2;}
void num_of_players_3(){players = 3;}
void num_of_players_4(){players = 4;}
GtkWidget *window;

int start_score = 301;
int score_p1, score_p2, score_p3, score_p4;
void start_score_301(){
    start_score = 301;
    score_p1 = score_p2 = score_p3 = score_p4 = 301;
}
void start_score_501(){
    start_score = 501;
    score_p1 = score_p2 = score_p3 = score_p4 = 501;
}
void start_score_701(){
    start_score = 701;
    score_p1 = score_p2 = score_p3 = score_p4 = 701;
}
void start_score_901(){
    start_score = 901;
    score_p1 = score_p2 = score_p3 = score_p4 = 901;
}

int current_shooter = 1;
int first_dart = 0;
char first_dart_print[10];
int second_dart = 0;
char second_dart_print[10];
int third_dart = 0;
char third_dart_print[10];
int round_score = 0;

int winner = 0;

int num_hit = 0;
int multiplier = 0;
#define INNER 1
#define OUTER 2
int in_out = 0;

int cricket_multipliers[29];

void game_type_window();
void end_of_game();
void oh_one_games();
void game_page_01();
void game_page_cricket();
void cricket();
int get_player_score(int player_num);
void set_player_score(int player_num, int score);

int calc_val(int top_pin, int bottom_pin){
	gpioWrite(L_GREEN, 0);
	gpioWrite(L_RED, 1);
	system("aplay chaching.wav");
    if((bottom_pin == P_A) || (bottom_pin == P_B) || (bottom_pin == P_C) || (bottom_pin == P_D)){
        if(bottom_pin == P_A){
            in_out = INNER;
            multiplier = 1;
        }else if(bottom_pin == P_B){
            in_out = 0;
            multiplier = 3;
        }else if(bottom_pin == P_C){
            in_out = OUTER;
            multiplier = 1;
        }else if(bottom_pin == P_D){
            in_out = 0;
            multiplier = 2;
        }
        if(top_pin == P_ORANGE){
            num_hit = 10;
        }else if(top_pin == P_RED){
            num_hit = 15;
        }else if(top_pin == P_BROWN){
            num_hit = 2;
        }else if(top_pin == P_BLACK){
            num_hit = 17;
        }else if(top_pin == P_WHITE){
            num_hit = 3;
        }else if(top_pin == P_PURPLE){
            num_hit = 13;
        }else if(top_pin == P_BLUE){
            num_hit = 4;
        }else if(top_pin == P_GREEN){
            num_hit = 18;
        }else if(top_pin == P_YELLOW){
            num_hit = 6;
        }else if(top_pin == P_ORANGE2){
            num_hit = 1;
        }
    }else if((bottom_pin == P_E) || (bottom_pin == P_F) || (bottom_pin == P_G) || (bottom_pin == P_H)){
        if(bottom_pin == P_E){
            in_out = INNER;
            multiplier = 1;
            if(top_pin == P_GREY){
                num_hit = 25;
                in_out = 0;
                multiplier = 2;
            }
        }else if(bottom_pin == P_F){
            in_out = 0;
            multiplier = 3;
            if(top_pin == P_GREY){
                num_hit = 25;
                multiplier = 1;
            }
        }else if(bottom_pin == P_G){
            in_out = OUTER;
            multiplier = 1;
        }else if(bottom_pin == P_H){
            in_out = 0;
            multiplier = 2;
        }
        if(top_pin == P_ORANGE){
            num_hit = 11;
        }else if(top_pin == P_RED){
            num_hit = 8;
        }else if(top_pin == P_BROWN){
            num_hit = 16;
        }else if(top_pin == P_BLACK){
            num_hit = 7;
        }else if(top_pin == P_WHITE){
            num_hit = 19;
        }else if(top_pin == P_PURPLE){
            num_hit = 14;
        }else if(top_pin == P_BLUE){
            num_hit = 9;
        }else if(top_pin == P_GREEN){
            num_hit = 12;
        }else if(top_pin == P_YELLOW){
            num_hit = 5;
        }else if(top_pin == P_ORANGE2){
            num_hit = 20;
        }
    }
    return 0;
}

int detect_hit(){
	gpioWrite(L_RED, 0);
	gpioWrite(L_GREEN, 1);
    if(gpioInitialise()< 0) {
        printf("setup failed !\n");
        return -1;
    }
    int i,j,k,l;
    for (i = 0; i < NUM_BOTTOM_PINS; i++) {
        char bottom_pin = bottom_pins[i];
        gpioSetMode(bottom_pin, PI_OUTPUT);
        gpioWrite(bottom_pin, 0);
    }
    for (l = 0; l < NUM_TOP_PINS; l++) {
        gpioSetMode(top_pins[l], PI_INPUT);
    }
    while(1){
        for (j = 0; j < NUM_BOTTOM_PINS; j++) {
            char bottom_pin = bottom_pins[j];
            gpioWrite(bottom_pin, 1);
            for (k = 0; k < NUM_TOP_PINS; k++) {
                char top_pin = top_pins[k];
                if (gpioRead(top_pin)) {
                    calc_val(top_pin, bottom_pin);
                    return 0;
                }
            }
            gpioWrite(bottom_pin, 0);
        }
    }
}

void dart_throw_01(){
    if (dart <= 3) {
        detect_hit();
        int remaining_points = get_player_score(current_shooter);
        int throw_score = num_hit*multiplier;
        round_score+=throw_score;
        remaining_points-=throw_score;
        set_player_score(current_shooter, remaining_points);
        if(dart == 1){
            first_dart = throw_score;
            sprintf(first_dart_print,"%d*%d ",multiplier,num_hit);
        }else if(dart == 2){
            second_dart = throw_score;
            sprintf(second_dart_print,"%d*%d ",multiplier,num_hit);
        }else if(dart == 3){
            third_dart = throw_score;
            sprintf(third_dart_print,"%d*%d ",multiplier,num_hit);
        }
        dart++;
        if (remaining_points == 0){
            winner = current_shooter;
            end_of_game();
        } else if (remaining_points < 0){
            set_player_score(current_shooter, remaining_points + first_dart + second_dart + third_dart);
            dart = 4;
            gtk_widget_destroy(window);
            game_page_01();
        } else {
            gtk_widget_destroy(window);
            game_page_01();
        }
    }
}

int get_player_score(int player_num){
    if(player_num == 1){return score_p1;}
    if(player_num == 2){return score_p2;}
    if(player_num == 3){return score_p3;}
    if(player_num == 4){return score_p4;}
}

void set_player_score(int player_num, int score){
    if(player_num == 1){score_p1 = score;}
    if(player_num == 2){score_p2 = score;}
    if(player_num == 3){score_p3 = score;}
    if(player_num == 4){score_p4 = score;}
}

void end_of_turn_01(){
    if(current_shooter == players){
        current_shooter = 1;
    } else {
        current_shooter++;
    }
    first_dart = 0;
    sprintf(first_dart_print,"%d",first_dart);
    second_dart = 0;
    sprintf(second_dart_print,"%d",second_dart);
    third_dart = 0;
    sprintf(third_dart_print,"%d",third_dart);
    round_score = 0;
    dart = 1;
    game_page_01();
}

void end_of_turn_cricket(){
    if(current_shooter == players){
        current_shooter = 1;
    } else {
        current_shooter++;
    }
    first_dart = 0;
    sprintf(first_dart_print,"%d",first_dart);
    second_dart = 0;
    sprintf(second_dart_print,"%d",second_dart);
    third_dart = 0;
    sprintf(third_dart_print,"%d",third_dart);
    round_score = 0;
    dart = 1;
    game_page_cricket();
}


void end_of_game(){
	//Reinitialize variables for next game
	gpioWrite(L_GREEN, 0);
	gpioWrite(L_RED, 0);
	gpioWrite(L_BLUE, 1);
	dart = 0;
	first_dart = 0;
    sprintf(first_dart_print,"%d",first_dart);
    second_dart = 0;
    sprintf(second_dart_print,"%d",second_dart);
    third_dart = 0;
    sprintf(third_dart_print,"%d",third_dart);
    round_score = 0;
	
    GtkWidget *label, *grid, *button, *button_box;

    // Set up window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Cricket");
    gtk_container_set_border_width(GTK_CONTAINER(window),10);
    gtk_window_set_default_size(GTK_WINDOW(window),1000,400);

    // Add a button box to center buttons on screen
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window),button_box);
    
    // Set up grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(button_box),grid);

    //Congratulate the Winner
    label = gtk_label_new("Congratulations to the Winner,\n");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
    gtk_widget_set_name(label, "mid_label");
    char congratulations[10];
    sprintf(congratulations, "Player %d!", winner);
    label = gtk_label_new(congratulations);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);
    gtk_widget_set_name(label, "huge_label1");

    // Button to start a new game
    button = gtk_button_new_with_label("New Game");
    g_signal_connect(button,"clicked",G_CALLBACK(game_type_window), NULL);
    g_signal_connect_swapped(button,"clicked",G_CALLBACK(gtk_widget_destroy),window);
    gtk_grid_attach(GTK_GRID(grid),button,0,2,1,1);

    gtk_widget_show_all(window);
}

void game_page_01(){
    GtkWidget *label, *big_label, *mid_label, *huge_label, *grid, *button, *button_box;

    //Set up the window
    char title[3];
    sprintf(title, "%d", start_score);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),title);
    gtk_container_set_border_width(GTK_CONTAINER(window),10);
    gtk_window_set_default_size(GTK_WINDOW(window),1000,400);

    // Add a button box to center buttons on screen
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window),button_box);
    
    // Set up grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(button_box),grid);

    //Populate the grid with buttons and labels
    char heading[25];
    sprintf(heading, "Now Shooting: Player %d", current_shooter);
    mid_label = gtk_label_new(heading);
    gtk_label_set_justify(GTK_LABEL(mid_label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach (GTK_GRID (grid), mid_label, 0, 0, 5, 1);
    gtk_widget_set_name(mid_label, "mid_label");

    //put opponents' scores on the left hand side, accounting for # of players and who is throwing currently
    int i;
    char opponents[50];
    int opponent_score;
    int row = 1;
    for(i = 1; i <= players; i++){
        if (i != current_shooter){
            opponent_score = get_player_score(i);
            sprintf(opponents, "          Player %d: %d          ", i, opponent_score);
            big_label = gtk_label_new(opponents);
            gtk_label_set_justify(GTK_LABEL(big_label), GTK_JUSTIFY_LEFT);
            gtk_grid_attach(GTK_GRID(grid), big_label, 0, row, 1, 1);
            gtk_widget_set_name(big_label, "big_label1");
            row++;
        }
    }

    //Next player button in case someone misses the board
    button = gtk_button_new_with_label ("End Turn");
    g_signal_connect (button, "clicked", G_CALLBACK (end_of_turn_01),NULL);
    g_signal_connect_swapped(button,"clicked",G_CALLBACK(gtk_widget_destroy),window);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 6, 1, 1);
    
    //Next dart
    button = gtk_button_new_with_label ("Next Dart");
    g_signal_connect (button, "clicked", G_CALLBACK (dart_throw_01),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 5, 1, 1);
    gtk_widget_show_all(window);

    //Add the current shooter's score for round and game
    big_label = gtk_label_new("Remaining Points");
    gtk_label_set_justify(GTK_LABEL(big_label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), big_label, 1,1,4,1);
    gtk_widget_set_name(big_label, "big_label1");
    char score[3];
    int remaining_points = get_player_score(current_shooter);
    sprintf(score, "%d", remaining_points);
    huge_label = gtk_label_new(score);
    gtk_label_set_justify(GTK_LABEL(huge_label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), huge_label, 1,2,4,1);
    gtk_widget_set_name(huge_label, "huge_label1");
    big_label = gtk_label_new("Round Score");
    gtk_label_set_justify(GTK_LABEL(big_label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), big_label, 1,4,4,1);
    gtk_widget_set_name(big_label, "big_label1");
    //sprintf(score,"%d",first_dart);
    big_label = gtk_label_new(first_dart_print);
    gtk_label_set_justify(GTK_LABEL(big_label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), big_label, 1,5,1,1);
    gtk_widget_set_name(big_label, "big_label1");
    //sprintf(score,"%d",second_dart);
    big_label = gtk_label_new(second_dart_print);
    gtk_label_set_justify(GTK_LABEL(big_label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), big_label, 2,5,1,1);
    gtk_widget_set_name(big_label, "big_label1");
    //sprintf(score,"%d",third_dart);
    big_label = gtk_label_new(third_dart_print);
    gtk_label_set_justify(GTK_LABEL(big_label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), big_label, 3,5,1,1);
    gtk_widget_set_name(big_label, "big_label1");
    sprintf(score,"%d",round_score);
    big_label = gtk_label_new(score);
    gtk_label_set_justify(GTK_LABEL(big_label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), big_label, 4,5,1,1);
    gtk_widget_set_name(big_label, "big_label1");
    
    gtk_widget_show_all(window);
}

void dart_throw_cricket(){
    if (dart <= 3) {
        //Detect hit and update scores
        detect_hit();
        int curr_score, offset, index;
        if (num_hit == 15) {offset = 2;}
        else if (num_hit == 16) {offset = 3;}
        else if (num_hit == 17) {offset = 4;}
        else if (num_hit == 18) {offset = 5;}
        else if (num_hit == 19) {offset = 6;}
        else if (num_hit == 20) {offset = 7;}
        else if (num_hit == 25) {offset = 1;}
        //getter and setter
        curr_score = get_player_score(current_shooter);
        curr_score = curr_score + num_hit* multiplier;
        set_player_score(current_shooter, curr_score);
        //multiplier matrix
        index = current_shooter * 7 - offset;
        if (num_hit < 15) {index = 28;}
        if ((cricket_multipliers[index] + multiplier) <= 3){
            cricket_multipliers[index] = cricket_multipliers[index] + multiplier;
        } else {
            cricket_multipliers[index] = 3;
        }
        //scores at top
        if (dart == 1) {
            first_dart = num_hit * multiplier;
            sprintf(first_dart_print,"%d*%d",multiplier,num_hit);
        } else if (dart == 2) {
            second_dart = num_hit * multiplier;
            sprintf(second_dart_print,"%d*%d",multiplier,num_hit);
        } else {
            third_dart = num_hit * multiplier;
            sprintf(third_dart_print,"%d*%d",multiplier,num_hit);
        }
        round_score = round_score + num_hit * multiplier;
        dart++;
        //If someone has won
        int p_index;
        p_index = (current_shooter * 7) - 7;
        if ((cricket_multipliers[p_index] == 3) && (cricket_multipliers[p_index + 1] == 3) && (cricket_multipliers[p_index + 2] == 3) && (cricket_multipliers[p_index + 3] == 3) &&
            (cricket_multipliers[p_index + 4] == 3) && (cricket_multipliers[p_index + 5] == 3) && (cricket_multipliers[p_index + 6] == 3)) {
            winner = current_shooter;
            gtk_widget_destroy(window);
            end_of_game();
        } else {
            gtk_widget_destroy(window);
            game_page_cricket();
        }   
    } 
}

void game_page_cricket(){
    GtkWidget *label, *grid, *button, *button_box;

    // Set up window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Cricket");
    gtk_container_set_border_width(GTK_CONTAINER(window),10);
    gtk_window_set_default_size(GTK_WINDOW(window),1000,400);

    // Add a button box to center buttons on screen
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window),button_box);
    
    // Set up grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(button_box),grid);

    // Set up round score on top of screen
    char heading[25];
    sprintf(heading, "Now Shooting: Player %d", current_shooter);
    label = gtk_label_new(heading);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), label, 0,0,8,1);
    gtk_widget_set_name(label, "mid_label");
    char score[3];
    //sprintf(score,"%d",first_dart);
    label = gtk_label_new(first_dart_print);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), label, 0,1,2,1);
    gtk_widget_set_name(label, "round_score");
    //sprintf(score,"%d",second_dart);
    label = gtk_label_new(second_dart_print);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), label, 2,1,2,1);
    gtk_widget_set_name(label, "round_score");
    //sprintf(score,"%d",third_dart);
    label = gtk_label_new(third_dart_print);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), label, 4,1,2,1);
    gtk_widget_set_name(label, "round_score");
    sprintf(score,"%d",round_score);
    label = gtk_label_new(score);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), label, 6,1,2,1);
    gtk_widget_set_name(label, "round_score");

    // Set up list of required numbers to hit
    label = gtk_label_new("20");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), label, 1,2,1,1);
    gtk_widget_set_name(label, "cricket_player");
    label = gtk_label_new("19");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), label, 2,2,1,1);
    gtk_widget_set_name(label, "cricket_player");
    label = gtk_label_new("18");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), label, 3,2,1,1);
    gtk_widget_set_name(label, "cricket_player");
    label = gtk_label_new("17");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), label, 4,2,1,1);
    gtk_widget_set_name(label, "cricket_player");
    label = gtk_label_new("16");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), label, 5,2,1,1);
    gtk_widget_set_name(label, "cricket_player");
    label = gtk_label_new("15");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), label, 6,2,1,1);
    gtk_widget_set_name(label, "cricket_player");
    label = gtk_label_new("B");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), label, 7,2,1,1);
    gtk_widget_set_name(label, "cricket_player");

    // Add player numbers
    int k;
    char pnum[50];
    int row = 3;
    for(k = 1; k <= players; k++){
        sprintf(pnum, "%d", k);
        label = gtk_label_new(pnum);
        gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
        gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
        gtk_widget_set_name(label, "cricket_player");
        row++;
    }


    //print  on board
    int i,j;
    int m = 0;
    char name[1];
    for(j = 3; j < (3+players); j++){
        for(i = 1; i < 8; i++){
            sprintf(name, "%d", cricket_multipliers[m]);
            label = gtk_label_new(name);
            gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
            gtk_grid_attach(GTK_GRID(grid), label, i,j,1,1);
            if (cricket_multipliers[m] == 0) {
                gtk_widget_set_name(label, "cricket_hit_0");
            } else if (cricket_multipliers[m] == 1) {
                gtk_widget_set_name(label, "cricket_hit_1");
            } else if (cricket_multipliers[m] == 2) {
                gtk_widget_set_name(label, "cricket_hit_2");
            } else {
                gtk_widget_set_name(label, "cricket_hit_3");
            }
            m++;
        }
    }

    //Next player button in case someone misses the board
    button = gtk_button_new_with_label ("End Turn");
    g_signal_connect (button, "clicked", G_CALLBACK (end_of_turn_cricket),NULL);
    g_signal_connect_swapped(button,"clicked",G_CALLBACK(gtk_widget_destroy),window);
    gtk_grid_attach (GTK_GRID (grid), button, 4, 7, 4, 1);
    
    //Next dart
    button = gtk_button_new_with_label ("Next Dart");
    g_signal_connect (button, "clicked", G_CALLBACK (dart_throw_cricket),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 7, 4, 1);
    gtk_widget_show_all(window);
}

void oh_one_games(){
    GtkWidget *label, *grid, *button, *button_box;
    
    // Set up window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"'01 Games");
    gtk_container_set_border_width(GTK_CONTAINER(window),10);
    gtk_window_set_default_size(GTK_WINDOW(window),1000,400);

    // Add a button box to center buttons on screen
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window),button_box);
    
    // Set up grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(button_box),grid);
    
    //Label number of players selection and add buttons
    label = gtk_label_new("             Select # of Players             ");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
    button = gtk_button_new_with_label ("\n1\n");
    g_signal_connect (button, "clicked", G_CALLBACK (num_of_players_1),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 1);
    button = gtk_button_new_with_label ("\n2\n");
    g_signal_connect (button, "clicked", G_CALLBACK (num_of_players_2),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 2, 1, 1);
    button = gtk_button_new_with_label ("\n3\n");
    g_signal_connect (button, "clicked", G_CALLBACK (num_of_players_3),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 3, 1, 1);
    button = gtk_button_new_with_label ("\n4\n");
    g_signal_connect (button, "clicked", G_CALLBACK (num_of_players_4),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 4, 1, 1);
    
    // Setup '01 game types and populate grid
    label = gtk_label_new("             Select '01 Game Type             ");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID (grid), label, 2, 0, 1, 1);
    button = gtk_button_new_with_label ("301");
    g_signal_connect(button,"clicked",G_CALLBACK(start_score_301),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 2, 1, 1, 1);
    button = gtk_button_new_with_label ("501");
    g_signal_connect(button,"clicked",G_CALLBACK(start_score_501),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 2, 2, 1, 1);
    button = gtk_button_new_with_label ("701");
    g_signal_connect(button,"clicked",G_CALLBACK(start_score_701),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 2, 3, 1, 1);
    button = gtk_button_new_with_label ("901");
    g_signal_connect(button,"clicked",G_CALLBACK(start_score_901),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 2, 4, 1, 1);

    // Add a back button incase they selected wrong game type
    button = gtk_button_new_with_label("             Back             ");
    g_signal_connect(button,"clicked",G_CALLBACK(game_type_window), NULL);
    g_signal_connect_swapped(button,"clicked",G_CALLBACK(gtk_widget_destroy),window);
    gtk_grid_attach(GTK_GRID(grid),button,3,3,2,2);

    // Add next button to submit selections
    button = gtk_button_new_with_label("Next");
    g_signal_connect(button,"clicked",G_CALLBACK(game_page_01), NULL);
    g_signal_connect_swapped(button,"clicked",G_CALLBACK(gtk_widget_destroy),window);
    gtk_grid_attach(GTK_GRID(grid),button,3,1,2,2);
    
    gtk_widget_show_all(window);
}

void cricket(){
    GtkWidget *label, *grid, *button, *button_box;
    
    // Set up window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Cricket");
    gtk_container_set_border_width(GTK_CONTAINER(window),10);
    gtk_window_set_default_size(GTK_WINDOW(window),1000,400);
    
    // Add a button box to center buttons on screen
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window),button_box);
    
    // Set up grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(button_box),grid);
    
    //Label number of players selection and add buttons
    label = gtk_label_new("                    Select # of Players                    ");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
    button = gtk_button_new_with_label ("\n1\n");
    g_signal_connect (button, "clicked", G_CALLBACK (num_of_players_1),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 1);
    button = gtk_button_new_with_label ("\n2\n");
    g_signal_connect (button, "clicked", G_CALLBACK (num_of_players_2),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 2, 1, 1);
    button = gtk_button_new_with_label ("\n3\n");
    g_signal_connect (button, "clicked", G_CALLBACK (num_of_players_3),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 3, 1, 1);
    button = gtk_button_new_with_label ("\n4\n");
    g_signal_connect (button, "clicked", G_CALLBACK (num_of_players_4),NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 4, 1, 1);

    // Add a back button incase they selected wrong game type
    button = gtk_button_new_with_label("                    Back                    ");
    g_signal_connect(button,"clicked",G_CALLBACK(game_type_window), NULL);
    g_signal_connect_swapped(button,"clicked",G_CALLBACK(gtk_widget_destroy),window);
    gtk_grid_attach(GTK_GRID(grid),button,1,3,1,2);

    // Add next button to submit selections
    button = gtk_button_new_with_label("          Next          ");
    g_signal_connect(button,"clicked",G_CALLBACK(game_page_cricket), NULL);
    g_signal_connect_swapped(button,"clicked",G_CALLBACK(gtk_widget_destroy),window);
    gtk_grid_attach(GTK_GRID(grid),button,1,1,1,2);
    
    gtk_widget_show_all(window);
}

void game_type_window(){
	gpioWrite(L_BLUE,0);
    GtkWidget *grid, *button, *button_box;
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Choose Game Type");
    gtk_container_set_border_width(GTK_CONTAINER(window),10);
    gtk_window_set_default_size(GTK_WINDOW(window),1000,400);
    
    // Add a button box to center buttons on screen
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window),button_box);
    
    // Set up grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(button_box),grid);
    
    button = gtk_button_new_with_label ("\n\n                                                  '01 Games                                                  \n\n");
    g_signal_connect (button, "clicked", G_CALLBACK (oh_one_games), NULL);
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);
    
    button = gtk_button_new_with_label ("\n\nCricket\n\n");
    g_signal_connect (button, "clicked", G_CALLBACK (cricket), NULL);
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 1);
    
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
	if(gpioInitialise()< 0) {
        printf("setup failed !\n");
        return -1;
    }
	sprintf(first_dart_print,"%d",first_dart);
	sprintf(second_dart_print,"%d",second_dart);
	sprintf(third_dart_print,"%d",third_dart);
	gpioSetMode(L_RED, PI_OUTPUT);
	gpioSetMode(L_GREEN, PI_OUTPUT);
	gpioSetMode(L_BLUE, PI_OUTPUT);
	gpioWrite(L_RED, 0);
	gpioWrite(L_GREEN, 0);
	gpioWrite(L_BLUE, 0);
	
    GtkWidget *button, *button_box;
    
    gtk_init(&argc,&argv);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Welcome!");
    gtk_window_set_default_size(GTK_WINDOW(window),1000,400);
    
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window),button_box);

    button = gtk_button_new_with_label("\n\n\n                                                  Get Started                                                  \n\n\n");
    g_signal_connect (button, "clicked", G_CALLBACK (game_type_window), NULL);
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
    gtk_container_add(GTK_CONTAINER(button_box),button);
    
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "theme.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(cssProvider),GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_widget_show_all(window);
    
    gtk_main();
    
    return(0);
}
