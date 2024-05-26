#include <gtk/gtk.h>

// Function to move the file
void move_file(GtkWidget *button, gpointer user_data) {
    // Retrieve the source and destination file paths from the entry fields
    GtkWidget *entry_src = GTK_WIDGET(user_data);
    const gchar *srcPath = gtk_entry_get_text(GTK_ENTRY(entry_src));

    GtkWidget *entry_dest = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "destination"));
    const gchar *destPath = gtk_entry_get_text(GTK_ENTRY(entry_dest));

    // Attempt to move the file
    if (rename(srcPath, destPath) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    "File moved successfully.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_ERROR,
                                                    GTK_BUTTONS_OK,
                                                    "Error moving file: %s", g_strerror(errno));
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "File Mover");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Source file path entry field
    GtkWidget *entry_src = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_src), "Source File Path");
    gtk_box_pack_start(GTK_BOX(vbox), entry_src, FALSE, FALSE, 0);

    // Destination file path entry field
    GtkWidget *entry_dest = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_dest), "Destination File Path");
    gtk_box_pack_start(GTK_BOX(vbox), entry_dest, FALSE, FALSE, 0);

    // Move file button
    GtkWidget *button_move = gtk_button_new_with_label("Move File");
    g_object_set_data(G_OBJECT(button_move), "destination", entry_dest); // Pass destination entry field as user data
    g_signal_connect(button_move, "clicked", G_CALLBACK(move_file), entry_src); // Pass source entry field to move_file function
    gtk_box_pack_start(GTK_BOX(vbox), button_move, FALSE, FALSE, 0);

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

