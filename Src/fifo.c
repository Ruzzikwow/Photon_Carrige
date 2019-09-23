#include "fifo.h"

int8_t fifo_put(fifo_t *fifo, void *data, uint8_t offset, uint8_t count)
{
  if ((FIFO_LENGTH - fifo->count) < count)                      //���������, ���������� �� �����
    return -1;                                                  //���? ���������� "-1" � ������
  for (uint8_t i = offset; i < (offset + count); i++)                //���� ����������
  {
    fifo->data[fifo->tail++] =((uint8_t*) data)[i];                  //������ ���� ������ � ����� tail++
    fifo->count++;                                              //����������� ����������-�������
    if (fifo->tail == FIFO_LENGTH)                              //���� �������� � ������� �����
    {
      fifo->tail=0;                                             //�������� �����
    }                                                           //�.�. "�����������" ����� � ������
  }
  return 0;                                                     //���������� "��"
}

int8_t fifo_get(fifo_t *fifo, void *data, uint8_t offset, uint8_t count)
{
  if (fifo->count < count)                                      //���������, ����� �� �� ������ �������,
    return -1;                                                  //������� � ��� ������
  for (uint8_t i = offset; i < (offset + count); i++)                //���� ������
  {
    ((uint8_t*)data)[i] = fifo->data[fifo->head++];                  //����� ���� �� ���������� ������, head++
    fifo->count--;                                              //��������� ������� ����
    if (fifo->head == FIFO_LENGTH)                              //���� �������� � ������� �����
    {
      fifo->head=0;                                             //�������� ������
    }
  }
  return 0;
}



uint8_t fifo_count(fifo_t *fifo)
{
  return fifo->count;
}

void fifo_init(fifo_t *fifo)
{
  fifo->head = fifo->tail = fifo->count = 0;
}

