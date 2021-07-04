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

function main()
{
  var y = 8;
  var c = 0;
  while (c < 10)
  {
    print(fibonach(c));
    c = c + 1;
  }

}


function fibonach(x)
{
  if (x == 0)
  {

    return 0;
  }

  if (x == 1)
  {

    return 1;
  }

  var a = x - 1;
  var b = x - 2;
  var c = fibonach(a) + fibonach(b);

  return c;
}


```