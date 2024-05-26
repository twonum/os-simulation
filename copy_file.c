#include <gtk/gtk.h>
#include <stdlib.h>

// Function to display messages in a GTK label
void display_message(GtkWidget *label, const gchar *message) {
    gtk_label_set_text(GTK_LABEL(label), message);
}

// Callback function for when the "Copy File" button is clicked
void on_copy_button_clicked(GtkWidget *button, gpointer user_data) {
    // Get the file names from the entry widgets
    GtkWidget *source_entry = GTK_WIDGET(user_data);
    GtkWidget *target_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "target_entry"));
    const gchar *source_filename = gtk_entry_get_text(GTK_ENTRY(source_entry));
    const gchar *target_filename = gtk_entry_get_text(GTK_ENTRY(target_entry));

    // Open the source file in read mode
    FILE *sourceFile = fopen(source_filename, "r");
    if (sourceFile == NULL) {
        display_message(GTK_WIDGET(g_object_get_data(G_OBJECT(button), "message_label")), "Error! Unable to open the file.");
        return;
    }

    // Open the target file in write mode
    FILE *targetFile = fopen(target_filename, "w");
    if (targetFile == NULL) {
        fclose(sourceFile);
        display_message(GTK_WIDGET(g_object_get_data(G_OBJECT(button), "message_label")), "Error! Unable to create the target file.");
        return;
    }

    // Copy contents of source file to target file
    char ch;
    while ((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, targetFile);
    }

    display_message(GTK_WIDGET(g_object_get_data(G_OBJECT(button), "message_label")), "File copied successfully.");

    // Close files
    fclose(sourceFile);
    fclose(targetFile);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "File Copy");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create entry widgets for source and target file names
    GtkWidget *source_entry = gtk_entry_new();
    GtkWidget *target_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(source_entry), "Source File Name");
    gtk_entry_set_placeholder_text(GTK_ENTRY(target_entry), "Target File Name");
    gtk_box_pack_start(GTK_BOX(vbox), source_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), target_entry, FALSE, FALSE, 0);

    // Create a button to copy the file
    GtkWidget *copy_button = gtk_button_new_with_label("Copy File");
    g_object_set_data(G_OBJECT(copy_button), "target_entry", target_entry);
    GtkWidget *message_label = gtk_label_new("");
    g_object_set_data(G_OBJECT(copy_button), "message_label", message_label);
    g_signal_connect(copy_button, "clicked", G_CALLBACK(on_copy_button_clicked), source_entry);
    gtk_box_pack_start(GTK_BOX(vbox), copy_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), message_label, FALSE, FALSE, 0);

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

