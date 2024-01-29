# Qt-Calculator

## About
This calculator mimics the iPhone calculator app, in which it can perform operations on multiple sets of values, handle decimal/negative values, and clear the display when you enter an unwanted value without resetting the equation. 

It was quite a challenge to get this app working as every calculator tutorial I could find dealt with no more than 2 sets of values at a time and I had little to no experience in building a calculator that goes beyond those boundaries. With that being said, there was lots of trial and error as well as comparisons between this app and the iPhone calculator to make sure it remained accurate throughout the whole process. It's not perfect, but I think I got pretty close; Try it for yourself!

- This project uses the Qt Creator library for the UI and code in C++
- I may come back from time to time to fix bugs and maybe add quality-of-life updates if there is a demand for them

## Buttons
- ```"0-9"``` button: adds that specific number to the display 
- ```"AC"``` button (All Clear): Resets the display and the entire equation
- ```"C"``` button (Clear): Resets the display to 0 (not the equation!)
- ```"/"``` button: activates division
- ```"x"``` button: activates multiplication 
- ```"-"``` button: activates subtraction
- ```"+"``` button: activates addition
- ```"="``` button: calculates the result and displays it
- ```"."``` button: adds a decimal point to the number on the screen
- ```"%"``` button: divides the number on the screen by 100
- ```"+/-"``` button: negation toggle

## Known Bugs
- There is a very strange glitch, potentially qt specific, where if you enter the equation (number % * = * number - =) in this order, you'll get an inaccurate number that doesn't occur on the iPhone app. I say "glitch" because you get the correct answer if you enter the value manually through debug logs. I've already run plenty of tests to try and get around the issue but just couldn't figure it out :(

## Systems
- At the moment there is only a Windows version, but I do want to get around to making a Mac version in the near future!
