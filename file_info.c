#include <gtk/gtk.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to display file information using a message dialog
void display_file_info(const gchar *filename) {
    struct stat fileStat;

    // Check if the file exists
    if (stat(filename, &fileStat) < 0) {
        GtkWidget *error_dialog = gtk_message_dialog_new(NULL,
                                                          GTK_DIALOG_MODAL,
                                                          GTK_MESSAGE_ERROR,
                                                          GTK_BUTTONS_OK,
                                                          "Error: Unable to stat file.");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }

    // Format file information
    char info_message[500];
    snprintf(info_message, sizeof(info_message), "File Information:\n"
                                                  "File Name: %s\n"
                                                  "Size: %ld bytes\n"
                                                  "Permissions: %c%c%c%c%c%c%c%c%c%c\n"
                                                  "Last Modified: %s",
                                                  filename,
                                                  fileStat.st_size,
                                                  (S_ISDIR(fileStat.st_mode)) ? 'd' : '-',
                                                  (fileStat.st_mode & S_IRUSR) ? 'r' : '-',
                                                  (fileStat.st_mode & S_IWUSR) ? 'w' : '-',
                                                  (fileStat.st_mode & S_IXUSR) ? 'x' : '-',
                                                  (fileStat.st_mode & S_IRGRP) ? 'r' : '-',
                                                  (fileStat.st_mode & S_IWGRP) ? 'w' : '-',
                                                  (fileStat.st_mode & S_IXGRP) ? 'x' : '-',
                                                  (fileStat.st_mode & S_IROTH) ? 'r' : '-',
                                                  (fileStat.st_mode & S_IWOTH) ? 'w' : '-',
                                                  (fileStat.st_mode & S_IXOTH) ? 'x' : '-',
                                                  ctime(&fileStat.st_mtime));

    // Display file information in a message dialog
    GtkWidget *info_dialog = gtk_message_dialog_new(NULL,
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    "%s", info_message);
    gtk_dialog_run(GTK_DIALOG(info_dialog));
    gtk_widget_destroy(info_dialog);
}

// Callback function for when the "Get Info" button is clicked
void on_get_info_button_clicked(GtkWidget *button, gpointer user_data) {
    // Get the filename from the text entry
    const gchar *filename = gtk_entry_get_text(GTK_ENTRY(user_data));
    display_file_info(filename);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "File Information");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a text entry widget for entering the filename
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter filename");
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    // Create a button to get file info
    GtkWidget *get_info_button = gtk_button_new_with_label("Get Info");
    g_signal_connect(get_info_button, "clicked", G_CALLBACK(on_get_info_button_clicked), entry);
    gtk_box_pack_start(GTK_BOX(vbox), get_info_button, FALSE, FALSE, 0);

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

