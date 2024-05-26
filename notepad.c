#include <gtk/gtk.h>
#include <stdio.h>

// Function prototype
void save_and_close(GtkWidget *dialog, gint response, gpointer user_data);

// Function to create or open a file and display its contents
void create_or_open_file(GtkWidget *button, gpointer user_data) {
    // Retrieve the filename from the entry field
    const gchar *filename = gtk_entry_get_text(GTK_ENTRY(user_data));

    FILE *fp;

    // Opening the file in read mode
    fp = fopen(filename, "r");

    // If the file does not exist, create a new one
    if (fp == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "File does not exist. Creating a new one.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        fp = fopen(filename, "w");
        fclose(fp);
        fp = fopen(filename, "r");
    }

    // Display file contents in a dialog window
    GtkWidget *dialog = gtk_dialog_new_with_buttons("File Contents", NULL, GTK_DIALOG_MODAL, "_OK", GTK_RESPONSE_OK, NULL);
    g_object_set_data(G_OBJECT(dialog), "entry_filename", user_data);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), TRUE); // Set text view editable
    gtk_container_add(GTK_CONTAINER(scroll), text_view);
    gtk_container_add(GTK_CONTAINER(content_area), scroll);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    if (fp != NULL) {
        char line[256]; // Adjust the buffer size as needed
        while (fgets(line, sizeof(line), fp) != NULL) {
            gtk_text_buffer_insert_at_cursor(buffer, line, -1);
            // Let GTK process events to prevent freezing
            while (gtk_events_pending()) {
                gtk_main_iteration();
            }
        }
        fclose(fp);
    }

    // Save the text buffer to the file when the "OK" button is pressed
    g_signal_connect(dialog, "response", G_CALLBACK(save_and_close), text_view);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Function to save the text buffer to the file and close the dialog
void save_and_close(GtkWidget *dialog, gint response, gpointer user_data) {
    if (response == GTK_RESPONSE_OK) {
        GtkWidget *text_view = GTK_WIDGET(user_data);
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        GtkWidget *entry = g_object_get_data(G_OBJECT(dialog), "entry_filename");
        const gchar *filename = gtk_entry_get_text(GTK_ENTRY(entry));

        FILE *fp = fopen(filename, "w");
        if (fp != NULL) {
            fputs(text, fp);
            fclose(fp);
        }

        g_free(text);
    }
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "File Creator/Opener");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Filename entry field
    GtkWidget *entry_filename = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_filename), "Enter Filename");
    gtk_box_pack_start(GTK_BOX(vbox), entry_filename, FALSE, FALSE,0);

    // Create/open file button
    GtkWidget *button_create_or_open = gtk_button_new_with_label("Create/Open File");
    g_signal_connect(button_create_or_open, "clicked", G_CALLBACK(create_or_open_file), entry_filename);
    gtk_box_pack_start(GTK_BOX(vbox), button_create_or_open, FALSE, FALSE, 0);

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
