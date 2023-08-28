#include "gps.h"
#include <string.h>
#include "crc.h"
#include <stdlib.h>
#include <stdint.h>

enum {
  SENTENCE_GGA = 0,
  SENTENCE_GLL = 1,
  SENTENCE_GSA = 2,
  SENTENCE_GSV = 3,
  SENTENCE_MSS = 4,
  SENTENCE_RMC = 5,
  SENTENCE_VTG = 6,
  SENTENCE_UNKNOWN,
};

const char* sentence_prefix[] = { "GGA", "GLL", "GSA", "GSV",
                                    "MSS", "RMC", "VTG" };

typedef struct {
  float lat;
  float lon;
  union {
    struct {
      char time[8];
      int satc;
      float hdop;
      float altitude;
      float geoid_sep_metres;
    } gga;
    struct {
      float time;
      int flags;
    } gll;
  } data;
  int type; // SENTENCE_GGA or SENTENCE_GLL
} gps_sentence_t;

gps_error_code_t parse_gga(gps_t gps_instance, const char* sentence, int len);
gps_error_code_t parse_gll(gps_t gps_instance, const char* sentence, int len);
// ... (other function declarations)

struct gps_instance_t {
  gps_sentence_t sentence;
  int last_msg_type;
};

int next_field(const char* sentence, int len, int offset) {
  // ... (next_field implementation)
}

gps_t gps_init() {
  struct gps_instance_t* var = calloc(1, sizeof(struct gps_instance_t));
  return var;
}

gps_error_code_t gps_destroy(gps_t gps_instance) {
  free(gps_instance);
  return GPS_NO_ERROR;
}

gps_error_code_t gps_update(gps_t gps_instance, const char* sentence, int len) {
  // ... (gps_update implementation)
}

gps_error_code_t gps_get_lat_lon(gps_t gps_instance, int* degmin, int* minfrac) {
  // ... (gps_get_lat_lon implementation)
}

gps_error_code_t parse_gga(gps_t gps_instance, const char* sentence, int len) {
  // ... (parse_gga implementation)
}

gps_error_code_t parse_gll(gps_t gps_instance, const char* sentence, int len) {
  // ... (parse_gll implementation)
}

// ... (other function implementations)

