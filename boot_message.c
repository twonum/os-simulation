#include <gtk/gtk.h>

// Global variables for labels
GtkWidget *boot_label;
GtkWidget *shutdown_label;

// Function to display boot message
gboolean boot_message(gpointer data) {
    gtk_label_set_text(GTK_LABEL(boot_label), "Welcome! System is booting up...");
    return FALSE; // Remove the idle handler after running once
}

// Function to display shutdown message
gboolean shutdown_message(gpointer data) {
    gtk_label_set_text(GTK_LABEL(shutdown_label), "Goodbye! System is shutting down...");
    return FALSE; // Remove the idle handler after running once
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "System Messages");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a label for displaying boot message
    boot_label = gtk_label_new("Welcome! System is booting up...");
    gtk_box_pack_start(GTK_BOX(vbox), boot_label, FALSE, FALSE, 0);

    // Create a label for displaying shutdown message
    shutdown_label = gtk_label_new("Goodbye! System is shutting down...");
    gtk_box_pack_start(GTK_BOX(vbox), shutdown_label, FALSE, FALSE, 0);

    // Show the window
    gtk_widget_show_all(window);

    // Display boot message using an idle handler
    g_idle_add(boot_message, NULL);

    // Start the GTK main loop
    gtk_main();

    // Display shutdown message using an idle handler before exiting
    g_idle_add(shutdown_message, NULL);

    return 0;
}

