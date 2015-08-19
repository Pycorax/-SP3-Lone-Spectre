# Lone Spectre  - Convention Document `Revision 4.1.0`

Lone Spectre is a 2D top down realtime stealth strategy game. It is currently in development to be Team 4's Studio Project 3 game.

## Commit Convention 
`Title of Commit`</br>
`-----` (5 Dashes)</br>
`Commit Message`</br>

## Coding Conventions
- Do not use `<#pragma once>` use proper C++ standard header guards!
- Always use `static_cast` instead of C-styled casts!
- Always use `size_t` when iterating through a `array` or a `vector`.
- Always use pre-increment when iterating in a for loop.
- Always use `void` when a function accepts no parameters.
- Describe every member variable if possible.
- For loop iterator variables should be descriptive. E.g. `row` instead of `i`.

## Naming Conventions
### General 
| Type  | Formatting |
| ------------- | ------------- |
| Constants | `ALL_CAPS`  |
| Global Variables | `g_` |
| Pointer Type | `<Data Type>*` |
| Pointer | `_pointer` |
| Enumeration | `E_ALL_CAPS`|
| Enumeration Members | `<Enumeration Acronym>_ALL_CAPS` |

### Classes
| Type  | Formatting |
| ------------- | ------------- |
| Class | `PascalCase` |
| Public Symbols | `PascalCase` |
| Protected/Private Symbols | `camelCase` |
| Member Variables | `m_` |
| Static Variables | `s_` |
| Static Constants | `S_ALL_CAPS` |
| Abstract Public Functions | `className_FunctionName` |
| Abstract Protected/ Private Functions | `className_functionName` |


- In class access specifiers should be in line with the class's curly braces.

The order of declarations in a class should be in this order:

```C++
class MyClass
{
	// Enums
public:
protected:
private:

	// Static Constant Variables
public:
protected:
private:

	// Variables
public:
protected:
private:
	
	// Functions
public:
protected:
private:
	
	// Abstract Functions
public:
protected:
private:
};
```
### Files
- Use `lowercase_with_underscores` naming convention
