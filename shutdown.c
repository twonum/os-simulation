#include <gtk/gtk.h>
#include <stdlib.h>

// Function to execute the shutdown command
void execute_shutdown(GtkWidget *button, gpointer user_data) {
    // Execute the shutdown command
    system("shutdown -h now");
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Shutdown App");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Shutdown button
    GtkWidget *button_shutdown = gtk_button_new_with_label("Shutdown");
    g_signal_connect(button_shutdown, "clicked", G_CALLBACK(execute_shutdown), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button_shutdown, FALSE, FALSE, 0);

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

