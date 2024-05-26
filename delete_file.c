#include <gtk/gtk.h>
#include <stdio.h>

// Callback function for when the "Delete" button is clicked
void on_delete_button_clicked(GtkWidget *button, gpointer user_data) {
    // Get the filename from the text entry
    const gchar *filename = gtk_entry_get_text(GTK_ENTRY(user_data));

    // Attempt to delete the file
    int status = remove(filename);
    if (status == 0) {
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(NULL,
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_MESSAGE_INFO,
                                         GTK_BUTTONS_OK,
                                         "%s file deleted successfully.", filename);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(NULL,
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_MESSAGE_ERROR,
                                         GTK_BUTTONS_OK,
                                         "Unable to delete the file %s.", filename);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Delete File");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a text entry widget for entering the filename
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter filename to delete");
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    // Create a button to delete the file
    GtkWidget *delete_button = gtk_button_new_with_label("Delete");
    g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_button_clicked), entry);
    gtk_box_pack_start(GTK_BOX(vbox), delete_button, FALSE, FALSE, 0);

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

