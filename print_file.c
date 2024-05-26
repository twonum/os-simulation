#include <gtk/gtk.h>
#include <cups/cups.h>
#include <stddef.h>

// Function to print error message in a dialog box
void show_error_dialog(const gchar *message) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Function to print the selected file
void print_file(GtkWidget *button, gpointer user_data) {
    const gchar *filename = gtk_entry_get_text(GTK_ENTRY(user_data));

    cups_dest_t *dests;
    int num_dests;
    int job_id;
    FILE *file;

    num_dests = cupsGetDests(&dests);

    if (num_dests <= 0) {
        show_error_dialog("No printers found.");
        return;
    }

    file = fopen(filename, "rb");
    if (file == NULL) {
        show_error_dialog("Failed to open file.");
        return;
    }

    job_id = cupsPrintFile(dests->name, filename, "Print File", 0, NULL);
    if (job_id <= 0) {
        show_error_dialog("Failed to print file.");
        fclose(file);
        return;
    }

    g_print("File sent to printer successfully. Job ID: %d\n", job_id);

    fclose(file);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "File Printer");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *entry_filename = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_filename), "Enter Filename");
    gtk_box_pack_start(GTK_BOX(vbox), entry_filename, FALSE, FALSE, 0);

    GtkWidget *button_print_file = gtk_button_new_with_label("Print File");
    g_signal_connect(button_print_file, "clicked", G_CALLBACK(print_file), entry_filename);
    gtk_box_pack_start(GTK_BOX(vbox), button_print_file, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

