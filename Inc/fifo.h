/*
 * fifo.h
 *
 *  Created on: 28 февр. 2023 г.
 *      Author: Denis
 */

#ifndef FIFO_H_
#define FIFO_H_
// Size it should be power of two: 4,8,16,32...128
#define FIFO( size )\
struct  {\
 void* buf[(size)];\
unsigned int tail ;\
unsigned int head ;\
}

//number of items in the fifo
#define FIFO_COUNT(fifo)     (fifo.head-fifo.tail)

//size fifo
#define FIFO_SIZE(fifo)      ( sizeof(fifo.buf)/sizeof(fifo.buf[0]) )

//fifo is full?
#define FIFO_IS_FULL(fifo)   (FIFO_COUNT(fifo)==FIFO_SIZE(fifo))

//fifo is empty?
#define FIFO_IS_EMPTY(fifo)  (fifo.tail==fifo.head)

//amount of free space in fifo
#define FIFO_SPACE(fifo)     (FIFO_SIZE(fifo)-FIFO_COUNT(fifo))

//push to fifo
#define FIFO_PUSH(fifo, byte) \
  {\
    fifo.buf[fifo.head & (FIFO_SIZE(fifo)-1)]=byte;\
    fifo.head++;\
  }

//get of fifo
#define FIFO_FRONT(fifo) 	 (fifo.buf[fifo.tail++ & (FIFO_SIZE(fifo)-1)])

#define FIFO_HEAD(fifo)		(fifo.buf[fifo.head & (FIFO_SIZE(fifo)-1)])
#define FIFO_TAIL(fifo)		(fifo.buf[fifo.tail & (FIFO_SIZE(fifo)-1)])
#define FIFO_INCREMENT(fifo) (fifo.head++)

//get first item of fifo
#define FIFO_PEEK(fifo, ind) 	 (fifo.buf[(fifo.tail + ind) & (FIFO_SIZE(fifo)-1)])

//reduce the number in fifo
#define FIFO_POP(fifo)   (fifo.tail++)

//clear fifo
#define FIFO_FLUSH(fifo)   \
  {\
    fifo.tail=0;\
    fifo.head=0;\
  }



#endif /* FIFO_H_ */
