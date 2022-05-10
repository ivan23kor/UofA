int function_inner(int x) {
	return x - 1;
}

int function(int x) {
	return function_inner(x + 1);
}

int main() {
	int i = 42;
	int j = function(i);
	return 0;
}
