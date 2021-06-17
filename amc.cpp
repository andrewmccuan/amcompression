//Andrew Mccuan
//date: 1-29-21
//
//read a file
//
//One of my sources...
/*  https://stackoverflow.com/questions/174531/
    how-to-read-the-content-of-a-file-to-a-string-in-c
*/
// file format specification
//
// TO RUN:
// USE Makefile to compile c++ code, then
// ./amc <file name>
// This will create a new file in the same directory with either
// .amc extension or the files original extension.
//
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

const char* HEADER = "AMC";
const char* EXTENSION = ".amc";

char filename[256];
char nameOfFile[224];
char fileExtension[32];

//const char filename[] = "compressed";
//const char filename[] = "testfile.txt";

//=============================================================
//#define PROFILE
#ifdef PROFILE
//This code is activated only for profiling while optimization
//is attempted.
const double oobillion = 1.0 / 1e9; //one over a billion.
struct timespec timeStart, timeCurrent;
double total_0 = 0.0;
double total_1 = 0.0;
double total_2 = 0.0;
double total_3 = 0.0;
double total_4 = 0.0;
double total_5 = 0.0;
double total_6 = 0.0;
double total_7 = 0.0;
double total_8 = 0.0;
void timeGet(struct timespec *t)
{
	clock_gettime(CLOCK_REALTIME, t);
}
double timeDiff(struct timespec *start, struct timespec *end)
{
	//return the difference in two times.
	double ret =
		(double)(end->tv_sec - start->tv_sec ) +
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
	return ret;
}
void timeCopy(struct timespec *dest, struct timespec *source)
{
	//copy one time structure to another.
	memcpy(dest, source, sizeof(struct timespec));
}
#endif
//=============================================================

void findExtension(char* file);
void findName(char* file);
int fillHeader(char* buff, char* filename, int pos);

int main(int argc, char *argv[])
{
    #ifdef PROFILE
	struct timespec tstart, tend;
	timeGet(&tstart);
	#endif

    if (argc < 2) {
		printf("\nUsage: %s <file name>\n\n", argv[0]);
		return 0;
	}
    //char filename[256];
    //char nameOfFile[224];
    //char fileExtension[32];

	strcpy(filename, argv[1]);
    strcpy(nameOfFile, filename);
    strcpy(fileExtension, filename);

    //Finds the Name of file and seperates Extension and Name
    findName(nameOfFile);
    findExtension(fileExtension);
    //Debug
    //printf("findName: %s\n", nameOfFile);
    //printf("findExtension: %s\n", fileExtension);

    //read the 'compressed' file.
    char * buffer = 0;
    long length;
    FILE * fpi = fopen(filename, "rb");
    if (!fpi) {
        printf("ERROR opening **%s**\n", filename);
        exit(0);
    }

    if (fpi) {
        fseek(fpi, 0, SEEK_END);
        length = ftell (fpi);
        fseek(fpi, 0, SEEK_SET);
        buffer = (char *)malloc(length * sizeof(char));
        if (buffer) {
            fread(buffer, sizeof(char), length, fpi);
        }
        fclose(fpi);
    }
    //printf("%s", newFileName(filename));
    //FILE * fpo = fopen("test.amc", "w");
    if (buffer) {
        //Check for header
        //int i = 0;
        int encode = 0;

        //Simple Check to see if HEADER is correct
        /*
        if (buffer[0] == 'A' && buffer[1] == 'M' && buffer[2] == 'C' && 
            (buffer[3] == '\n' || buffer[3] == '\r')) {
                printf("Correct Header\n");
        }
        */
        //---------------------------------------------------------------------
        //-- Determine if file inputed is in AMC format
        //---------------------------------------------------------------------
        // This checks the begining of the file to determine if the AMC format 
        // header exists. If so then program knows to uncompress file back
        // to original form.
        //
        for (int i = 0; buffer[i] != '\n'; i++) {
            //3 is length of Header
            if (i < 3) {
                //Comparing Header with Beggining of Buffer
                if (HEADER[i] != buffer[i]) {
                    //Debug
                    //printf("NOT ~~~AMC~~~ FILE TYPE\n");
                    encode = 1;
                    break;
                }
            }
            else {
                //Checks to see if there is a Carraige Return
                if (buffer[i] == '\r') {
                    //printf("Carriage Return, ignoring\n");
                    break;
                }
                //Longer than Header so NOT Header
                //printf("NOT ~~~AMC~~~ FILE TYPE\n");
                encode = 1;
                break;
            }

        }


        //Set up Output buffer and set pointers
        char *out_buffer = (char *)malloc(length * 4 * sizeof(char));
        char *p1 = buffer;
        char *p2 = out_buffer;

        //---------------------------------------------------------------------
        //-- ENCODING FILE SECTION --
        //---------------------------------------------------------------------
        if (encode == 1) {
            #ifdef PROFILE
	            struct timespec tstart1, tend1;
	            timeGet(&tstart1);
	        #endif
            //ENCODE
            //populate output buffer...
            printf("Encoding file: %s\n\n", filename);

            int out_length = 0;
            //out_length = fillHeader(p2, filename, out_length);
            //printf("out_length: %d\n", out_length);
            //printf("out_buffer: %s\n", out_buffer);

            //printf("Adding Headers\n");
            //-----------------------------------------------------------------
            //-- ADDING HEADERS --
            //-----------------------------------------------------------------
            //AMC file header
            #ifdef PROFILE
	            struct timespec tstart3, tend3;
	            timeGet(&tstart3);
	        #endif
            int len = strlen(HEADER) + 2;
            for (int i = 0; i < len; i++) {
                if (i < len-2) {
                    *p2 = HEADER[i];
                    p2 += 1;
                }
                else if (i == len-2) {
                    *p2 = '\n';
                    p2 += 1;
                }
                else {
                    *p2 = '\"';
                    p2 += 1;
                }
            }
            //Filename header
            int lenName = strlen(nameOfFile) + 2;
            for (int i = 0; i < lenName; i++) {
                if (i < lenName-2) {
                    *p2 = nameOfFile[i];
                    p2 += 1;
                }
                else if (i == lenName-2) {
                    *p2 = '\"';
                    p2 += 1;
                }
                else {
                    *p2 = '\n';
                    p2 += 1;
                }
            }
            *p2 = '\"';
            p2 += 1;
            out_length += 1;
            //Adding File Extension
            int lenExt = strlen(fileExtension) + 2;
            for (int i = 0; i < lenExt; i++) {
                if (i < lenExt-2) {
                    *p2 = fileExtension[i];
                    p2 += 1;
                }
                else if (i == lenExt-2) {
                    *p2 = '\"';
                    p2 += 1;
                }
                else {
                    *p2 = '\n';
                    p2 += 1;
                }
            }
            #ifdef PROFILE
		        timeGet(&tend3);
		        total_3 += timeDiff(&tstart3, &tend3);
	        #endif
            out_length += (len + lenName + lenExt);
            //--END: (ADDING FILE HEADER)--------------------------------------
            //*p2 = '\n';
            //p2++;
            
            //test to make sure file is being read in/out properly.
            /*
            for (int i = 0; i < length; i++) {
                *p2 = *p1;
                p1 += 1;
                p2 += 1;
                out_length += 1;
            }
            */
            ///*
            #ifdef PROFILE
	            struct timespec tstart4, tend4;
	            timeGet(&tstart4);
	        #endif
            for (int i = 0; i < length; i++) {
                if (*p1 == *(p1-1)) {
                    *(p2 + 0) = 1;
                    *(p2 + 1) = 1;
                    *(p2 + 2) = *(p1 + 1);
                    p2 = p2 + 3;
                    p1 = p1 + 2;
                    ++i;
                } else {
                    *(p2 + 0) = 0;
                    *(p2 + 1) = 0;
                    *(p2 + 2) = *p1;
                    p2 = p2 + 3;
                    p1 = p1 + 1;
                }
                out_length += 3;
            }
            #ifdef PROFILE
		        timeGet(&tend4);
		        total_4 += timeDiff(&tstart4, &tend4);
	        #endif
            //*/
            /*
            //insert first val
            *(p2 + 0) = 0;
            *(p2 + 1) = 0;
            *(p2 + 0) = *p1;
            p1 += 1;
            p2 += 3;

            int searchBuffLen = 1;
            printf("*p1: %c\n", *p1);
            for (int i = 0; i < length; i++) {
                printf("p1[%d]: %c\n", i, p1[i]);

                for (int j = 0; j < searchBuffLen; j++) {
                    printf("Comparing: *p1: %c to p1[%d]: %c\n", *p1, i, p1[i]);
                    if (*p1 == p1[i]) {
                        printf("match found: p1[%d]: %c\n", i, p1[i]);
                    }

                }
                p1 += 1;
                searchBuffLen += 1;
                printf("*p1: %c\n", *p1);
            }
            
            */
            //NAME THE AMC FILE original filename with the ".amc" file 
            // extension
            char fnameOut[256];
            lenName = strlen(nameOfFile);
            lenExt = strlen(EXTENSION);
            for (int i = 0; i < (lenName + lenExt); i++) {
                if (i < lenName) {
                    fnameOut[i] = nameOfFile[i];
                    //printf("nameOfFile[%d]: %c\n", i, nameOfFile[i]);
                }
                else {
                    fnameOut[i] = EXTENSION[i-lenName];
                    //printf("EXTENSION[%d]: %c\n", i, EXTENSION[i]);
                }
            }
            //Debug, check what the file output name is
            //printf("fnameOut: %s\n", fnameOut);

            //open output file
            printf("out_buffer:\n%s\n\n", out_buffer);
            printf("compressed size: %i\n", out_length);
            //FILE *fpo = fopen("lz77.cmp", "wb");
            FILE *fpo = fopen(fnameOut, "wb");
            fwrite(out_buffer, sizeof(char), out_length, fpo);
            fclose(fpo);

            free(out_buffer);
            #ifdef PROFILE
		        timeGet(&tend1);
		        total_1 += timeDiff(&tstart1, &tend1);
	        #endif
        }
        //---------------------------------------------------------------------
        //-- DECODING FILE SECTION --
        //---------------------------------------------------------------------
        if (encode == 0) {
            #ifdef PROFILE
	            struct timespec tstart2, tend2;
	            timeGet(&tstart2);
	        #endif
            printf("Decoding file: %s\n\n", filename);
            //Read header info and grab file info
            //Count represents the number of Header files
            //in the files specification to ingore.
            #ifdef PROFILE
	            struct timespec tstart5, tend5;
	            timeGet(&tstart5);
	        #endif
            int count = 0;
            int par = 0;
            int dotAdded = 0;
            char fnameOut[256];
            char *pname = fnameOut;
            while (count != 3) {
                if (*p1 == '\n') {
                    //printf("newline: increment count\n");
                    p1 += 1;
                    count++;
                }
                else {
                    //Check for {"} in the file
                    //if found flip bit on var "par"
                    if (*p1 == '\"') {
                        if (par == 1) {
                            if (dotAdded == 0) {
                                *pname = '.';
                                pname += 1;
                                dotAdded = 1;
                            }
                        }
                        par = par ^ 1;
                    }
                    else {
                        if (par == 1) {
                            *pname = *p1;
                            pname += 1; 
                        }
                    }
                    p1 += 1;
                }
                //printf("p1: %c\n", *p1);
            }
            //Debug, check what the file output name is
            //printf("fnameOut: %s\n", fnameOut);
            #ifdef PROFILE
		        timeGet(&tend5);
		        total_5 += timeDiff(&tstart5, &tend5);
	        #endif

            #ifdef PROFILE
	            struct timespec tstart6, tend6;
	            timeGet(&tstart6);
	        #endif
            //Start decoding
            int out_length = 0;
            for (int i = 0; i < length; i++) {
                /*
                *p2 = *(p1);
                p1 += 3;
                p2 += 1;
                out_length += 1;
                //printf("*p1: %c\n", *p1);
                //printf("buffer[%d]: %c\n", i, buffer[i]);
                */

                if (*p1 == 1) {
                    *p2 = *(p1 - 1);
                    p2 += 1;
                    *p2 = *(p1 + 2);
                    p2 += 1;
                    p1 += 3;
                    //printf("*p1: %c\n", *p1);
                    out_length += 1;
                } else {
                    *p2 = *(p1 + 2);
                    p1 += 3;
                    p2 += 1;
                    //printf("*p1: %c\n", *p1);
                    out_length += 1;
                }
            }
            #ifdef PROFILE
		        timeGet(&tend6);
		        total_6 += timeDiff(&tstart6, &tend6);
	        #endif

            //Find the length of the output buffer and 
            // set the out_length to it for
            out_length = strlen(out_buffer);

            printf("out_buffer:\n%s\n\n", out_buffer);
            printf("compressed size: %i\n", out_length);
            //FILE *fpo = fopen("text.txt", "wb");
            FILE *fpo = fopen(fnameOut, "wb");
            fwrite(out_buffer, sizeof(char), out_length, fpo);
            fclose(fpo);

            free(out_buffer);
            #ifdef PROFILE
		        timeGet(&tend2);
		        total_2 += timeDiff(&tstart2, &tend2);
	        #endif
        }
    }

    #ifdef PROFILE
		timeGet(&tend);
		total_0 += timeDiff(&tstart, &tend);
	#endif

    #ifdef PROFILE
	printf("\nProfiling times:\n");
	//printf("              main(): %f\n", total_0);
	printf("              main(): %f\n", total_0);
	printf("            encoding: %f\n", total_1);
	printf("            decoding: %f\n", total_2);
    printf("      Adding Headers: %f\n", total_3);
    printf("    Encoding Contnet: %f\n", total_4);
    printf("    Decoding Headers: %f\n", total_5);
    printf("    Decoding Content: %f\n", total_6);
	printf("     findExtension(): %f\n", total_7);
    printf("          findName(): %f\n", total_8);
	#endif

    return 0;
}

void findExtension(char* filename)
{
    #ifdef PROFILE
	    struct timespec tstart, tend;
	    timeGet(&tstart);
	#endif

    int length = strlen(filename);

    char tempExtension[20];
    int extPos = length;

    for (int i = length-1; i > 0; i--) {
        if (filename[i] != '.') {
            extPos--;
        }
        else {
            break;
        }
    }
    for (int i = extPos; i < length; i++) {
        tempExtension[i - extPos] = filename[i];
    }

    //Debug
    //printf("findExtension() filename: %s\n", filename);
    //printf("findExtension() filename size: %d\n", length);
    //printf("findExtension() tempExtension: %s\n", tempExtension);
    //Replace passed in string with tempstring
    strcpy(filename, tempExtension);
    //printf("findExtension() filename: %s\n", filename);
    #ifdef PROFILE
		timeGet(&tend);
		total_7 += timeDiff(&tstart, &tend);
	#endif
}

void findName(char* filename)
{
    #ifdef PROFILE
	    struct timespec tstart, tend;
	    timeGet(&tstart);
	#endif

    int length = strlen(filename);

    char tempName[256];
    //int extPos = 0;

    for (int i = 0; i < length; i++) {
        if (filename[i] != '.') {
            tempName[i] = filename[i];
            //extPos++;
        }
        else {
            break;
        }
    }
    //Debug
    //printf("findName() filename: %s\n", filename);
    //printf("findName() filename size: %d\n", length);
    //printf("findName() tempName: %s\n", tempName);
    //Replace passed in string with tempstring
    strcpy(filename, tempName);
    //printf("findName() filename: %s\n", filename);
    #ifdef PROFILE
		timeGet(&tend);
		total_8 += timeDiff(&tstart, &tend);
	#endif
}
