#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

static char record[8192];
static char string[8192];

/* https://www.loc.gov/marc/bibliographic/bdleader.html */

static void print_record(char *string) {

	int i=0;
	while(1){
		if (string[i]==0x1e) break;
		if (string[i]==0x1f) {
			i++;
			printf("$%c ",string[i]);
			i++;
			continue;
		}
		printf("%c",string[i]);
		i++;
	}
}

int main(int argc, char **argv) {

	int result,i,length,base_address,tag,field_length,start;

	result=read(STDIN_FILENO,record,8192);

	if (result<0) {
		fprintf(stderr,"Error opening! %s\n",strerror(errno));
		exit(0);
	}

	if (result==8192) {
		fprintf(stderr,"Record too big!\n");
		exit(0);
	}

	printf("Record label\n");
	// first 5 bytes is length, in ASCII
	length=0;
	length+=(record[0]-'0')*10000;
	length+=(record[1]-'0')*1000;
	length+=(record[2]-'0')*100;
	length+=(record[3]-'0')*10;
	length+=(record[4]-'0')*1;

	printf("\tRecord Length: %d bytes\n",length);

	/* a=increase, c=corrected, d=deleted, n=nde, p=increase */
	printf("\tRecord status: %c\n",record[5]);


	printf("\tType of record: %c\n",record[6]);

	printf("\tBibliographic level: %c\n",record[7]);

	printf("\tType of control: %c\n",record[8]);

	printf("\tCharacter Coding: %c\n",record[9]);

	printf("\tIndicator count: %c\n",record[10]);

	printf("\tSubfield code count: %c\n",record[11]);

	printf("\tBase address of data: %c%c%c%c%c\n",
		record[12],record[13],record[14],record[15],record[16]);
	base_address=0;
	base_address+=(record[12]-'0')*10000;
	base_address+=(record[13]-'0')*1000;
	base_address+=(record[14]-'0')*100;
	base_address+=(record[15]-'0')*10;
	base_address+=(record[16]-'0')*1;


	printf("\tEncoding Level: %c\n",record[17]);

	printf("\tDescriptive catalog form: %c\n",record[18]);

	printf("\tMultipart resource record level: %c\n",record[19]);

	/* always 4 */
	printf("\tLength of length-of-field: %c\n",record[20]);

	/* always 5 */
	printf("\tLength of starting char position: %c\n",record[21]);

	printf("\tLength of implementation-defined: %c\n",record[22]);

	printf("\tUndefined: %c\n",record[23]);

	i=24;
	printf("Directory\n");
	/* what follows is directory, 12 char chunks */
	while(1) {

		printf("\t%d Tag: %c%c%c\n",i,
			record[i+0],record[i+1],record[i+2]);

		/* 0x1e= RS (record separator) */
		if (record[i+0]==0x1e) break;

		tag=0;
		tag+=(record[i+0]-'0')*100;
		tag+=(record[i+1]-'0')*10;
		tag+=(record[i+2]-'0')*1;

		printf("\t\tField Length: %c%c%c%c\n",
			record[i+3],record[i+4],record[i+5],record[i+6]);

		field_length=0;
		field_length+=(record[i+3]-'0')*1000;
		field_length+=(record[i+4]-'0')*100;
		field_length+=(record[i+5]-'0')*10;
		field_length+=(record[i+6]-'0')*1;

		printf("\t\tStarting char position: %c%c%c%c%c\n",
			record[i+7],record[i+8],record[i+9],
			record[i+10],record[i+11]);

		start=0;
		start+=(record[i+7]-'0')*10000;
		start+=(record[i+8]-'0')*1000;
		start+=(record[i+9]-'0')*100;
		start+=(record[i+10]-'0')*10;
		start+=(record[i+11]-'0')*1;

		memcpy(string,&record[base_address+start],field_length);
		string[field_length+1]='\0';

		/* 0x1f+letter = code */
		/* 0x1e = end of record? */

		switch(tag) {
			case 1: // 001
				printf("Control number: %s\n",string);
				break;
			case 3: // 003
				printf("Control number ID: %s\n",string);
				break;
			case 5: // 005
				printf("Date/Time: %s\n",string);
				break;
			case 20: // 020
				// maybe leading 'a'
				printf("ISBN: %s\n",string);
				break;
			case 100: // 100
				// maybe leading 'a'
				printf("Personal Name: ");
				print_record(string);
				printf("\n");
				break;
			case 245: // 245
				// maybe leading 'a'
				printf("Title Statement: ");
				print_record(string);
				printf("\n");
				break;
			case 246: // 246
				// maybe leading 'a'
				printf("Varying Form of Title: ");
				print_record(string);
				printf("\n");
				break;
			case 852: // 852
				// maybe leading 'a'
				printf("Location: ");
				print_record(string);
				printf("\n");
				break;
			case 500: // 500
				// maybe leading 'a'
				printf("General Note: ");
				print_record(string);
				printf("\n");
				break;
			case 350: // 350
				// maybe leading 'a'
				printf("Unknown: ");
				print_record(string);
				printf("\n");
				break;
			case 653: // 653
				// maybe leading 'a'
				printf("Uncontrolled: ");
				print_record(string);
				printf("\n");
				break;
			case 300: // physical description
				printf("Physical description: ");
				print_record(string);
				printf("\n");
				break;
			case 520: // summary
				printf("Summary: ");
				print_record(string);
				printf("\n");
				break;
			case 650: // topic
				printf("Topic: ");
				print_record(string);
				printf("\n");
				break;

			default: fprintf(stderr,"***UNKNOWN TAG %d\n",tag);
		}


		i=i+12;
		if (i>=base_address) break;
	}

	return 0;
}
