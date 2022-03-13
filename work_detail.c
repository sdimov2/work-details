/*
author: Steven Dimov of Sigma Phi Delta Delta Chapter
I don't always do my work detail
this is my source code, bitch.
*/
//#include "stdafx.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
//#include <xlsxio_read.h>
//#include <xlsxio_write.h>
#include <math.h>
#define MEMBERFILENAME "priority"
#define ROSTERFILENAME "roster.csv"
#define DETAILFILENAME "detail_list"
#define WORKDETAILSOUT "work details.csv"


// taken from geeks for geeks
	void countlines(char* filename, int* linecount, int* detailcount, int* membercount) {
		// Line counter (result)
		int i, j, k;
		int col = 0;
		int row = 0; 
		int prevrow = -1; 
		int prevrow2 = -1;
		int quotes = 1;
		*detailcount = 0;
		*membercount = 0;
		char c = '0';  // To store a character read from file
		//int EOF = -1;
	  
		// Get file name from user. The file should be
		// either in current folder or complete path should be provided
	  
		// Open the file
		FILE* file;
		file = fopen(filename, "r");
	  
		// Check if file exists
		if (file == NULL)
		{
			printf("Could not open file %s", filename);
			return;
		}
		// Extract characters from file and store in character c
		for (c = getc(file); c != EOF; c = getc(file)) {
			if (c == '\n') {// Increment count if this character is newline
				row += 1;
				col = 0;
			}
			else if (c == '"') {
				quotes*=-1;
			}
			else if (quotes != -1 && c == ',') {
				col++;
			}
			else if (col == 10 && row != prevrow && c != '\0') {
				*detailcount+=1;
				prevrow = row;
			}
			else if (col == 0 && row != prevrow2 && c != '\0') {
				*membercount+=1;
				prevrow2 = row;
			}
			
		}
		// Close the file
		fclose(file);
		//fix the values from the title offset row
		*detailcount -= 1;
		*membercount -= 1;
		*linecount = row;
		return;
	}
	
	char* removenewline(char* string) {
		int i = 0;
		while(string[i] != '\n'){
				i++;
		}
		
		string[i-1] = '\0';
		return string;
	}
	
void main()
{ 
	printf("Welcome to the work detail generator!\nMake sure that %s is not currently opened for this program to work, otherwise there may be a segmentation fault.\n", WORKDETAILSOUT);
	char line[201];
    int i,j,k,l;
	int detailcount, membercount, linecount;
	
	countlines(ROSTERFILENAME, &linecount, &detailcount, &membercount);
	printf("%d, %d, %d\n", linecount, detailcount, membercount);
	
	char** Weeklydetails = (char**) calloc(7*detailcount, sizeof(char*));
	for (i=0; i<7*detailcount; i++) {
		Weeklydetails[i] = (char*) calloc(50, sizeof(char));
	}
	//int membercount = countlines(MEMBERFILENAME);
	//int detailcount = countlines(DETAILFILENAME);
	//printf("%d, %d ", membercount, detailcount);
	
	struct Member {
		char name[50];
		int floor;
		int priority;
		int restrictions[7*detailcount];
	};
	struct Detail {
		char name[50];
		int floor;
		int daysofweek[7];
		int laborers;
	};
	struct Member* members = (struct Member*) calloc(membercount, sizeof(struct Member));
	struct Detail* details = (struct Detail*) calloc(detailcount, sizeof(struct Detail));
	
	void Parsefile(char* filename, int linecount, struct Member* members, struct Detail* details) {
		void cutstr() {
			
		}
		FILE* file = fopen( filename, "r" );
		if (file == NULL ) {
			printf( "%s could not be opened\n", filename );
		} 
		char charstream;
		int col = 0;
		int row = 0;
		int k = 0;
		int i = 0;
		int	j = 0;
		int l = 0;
		int quotes = 1;
		char str[50] = {};
		// begin parsing
		for (charstream = getc(file); charstream != EOF; charstream = getc(file)){
			//run through the first line of titles
			//printf("%c", charstream);
			if (row==0){
				while(charstream != '\n'){
					//printf("%c", charstream);
					charstream = fgetc(file);
				}
				//charstream = fgetc(file);
				col = 0;
			}
			//parse actual information
			if(charstream != '\n'){
				if (charstream != ',' || quotes == -1 ) {
					// string parse with quotes
					if (charstream == '"') {
						quotes *= -1;
						//j=0;
					}
					else {
						str[j] = charstream;
						//printf("%c",charstream);
						j++;
					}
				}
				else {
					str[j] = '\0';
					j=0;
					//printf("%d, %d: %s\n", row, col, str);
					if (i<membercount && str != NULL) {
						
						// restrictions
						if (col > 1 && col < 9) {
							k = 0;
							while (str[k] != '\0') {
								if (str[k] != ',')
									members[i].restrictions[col-2] = atoi(&str[k]);
								k++;
							}
						}
						if (col == 0) {
							strcpy(members[i].name,str);
						}
						if (col == 1) {
							members[i].floor = atoi(str);
							i++;
						}
					}
					if (l<detailcount) {
						if (col == 9) {
							strcpy(details[l].name,str);
						}
						else if (col == 10) {
							details[l].floor = atoi(str);
						}
						else if (col == 11) {
							details[l].laborers = atoi(str);
							l++;
						}
					}
					col++;
				}
			}
			else {
				str[j] = '\0';
					j=0;
					//printf("%d, %d: %s\n", row, col, str);
					if (i<membercount && str != NULL) {
						// restrictions
						if (col > 1 && col < 9) {
							k = 0;
							while (str[k] != '\0') {
								if (str[k] != ',')
									members[i].restrictions[col-2] = atoi(&str[k]);
								k++;
							}
						}
						if (col == 0) {
							strcpy(members[i].name,str);
						}
						if (col == 1) {
							members[i].floor = atoi(str);
							i++;
						}
					}
					if (l<detailcount) {
						if (col == 9) {
							strcpy(details[l].name,str);
						}
						if (col == 10) {
							details[l].floor = atoi(str);
						}
						if (col == 11) {
							details[l].laborers = atoi(str);
							l++;
						}
					}
				row++;
				col = 0;
			}
		}
		fclose(file);
		return;
	}
	Parsefile(ROSTERFILENAME, linecount, members, details);
	
	/*
    FILE *memberfile;  
	FILE *detailfile;

	memberfile = fopen( MEMBERFILENAME, "r" );

   if (memberfile == NULL ) 
   {
      printf( "File could not be opened\n" );
   } 
   else 
   { 		
		i=0;
		while (membercount>i) {
			strcpy(members[i].name, removenewline(fgets(line,201,memberfile)));
			//members[i].name[strcspn(members[i].name, "\n")] = 0;
			members[i].priority = i+1;
			//printf("%d: %s", members[i].priority, members[i].name);
			i++;
		}
		
	}
    fclose( memberfile ); 
	detailfile = fopen( DETAILFILENAME, "r" );

   if (detailfile == NULL ) 
   {
      printf( "File could not be opened\n" );
   } 
   else 
   { 		
		i=0;
		while (detailcount>i) {
			strcpy(details[i].name, removenewline(fgets(line,201,detailfile)));
			//details[i].name[strcspn(details[i].name, "\n")] = 0;
			//printf("%s", details[i].name);
			i++;
		}
	}
    fclose( detailfile ); 
	*/
	
	void recurse (int i, int j, int k, int detailcount, struct Detail* details, struct Member* members, char** Weeklydetails) {
		int l;
		int daydate = 0;
		int weekdate = 0;
		int dayofyear = 0;
		
		// base case: are we done?
		if ( Weeklydetails[detailcount*7] != 0) {
			printf("\n");
			return;
		}
		if (Weeklydetails[detailcount*i+detailcount] !=0) {
			i++;
			j=0;
			printf(".");
		}
		// try something
		// check if valid
		
			if (members[k].restrictions[i+j*i]==0 && j < detailcount) {
				strcpy(Weeklydetails[i+j*i], members[k].name);
				k++;
				j++;
			}
			// undo and replace
		recurse(i,j,k+1,detailcount, details, members, Weeklydetails);
	
		//strcpy(Weeklydetails[i+j*i], members[rand()%membercount].name);
		//printf("%s ", Weeklydetails[i+j*i]);
	}
	
	// determine restrictions: the floor rule
	char* daysofweek[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	for (k=0;k<membercount;k++) {
		printf("\n%s: ", members[k].name);
		for (i=0; i<7; i++){
			printf("\n%s:", daysofweek[i]);
			for (j=0; j<detailcount; j++){
				//check for floor mismatches
				if (details[j].floor != 0 && details[j].floor != -1 && details[j].floor != 1 && details[j].floor != members[k].floor ) {
					members[k].restrictions[i+j*i] = 1;
					//printf("%s, %d,",details[j].name, members[k].restrictions[i+j*i]);
				}
				else {
					
					
					// this will override the input from the roster
					members[k].restrictions[i+j*i] = 0;
				}
				printf("%d,", members[k].restrictions[i+j*i]);
			}
		}
	}
	printf("\ncalculating work details according to algorithmn");
	//recurse(0,0,0,detailcount, details, members, Weeklydetails);
	
	void Printweek ( char** Weeklydetails, struct Detail* details) {
		FILE* output = fopen(WORKDETAILSOUT, "w");
		if (output == NULL) {
			printf("cannot write to %s, make sure the file is not opened", WORKDETAILSOUT);
		}
		fprintf(output, ",");
		char* daysofweek[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
		for (i=0; i<7; i++) {
			fprintf(output, "%s,", daysofweek[i]);
		}
		fprintf(output, "\n");
		for (i=0; i<detailcount; i++) {
			fprintf(output, "%s: ,", details[i].name);
			for (j=0; j<7; j++) {
				fprintf(output, "\"%s\",", Weeklydetails[i+j*i]);
			}
			fprintf(output, "\n");
		}
		fclose(output);
		return;
	}
	Printweek(Weeklydetails, details);
	
	for (i=0; i<7*detailcount; i++) {
		free(Weeklydetails[i]);
	}
	
	free(Weeklydetails);
	/*
	for (i=0; i<membercount; i++) {
		for (j=0; j<19*7; j++){
			free(members[i].restrictions[j]);
		}
		//free(members[i].restrictions);
		//free(members[i]);
		//free(members[i].name);
		//free(members[i].priority);
		//free(members[i].floor);
	}
	*/

	free(members);

	/*
	for (i=0; i<detailcount; i++) {
		for (j=0; j<19*7; j++){
			free(details[i].daysofweek[j]);
		}
		//free(details[i]);
		//free(details[i].daysofweek);
		//free(details[i].name);
		//free(details[i].requirestwo);
		//free(details[i].floor);
	}
	*/
	free(details);
	printf("\n%s created!\n", WORKDETAILSOUT);
	 printf("Do your work details!\n");
	
   } 