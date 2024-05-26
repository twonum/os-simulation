#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

// Function to perform arithmetic operations
void calculate(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;

    GtkEntry *num1_entry = GTK_ENTRY(entries[0]);
    const char *num1_str = gtk_entry_get_text(num1_entry);
    double num1 = atof(num1_str);

    GtkEntry *num2_entry = GTK_ENTRY(entries[1]);
    const char *num2_str = gtk_entry_get_text(num2_entry);
    double num2 = atof(num2_str);

    GtkEntry *operator_entry = GTK_ENTRY(entries[2]);
    const char *operator = gtk_entry_get_text(operator_entry);

    GtkEntry *result_entry = GTK_ENTRY(entries[3]);

    double result = 0;
    switch (*operator) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
            } else {
                gtk_entry_set_text(result_entry, "Error! Division by zero.");
                return;
            }
            break;
        default:
            gtk_entry_set_text(result_entry, "Error! Invalid operator.");
            return;
    }

    char result_str[50];
    snprintf(result_str, sizeof(result_str), "%.2lf", result);
    gtk_entry_set_text(result_entry, result_str);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *num1_label = gtk_label_new("Enter first number:");
    gtk_box_pack_start(GTK_BOX(vbox), num1_label, FALSE, FALSE, 0);

    GtkWidget *num1_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), num1_entry, FALSE, FALSE, 0);

    GtkWidget *num2_label = gtk_label_new("Enter second number:");
    gtk_box_pack_start(GTK_BOX(vbox), num2_label, FALSE, FALSE, 0);

    GtkWidget *num2_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), num2_entry, FALSE, FALSE, 0);

    GtkWidget *operator_label = gtk_label_new("Enter operator (+, -, *, /):");
    gtk_box_pack_start(GTK_BOX(vbox), operator_label, FALSE, FALSE, 0);

    GtkWidget *operator_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), operator_entry, FALSE, FALSE, 0);

    GtkWidget *result_label = gtk_label_new("Result:");
    gtk_box_pack_start(GTK_BOX(vbox), result_label, FALSE, FALSE, 0);

    GtkWidget *result_entry = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(result_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), result_entry, FALSE, FALSE, 0);

    GtkWidget *entries[4] = { num1_entry, num2_entry, operator_entry, result_entry };

    GtkWidget *calculate_button = gtk_button_new_with_label("Calculate");
    g_signal_connect(calculate_button, "clicked", G_CALLBACK(calculate), entries);

    GtkWidget *calculate_button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), calculate_button_box, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(calculate_button_box), calculate_button);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

