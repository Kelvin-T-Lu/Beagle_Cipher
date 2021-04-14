/*Kelvin Lu G01194210
 * CS 262 Lab 205
 * Project 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WORDLIMIT 5000
#define CHARLIMIT 15
#define SEED 4210

void menu(char **, int *);  
int choice1(char **); //Reads a Key Cipher and Converts it.  
void choice2(char **, int* ); 
void choice3(char **, int*); 
void searchChar(char **, int *, char, int *, int *, int); 

int main(int argc, char **argv){
	char **key = (char **) calloc(WORDLIMIT, sizeof(char*));  //Key of Cipher
	int keySize = 0; //Size of the Key
	srand(SEED); 
	menu(key,&keySize);
	free(key);  
 	return 0; 
}

//Entry to the program and check arguements of the program. 
void menu(char **key, int *keySize){
	char inBuf[5]; 
	int option;  
	do{
		printf("Choose the following options by typing in the number. Ex. 1\n");
		printf("1.Read in the name of a text file to use as a cipher key\n");
		printf("2.Create a cipher key using the input text file to output.\n");
		printf("3.Decode an existing cipher.\n"); 
		printf("4.Exit the program.\n"); 
	
		fgets(inBuf,5,stdin); 
		sscanf(inBuf,"%d",&option); 
	}while(option > 4 || option<1); 
	 
	switch(option){
		case 1:
			*keySize = choice1(key);
			//printf("KeySize: %d\n", *keySize); 
			//printf("%s\n", key[*keySize -1]); 
			menu(key, keySize);  
			return;
		case 2:  
			choice2(key, keySize);
			menu(key, keySize); 
			return;
		case 3: 
			choice3(key, keySize); 
			menu(key,keySize); 
			return; 
		case 4: 
			return; 
		default:
			printf("Error, option not available.\n");
			menu(key,keySize); 
	} 
}

//Reads a text file and turns it into a key. 
int choice1(char **key){
	char inBuf[500], *docTitle;  
	int i=0; 
	FILE *fp; 
	
	//Gets the title of the docuement and reads it. 	 
	printf("Enter the text document of the cipher key.\n"); 
	fgets(inBuf,50,stdin);
	docTitle = (char*)calloc(strlen(inBuf)+1,sizeof(char));  
	sscanf(inBuf,"%s",docTitle);

	//Document Error
	if((fp=fopen(docTitle,"r"))==NULL){
		printf("Error. Document could not be opened.\n");
		free(docTitle);  
		return choice1(key); 	  
	}
	free(docTitle);  
	//Creates the key from cipher docuement. 
	char token[] = " \n";
	while((fgets(inBuf, 500, (FILE *)fp))!= NULL){
		if(strcmp(inBuf,"\n")==0){
			continue; 
		} 
		char *temp= strtok(inBuf,token);   
		while(temp != NULL){
			//Lowercase string
			int j; 
			for(j=0; j<strlen(temp); j++){
				temp[j] = (char) tolower(temp[j]); 
			}
			key[i] = (char *)calloc(strlen(temp)+1,sizeof(char));
			strcpy(key[i], temp);
			 
		//	printf("%s ", key[i]); 
			//Next Token
			temp= strtok(NULL, token); 
			i++; 
		}
	}
	//if((key= realloc(key,i))==NULL){
	//	printf("Reallocation Error."); 
	//	exit(-1); 	
	//}
	//	int j; 
	
	//	for(j=0;j<i; j++){
	//	printf("%s", key[j]); 
	//}	
	return i;	 
}
void choice2(char **key, int *keySize){
	if(key[0] == 0 ){
		//	printf("Array Null Conditional Activated\n"); 
		*keySize = choice1(key); 
	}
	char *inBuf = (char *) calloc(1500, sizeof(char));
	int *wordIndex, *charIndex; 
 
	int i, len; 	
	printf("Enter the message that you want to encode.\n"); 
	fgets(inBuf,1500, stdin); 

	//inBuf = (char *) realloc(inBuf, strlen(inBuf)+1); 	
	len = strlen(inBuf); 
	wordIndex = (int *) calloc(len, sizeof(int));
	charIndex = (int *) calloc(len, sizeof(int)); 
	//	printf("%s\n", inBuf);//Check inBuf output 

	for(i=0; i<len; i++){
		searchChar(key, keySize, inBuf[i], &wordIndex[i], &charIndex[i], 9); 		
	}
	

	//Output file
	FILE *fp; 
	free(inBuf); 
	inBuf = (char *) calloc(50, sizeof(char)); 
	
	//Gets the name of Output file
	printf("Enter the name of the output file.\n"); 
	fgets(inBuf, 50, stdin);
	sscanf(inBuf,"%s", inBuf); 
//	len = strlen(inBuf) + 1;  
//	inBuf = realloc(inBuf, len); 	 
	

	if((fp = fopen(inBuf, "w"))== NULL){
		printf("Error. Output file can not be openend.");
		free(inBuf);
		free(key); 
		free(wordIndex); 
		free(charIndex); 
		exit(-1); 
	}
 
	for(i =0; i<len; i++){
		if(i==(len-2)){
			if(wordIndex[i] == -2){
				fprintf(fp,"#"); 
				break;
			}
			fprintf(fp,"%d,%d\n", wordIndex[i], charIndex[i]); 
			break; 
		}
		if(wordIndex[i] == -1){
			fprintf(fp," ");
		//	printf("$");  
			continue;
		}
		else if(wordIndex[i] == -2){
			fprintf(fp,"#,"); 
		//	printf("#"); 
			continue; 
		}
		fprintf(fp,"%d,%d,",wordIndex[i],charIndex[i]); 
		//	printf("%d,%d,", wordIndex[i],charIndex[i]); 
	} 
	fclose(fp); 
	free(inBuf); 
	free(wordIndex);
	free(charIndex); 
 }

void choice3(char **key,int *keySize){
	if(key[0] == 0){
		*keySize = choice1(key); 
	}
	char *inBuf = (char *) calloc(5000, sizeof(char)); 
	FILE *fp; 	
	char **storage = (char **) calloc(5000, sizeof(char*)); 
	char *message = (char *) calloc(WORDLIMIT, sizeof(char)); 
	int i = 0; 

	printf("Enter the text file of the encoded text.\n"); 
	fgets(inBuf, 5000, stdin);
	sscanf(inBuf, "%s", inBuf); 
	if((fp= fopen(inBuf, "r"))==NULL){
		printf("Error opening file.\n");
		free(inBuf); 
		choice3(key,keySize); 
	}
	char token[] = ",";

	while((fgets(inBuf, WORDLIMIT, (FILE *)fp))!=NULL){
	//	printf("%s", inBuf);
		char *temp = strtok(inBuf, token); 
 		while(temp != NULL){
		//	printf("%s\n", temp);
		//	if((strchr(temp, ' '))==NULL){
				storage[i] = (char *) calloc(strlen(temp)+1, sizeof(char)); 
				strcpy(storage[i], temp); 
				i++; 
		//	}/*
		/*	else{//If there's a space in between. 
				char *spaceTemp = strtok(temp, " "); 
				while(spaceTemp != NULL){
					printf("Space temp %s\n", spaceTemp); 
					storage[i] = (char *) calloc(strlen(spaceTemp)+1, sizeof(char));
					strcpy(storage[i], spaceTemp); 
					
					i++;
					spaceTemp = strtok(NULL, " "); 
				}	
			}*/ 
			temp = strtok(NULL, token); 
		}	
	}
	int j, wordIndex, charIndex, scratch, counter=0; 
	for(j=0; j<i; j++){
		if(counter%2 ==0){
			wordIndex = atoi(storage[j]); 
			counter++; 
			continue; 
		}
		else if((strchr(storage[j], ' '))==NULL){//Token without space. 
			charIndex = atoi(storage[j]); 
:set number
			counter = 0;
			char temp[2];
			temp[0] = key[wordIndex][charIndex]; 
			temp[1] = '\0';  
			strcat(message,temp); 
			continue; 	
		}
		else{
			sscanf(storage[j],"%d %d", &charIndex,&scratch); 
			char temp[3]; 
			temp[0] = key[wordIndex][charIndex]; 
			temp[1] = ' '; 
			temp[2] = '\0';  
			strcat(message,temp); 
			wordIndex = scratch; 
			continue; 
		}
		
	}

	printf("%s\n", message); 
	fclose(fp);
	free(inBuf);
	free(storage);
	free(message);  	

}

void searchChar(char **key, int *keySize, char search, int *wordIndex, int *charIndex, int maxOccurence){
	int i,j,len; 
 
	int counter=0, randNum=rand() % maxOccurence + 1;  
	//-1 for space
	//-2 for #	

	//printf("Search Char: %d, RanNum: %d\n", search, randNum); 
	//Encrypts the message. 
	//If char is a space. 
	if(search==' '|| search == 10){
//		printf("Space activated"); 
		*wordIndex = -1; 
		*charIndex = -1;
		return; 			
	}
	else if(search == '\0'){
		return; 
	}

	for(i = 0; i<*keySize; i++){//Each Word in Key
		len = strlen(key[i]); 
		//printf("Search Instance %s Len: %d\n", key[i], len); 
		if((strchr(key[i],search)== NULL)){//Continue if character is not in word. 
			//printf("Continue search\n"); 
			continue; 
		} 
		for(j=0; j<len;j++){//Each letter in key.
			if(key[i][j] == search){
				counter++; 
				//printf("Match: Counter: %d\n", counter); 
			}
			if(counter == randNum){ //Randnum Occurences reached
				//printf("Reached Char"); 
				*wordIndex = i;
				*charIndex = j; 
				return; 							
			}
		}
	}
	if(counter == 0){//No occurences of search Char
//		printf("No symbole found\n"); 
		*wordIndex = -2; 
		*charIndex = -2;
		return;
	}
	//No randNum occurence inside search string, resets search
	searchChar(key, keySize, search, wordIndex, charIndex,counter); 
}
