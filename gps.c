#include "gps_uart.h"

#include <furi.h>
#include <gui/gui.h>
#include <string.h>

char* resolve_course(float data);

typedef enum
{
  EventTypeTick,
  EventTypeKey,
} EventType;

typedef struct
{
  EventType type;
  InputEvent input;
} PluginEvent;

static void render_callback(Canvas* const canvas, void* context)
{
  const GpsUart* gps_uart = acquire_mutex((ValueMutex*)context, 25);
  if (gps_uart == NULL)
  {
    return;
  }

  canvas_set_font(canvas, FontPrimary);
  canvas_draw_str_aligned(canvas, 32, 8, AlignCenter, AlignBottom, "Latitude");
  canvas_draw_str_aligned(canvas, 96, 8, AlignCenter, AlignBottom, "Longitude");
  canvas_draw_str_aligned(canvas, 21, 30, AlignCenter, AlignBottom, "Course");
  canvas_draw_str_aligned(canvas, 64, 30, AlignCenter, AlignBottom, "Speed");
  canvas_draw_str_aligned(canvas, 107, 30, AlignCenter, AlignBottom, "Altitude");
  canvas_draw_str_aligned(canvas, 32, 52, AlignCenter, AlignBottom, "Satellites");
  canvas_draw_str_aligned(canvas, 96, 52, AlignCenter, AlignBottom, "Last Fix");

  canvas_set_font(canvas, FontSecondary);
  char buffer[64];
  snprintf(buffer, 64, "%f", (double)gps_uart->status.latitude);
  canvas_draw_str_aligned(canvas, 32, 18, AlignCenter, AlignBottom, buffer);
  snprintf(buffer, 64, "%f", (double)gps_uart->status.longitude);
  canvas_draw_str_aligned(canvas, 96, 18, AlignCenter, AlignBottom, buffer);
  
  /*
  if (gps_uart->status.course <= 11.25 && gps_uart->status.course >= 348.76)
  {
    snprintf(buffer, 64, "N");
  } else if (gps_uart->status.course > 11.25 && gps_uart->status.course <= 33.75)
  {
    snprintf(buffer, 64, "N-NE");
  } else if (gps_uart->status.course > 33.75 && gps_uart->status.course <= 56.25)
  {
    snprintf(buffer, 64, "NE");
  } else if (gps_uart->status.course > 56.25 && gps_uart->status.course <= 78.75)
  {
    snprintf(buffer, 64, "E-NE");
  } else if (gps_uart->status.course > 78.75 && gps_uart->status.course <= 101.25)
  {
    snprintf(buffer, 64, "E");
  } else if (gps_uart->status.course > 101.25 && gps_uart->status.course <= 123.75)
  {
    snprintf(buffer, 64, "E-SE");
  } else if (gps_uart->status.course > 123.75 && gps_uart->status.course <= 146.25)
  {
    snprintf(buffer, 64, "SE");
  } else if (gps_uart->status.course > 146.25 && gps_uart->status.course <= 168.75)
  {
    snprintf(buffer, 64, "S-SE");
  } else if (gps_uart->status.course > 168.75 && gps_uart->status.course <= 191.25)
  {
    snprintf(buffer, 64, "S");
  } else if (gps_uart->status.course > 191.25 && gps_uart->status.course <= 213.75)
  {
    snprintf(buffer, 64, "S-SW");
  } else if (gps_uart->status.course > 213.75 && gps_uart->status.course <= 236.25)
  {
    snprintf(buffer, 64, "SW");
  } else if (gps_uart->status.course > 236.25 && gps_uart->status.course <= 258.75)
  {
    snprintf(buffer, 64, "W-SW");
  } else if (gps_uart->status.course > 258.75 && gps_uart->status.course <= 281.25)
  {
    snprintf(buffer, 64, "W");
  } else if (gps_uart->status.course > 281.25 && gps_uart->status.course <= 303.75)
  {
    snprintf(buffer, 64, "W-NW");
  } else if (gps_uart->status.course > 303.75 && gps_uart->status.course <= 326.25)
  {
    snprintf(buffer, 64, "NW");
  } else if (gps_uart->status.course > 326.25 && gps_uart->status.course <= 348.75)
  {
    snprintf(buffer, 64, "N-NW");
  } */
  
  // snprintf(buffer, 64, "%.1f", (double)gps_uart->status.course);
  // snprintf(buffer, 64, data);
  snprintf(buffer, 64, "%s", resolve_course((double)gps_uart->status.course));
  
  canvas_draw_str_aligned(canvas, 21, 40, AlignCenter, AlignBottom, buffer);
  snprintf(buffer, 64, "%.2f Kph", (double)(gps_uart->status.speed * 1.852));
  canvas_draw_str_aligned(canvas, 64, 40, AlignCenter, AlignBottom, buffer);
  snprintf(buffer, 64, "%.1f %c", (double)gps_uart->status.altitude, tolower(gps_uart->status.altitude_units));
  canvas_draw_str_aligned(canvas, 107, 40, AlignCenter, AlignBottom, buffer);
  snprintf(buffer, 64, "%d", gps_uart->status.satellites_tracked);
  canvas_draw_str_aligned(canvas, 32, 62, AlignCenter, AlignBottom, buffer);
  snprintf(buffer, 64, "%02d:%02d:%02d UTC", gps_uart->status.time_hours, gps_uart->status.time_minutes,
           gps_uart->status.time_seconds);
  canvas_draw_str_aligned(canvas, 96, 62, AlignCenter, AlignBottom, buffer);

  release_mutex((ValueMutex*)context, gps_uart);
}


char* resolve_course(float data)
{
  if (data <= 11.25 && data >= 348.76)
  {
    return "N";
  } else if (data > 11.25 && data <= 33.75)
  {
    return "N-NE";
  } else if (data > 33.75 && data <= 56.25)
  {
    return "NE";
  } else if (data > 56.25 && data <= 78.75)
  {
    return "E-NE";
  } else if (data > 78.75 && data <= 101.25)
  {
    return "E";
  } else if (data > 101.25 && data <= 123.75)
  {
    return "E-SE";
  } else if (data > 123.75 && data <= 146.25)
  {
    return "SE";
  } else if (data > 146.25 && data <= 168.75)
  {
    return "S-SE";
  } else if (data > 168.75 && data <= 191.25)
  {
    return "S";
  } else if (data > 191.25 && data <= 213.75)
  {
    return "S-SW";
  } else if (data > 213.75 && data <= 236.25)
  {
    return "SW";
  } else if (data > 236.25 && data <= 258.75)
  {
    return "W-SW";
  } else if (data > 258.75 && data <= 281.25)
  {
    return "W";
  } else if (data > 281.25 && data <= 303.75)
  {
    return "W-NW";
  } else if (data > 303.75 && data <= 326.25)
  {
    return "NW";
  } else if (data > 326.25 && data <= 348.75)
  {
    return "N-NW";
  } else 
  {
    return "nan";
  }
}

static void input_callback(InputEvent* input_event, FuriMessageQueue* event_queue)
{
  furi_assert(event_queue);

  PluginEvent event = {.type = EventTypeKey, .input = *input_event};
  furi_message_queue_put(event_queue, &event, FuriWaitForever);
}

int32_t gps_app(void* p)
{
  UNUSED(p);

  FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(PluginEvent));

  GpsUart* gps_uart = gps_uart_enable();

  ValueMutex gps_uart_mutex;
  if (!init_mutex(&gps_uart_mutex, gps_uart, sizeof(GpsUart)))
  {
    FURI_LOG_E("GPS", "cannot create mutex\r\n");
    free(gps_uart);
    return 255;
  }

  // set system callbacks
  ViewPort* view_port = view_port_alloc();
  view_port_draw_callback_set(view_port, render_callback, &gps_uart_mutex);
  view_port_input_callback_set(view_port, input_callback, event_queue);

  // open GUI and register view_port
  Gui* gui = furi_record_open("gui");
  gui_add_view_port(gui, view_port, GuiLayerFullscreen);

  PluginEvent event;
  for (bool processing = true; processing;)
  {
    FuriStatus event_status = furi_message_queue_get(event_queue, &event, 100);

    GpsUart* gps_uart = (GpsUart*)acquire_mutex_block(&gps_uart_mutex);

    if (event_status == FuriStatusOk)
    {
      // press events
      if (event.type == EventTypeKey)
      {
        if (event.input.type == InputTypePress)
        {
          switch (event.input.key)
          {
            case InputKeyBack:
              processing = false;
              break;
            default:
              break;
          }
        }
      }
    } 
    else
    {
      FURI_LOG_D("GPS", "FuriMessageQueue: event timeout");
    }

    view_port_update(view_port);
    release_mutex(&gps_uart_mutex, gps_uart);
  }

  view_port_enabled_set(view_port, false);
  gui_remove_view_port(gui, view_port);
  furi_record_close("gui");
  view_port_free(view_port);
  furi_message_queue_free(event_queue);
  delete_mutex(&gps_uart_mutex);
  gps_uart_disable(gps_uart);

  return 0;
}
