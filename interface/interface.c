#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

//global variables
gchar* File;
gchar* TempFile;
int Filel;
int TempFilel;
int state = 0;
GtkImage* sudoku_i;

// Signal handler for the "clicked" signal of programmed button.
void debug(GtkButton *button, gpointer user_data)
{
    g_print("this shit ain't programmed!\n");
}

//File stuff
///////////////////////////////////////////////////////////////////////////////

// Signal handler for open file.
void openfilechooser(GtkButton *button, gpointer user_data)
{
    gtk_widget_show(user_data);
}

// Signal handler for save file
void openfilesaver(GtkButton *button, gpointer user_data)
{
    if (state > 0)
        gtk_widget_show(user_data);
}

// Signal handler for cancel in file chooser
void closefilechooser(GtkButton *button, gpointer user_data)
{
    gtk_widget_hide(user_data);
}

// Signal handler for open file in file chooser.
void openfile(GtkButton *button, gpointer user_data)
{
    gchar* filename = gtk_file_chooser_get_filename((GtkFileChooser*) gtk_widget_get_toplevel((GtkWidget*) button));
    File = filename;
    Filel = strlen(File);
    gchar* tempfolder = gtk_file_chooser_get_current_folder((GtkFileChooser*) gtk_widget_get_toplevel((GtkWidget*) button));
    char* tempname = "/ocrinterfacetempfile.jpeg";
    char* fulltemp = (char*) malloc(1 + strlen(tempfolder) + strlen(tempname));
    strcpy(fulltemp, tempfolder);
    strcat(fulltemp, tempname);
    TempFile = fulltemp;
    TempFilel= strlen(TempFile);
    char com[4 + Filel + TempFilel];
    strcpy(com, "cp ");
    strcat(com, File);
    strcat(com, " ");
    strcat(com, TempFile);
    system(com);
    GdkPixbuf* img = gdk_pixbuf_new_from_file_at_scale(filename, 270, 270, 0, NULL);
    gtk_image_set_from_pixbuf(sudoku_i, img);
    state = 1;
    gtk_widget_hide(gtk_widget_get_toplevel((GtkWidget*) button));
}

// Signal handler for saving current file
void savequick(GtkButton *button, gpointer user_data)
{
    if (state == 0)
        return;
    printf("%s\n", TempFile); 
    char com[4 + TempFilel + Filel];
    strcpy(com, "cp ");
    strcat(com, TempFile);
    strcat(com, " ");
    strcat(com, File);
    system(com);
    //gdk_pixbuf_save(gtk_image_get_pixbuf(sudoku_i), File, "jpeg", NULL, NULL);
}

// Signal handler for saving file as another in file saver
void savefile(GtkButton *button, gpointer user_data)
{
    printf("%s\n", TempFile);
    GtkEntry* name = (GtkEntry*) user_data;
    gchar* folder = gtk_file_chooser_get_current_folder((GtkFileChooser*) gtk_widget_get_toplevel((GtkWidget*) button));
    char com[8 + TempFilel + strlen(folder) + strlen(gtk_entry_get_text(name))];
    strcpy(com, "cp ");
    strcat(com, TempFile);
    strcat(com, " ");
    strcat(com, folder);
    strcat(com, "/");
    strcat(com, gtk_entry_get_text(name));
    system(com);
    //gdk_pixbuf_save(gtk_image_get_pixbuf(sudoku_i), fullname, "jpeg", NULL, NULL);
}

void txt_to_img(char* file1, char* file2)
{
    if (file2 == NULL)
    {
        char com[14 + strlen(file1)];
        strcpy(com, "./tticonvert ");
        strcat(com, file1);
	system(com);
    }
    else
    {
        char com[14 + strlen(file1) + strlen(file2)];
        strcpy(com, "./tticonvert ");
        strcat(com, file1);
	strcat(com, file2);
	system(com);
    }
}

//Sub programs
///////////////////////////////////////////////////////////////////////////////

void preprocess(GtkButton *buttpn, gpointer user_data)
{
    if (state != 1)
        return;
    rotateauto(button, NULL);
    noisecancel(button, NULL);
    contrastenhance(button, NULL);
}

void rotate(GtkButton *buttpn, gpointer user_data)
{
    if (state != 1)
        return;
     GtkSpinButton* angle = user_data;
}

void rotateauto(GtkButton *buttpn, gpointer user_data)
{
    if (state != 1)
        return;
}

void noisecancel(GtkButton *buttpn, gpointer user_data)
{
    if (state != 1)
        return;
}

void contrastenhance(GtkButton *buttpn, gpointer user_data)
{
    if (state != 1)
        return;
}

void splitimage(GtkButton *buttpn, gpointer user_data)
{
    if (state != 1)
        return;

    state = 2;
}

void detectdigits(GtkButton *buttpn, gpointer user_data)
{
    if (state != 2)
        return;

    state = 3
}

void solvesudoku(GtkButton *buttpn, gpointer user_data)
{
    if (state != 3)
        return;
}

//Main everything
///////////////////////////////////////////////////////////////////////////////

// Main function.
int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new ();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.mainwindow"));
    sudoku_i = GTK_IMAGE(gtk_builder_get_object(builder, "sudoku"));
    // File
    GtkButton* openfile_b = GTK_BUTTON(gtk_builder_get_object(builder, "openfile"));
    GtkButton* savefile_b = GTK_BUTTON(gtk_builder_get_object(builder, "savefile"));
    GtkButton* saveas_b = GTK_BUTTON(gtk_builder_get_object(builder, "saveas"));
    // Pre-process
    GtkButton* preprocessauto_b= GTK_BUTTON(gtk_builder_get_object(builder, "preprocessauto"));
    GtkSpinButton* inputrot_b = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "inputrot"));
    GtkButton* rotate_b = GTK_BUTTON(gtk_builder_get_object(builder, "rotate"));
    GtkButton* rotateauto_b = GTK_BUTTON(gtk_builder_get_object(builder, "rotateauto"));
    GtkButton* noisecancel_b = GTK_BUTTON(gtk_builder_get_object(builder, "noisecancel"));
    GtkButton* contrastenhance_b = GTK_BUTTON(gtk_builder_get_object(builder, "contrastenhance"));
    // Process
    GtkButton* splitimage_b = GTK_BUTTON(gtk_builder_get_object(builder, "splitimage"));
    GtkButton* detectdigits_b = GTK_BUTTON(gtk_builder_get_object(builder, "detectdigits"));
    GtkButton* solvesudoku_b = GTK_BUTTON(gtk_builder_get_object(builder, "solvesudoku"));
    // Open file
    GtkFileChooser* filechooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "filefinderwindow"));
    GtkButton* filechooseropen = GTK_BUTTON(gtk_builder_get_object(builder, "filefinderopen"));
    GtkButton* filechoosercancel = GTK_BUTTON(gtk_builder_get_object(builder, "filefindercancel"));
    // Save file
    GtkFileChooser* filesaver = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "filesaverwindow"));
    GtkButton* filesaversave = GTK_BUTTON(gtk_builder_get_object(builder, "filesaversave"));
    GtkButton* filesavercancel = GTK_BUTTON(gtk_builder_get_object(builder, "filesavercancel"));
    GtkEntry* filesavername = GTK_ENTRY(gtk_builder_get_object(builder, "filesavername"));

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // File
    g_signal_connect(openfile_b, "clicked", G_CALLBACK(openfilechooser), filechooser);
    g_signal_connect(savefile_b, "clicked", G_CALLBACK(savequick), NULL);
    g_signal_connect(saveas_b, "clicked", G_CALLBACK(openfilesaver), filesaver);
    // Pre-process
    g_signal_connect(preprocessauto_b, "clicked", G_CALLBACK(debug), NULL);
    g_signal_connect(rotate_b, "clicked", G_CALLBACK(debug), inputrot_b);
    g_signal_connect(rotateauto_b, "clicked", G_CALLBACK(debug), NULL);
    g_signal_connect(noisecancel_b, "clicked", G_CALLBACK(debug), NULL);
    g_signal_connect(contrastenhance_b, "clicked", G_CALLBACK(debug), NULL);
    // Process
    g_signal_connect(splitimage_b, "clicked", G_CALLBACK(debug), NULL);
    g_signal_connect(detectdigits_b, "clicked", G_CALLBACK(debug), NULL);
    g_signal_connect(solvesudoku_b, "clicked", G_CALLBACK(debug), NULL);
    // Open file
    g_signal_connect(filechooseropen, "clicked", G_CALLBACK(openfile), NULL);
    g_signal_connect(filechoosercancel, "clicked", G_CALLBACK(closefilechooser), filechooser);
    // Save file
    g_signal_connect(filesaversave, "clicked", G_CALLBACK(savefile), filesavername);
    g_signal_connect(filesavercancel, "clicked", G_CALLBACK(closefilechooser), filesaver);

    // Runs the main loop.
    gtk_main();

    //clears temporary file
    char com[3 + TempFilel];
    strcpy(com, "rm ");
    strcat(com, TempFile);
    system(com);
 
    // Exits.
    return 0;
}
