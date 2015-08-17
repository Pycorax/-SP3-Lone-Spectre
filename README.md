# Lone Spectre

Lone Spectre is a 2D top down realtime stealth strategy game. It is currently in development to be Team 4's Studio Project 3 game.

## Commit Convention
`Title of Commit`
`-----` (5 Dashes)
`Commit Message`

## Coding Conventions
- Do not use `<#pragma once>` use proper C++ standard header guards!
- Always use `static_cast` instead of C-styled casts!
- Always use `size_t` when iterating through a `array` or a `vector`.
- Always use pre-increment when iterating in a for loop.
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


- In class access specifiers should be in line with the class's curly braces.

The order of declarations in a class should be in this order:

    class MyClass
    {
    	// Public Enums
    	// Static Variables
    	// Public Variables
    	
		// Protected Enums
    	// Protected Variables
    	
    	// Private Enums
    	// Private Variables
    	
    	// Public Functions
    	// Protected Functions
    	// Private Functions
    };
