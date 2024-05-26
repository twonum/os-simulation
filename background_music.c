#include <stdio.h>
#include <SDL2/SDL.h>
#include <unistd.h>
#include <gtk/gtk.h>

// Global variables for SDL
SDL_AudioSpec wavSpec;
Uint32 wavLength;
Uint8 *wavBuffer;
SDL_AudioDeviceID deviceId;

// Callback function to close the SDL audio device
gboolean close_audio_device(gpointer data) {
    SDL_AudioDeviceID dev = *((SDL_AudioDeviceID *)data);
    SDL_CloseAudioDevice(dev);
    return G_SOURCE_REMOVE;
}

// Callback function to free the WAV buffer
gboolean free_wav_buffer(gpointer data) {
    Uint8 *buffer = (Uint8 *)data;
    SDL_FreeWAV(buffer);
    return G_SOURCE_REMOVE;
}

// Function to play background music
void play_background_music() {
    // Load the music file
    if (SDL_LoadWAV("background_music.wav", &wavSpec, &wavBuffer, &wavLength) == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "SDL_LoadWAV failed: %s", SDL_GetError());
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Open the audio device
    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    if (deviceId == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "SDL_OpenAudioDevice failed: %s", SDL_GetError());
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        SDL_FreeWAV(wavBuffer);
        return;
    }

    // Queue the audio buffer for playback
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    
    // Start audio playback
    SDL_PauseAudioDevice(deviceId, 0);

    // Calculate the duration of the playback in seconds
    int playback_duration = wavLength / wavSpec.freq / wavSpec.channels / (wavSpec.format == AUDIO_U8 ? 1 : 2);

    // Set timers to clean up after playback
    g_timeout_add_seconds(playback_duration, close_audio_device, &deviceId);
    g_timeout_add_seconds(playback_duration, free_wav_buffer, wavBuffer);
}

// Callback function for the "Play Music" button
void play_music_button_clicked(GtkWidget *widget, gpointer data) {
    // Run music playback in a separate thread
    g_thread_new("background_music_thread", (GThreadFunc)play_background_music, NULL);
}

// Callback function for the "Return to Main Menu" button
void return_to_menu_button_clicked(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Background Music Player");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a "Play Music" button
    GtkWidget *play_music_button = gtk_button_new_with_label("Play Music");
    g_signal_connect(play_music_button, "clicked", G_CALLBACK(play_music_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), play_music_button, FALSE, FALSE, 0);

    // Create a "Return to Main Menu" button
    GtkWidget *return_to_menu_button = gtk_button_new_with_label("Return to Main Menu");
    g_signal_connect(return_to_menu_button, "clicked", G_CALLBACK(return_to_menu_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), return_to_menu_button, FALSE, FALSE, 0);

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

