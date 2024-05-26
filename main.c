#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include <pthread.h>

// Function prototypes
bool canRunTask(int taskChoice);
void *runTask(void *arg);

// Define task constants
#define NUM_TASKS 12
enum Task { CLOCK = 1, BACKGROUND_MUSIC, CALCULATOR, NOTEPAD, CREATE_FILE, COPY_FILE, DELETE_FILE, MOVE_FILE, FILE_INFO, PRINT_FILE, GAME, SHUTDOWN };

// Task labels
const char *taskLabels[NUM_TASKS] = {
    "Clock",
    "Background Music",
    "Calculator",
    "Notepad",
    "Create File",
    "Copy File",
    "Delete File",
    "Move File",
    "File Info",
    "Print File",
    "Game",
    "Shutdown"
};

// Callback function for when a task button is clicked
void on_task_button_clicked(GtkWidget *button, gpointer user_data) {
    int taskChoice = GPOINTER_TO_INT(user_data);
    // Check if the selected task can run
    if (!canRunTask(taskChoice)) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "Cannot run selected task. Necessary files are missing.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    // Execute the selected task in a separate thread
    pthread_t tid;
    pthread_create(&tid, NULL, runTask, GINT_TO_POINTER(taskChoice));
}

// Function to check if a task can run
bool canRunTask(int taskChoice) {
    // Check if the necessary files exist for each task
    switch (taskChoice) {
        case CLOCK:
            return access("./clock", X_OK) != -1;
        case BACKGROUND_MUSIC:
            return access("./background_music", X_OK) != -1;
        case CALCULATOR:
            return access("./calculator", X_OK) != -1;
        case NOTEPAD:
            return access("./notepad", X_OK) != -1;
        case CREATE_FILE:
            return access("./create_file", X_OK) != -1;
        case COPY_FILE:
            return access("./copy_file", X_OK) != -1;
        case DELETE_FILE:
            return access("./delete_file", X_OK) != -1;
        case MOVE_FILE:
            return access("./move_file", X_OK) != -1;
        case FILE_INFO:
            return access("./file_info", X_OK) != -1;
        case PRINT_FILE:
            return access("./print_file", X_OK) != -1;
        case GAME:
            return access("./minigame", X_OK) != -1;
        case SHUTDOWN:
            return access("./shutdown", X_OK) != -1;
        default:
            return false;
    }
}

// Function to run a task
void *runTask(void *arg) {
    int taskChoice = GPOINTER_TO_INT(arg);
    // Execute the selected task
    switch (taskChoice) {
        case CLOCK:
            system("./clock");
            break;
        case BACKGROUND_MUSIC:
            system("./background_music");
            break;
        case CALCULATOR:
            system("./calculator");
            break;
        case NOTEPAD:
            system("./notepad");
            break;
        case CREATE_FILE:
            system("./create_file");
            break;
        case COPY_FILE:
            system("./copy_file");
            break;
        case DELETE_FILE:
            system("./delete_file");
            break;
        case MOVE_FILE:
            system("./move_file");
            break;
        case FILE_INFO:
            system("./file_info");
            break;
        case PRINT_FILE:
            system("./print_file");
            break;
        case GAME:
            system("./minigame");
            break;
        case SHUTDOWN:
            system("./shutdown");
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MyOS - Majestic TAHA");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Add a label widget at the top
    GtkWidget *label = gtk_label_new("Majestic TAHA");
    gtk_label_set_markup(GTK_LABEL(label), "<span font_desc=\"48\" foreground=\"#6A5ACD\"><b>Majestic TAHA</b></span>");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    // Add task buttons with CSS styling
    for (int i = 0; i < NUM_TASKS; i++) {
        GtkWidget *button = gtk_button_new_with_label(taskLabels[i]);
        gtk_widget_set_name(button, "task-button");
        g_signal_connect(button, "clicked", G_CALLBACK(on_task_button_clicked), GINT_TO_POINTER(i + 1));
        gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
    }

    // Apply CSS styling to the buttons
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "button#task-button {"
        "   background-color: #E0FFFF;"
        "   color: #000000;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   font-size: 16px;"
        "   margin-top: 5px;"
        "}\n"
        "button#task-button:hover {"
        "   background-color: #ADD8E6;"
        "}\n",
        -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

