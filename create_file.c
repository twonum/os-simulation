#include <gtk/gtk.h>
#include <stdlib.h>

// Callback function for when the "Save" button is clicked
void on_save_button_clicked(GtkWidget *button, gpointer user_data) {
    // Get the file pointer and text buffer from user data
    FILE *filePointer = (FILE *)user_data;
    GtkTextBuffer *textBuffer = GTK_TEXT_BUFFER(g_object_get_data(G_OBJECT(button), "text_buffer"));

    // Get the text from the text buffer
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(textBuffer, &start, &end);
    gchar *text = gtk_text_buffer_get_text(textBuffer, &start, &end, FALSE);

    // Write data to the file
    fprintf(filePointer, "%s", text);

    // Check for any error while writing
    if (ferror(filePointer)) {
        g_print("Error while writing to file.\n");
    } else {
        g_print("File saved successfully.\n");
    }

    // Free memory allocated for text
    g_free(text);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Create File");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a text view widget for entering text
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);

    // Create a button to save the text to file
    GtkWidget *save_button = gtk_button_new_with_label("Save");
    g_object_set_data(G_OBJECT(save_button), "text_buffer", gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)));

    // Open file dialog to get the filename
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File",
                                                    GTK_WINDOW(window),
                                                    GTK_FILE_CHOOSER_ACTION_SAVE,
                                                    "Cancel",
                                                    GTK_RESPONSE_CANCEL,
                                                    "Save",
                                                    GTK_RESPONSE_ACCEPT,
                                                    NULL);

    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        FILE *filePointer = fopen(filename, "w"); // Open file for writing
        g_object_set_data(G_OBJECT(save_button), "file_pointer", filePointer);
        g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_button_clicked), filePointer);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);

    gtk_box_pack_start(GTK_BOX(vbox), save_button, FALSE, FALSE, 0);

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

