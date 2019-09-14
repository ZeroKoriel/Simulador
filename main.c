#include <gtk/gtk.h>

#include "Proceso.h"

#define FPS  60
#define WIDTH 800
#define HEIGTH 600

static gint64 last_tick = 0;
static guint tick_cb = 0;
static guint sizeL =32;

GtkWidget *drawing;

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

static void on_changed (GtkComboBox *widget, gpointer   user_data)
{
  GtkComboBox *combo_box = widget;

  int opcion;
  opcion = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
  switch(opcion) {
    case 1://rr
      prosessPlanificador->algoritmoActual = ARoundR;
      printf("%s\n", "Cambia a Round Robin");
      break;
    case 2: //fcfs
      prosessPlanificador->algoritmoActual = AFcfs;
      printf("%s\n", "Cambia a FCFS");
      break;
    case 3://a prioridad
      prosessPlanificador->algoritmoActual = APrioridad;
      printf("%s\n", "Cambia a planificación por prioridad");
      break;
    case 4:
      prosessPlanificador->algoritmoActual = tiempoReal;
      printf("%s\n", "Cambia a planificación por tiempo real");
      break;
    default:
      printf("%s\n", "Opción no soportada por el combobox");
      break;
    }
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
  GtkWidget *window;
  GtkWidget *combo_box;  

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
  const char *algoritmos[] = {"Seleccione un algoritmo", "Round Robin", "FCFS", "P. Prioridad", "Tiempo real"};

 
  /* G_N_ELEMENTS is a macro which determines the number of elements in an array.*/ 
  for (i = 0; i < G_N_ELEMENTS (algoritmos); i++){
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), algoritmos[i]);
  }

  /* Choose to set the first row as the active one by default, from the beginning */
  gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box), 0);

  /* Connect the signal emitted when a row is selected to the appropriate
   * callback function.
   */
  g_signal_connect (combo_box, "changed", G_CALLBACK (on_changed), NULL);

  gtk_fixed_put(GTK_FIXED(fixed), combo_box, 300, 450);
  gtk_widget_set_size_request(combo_box, 80, 30); 

  gtk_widget_show_all (window);

  tick_cb = g_timeout_add(10000 / FPS / 2, (GSourceFunc) on_tick, GINT_TO_POINTER(sizeL)); 
}


int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}