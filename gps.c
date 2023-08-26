#include "gps.h"
#include <string.h>
#include "crc.h"
#include <stdlib.h>
#include <time.h>
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
gps_error_code_t parse_gga(const char* sentence, int len);
gps_error_code_t (* const sentence_parsers[])(const char*, int) = {
  parse_gga,
};
struct gps_gga_t {
  float lat;
  float lon;
  char time[8];
};
struct gps_instance_t
{};

int next_field(const char* sentence, int len, int offset) {
  for (int i = offset; i < len; i++) {
    if (sentence[i] == ',') return i;
  }
  return -1;
}
gps_error_code_t gps_update(gps_t gps_instance, const char* sentence, int len) {
  if (verify_checksum(sentence, len) == 0) return GPS_INVALID_CHECKSUM;
  int current_sentence = SENTENCE_UNKNOWN;
  for (int i = 0; i < SENTENCE_UNKNOWN; i++) {
    if (strncmp(sentence+2, sentence_prefix[i], 3) == 0) {
      current_sentence = i;
      break;
    }
  }
  if (current_sentence == SENTENCE_UNKNOWN) {
    return GPS_UNKNOWN_PREFIX;
  }
  return GPS_NO_ERROR;
}
gps_error_code_t parse_gga(const char* sentence, int len) {
  int fieldc = 0;
  struct gps_gga_t gga;
  for (int i = 0, j = 0; i < len; i = j+1) {
    j = next_field(sentence, len, i);
    if (j == -1) j = len;
    int empty_field = i == j;

    if (fieldc == 1) {
      // Time
      strncpy(gga.time, sentence, 6);
    }
    else if (fieldc == 2) {
      // Latitude
      gga.lat = strtod(sentence+i, NULL);
    }
    else if (fieldc == 3) {
      // Latitude direction
      if (sentence[i] == 'S')
        gga.time[7] &= 0b10;
    }
    else if (fieldc == 4) {
      // Longitude
      gga.lon = strtod(sentence+i, NULL);
    }
    else if (fieldc == 5) {
      // Longitude direction
      if (sentence[i] == 'W')
        gga.time[7] |= 0b01;
    }
    else if (fieldc == 6) {
      // Fix validity
      gga.time[7] |= (sentence[i] != '0') << 3;
    }
    fieldc++;
  }
  return GPS_NO_ERROR;
}
