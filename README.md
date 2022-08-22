# `muse` 

muse, a static, compiled language that features type inference, multiple dispatch and a rust-like memory model, with a readable pythonic syntax.

## Language plan
- [x] Define language syntax and semmantics. 
- [-] Lexer and Parser written in C++
- [ ] local type inference and type checking
- [ ] data race lifetime checking
- [ ] Multiple MLIR dialects as pre-transpile targets to LLVM, suporting the tensor dialect natively.
- [ ] Robust CLI that supports WebAssembly as a target and modern app bundling features.
- [ ] Smart linter that helps with type checking and memory management errors.
- [ ] Syntax highlighter for IDE and documentation
- [ ] Documentation
- [ ] `mux`: a jsx-like html inspired sub-syntax that allows for simple decalarative GUI programming for web dev.

# Syntax

## General Syntax

```
<Statement>             ::= <VariableDecleration>
                          | <Reassignment>
                          | <FunctionDef>
                          | <ForLoop>
                          | <WhileLoop>
                          | <Expression>

<Expression>            ::= <Value>
                          | <ListComprehension>
                          | <TernaryOperator>
                          | <WalrusOperator>
                          | <FunctionCall>
                          | <Generator>
                          | <BooleanExpression>
```

## Variable Decleration and Assignment
Same as most languages, can be declared with or without types as the compiler will preform global type inference to map out all the types statically.

Type defenitions use the colon `:` with the variable name on the left hand side and the type on the right.
```
<TypeDef>               ::= : <Type>
<VariableDecleration>   ::= <Identifier> <TypeDef>? = <Expression>
<Reassignment>          ::= <SymbolTableVariable> = <Expression>
```

Examples:
```julia
x = 0
y: int = 0
```

## Function Decleration

Functions can be declared both imperatively and declaratively (lambdas).

`muse` implements `multiple dispatch` which allows functions to be dispatches on abstract types without collisions.

```
<Arguments>             ::= <Identifier> <TypeDef>? 
<Prototype>             ::= <Identifier>(<Arguments>*)
<StatementBlock>        ::= INDENT <Statement>* DEDENT
<FunctionDef>           ::= <Prototype> <TypeDef>? = <Expression> 
                          | <Prototype> (: <Type>)? <StatementBlock>
```


```julia
fibonacci(x)
    if x <= 1
        return n
    else    
        return fibonacci(n-1) + fibonacci(n-2)

add(x: int, y: int): int = x + y
```

## Function Call

Function calls are standard, although `muse` also supports infix notation with functions that have one or two parameters.
```
<FunctionCall>          ::= <SymolTableFunction>(<Value>?) 
                          | <Value> <SymoblTableFunction> <Value>?
```

```julia
isMultipleOf(a, b)

a isMultipleOf b       

a isEven        # infix function call with a single argument
```

## For Loop

For loops (statements) are similar to python (ommiting the colon `:` to indicate a statement block). 

A new feature for creating new lists (more specifically sets) is the list generator, which is also used to replace pythons `range()` function.

Furthermore `muse` also supports list comprehensions from python, which work in exactly the same way. They are the programatical analogue of set notation in mathematics.

```
<Step>                  ::= , <Number>
<Generator>             ::= [<Number>?<Step>?..<Number>]
<ForLoop>               ::= for <Identifier> in <Iterable>|<Generator> <StatementBlock>
<ListComprehension>     ::= [<Expression> for <Identifier> in <Iterable>|<Generator> <TernaryOperator>?]
```

```julia
xs: set = [1..100]  # generate a list that goes from one to one hundred

for i in [1..10]
    print(i)

for i in [1, 3..11]
    print(i)

[2n for n in numbers if n%2==0]
```

## While Loop

```
<WhileLoop>             ::= while <BooleanExpression> <StatementBlock>
```

```julia
while x not in list
    print(f"{x} is not in the list")
```

## If Else Statement


```
<IfStatement>           ::= if <BooleanExpression> <StatementBlock> <ElseIfStatement>*? <ElseStatement>?
<ElseIfStatement>       ::= if else <BooleanExpression>  <StatementBlock>
<ElseStatement>         ::= else <StatementBlock> 
                          | else <Statement>
``` 

```py
if x>=0 and x isEven
    print(f"{X} is positive and even")

else if x<0 and isEven
    print(f"{x} is negative and even)

else 
    print(f"{x} is not even")
```

## Class Defenition and Instantiation
Classes can have fields and methods. Either of those can be public or protected. All class members are private by default.

Fields and methods can be static or default(singleton)

```
<ClassInstantiation> ::= new <SymbolTableConstructor>(<Arguments>*)

<ClassDefenition> ::= class <Identifier> 
INDENT 
?=<Constructor>* 
?=<FieldDeclaration>* 
?=<MethodDeclaration>*
DEDENT

<accessModifier> ::= public | protected | private

<Constructor> ::= <accessModifier>? constructor(<Parameters>) <StatementBlock>

<FieldDeclaration> ::= <accessModifier>? <VariableDeclaration>

<MethodDeclaration> ::= <accessModifier>? <FieldDeclaration>
```

```julia

class Cat
    
    hungry: bool = true

    Cat(name, age)
        this.name = name
        this.age = age
        while hungry
            print("meow")
    
    feed()
        this.hungry = false

```        





