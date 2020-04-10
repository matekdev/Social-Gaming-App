# C++ Coding Standards

## Functions

- Curly braces should start on the same line

- Function names should be CamelCase (Ex. fooBar)

  - Function names should typically be a verb

- Parameters should be named using CamelCase (same as public variables)

  ```cpp
  void doSomething(int myParam, ) {
      
  }
  ```

  

## Variables

- Public and private variables should NOT start with an underscore

```cpp
public:
	int example = 1;
private:
	int example2 = 2;
```



## Classes

- Class names should be PascalCase
- In a class, the ordering should follow in the public/private section
  - Public should always be declared first (public -> private)
  - Variables should be above function definitions



## Allocating Memory

- Never allocate memory unless you really need to
- Use a unique/shared pointer if you do have to allocate





