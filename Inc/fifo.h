#include "main.h"


#define FIFO_LENGTH 4095

typedef struct {
  uint8_t data[FIFO_LENGTH];
  uint16_t head;
  uint16_t tail;
  uint16_t count;
} fifo_t;


void fifo_init(fifo_t *fifo);

int8_t fifo_put(fifo_t *fifo, void *data, uint8_t offset, uint8_t count);
int8_t fifo_get(fifo_t *fifo, void *data, uint8_t offset, uint8_t count);
uint8_t fifo_count(fifo_t *fifo);

