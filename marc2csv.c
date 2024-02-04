#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

static char record[8192];
static char string[8192];
static int base_address;

/* https://www.loc.gov/marc/bibliographic/bdleader.html */

int find_tag(int which, int *length, int *start_addr) {

	int i=24;
	int tag,field_length,start;

	/* what follows is directory, 12 char chunks */
	while(1) {

//		printf("\t%d Tag: %c%c%c\n",i,
//			record[i+0],record[i+1],record[i+2]);

		/* 0x1e= RS (record separator) */
		if (record[i+0]==0x1e) break;

		tag=0;
		tag+=(record[i+0]-'0')*100;
		tag+=(record[i+1]-'0')*10;
		tag+=(record[i+2]-'0')*1;

//		printf("\t\tField Length: %c%c%c%c\n",
//			record[i+3],record[i+4],record[i+5],record[i+6]);

		field_length=0;
		field_length+=(record[i+3]-'0')*1000;
		field_length+=(record[i+4]-'0')*100;
		field_length+=(record[i+5]-'0')*10;
		field_length+=(record[i+6]-'0')*1;

//		printf("\t\tStarting char position: %c%c%c%c%c\n",
//			record[i+7],record[i+8],record[i+9],
//			record[i+10],record[i+11]);

		start=0;
		start+=(record[i+7]-'0')*10000;
		start+=(record[i+8]-'0')*1000;
		start+=(record[i+9]-'0')*100;
		start+=(record[i+10]-'0')*10;
		start+=(record[i+11]-'0')*1;

		if (which==tag) {
			*length=field_length;
			*start_addr=start;
			return 0;
		}

		i+=12;
	}

	return -1;
//		memcpy(string,&record[base_address+start],field_length);
//		string[field_length+1]='\0';
//	}
}

		/* 0x1f+letter = code */
		/* 0x1e = end of record? */
static void print_record(int tag) {

	int found,field_length,start;
	int i=0;
	int first_quote=1;

	found=find_tag(tag,&field_length,&start);

	if (found<0) {
		printf("\" \",");
		return;
	}
	else {
		memcpy(string,&record[base_address+start],field_length);
		string[field_length+1]='\0';
	}


	printf("\" ");

	while(1){
		if (string[i]==0x1e) {
			printf("\",");
			break;
		}
		if (string[i]==0x1f) {
			if (first_quote) {
				first_quote=0;
				printf(" ; ");
			}
			else {
				printf(" ; ");
			}
			i++;
//			printf("$%c ",string[i]);
			i++;
			continue;
		}

		/* escape quote as "" */
		if (string[i]=='"') printf("\"");

		printf("%c",string[i]);
		i++;
	}
}



void print_string(int tag) {

	int found,field_length,start,i;

	found=find_tag(tag,&field_length,&start);

	if (found<0) {
		printf("\" \",");
		return;
	}
	else {
		memcpy(string,&record[base_address+start],field_length);
		string[field_length+1]='\0';
		printf("\"");
		for(i=0;i<strlen(string);i++) {
			/* FIXME: break in this case? */

			if (string[i]==0x1e) continue;

			/* escape quote as "" */
			if (string[i]=='"') printf("\"");

			printf("%c",string[i]);
		}
		printf("\",");
	}

}

int main(int argc, char **argv) {

	int result,length;

	result=read(STDIN_FILENO,record,8192);

	if (result<0) {
		fprintf(stderr,"Error opening! %s\n",strerror(errno));
		exit(0);
	}

	if (result==8192) {
		fprintf(stderr,"Record too big!\n");
		exit(0);
	}

//	printf("Record label\n");
	// first 5 bytes is length, in ASCII
	length=0;
	length+=(record[0]-'0')*10000;
	length+=(record[1]-'0')*1000;
	length+=(record[2]-'0')*100;
	length+=(record[3]-'0')*10;
	length+=(record[4]-'0')*1;

#if 0
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
#endif

	base_address=0;
	base_address+=(record[12]-'0')*10000;
	base_address+=(record[13]-'0')*1000;
	base_address+=(record[14]-'0')*100;
	base_address+=(record[15]-'0')*10;
	base_address+=(record[16]-'0')*1;

#if 0
	printf("\tEncoding Level: %c\n",record[17]);

	printf("\tDescriptive catalog form: %c\n",record[18]);

	printf("\tMultipart resource record level: %c\n",record[19]);

	/* always 4 */
	printf("\tLength of length-of-field: %c\n",record[20]);

	/* always 5 */
	printf("\tLength of starting char position: %c\n",record[21]);

	printf("\tLength of implementation-defined: %c\n",record[22]);

	printf("\tUndefined: %c\n",record[23]);
#endif

	print_string(1);		// control number
//	print_string(3);		// control number id
	print_string(5);		// date/time
	print_record(20);		// ISBN
	print_record(100);		// author
	print_record(245);		// title statement
	print_record(246);		// varying title
	print_record(852);		// location
	print_record(500);		// general note
	print_record(350);		// unknown
	print_record(653);		// uncontrolled
	print_record(300);		// physical description
	print_record(520);		// summary
	print_record(650);		// topic

	printf("\n");
	return 0;
}
