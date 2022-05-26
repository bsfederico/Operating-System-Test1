#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <stdarg.h>

static void errorMessage(const char *message, ...)
{
	va_list args;
	va_start(args, message);
	vfprintf(stderr, message, args);
	va_end(args);

exit(1);
}

static char *generateData(int kbNum)
{
	int bytes = 1024 * kbNum;
	char *data = malloc(bytes);

	if (data == NULL)
		errorMessage("failed to allocate %d bytes of memory\n", bytes);

	memset(data, '*', bytes);

return data;
}

static void printTimes(long int *times)
{
    int fileSize = 1;
    bool bigger = false;

    for (int i = 0; i < 6; i++)
    {
        if (bigger)
            printf("El tiempo para %dMB fue de %ld μs usando tuberias\n", fileSize, times[i]);
        else
            printf("El tiempo para %dKB fue de %ld μs usando tuberias\n", fileSize, times[i]);

        if (fileSize < 100)
            fileSize = fileSize * 10;
        else
        {
            fileSize = 1;
            bigger = true;
        }
    }
}

static void checkErrors(int processId, int pipesPointers[2])
{
	for (int i = 0; i < 2; i++){
        	if (pipesPointers[i] < 0){
			// Generating the corresponding error message for pipe error
			char message[] = "Error starting the pipe ";
			strcat(message, (char *)&i);
		errorMessage(message);
		}
	}

	// Generating the corresponding error message for the fork
	if (processId < 0)
		errorMessage("Error generating the fork");
}

static size_t multiRead(int fd, char *buffer, ssize_t byteAmount)
{
	ssize_t numBytes = 0;
	ssize_t bytesLeft = byteAmount;
	ssize_t readBytes = 0;

	while (bytesLeft > 0 && (numBytes = read(fd, buffer, bytesLeft)) > 0){
		readBytes += numBytes;
		buffer += numBytes;
		bytesLeft  -= numBytes;
	}

    if (readBytes == 0)
        readBytes = numBytes;

return readBytes;
}

static ssize_t multiWrite(int fd, const char *buffer, size_t byteAmount)
{
	ssize_t numBytes = 0;
	size_t bytesLeft = byteAmount;
	ssize_t wrBytes = 0;
	while (bytesLeft  > 0 && (numBytes = write(fd, buffer, bytesLeft)) > 0){

	wrBytes += numBytes;
	buffer += numBytes;
 	bytesLeft  -= numBytes;
	}

	if (wrBytes == 0)
	wrBytes = numBytes;

return wrBytes;
}

static void childProcess(int pipeWrite, int pipeRead)
{
	int check = 1;

	for (int i = 1; i < 100001; i = i * 10){     // i = 1 is key!
	
		size_t sz = 1024 * i;
		char *data = malloc(sz);

		if (data == NULL)
			errorMessage("failed to allocate memory\n");

		//printf("Child reading %zu bytes of data\n", sz);

	        // Get Data Package
		if (multiRead(pipeRead, data, sz) != (ssize_t)sz)
			errorMessage("Read error in %s()\n", __func__);

 		// Sending check
 		if (write(pipeWrite, &check, sizeof(check)) != sizeof(check))
			errorMessage("Write error in %s()\n", __func__);
    	}
}

static void parentProcess(int pipeWrite, int pipeRead)
{
	struct timeval start, stop;
	long int times[6];
	int index = 0;

	for (int i = 1; i < 100001; i = i * 10){

		size_t size = 1024 * i;
		char *data = generateData(i);
		//printf("Parent writing %zu bytes of data\n", size);
		gettimeofday(&start, NULL);
        	int check;

		// Sending Data Package to consumer
		if (multiWrite(pipeWrite, data, size) != (ssize_t)size)
			errorMessage("Write error in %s()\n", __func__);

		// Getting the check confirmation
		if (read(pipeRead, &check, sizeof(check)) != sizeof(check))
			errorMessage("Read error in %s()\n", __func__);

		// Get the time elapsed time
		gettimeofday(&stop, NULL);

		times[index] = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
		index++;
		free(data);
	}

printTimes(times);
}

static void startProgram(void)
{
	int pipePointers[2], parentPipes[2], childrenPipes[2];
	pid_t processId = 0;

	// Creating emparented processes and pipes
	pipePointers[0] = pipe(parentPipes);
	pipePointers[1] = pipe(childrenPipes);
	processId = fork();
	checkErrors(processId, pipePointers);

	// Spliting the code that witch process will execute
	if (processId == 0){
		close(childrenPipes[0]);
		close(parentPipes[1]);
        	childProcess(childrenPipes[1], parentPipes[0]);
        	close(childrenPipes[1]);
       		close(parentPipes[0]);

    	}else if (processId > 0){
		close(childrenPipes[1]);
		close(parentPipes[0]);
		parentProcess(parentPipes[1], childrenPipes[0]);
		close(childrenPipes[0]);
		close(parentPipes[1]);
	}
}

int main(void)
{
    startProgram();
    return 0;
}
