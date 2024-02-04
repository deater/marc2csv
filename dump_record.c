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
	if (record[5]=='d') fprintf(stderr,"**** DELETED ****\n");

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

		printf("\tTag: %c%c%c (offset %d) ",
			record[i+0],record[i+1],record[i+2],i);

		/* 0x1e= RS (record separator) */
		if (record[i+0]==0x1e) break;

		tag=0;
		tag+=(record[i+0]-'0')*100;
		tag+=(record[i+1]-'0')*10;
		tag+=(record[i+2]-'0')*1;

		printf("Field Length: %c%c%c%c ",
			record[i+3],record[i+4],record[i+5],record[i+6]);

		field_length=0;
		field_length+=(record[i+3]-'0')*1000;
		field_length+=(record[i+4]-'0')*100;
		field_length+=(record[i+5]-'0')*10;
		field_length+=(record[i+6]-'0')*1;

		printf("Starting char position: %c%c%c%c%c\n",
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

		printf("\t\t");
		switch(tag) {
			case 1: // 001
				//printf("Control number: %s\n",string);
				printf("Control number: ");
				print_record(string);
				printf("\n");
				break;
			case 3: // 003
				//printf("Control number ID: %s\n",string);
				printf("Control number ID: ");
				print_record(string);
				printf("\n");
				break;
			case 5: // 005
				//printf("Date/Time: %s\n",string);
				printf("Date/Time: ");
				print_record(string);
				printf("\n");
				break;
			case 7: // 007
				//printf("Date/Time: %s\n",string);
				printf("Physical Description: ");
				print_record(string);
				printf("\n");
				break;
			case 8: // 008
				//printf("Fixed-length data: %s\n",string);
				printf("Fixed-length data: ");
				print_record(string);
				printf("\n");
				break;
			case 10: // 010
				printf("Library of Congress #: ");
				print_record(string);
				printf("\n");
				break;
			case 15: // 015
				printf("National Bibliography Number #: ");
				print_record(string);
				printf("\n");
				break;
			case 17: // 017
				printf("Copyright: ");
				print_record(string);
				printf("\n");
				break;
			case 20: // 020
				printf("ISBN: ");
				print_record(string);
				printf("\n");
				break;
			case 24: // 024
				printf("Other standard identifier: ");
				print_record(string);
				printf("\n");
				break;
			case 28: // 028
				printf("Publisher Number: ");
				print_record(string);
				printf("\n");
				break;
			case 34: // 034
				printf("Coded Cartographic Mathematical: ");
				print_record(string);
				printf("\n");
				break;
			case 35: // 035
				printf("System Control Number: ");
				print_record(string);
				printf("\n");
				break;
			case 37: // 037
				printf("Source of Acquisition: ");
				print_record(string);
				printf("\n");
				break;
			case 40: // 040
				printf("Catalog Source: ");
				print_record(string);
				printf("\n");
				break;
			case 41: // 041
				printf("Language Code: ");
				print_record(string);
				printf("\n");
				break;
			case 42: // 042
				printf("Authentication Code: ");
				print_record(string);
				printf("\n");
				break;
			case 43: // 043
				printf("Geographic Area Code: ");
				print_record(string);
				printf("\n");
				break;
			case 50: // 050
				printf("Library of Congress Call Number: ");
				print_record(string);
				printf("\n");
				break;
			case 51: // 051
				printf("Library of Congress Copy: ");
				print_record(string);
				printf("\n");
				break;
			case 52: // 052
				printf("Geographic Classification: ");
				print_record(string);
				printf("\n");
				break;
			case 70: // 070
				printf("National Agricultural Library #: ");
				print_record(string);
				printf("\n");
				break;
			case 72: // 072
				printf("Subject Category: ");
				print_record(string);
				printf("\n");
				break;
			case 82: // 082
				printf("Dewey Decimal Number: ");
				print_record(string);
				printf("\n");
				break;
			case 97: // 097
				printf("Local Call Number: ");
				print_record(string);
				printf("\n");
				break;
			case 100: // 100
				printf("Personal Name: ");
				print_record(string);
				printf("\n");
				break;
			case 110: // 110
				printf("Corporate Name: ");
				print_record(string);
				printf("\n");
				break;
			case 130: // 130
				printf("Uniform Title: ");
				print_record(string);
				printf("\n");
				break;
			case 240: // 240
				printf("Uniform Title: ");
				print_record(string);
				printf("\n");
				break;
			case 245: // 245
				printf("Title Statement: ");
				print_record(string);
				printf("\n");
				break;
			case 246: // 246
				printf("Varying Form of Title: ");
				print_record(string);
				printf("\n");
				break;
			case 250: // 250
				printf("Edition Statement: ");
				print_record(string);
				printf("\n");
				break;
			case 255: // 255
				printf("Cartographic Math: ");
				print_record(string);
				printf("\n");
				break;
			case 257: // 257
				printf("Courtesy of: ");
				print_record(string);
				printf("\n");
				break;
			case 260: // 260
				printf("Publication/Distribution: ");
				print_record(string);
				printf("\n");
				break;
			case 263: // 263
				printf("Projected Publication Date: ");
				print_record(string);
				printf("\n");
				break;
			case 852: // 852
				printf("Location: ");
				print_record(string);
				printf("\n");
				break;
			case 300: // physical description
				printf("Physical description: ");
				print_record(string);
				printf("\n");
				break;
			case 306: // playing time
				printf("Playing time: ");
				print_record(string);
				printf("\n");
				break;
			case 350: // 350
				printf("Unknown: ");
				print_record(string);
				printf("\n");
				break;
			case 440: // 440
				printf("Series Statement (obsolete): ");
				print_record(string);
				printf("\n");
				break;
			case 490: // 490
				printf("Series Statement: ");
				print_record(string);
				printf("\n");
				break;
			case 500: // 500
				printf("General Note: ");
				print_record(string);
				printf("\n");
				break;
			case 502: // 502
				printf("Dissertation Note: ");
				print_record(string);
				printf("\n");
				break;
			case 504: // 504
				printf("Bibliography: ");
				print_record(string);
				printf("\n");
				break;
			case 505: // 505
				printf("Formatted Contents: ");
				print_record(string);
				printf("\n");
				break;
			case 508: // 508
				printf("Creation Credit: ");
				print_record(string);
				printf("\n");
				break;
			case 511: // 511
				printf("Participant Note: ");
				print_record(string);
				printf("\n");
				break;
			case 520: // summary
				printf("Summary: ");
				print_record(string);
				printf("\n");
				break;
			case 521: // target audience
				printf("Target audience: ");
				print_record(string);
				printf("\n");
				break;
			case 530: // additional physical form
				printf("Additional Physical Form: ");
				print_record(string);
				printf("\n");
				break;
			case 538: // system details
				printf("System details: ");
				print_record(string);
				printf("\n");
				break;
			case 541: // immediate source
				printf("Immediate Source: ");
				print_record(string);
				printf("\n");
				break;
			case 586: // awards note
				printf("Awards Note: ");
				print_record(string);
				printf("\n");
				break;
			case 590: // local note
				printf("Local Note: ");
				print_record(string);
				printf("\n");
				break;
			case 600: // 600
				printf("Subject Added (Name): ");
				print_record(string);
				printf("\n");
				break;
			case 610: // 610
				printf("Subject Added (Corporate): ");
				print_record(string);
				printf("\n");
				break;
			case 611: // 611
				printf("Meeting name: ");
				print_record(string);
				printf("\n");
				break;
			case 630: // 630
				printf("Subject Added (Uniform Title): ");
				print_record(string);
				printf("\n");
				break;
			case 650: // topic
				printf("Topic: ");
				print_record(string);
				printf("\n");
				break;
			case 651: // subject, geographic
				printf("Subject, Geographic: ");
				print_record(string);
				printf("\n");
				break;
			case 653: // 653
				printf("Index -- Uncontrolled: ");
				print_record(string);
				printf("\n");
				break;
			case 655: // 655
				printf("Index -- Genre: ");
				print_record(string);
				printf("\n");
				break;
			case 700: // added personal name
				printf("Added Personal Name: ");
				print_record(string);
				printf("\n");
				break;
			case 710: // added corporate name
				printf("Added Corporate Name: ");
				print_record(string);
				printf("\n");
				break;
			case 730: // added uniform title
				printf("Added Uniform Title: ");
				print_record(string);
				printf("\n");
				break;
			case 740: // added uncontrolled title
				printf("Added Uncontrolled Title: ");
				print_record(string);
				printf("\n");
				break;
			case 800: // series added - personal name
				printf("Series Added - Personal Name: ");
				print_record(string);
				printf("\n");
				break;
			case 830: // series added - uniform title
				printf("Series Added - Uniform Title: ");
				print_record(string);
				printf("\n");
				break;
			case 856: // electronic location
				printf("Electronic Location: ");
				print_record(string);
				printf("\n");
				break;
			case 902: // local data
				printf("Local Data 2: ");
				print_record(string);
				printf("\n");
				break;
			case 905: // local data
				printf("Local Data 5: ");
				print_record(string);
				printf("\n");
				break;
			case 906: // local data
				printf("Local Data 6: ");
				print_record(string);
				printf("\n");
				break;
			case 920: // local data
				printf("Local Data 20: ");
				print_record(string);
				printf("\n");
				break;
			case 922: // local data
				printf("Local Data 22: ");
				print_record(string);
				printf("\n");
				break;
			case 925: // local data
				printf("Local Data 25: ");
				print_record(string);
				printf("\n");
				break;
			case 948: // local data
				printf("Local Data 48: ");
				print_record(string);
				printf("\n");
				break;
			case 950: // local data
				printf("Local Data 50: ");
				print_record(string);
				printf("\n");
				break;
			case 952: // local data
				printf("Local Data 52: ");
				print_record(string);
				printf("\n");
				break;
			case 953: // local data
				printf("Local Data 53: ");
				print_record(string);
				printf("\n");
				break;
			case 955: // local data
				printf("Local Data 55: ");
				print_record(string);
				printf("\n");
				break;
			case 963: // local data
				printf("Local Data 63: ");
				print_record(string);
				printf("\n");
				break;
			case 984: // local data
				printf("Local Data 84: ");
				print_record(string);
				printf("\n");
				break;
			case 985: // local data
				printf("Local Data 85: ");
				print_record(string);
				printf("\n");
				break;
			case 991: // local data
				printf("Local Data 91: ");
				print_record(string);
				printf("\n");
				break;
			case 994: // local data
				printf("Local Data 94: ");
				print_record(string);
				printf("\n");
				break;
			case 995: // local data
				printf("Local Data 95: ");
				print_record(string);
				printf("\n");
				break;
			case 998: // local data
				printf("Local Data 98: ");
				print_record(string);
				printf("\n");
				break;

			default:
				fprintf(stderr,"***UNKNOWN TAG %d\n",tag);
				printf("***UNKNOWN TAG %d\n",tag);

		}


		i=i+12;
		if (i>=base_address) break;
	}
	printf("\n");

	return 0;
}
