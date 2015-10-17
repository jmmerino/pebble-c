/**
* Sustring function
*/
void substr(char dest[], char src[], int offset, int len) {
  int i;
  for(i = 0; i < len && src[offset + i] != '\0'; i++)
	  dest[i] = src[i + offset];
  
  dest[i] = '\0';
}

  