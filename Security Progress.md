Security Fixes:
1. Vulnerabilities in included libraries. I only include 1 library in this code (Keyboard.h). After extensive online research, I have found no known relevant security vulnerabilities included in that library. Therefore, no changes need to be considered in the code regarding this library.
2. Physical security of actual hardware. Due to the nature of the product and project, very little can be done to improve the physical security of each keypad. This is because this project focuses on providing a design for a keypad that other people can use as a starting point and modify to meet their needs. Therefore, the target use case is very broad and involves very different physical security considerations, thus it will go against the nature of the project to include physical security precautions in the design. However, a disclaimer was added in the ReadMe.md file of the project in [commit e0c8cb9](https://github.com/jordankooyman/Diacritic_Keypad/blob/e0c8cb98c9d53e41b8528363b02cca5fffdffaf5/README.md).
3. Fix product code being overwritten. To prevent the code from being overwritten accidentally or maliciously, several methods have been added to the ReadMe.md file of the project detailing different things that can be done to reduce or eliminate this vulnerability. These were added in [commit e0c8cb9](https://github.com/jordankooyman/Diacritic_Keypad/blob/e0c8cb98c9d53e41b8528363b02cca5fffdffaf5/README.md).

Upcoming Fixes:
1. Add security design considerations section (including issues with adding wireless connectivity). Priority: Mid Difficulty: Moderate
2. Add tutorial sections for different ways to fix code overwrite problem. Priority: High Difficulty: Moderate
3. Add logging option to code, primarily for debugging, but with EEPROM as an option if desired. Priority: Low Difficulty: Difficult
4. Add suggested product test procedure to project to help with debugging and building. Priority: Mid Difficulty: Easy
