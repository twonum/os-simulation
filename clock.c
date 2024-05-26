#include <gtk/gtk.h>
#include <sys/time.h> // Added for gettimeofday

gboolean update_time(GtkWidget *label) {
    // Getting current time
    struct timeval tv;
    struct tm *localTime;
    gettimeofday(&tv, NULL); // Updated to use gettimeofday
    localTime = localtime(&tv.tv_sec);

    // Format the time string
    char time_str[9];
    snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

    // Update the label with the current time
    gtk_label_set_text(GTK_LABEL(label), time_str);

    return TRUE; // Continue calling the timeout
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Continuous Clock");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a label to display the time
    GtkWidget *time_label = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(window), time_label);

    // Update the time label every second
    g_timeout_add(1000, (GSourceFunc)update_time, time_label); // Updated to use g_timeout_add

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

