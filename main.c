#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

#include "Proceso.h"

#define FPS  60
#define WIDTH 800
#define HEIGTH 600

static gint64 last_tick = 0;
static guint tick_cb = 0;
static guint sizeL =32;

int prioridad;
TipoProceso tipoP;

GtkWidget *window;
GtkWidget *drawing;
GtkWidget *entryID;
GtkWidget *entryBITS;

static gboolean on_tick (gpointer user_data) {
    gint64 current = g_get_real_time ();
    gboolean changed = FALSE;
    if ((current - last_tick) < (1000/ FPS)) {
        last_tick = current;
        return G_SOURCE_CONTINUE;
    }

    gtk_widget_queue_draw_area(drawing, 0, 0, WIDTH, HEIGTH);

    last_tick = current;
    return G_SOURCE_CONTINUE;
}

static void on_changedAlgoritmos (GtkComboBox *widget, gpointer   user_data)
{
  GtkComboBox *combo_box = widget;

  int opcion;
  opcion = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
  switch(opcion) {
    case 0:
      prosessPlanificador->algoritmoActual = ARoundR;
      printf("%s\n", "Cambia a Round Robin");
      break;
    case 1:
      prosessPlanificador->algoritmoActual = AFcfs;
      printf("%s\n", "Cambia a FCFS");
      break;
    case 2:
      prosessPlanificador->algoritmoActual = APrioridad;
      printf("%s\n", "Cambia a planificación por prioridad");
      break;
    case 3:
      prosessPlanificador->algoritmoActual = tiempoReal;
      printf("%s\n", "Cambia a planificación de tiempo real");
      break;
    case 4:
      prosessPlanificador->algoritmoActual = sfj;
      printf("%s\n", "Cambia a SFJ");
      break;
    }
}

static void on_changedPrioridad (GtkComboBox *widget, gpointer   user_data)
{
  GtkComboBox *combo_box = widget;

  int opcion;
  opcion = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
  switch(opcion) {
    case 1://rr
     prioridad = 1;
      printf("%s\n", "Cambia a 1");
      break;
    case 2: //fcfs
      prioridad = 2;
      printf("%s\n", "Cambia a 2");
      break;
    case 3://a prioridad
       prioridad = 3;
      printf("%s\n", "Cambia a 3");
      break;
    case 4://rr
     prioridad = 4;
      printf("%s\n", "Cambia a 4");
      break;
    case 5://rr
     prioridad = 5;
      printf("%s\n", "Cambia a 5");
      break;
    case 6://rr
     prioridad = 6;
      printf("%s\n", "Cambia a 6");
      break;
    case 7://rr
     prioridad = 7;
      printf("%s\n", "Cambia a 7");
      break;
    case 8://rr
     prioridad = 8;
      printf("%s\n", "Cambia a 8");
      break;
    case 9://rr
     prioridad = 9;
      printf("%s\n", "Cambia a 9");
      break;
    case 10://rr
     prioridad = 10;
      printf("%s\n", "Cambia a 10");
      break;
    default:
      printf("%s\n", "Opción no soportada por el combobox");
      break;
    }
}

static void on_changedTipo (GtkComboBox *widget, gpointer   user_data)
{
  GtkComboBox *combo_box = widget;

  int opcion;
  opcion = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
  switch(opcion) {
    case 1:
      tipoP = lotes;
      printf("%s\n", "Cambia a Lotes");
      break;
    case 2: 
      tipoP = tReal;
      printf("%s\n", "Cambia a Tiempo Real");
      break;
    case 3:
      tipoP = interactivo;
      printf("%s\n", "Cambia a Iterativo");
      break;
    default:
      printf("%s\n", "Opción no soportada por el combobox");
      break;
  }
}

static void bCrearAleatorio (GtkWidget *widget, gpointer   user_data)
{
  int tipo = 0;
  int prioridad = 0;
  int id = 0;
  int bits = 0;

  TipoProceso tipoTemp;
  srand(time(NULL));

  for (int i = 0; i < 10; ++i)
  {
    tipo = rand() % 3;
    prioridad = 1 + rand() % 9;
    id = rand() %1000;
    bits = rand() %1000;
    switch (tipo) {
      case 0://lotes
        crearProceso(id, bits, prioridad, lotes, "asm.txt");
        break;
      case 1://t real
        crearProceso(id, bits, prioridad, tReal, "asm.txt");
        break;
      case 2: //interactivo
        crearProceso(id, bits, prioridad, interactivo, "asm.txt");
        break;
    }
  }
}

static void bCrear (GtkWidget *widget, gpointer   user_data)
{
  //const gchar *gtk_entry_get_text( GtkEntry *entry );
  GtkWidget* wdg = gtk_file_chooser_dialog_new("Open file", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, 
    "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_OK, NULL);
  char *filename;
  if (gtk_dialog_run(GTK_DIALOG(wdg)) == GTK_RESPONSE_OK) {
    
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (wdg);
    filename = gtk_file_chooser_get_filename (chooser);
    printf("%s\n", filename);
  }
  gtk_widget_destroy (wdg);

  const gchar *tID = gtk_entry_get_text((GtkEntry *)entryID);
  const gchar *tBITS = gtk_entry_get_text((GtkEntry *)entryBITS);

  int id = atoi(tID);
  int bits = atoi(tBITS);
  
  gtk_entry_set_text((GtkEntry *)entryID, " ");
  gtk_entry_set_text((GtkEntry *)entryBITS, " ");

  crearProceso(id, bits, prioridad, tipoP, filename);
}


static void on_draw (GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  drawCicle(cr);
  drawProcess(cr);
}

static void activate (GtkApplication *app, gpointer user_data)
{
  initPlanificador();
  gint i;
  GtkWidget *fixed;
  GtkWidget *combo_box;
  GtkWidget *combo_boxPrioridad; 
  GtkWidget *combo_boxTipo;   

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Simulación");
  gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  fixed = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), fixed);

  drawing = gtk_drawing_area_new();
  gtk_fixed_put(GTK_FIXED(fixed), drawing, 0, 0);
  gtk_widget_set_size_request(drawing, WIDTH, HEIGTH); 
  g_signal_connect(drawing, "draw", G_CALLBACK(on_draw), NULL);
  
  /* Create the combo box and append your string values to it. */
  combo_box = gtk_combo_box_text_new ();
  const char *algoritmos[] = {"Round Robin", "FCFS", "Prioridad", "Tiempo R", "SFJ"};
  for (i = 0; i < G_N_ELEMENTS (algoritmos); i++){
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), algoritmos[i]);
  }
  gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box), 0);
  g_signal_connect (combo_box, "changed", G_CALLBACK (on_changedAlgoritmos), NULL);
  gtk_fixed_put(GTK_FIXED(fixed), combo_box, 400, 150);
  gtk_widget_set_size_request(combo_box, 80, 30);

  GtkWidget *labelAlgoritmo;
  labelAlgoritmo = gtk_label_new("Algoritmo");
  gtk_fixed_put(GTK_FIXED(fixed), labelAlgoritmo, 300, 150);
  gtk_widget_set_size_request(labelAlgoritmo, 75, 30);

  combo_boxPrioridad = gtk_combo_box_text_new ();
  const char *prioridad[] = {"Seleccione una prioridad", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
  for (i = 0; i < G_N_ELEMENTS (prioridad); i++){
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_boxPrioridad), prioridad[i]);
  }
  gtk_combo_box_set_active (GTK_COMBO_BOX (combo_boxPrioridad), 0);
  g_signal_connect (combo_boxPrioridad, "changed", G_CALLBACK (on_changedPrioridad), NULL);
  gtk_fixed_put(GTK_FIXED(fixed), combo_boxPrioridad, 400, 200);
  gtk_widget_set_size_request(combo_boxPrioridad, 80, 30);

  GtkWidget *labelPrioridad;
  labelPrioridad = gtk_label_new("Prioridad");
  gtk_fixed_put(GTK_FIXED(fixed), labelPrioridad, 300, 200);
  gtk_widget_set_size_request(labelPrioridad, 75, 30);

  combo_boxTipo = gtk_combo_box_text_new ();
  const char *tipo[] = {"Lotes", "Tiempo Real", "Iterativo"};
  for (i = 0; i < G_N_ELEMENTS (tipo); i++){
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_boxTipo), tipo[i]);
  }
  gtk_combo_box_set_active (GTK_COMBO_BOX (combo_boxTipo), 0);
  g_signal_connect (combo_boxTipo, "changed", G_CALLBACK (on_changedTipo), NULL);
  gtk_fixed_put(GTK_FIXED(fixed), combo_boxTipo, 400, 250);
  gtk_widget_set_size_request(combo_boxTipo, 80, 30);

  GtkWidget *labelTipo;
  labelTipo = gtk_label_new("Tipo");
  gtk_fixed_put(GTK_FIXED(fixed), labelTipo, 300, 250);
  gtk_widget_set_size_request(labelTipo, 75, 30);

  GtkWidget *labelID;
  labelID = gtk_label_new("ID");
  gtk_fixed_put(GTK_FIXED(fixed), labelID, 300, 300);
  gtk_widget_set_size_request(labelID, 75, 30);

  entryID = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(fixed), entryID, 400, 300);
  gtk_widget_set_size_request(entryID, 75, 30);  

  GtkWidget *labelBITS;
  labelBITS = gtk_label_new("Cantidad de bits");
  gtk_fixed_put(GTK_FIXED(fixed), labelBITS, 275, 350);
  gtk_widget_set_size_request(labelBITS, 75, 30);

  entryBITS = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(fixed), entryBITS, 400, 350);
  gtk_widget_set_size_request(entryBITS, 75, 30);  

  GtkWidget *bottonCrearAleatorio;
  bottonCrearAleatorio = gtk_button_new_with_label("Aleatorio");
  g_signal_connect(bottonCrearAleatorio, "clicked", G_CALLBACK(bCrearAleatorio), NULL);
  gtk_widget_set_size_request(bottonCrearAleatorio, 75, 30);
  gtk_fixed_put(GTK_FIXED(fixed), bottonCrearAleatorio, 250, 400);

  GtkWidget *bottonCrear;
  bottonCrear = gtk_button_new_with_label("Crear Proceso");
  g_signal_connect(bottonCrear, "clicked", G_CALLBACK(bCrear), NULL);
  gtk_widget_set_size_request(bottonCrear, 75, 30);
  gtk_fixed_put(GTK_FIXED(fixed), bottonCrear, 450, 400);

  gtk_widget_show_all (window);
  tick_cb = g_timeout_add(175, (GSourceFunc) on_tick, GINT_TO_POINTER(sizeL));   
}


int main (int argc, char **argv)
{
  prioridad = 0;
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);


  return status;
}