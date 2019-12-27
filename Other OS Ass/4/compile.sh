# Write a program for the Reader-Writer process for the following situations:
#       a) Multiple readers and one writer: writer gets to write whenever it is ready (reader/s wait)
#       b) Multiple readers and multiple writers: any writer gets to write whenever it is ready, 
#       provided no other writer is currently writing (reader/s wait)

clear
echo "COMPILING..."
gcc -o reader_writer reader_writer.c -lpthread -lrt
echo
echo "COMPILED..."
echo "RUNNING..."
echo
./reader_writer