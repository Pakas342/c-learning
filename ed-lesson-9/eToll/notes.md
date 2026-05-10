I'm supposed to write a loop that processes tolls for a set of cars?

A car generator uses an infinite loop where it sends (write) a car every random seconds to a lane FIFO

A lane is a file descriptor, a bufer (I'm not sure what for), id and len.

The process_lines reads a given lane thing, and process a set of existing cars while there's new lines. Then, at the end, moves the non-processed memory back to the buf, as per maybe something write in this loop, and returns

What's pending? I need to connect the car processors to the lanes through fifos, then connect those fifos to the actual lanes by epolling on their reading side and, when triggered, copying into their buffer and star the processing
