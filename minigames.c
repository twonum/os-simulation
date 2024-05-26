#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_ROWS 100
#define MAX_COLS 100

#define STATE_UNREVEALED 0
#define STATE_REVEALED 1
#define STATE_FLAGGED 2

int rows, cols;
int num_mines;
int grid[MAX_ROWS][MAX_COLS];
int state[MAX_ROWS][MAX_COLS];

// Function prototypes
void initialize();
void placeMines(int x, int y);
void reveal(int x, int y);
void flagCell(int x, int y);
bool checkWin();
void free_coordinates(int *coordinates);

// Callback function for when a button is clicked
void on_button_clicked(GtkWidget *button, gpointer user_data) {
    int *coordinates = (int *)user_data;
    int x = coordinates[0];
    int y = coordinates[1];

    if (grid[x][y] == -1) {
        GtkWidget *game_over_dialog = gtk_message_dialog_new(NULL,
                                                             GTK_DIALOG_MODAL,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_OK,
                                                             "Game Over! You hit a mine!");
        gtk_dialog_run(GTK_DIALOG(game_over_dialog));
        gtk_widget_destroy(game_over_dialog);
        state[x][y] = STATE_REVEALED;
    } else {
        reveal(x, y);
        if (checkWin()) {
            GtkWidget *win_dialog = gtk_message_dialog_new(NULL,
                                                            GTK_DIALOG_MODAL,
                                                            GTK_MESSAGE_INFO,
                                                            GTK_BUTTONS_OK,
                                                            "Congratulations! You won!");
            gtk_dialog_run(GTK_DIALOG(win_dialog));
            gtk_widget_destroy(win_dialog);
        }
    }
}

// Function to initialize the grid
void initialize() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = 0;
            state[i][j] = STATE_UNREVEALED;
        }
    }
}

// Function to place mines on the grid
void placeMines(int x, int y) {
    srand(time(NULL));
    int count = 0;
    while (count < num_mines) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (grid[r][c] == 0 && !(r == x && c == y)) {
            grid[r][c] = -1;
            count++;
        }
    }
}

// Function to reveal a cell and adjacent cells recursively
void reveal(int x, int y) {
    if (x < 0 || x >= rows || y < 0 || y >= cols || state[x][y] == STATE_REVEALED)
        return;

    state[x][y] = STATE_REVEALED;

    if (grid[x][y] == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int newX = x + i;
                int newY = y + j;
                reveal(newX, newY);
            }
        }
    }
}

// Function to check if the player has won
bool checkWin() {
    int unrevealed_count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (state[i][j] == STATE_UNREVEALED)
                unrevealed_count++;
        }
    }
    return unrevealed_count == num_mines;
}

// Callback function for starting the game
void on_start_button_clicked(GtkWidget *button, gpointer user_data) {
    // Get game parameters using input dialogs
    GtkWidget *dialog;
    GtkWidget *content_area;

    // Get number of rows
    dialog = gtk_message_dialog_new(NULL,
                                     GTK_DIALOG_MODAL,
                                     GTK_MESSAGE_QUESTION,
                                     GTK_BUTTONS_OK_CANCEL,
                                     "Enter number of rows:");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *rows_entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), rows_entry);
    gtk_widget_show_all(dialog);
    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        rows = atoi(gtk_entry_get_text(GTK_ENTRY(rows_entry)));
    }
    gtk_widget_destroy(dialog);

    // Get number of columns
    dialog = gtk_message_dialog_new(NULL,
                                     GTK_DIALOG_MODAL,
                                     GTK_MESSAGE_QUESTION,
                                     GTK_BUTTONS_OK_CANCEL,
                                     "Enter number of columns:");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *cols_entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), cols_entry);
    gtk_widget_show_all(dialog);
    result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        cols = atoi(gtk_entry_get_text(GTK_ENTRY(cols_entry)));
    }
    gtk_widget_destroy(dialog);

    // Get number of mines
    dialog = gtk_message_dialog_new(NULL,
                                     GTK_DIALOG_MODAL,
                                     GTK_MESSAGE_QUESTION,
                                     GTK_BUTTONS_OK_CANCEL,
                                     "Enter number of mines:");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *mines_entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), mines_entry);
    gtk_widget_show_all(dialog);
    result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        num_mines = atoi(gtk_entry_get_text(GTK_ENTRY(mines_entry)));
    }
    gtk_widget_destroy(dialog);

    initialize();

    // Place mines
    int x, y;
    dialog = gtk_message_dialog_new(NULL,
                                     GTK_DIALOG_MODAL,
                                     GTK_MESSAGE_QUESTION,
                                     GTK_BUTTONS_OK_CANCEL,
                                     "Enter row and column to start (separated by space):");
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *start_entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), start_entry);
    gtk_widget_show_all(dialog);
    result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        sscanf(gtk_entry_get_text(GTK_ENTRY(start_entry)), "%d %d", &x, &y);
    }
    gtk_widget_destroy(dialog);

    placeMines(x, y);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Minesweeper");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL    );

    // Create a grid layout
    GtkWidget *grid_layout = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid_layout);

    // Add buttons to the grid layout
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            GtkWidget *button = gtk_button_new_with_label(" ");
            int *coordinates = malloc(2 * sizeof(int));
            coordinates[0] = i;
            coordinates[1] = j;
            g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), coordinates);
            gtk_grid_attach(GTK_GRID(grid_layout), button, j, i, 1, 1);
        }
    }

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window for starting the game
    GtkWidget *start_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(start_window), "Minesweeper Setup");
    gtk_container_set_border_width(GTK_CONTAINER(start_window), 10);
    g_signal_connect(start_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(start_window), vbox);

    // Create a button to start the game
    GtkWidget *start_button = gtk_button_new_with_label("Start Game");
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), start_button, FALSE, FALSE, 0);

    // Show the window to start the game setup
    gtk_widget_show_all(start_window);
    gtk_main();

    return 0;
}


