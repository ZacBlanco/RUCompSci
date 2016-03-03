## Lab 3 - Computer Architecture

## Assignment 1

Pseudo Code

```

Get input 1 from user --> t0
Get input 2 from user --> t1

set operation to something not equal to exit
loop:
	get new input from user
	check whether input is equal to exit
	get input from user on choice
	switch (input)
		and
		or
		xnor
		xor
		nand
	loop
	
```
	
	
## Assignment 2

```
get array length from user

while we havent reached array length
	let user enter a number
	
sort array and print using bubble sort

enter a number for user

binary search for location where number should go
swap number in correct spot
put the swapped number to the next location
continue pushing the numbers on the right side until every number to the right is moved
```
## Assignment 4

```
Ask a user to enter inputs for m, n, p
Ask user to input x_0

Set x = x_0
Given inputs and the starting value x_0
	t2 = calculate m(x)^2 + n(x) + p
	t3 = calculate 2m(x) + n
	determine x_i+1 as x - (t2/t3)
	if(abs(x_i+1 - x_i)) is less than .00001
		set x = x_i+1
		loop back to calculate t2

print the value of x_i+1
exit
```


## Assignment 5


```
Get input r from user
Get input h from user

calculate (pi)r --> t1
multiply t1 * r --> t2
multiply t1 * sqrt(r^2 + h^2) --> t3
add t2 + t3 = surface area

print surface area

divide t1 by 3 --> t2
multiply t2 by r --> t2
multiply t2 by h --> Volume

print volume
```

## Assignment 6

```
Allocate a large amount of space for the array
Let user enter a number n, the size of the array of floating point values

set  i = 0
while i < n
	get input number from user
	store in 4*i + base

print calcMean()
print calcStdDev()

calcMean:
	loop over every number in the array
	continuously sum the numbers
	divide the total sum by n --> result
	return result
	
calcStdDev:
	stdDeviation sum = 0
	loop over every number in the array
		subtract the mean the value in the array
		sqaure the difference of the two
		add to stdDeviation sum
	
	take square root of stdDeviation sum
	divide the total sum by n --> result
	return result
		
```


















































