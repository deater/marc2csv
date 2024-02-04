#include <stdio.h>

int main(int argc, char **argv) {
	printf("\"Control Number\",");		// 001
//	printf("\"Control Number ID\",");	// 003
	printf("\"Date/Time\",");		// 005
	printf("\"ISBN\",");			// 020
	printf("\"Author\",");			// 100
	printf("\"Title\",");			// 245
	printf("\"Varying Title\",");		// 246
	printf("\"Location\",");		// 852
	printf("\"General Note\",");		// 500
	printf("\"Unknown\",");			// 350
	printf("\"Uncontrolled\",");		// 653
	printf("\"Physical Description\",");	// 300
	printf("\"Summary\",");			// 520
	printf("\"Topic\",");			// 650
	printf("\"LOC Call\",");		// 50
	printf("\"Dewey Decimal\",");		// 82
	printf("\n");
	return 0;
}
