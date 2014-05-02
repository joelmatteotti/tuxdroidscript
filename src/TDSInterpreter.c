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

int isFirstParam(char *str)
{
	if(!strcmp(strtolower(str),KW_MOUTH) || !strcmp(strtolower(str),KW_EYES_SING) || !strcmp(strtolower(str),KW_EYES_PLUR)
		|| !strcmp(strtolower(str),KW_RIGHT) || !strcmp(strtolower(str),KW_LEFT))
		{
			return 1;
		}
		
	return 0;
	
}

//get direction position
int isDirectionPos(char *str)
{
	if(!strcmp(strtolower(str),KW_LEFT) || !strcmp(strtolower(str),KW_RIGHT))
		return 1;
		
	return 0;
}


//get number position
int isNumberPos(char *str)
{
	if(atoi(str))
		return 1;
		
	
	return 0;
}


void analyseLine(char *line)
{

	line = trim(line);
	
	if(!strcmp(line,""))
		return;	
		
	if(line == NULL)
		return;
	
	int wc = countCharacterOccurency(line,' '); //count the number of word
	char **sp = explode(line,' '); //space is the separator in the line (Ex: turn left 10 time)
	
	
	//1 - Search the cmd
	//2 - Search the first param
	//3 - Search if more parameter exist
	
	char *CMD = NULL;

	int cmdPos = 0;
	
	while(!isCmd(sp[cmdPos]))
	{
		cmdPos++;
	}

	if(!strcmp(strtolower(sp[cmdPos]), KW_SPEAK))
	{
		//Speak only have phrase as parameter
		CMD = (char *)malloc(sizeof(char)*strlen(line)-strlen(sp[cmdPos])+27); //27 is for the command's len
		
		//Use all str except KW_SPEAK (sp[0])
		
		char phr[strlen(line)+5];
		int i;
		int pos = 0;
		
		for(i = strlen(KW_SPEAK); i < strlen(line); i++)
		{
			phr[i-strlen(KW_SPEAK)] = line[i];	
		}
		
		trim((char*)phr);
		
		sprintf(CMD,"Tux_TTS(%s,mb-fr1,50,115,1,1)",phr);
	}
	else
	{
		//All others commands have one or more parameters
		
		int fParamPos = 0;
		
		while(!isFirstParam(sp[fParamPos]))
		{
			fParamPos++;
		}
		

		if(!strcmp(strtolower(sp[cmdPos]),KW_OPEN))
		{
			//OPEN
			
			//sp[fParamPos] contain what tux must open ^^
			
			//Tux_Open(Mouth) or Tux_Open(Eyes)
			
			//15
			CMD = (char *)malloc(sizeof(char)*15);
			
			if(!strcmp(strtolower(sp[fParamPos]),KW_MOUTH))
				sprintf(CMD,"Tux_Open(Mouth)");
				
			if(!strcmp(strtolower(sp[fParamPos]),KW_EYES_PLUR))
				sprintf(CMD,"Tux_Open(Eyes)");

		}

		if(!strcmp(strtolower(sp[cmdPos]),KW_CLOSE))
		{
			//CLOSE
			
			CMD = (char *)malloc(sizeof(char)*16);
			
			if(!strcmp(strtolower(sp[fParamPos]),KW_MOUTH))
				sprintf(CMD,"Tux_Close(Mouth)");
				
			if(!strcmp(strtolower(sp[fParamPos]),KW_EYES_PLUR))
				sprintf(CMD,"Tux_Close(Eyes)");
		}
		
		
		
		if(!strcmp(strtolower(sp[cmdPos]),KW_ROTATE))
		{
			//Check if the command have third parameter
			
			//cmd => sp[cmdPos]
			//direction => sp[fParamPos]
			
			//Turn left (wc = 1)
			//Turn left 1 (wc = 2)
			//Turn left 1 times (WC = 3)
			
			
			int dPos = 0;
			
			while(!isDirectionPos(sp[dPos]))
			{
				dPos++;
			}
			
			
			if(wc >= 3)
			{
				CMD = (char *)malloc(sizeof(char)*19); //dont forget to re-alloc after getting the number of times the droid must turn
				
				//Tourne a droite 3 fois
				
				int nPos = 0;
				
				while(!isNumberPos(sp[nPos]))
				{
					nPos++;
				}
				

				CMD = (char *)realloc(CMD,sizeof(char)*19+strlen(sp[nPos]));
				
				
				if(!strcmp(strtolower(sp[dPos]),KW_LEFT))
					sprintf(CMD,"Tux_Rotate(left,%d)",atoi(sp[nPos]));
					
				if(!strcmp(strtolower(sp[dPos]),KW_RIGHT))
					sprintf(CMD,"Tux_Rotate(right,%d)",atoi(sp[nPos]));
					
			}
			else
			{
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


	//DEBUG:*
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
	
	FILE *fp = fopen(file,"r");
	if(fp)
	{
		char buffer[4096];
		int in_comment = 0;
		
		while(!feof(fp))
		{
			fgets(buffer,4096,fp);
			
			if(buffer[0] != '/' && buffer[1] != '/')
			{
				if(buffer[0] == '/' && buffer[1] == '*')
					in_comment = 1;
					
				if(!in_comment)
					analyseLine(buffer);
			
				if(buffer[0] == '*' && buffer[1] == '/')
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
