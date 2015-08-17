# Lone Spectre

Lone Spectre is a 2D top down realtime stealth strategy game. It is currently in development to be Team 4's Studio Project 3 game.

## Naming conventions
###General 
| Type  | Formatting |
| ------------- | ------------- |
| Constants | `ALL_CAPS`  |
| Global Variables | `g_` |
| Pointer Type | `<Data Type>*` |
| Pointer | `_pointer` |
| Enumeration | `E_ALL_CAPS`|
| Enumeration Members | `<Enumeration Acronym>_ALL_CAPS` |

Do not use `<#pragma once>` use proper C++ standard header guards!
###Classes
| Type  | Formatting |
| ------------- | ------------- |
| Class | `PascalCase` |
| Public Symbols | `PascalCase` |
| Protected/Private Symbols | `camelCase` |
| Member Variables | `m_` |
| Static Variables | `s_` |
| Static Constants | `S_ALL_CAPS` |

In class access specifiers should be in line with the class's curly braces.

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