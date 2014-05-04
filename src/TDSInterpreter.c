/* =============== GPL HEADER =====================
* TDSInterpreter.c
* Copyleft (C) 2014 - Joel Matteotti <joel _DOT_ matteotti _AT_ free _DOT_ fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*
* ====================================================
*/

//
//
// TuxDroidScript Interpreter (0.1-test)
//

//#define DEBUG uncomment me to display traces messages :)


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include <TDSIVersion.h>
#include <TuxCompat.h>	//Compatibility for compilation on many compiler for Windows
#include <TuxStrings.h>


#define TDS_LANG_FR		//FR or EN (maybe more language later)
						//This variable need to be defined before include TDSKeywords.h
						
#include <TDSIKeywords.h>

//Prototypes
void analyseLine(char *line);
void loadScript(char *file);
int isCmd(char *str);
int isDirectionParam(char *str);
int isDirectionPos(char *str);
int isNumberPos(char *str);


//Return 1 if str is the cmd else return 0
int isCmd(char *str)
{
	if(!strcmp(strtolower(str),KW_ROTATE) || !strcmp(strtolower(str),KW_OPEN) || !strcmp(strtolower(str),KW_CLOSE)
		|| !strcmp(strtolower(str),KW_SPEAK))
		{
			return 1;
		}		
		
	return 0;		
}

//Return 1 if str is the first param else return 0
int isFirstParam(char *str)
{
	if(!strcmp(strtolower(str),KW_MOUTH) || !strcmp(strtolower(str),KW_EYES_SING) || !strcmp(strtolower(str),KW_EYES_PLUR)
		|| !strcmp(strtolower(str),KW_RIGHT) || !strcmp(strtolower(str),KW_LEFT))
		{
			return 1;
		}
		
	return 0;
	
}

//Return 1 if the param is a pos like LEFT or RIGHT else return 0
int isDirectionPos(char *str)
{
	if(!strcmp(strtolower(str),KW_LEFT) || !strcmp(strtolower(str),KW_RIGHT))
		return 1;
		
	return 0;
}


//Return 1 if the param is a numeric number else return 0
//TODO: alphabetical number (Ex: Turn three times to your left)
int isNumberPos(char *str)
{
	if(atoi(str))
		return 1;
		
	
	return 0;
}

//Analyse the line and do action
void analyseLine(char *line)
{
	#if defined DEBUG
		printf("Enter analyseLine() => %s\n",line);
	#endif

	if(line == NULL) //if line is NULL stop here
	{
		#if defined DEBUG
			printf("The line is NULL !!");
		#endif
		
		return;
	}

	line = trim(line);
	
	if(!strcmp(line,"")) //don't analyse empty line
	{
		#if defined DEBUG
			printf("The line is empty\n");
		#endif
		
		return;	
	}
	
	int wc = countCharacterOccurency(line,' '); //count the number of word
	char **sp = explode(line,' '); //space is the separator in the line (Ex: turn left 10 time)
	
	#if defined DEBUG
		printf("WordCount => %d\n",wc);
	#endif
	
	//1 - Search the cmd
	//2 - Search the first param
	//3 - Search if more parameter exist
	
	char *CMD = NULL;


	//Search the command's position in the string
	int cmdPos = 0; 
	while(!isCmd(sp[cmdPos]))
		cmdPos++;

	#if defined DEBUG
		printf("Command's position => %d\n",cmdPos);
	#endif

	if(!strcmp(strtolower(sp[cmdPos]), KW_SPEAK))
	{
		#if defined DEBUG
			printf("Command is KW_SPEAK (%s)\n",KW_SPEAK);
		#endif
		
		//Speak only have phrase as parameter
		CMD = (char *)malloc(sizeof(char)*strlen(line)-strlen(sp[cmdPos])+27); //27 is for the command's len
		
	
		#if defined DEBUG
			printf("Reconstruction of the phrase ...\n");
			printf("Memory Allocation..");
		#endif	
		
	
		#if defined DEBUG
			printf("size of phr => %d\n",strlen(line)-strlen(KW_SPEAK)-1);
		#endif
		
		//Use all str except KW_SPEAK (sp[0])
		//char phr[strlen(line)-strlen(KW_SPEAK)-1]; //-1 is for the space betweek KW_SPEAK and the start of the phrase
		
		char *phr = (char *)malloc(sizeof(char)*strlen(line)-strlen(KW_SPEAK)-1);
		
		#if defined DEBUG
			printf("Memory allocation OK !\n");
		#endif
		
		int i=0;
		int pos = 0;
		
		#if defined DEBUG
			printf("Start of for loop...\n");
	
			printf("KW_SPEAK => %s\n",KW_SPEAK);
			printf("LINE => %s\n",line);
			
			printf("KW_SPEAK len => %d\n",strlen(KW_SPEAK));
			printf("Line len => %d\n",strlen(line));
		#endif
		
		
	
		
		for(i = strlen(KW_SPEAK)+1; i < strlen(line); i++) //+1 is for the space betweek KW_SPEAK and the start of the phrase
			phr[i-strlen(KW_SPEAK)-1] = line[i];	//-1 because of the space betweek KW_SPEAK and the start of the phrase
		
		#if defined DEBUG
			printf("before trim => %s\n",phr);
		#endif
		
		trim((char*)phr);
		
		#if defined DEBUG
			printf("after trim => %s\n",phr);
		#endif
		
		
		sprintf(CMD,"Tux_TTS(%s,mb-fr1,50,115,1,1)",phr);
	}
	else if(!strcmp(strtolower(sp[cmdPos]),KW_OPEN) || !strcmp(strtolower(sp[cmdPos]),KW_CLOSE) || !strcmp(strtolower(sp[cmdPos]),KW_ROTATE))
	{
		//All others commands have one or more parameters
	
		#if defined DEBUG
			printf("BASIC COMMANDS SEARCH\n");
		#endif
	
	
		#if defined DEBUG
			printf("Search the first parameter's position\n");
		#endif
		
		//Search the frist parameter's pos	
		int fParamPos = 0;
		while(!isFirstParam(sp[fParamPos]))
			fParamPos++;

		#if defined DEBUG
			printf("First parameter's position => %d\n",fParamPos);
		#endif		

		if(!strcmp(strtolower(sp[cmdPos]),KW_OPEN))
		{
			#if defined DEBUG
				printf("KW_OPEN !\n");
			#endif
			
			//OPEN
			//sp[fParamPos] contain what tux must open ^^
			//Tux_Open(Mouth) or Tux_Open(Eyes)
			CMD = (char *)malloc(sizeof(char)*15);
			
			if(!strcmp(strtolower(sp[fParamPos]),KW_MOUTH))
				sprintf(CMD,"Tux_Open(Mouth)");
				
			if(!strcmp(strtolower(sp[fParamPos]),KW_EYES_PLUR))
				sprintf(CMD,"Tux_Open(Eyes)");
		}

		if(!strcmp(strtolower(sp[cmdPos]),KW_CLOSE))
		{
			#if defined DEBUG
				printf("KW_CLOSE !\n");
			#endif
			
			//CLOSE
			CMD = (char *)malloc(sizeof(char)*16);
			
			if(!strcmp(strtolower(sp[fParamPos]),KW_MOUTH))
				sprintf(CMD,"Tux_Close(Mouth)");
				
			if(!strcmp(strtolower(sp[fParamPos]),KW_EYES_PLUR))
				sprintf(CMD,"Tux_Close(Eyes)");
		}
		
		if(!strcmp(strtolower(sp[cmdPos]),KW_ROTATE))
		{
			#if defined DEBUG
				printf("KW_ROTATE !\n");
			#endif
			
			//Check if the command have third parameter
			
			//cmd => sp[cmdPos]
			//direction => sp[fParamPos]
			
			//Turn left (wc = 1)
			//Turn left 1 (wc = 2)
			//Turn left 1 times (WC = 3)
			
			
			#if defined DEBUG
				printf("Search the direction instruction position ...\n");
			#endif
			
			//Search the direction instruction position
			int dPos = 0;
			while(!isDirectionPos(sp[dPos]))
				dPos++;
				
			#if defined DEBUG
				printf("The direction's position => %d\n",dPos);
			#endif
			
			
			if(wc >= 3) //if the user give a position, wc is >= 3
			{
				#if defined DEBUG
					printf("WC >= 3 so the user have gived a number of time\n");
				#endif
				
				CMD = (char *)malloc(sizeof(char)*19); //dont forget to re-alloc after getting the number of times the droid must turn
				
				//Tourne a droite 3 fois
				
				
				#if defined DEBUG
					printf("Search the number of times tux must turn...\n");
				#endif
				//Search the number of turn
				int nPos = 0;
				while(!isNumberPos(sp[nPos]))
					nPos++;
				
				#if defined DEBUG
					printf("Number of times => %d\n",nPos);
				#endif

				CMD = (char *)realloc(CMD,sizeof(char)*19+strlen(sp[nPos]));
				
				if(!strcmp(strtolower(sp[dPos]),KW_LEFT))
					sprintf(CMD,"Tux_Rotate(left,%d)",atoi(sp[nPos]));
					
				if(!strcmp(strtolower(sp[dPos]),KW_RIGHT))
					sprintf(CMD,"Tux_Rotate(right,%d)",atoi(sp[nPos]));
			}
			else //only turn 1 time
			{
				#if defined DEBUG
					printf("Turn only 1 time !\n");
				#endif
				
				//only 1 time
				//cmd => sp[0]
				//left => sp[1]
				
				CMD = (char *)malloc(sizeof(char)*19);
				
				if(!strcmp(strtolower(sp[dPos]),KW_LEFT))
					sprintf(CMD,"Tux_Rotate(left,1)");
				
				if(!strcmp(strtolower(sp[dPos]),KW_RIGHT))
					sprintf(CMD,"Tux_Rotate(right,1)");
			}
		}
	}
	else
	{
		//Others commands (Ex: variables)
		//TODO
	}

	//DEBUG:
	printf("CMD => %s\n",CMD);
}



void loadScript(char *file)
{
	//
	//1 - Open script file 
	//2 - Read it line by line
	//3 - Analyse line and keywords (need to split the line)
	//4 - do action of keywords
	//
	
	#if defined DEBUG
		printf("Enter loadScript() => %s\n",file);
	#endif
	
	FILE *fp = fopen(file,"r");
	if(fp)
	{
		char buffer[4096];
		int in_comment = 0;
		
		while(!feof(fp))
		{
			fgets(buffer,4096,fp);
			
			#if defined DEBUG
				printf("Buffer => %s\n",buffer);
				printf("buffer[0] => %c\n",buffer[0]);
				printf("buffer[1] => %c\n",buffer[1]);
				
				printf("buffer[strlen(buffer)-1] => %c\n",buffer[strlen(buffer)-1]);
				printf("buffer[strlen(buffer)] => %c\n",buffer[strlen(buffer)]);
			#endif
			
			if(buffer[0] == '/' && buffer[1] == '*') //multi line comment start
			{
				#if defined DEBUG
					printf("Enter in multiline comment\n");
				#endif
				
				in_comment = 1;
			}
			
			if(!in_comment)
			{
				if(buffer[0] != '/' && buffer[1] != '/') //one line comment
					analyseLine(buffer);
			}
			
			if( (buffer[strlen(buffer)-1] == '*' && buffer[strlen(buffer)] == '/') || (buffer[0] == '*' && buffer[1] == '/') ) //multiline comment stop
			{
				#if defined DEBUG
					printf("Exit multiline comment\n");
				#endif	
				in_comment = 0;
			}

		}
		
		fclose(fp);
	}
}

int main(int argc, char **argv)
{
	loadScript("default.tds");
	
	
	system("pause");
	
	return 0;
}
