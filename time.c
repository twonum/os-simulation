#include <gtk/gtk.h>
#include <time.h>

// Function to display current time
void display_current_time(GtkWidget *widget, gpointer user_data) {
    // Get current time
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);

    // Format time as string
    char time_string[9]; // HH:MM:SS + null terminator
    snprintf(time_string, sizeof(time_string), "%02d:%02d:%02d", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

    // Display current time in a message dialog
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_INFO,
                                    GTK_BUTTONS_OK,
                                    "Current time: %s",
                                    time_string);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Current Time");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Create a button to display current time
    GtkWidget *button = gtk_button_new_with_label("Show Current Time");
    g_signal_connect(button, "clicked", G_CALLBACK(display_current_time), window);

    // Add button to the window
    gtk_container_add(GTK_CONTAINER(window), button);

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

