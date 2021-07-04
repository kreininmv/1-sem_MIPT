# My own programming language.

Graduate project of 1-st semester in MIPT.

## That project consist of 4 parts:
1. Frontend - translates from text file into syntax tree.
2. Frontend^-1 - translates from syntax tree in text file.
3. Middle-end - simplifys expressions and takes derivative in syntax tree.
4. Backend - translates from syntax tree in my own assembler.

In this part, I will describe how each part works.

## Frontend.
At the input, the program receives a text file with the program code written in my own language.
Than my programm makes tokens and then a syntax tree is built using the recursive descent method, and I write it to a file.
This is example of code.

``` C++ 

function main() {
  var y = 8;
  var c = 0;
  while (c < 10) {
    print(fibonach(c));
    c = c + 1;
  }
}

function fibonach(x) {
  if (x == 0){
    return 0;
  }

  if (x == 1) {
    return 1;
  }

  var a = x - 1;
  var b = x - 2;
  var c = fibonach(a) + fibonach(b);

  return c;
}
```
## Frontend^-1.
At the input, the programm receives a file, where described a syntax tree. Programm reads tree from file and writes code of programm only by information that saved in syntax tree.

## Middle-end.
At the input, the programm receives, where described a syntax tree. Programm reads syntax tree and than starts looking for expressions that can simplify, with operator of taking derivative of the function and makes calculus of expressions.

## Back end.
At the input, the programm receives a file, where descibed a syntax tree. Programm read syntax tree and than translate into my own assembler and save in text file.
