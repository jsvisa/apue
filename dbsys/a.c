struct ssss {
	int a;
	char c[10];
	int b;
};
int main(int argc, char **argv) {
	struct ssss s;
	s.a = 10;
	s.b = 20;
	printf("s.a=%d,s.b=%d\n", s.a, s.b);
	strncpy(s.c, "ruewiqfjdskajfdksafdsa", 9);
//	memset(s.c, 0, 10);
	printf("s.a=%d,s.b=%d\n", s.a, s.b);
	return 0;
}
