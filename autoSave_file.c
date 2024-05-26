#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define FILENAME "data.txt"

// Function to save data to file
void saveToFile(const gchar *data) {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_ERROR,
                                                    GTK_BUTTONS_OK,
                                                    "Error opening file.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    fprintf(file, "%s\n", data);
    fclose(file);
}

// Callback function for the "Save" button
void save_button_clicked(GtkWidget *widget, gpointer entry) {
    const gchar *data = gtk_entry_get_text(GTK_ENTRY(entry));
    if (strlen(data) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_WARNING,
                                                    GTK_BUTTONS_OK,
                                                    "Please enter some data.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    saveToFile(data);
    gtk_entry_set_text(GTK_ENTRY(entry), ""); // Clear the entry field after saving
    GtkWidget *notification = gtk_message_dialog_new(NULL,
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_INFO,
                                                      GTK_BUTTONS_OK,
                                                      "Data saved to file.");
    gtk_dialog_run(GTK_DIALOG(notification));
    gtk_widget_destroy(notification);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Data Saver");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a text entry field
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter data to be saved");
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    // Create a "Save" button
    GtkWidget *save_button = gtk_button_new_with_label("Save");
    g_signal_connect(save_button, "clicked", G_CALLBACK(save_button_clicked), entry);
    gtk_box_pack_start(GTK_BOX(vbox), save_button, FALSE, FALSE, 0);

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

