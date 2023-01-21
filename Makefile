all:
	g++ main.cpp -o main.out
	g++ examples/realNumbers.cpp -o realNumbers.out
	g++ examples/concatAndIntersaction.cpp -o concatAndIntersaction.out
	g++ examples/arithmeticExpressions.cpp -o arithmeticExpressions.out
main:
	g++ main.cpp -o main.out
concatAndIntersaction:
	g++ examples/concatAndIntersaction.cpp -o concatAndIntersaction.out
realNumbers:
	g++ examples/realNumbers.cpp -o realNumbers.out
arithmeticExpressions:
	g++ examples/arithmeticExpressions.cpp -o arithmeticExpressions.out
