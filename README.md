<b> Design explanation </b>
The intended purpose for the project is to make a calculator using TM4C123x board, built-in switches from the board, a keypad, and 7-segment display LTC-4627JR. The calculator will be able to perform basic arithmetic such as addition, subtraction, multiplication, and division. In addition, it should be able to receive input from the keypad and display results on the four segments of 7SED. Division is a special case where the result is expected to be shown in decimal value. 
The code is written in Keil using five ports:
Port A: connect MC with columns of the keypad PA7-PA4
Port B: connect MC with 7SED the cathode ends PB6-PB0
Port D: connect MC with rows of the keypad PD3-PD0
Port E: connect MC with 7SED anode ends.  PE3-PE0
Port F: for switches PF4-PF0
First of all, all ports listed above, keypad, and 7SED are connected together to make sure the operation of these electrical components. Next step is to perform arithmetic from the keypad and display the result on 7SED.
To code the calculator, calculate(a, b1, oper) is used to evaluate the result of basic arithmetic from the two inputs of getKey(). Inside calculate(), addition, subtraction, multiplication, and division are made. In main(), the input is scanned and displayed to 7SED from the keypad, and then evaluate the result from calculate() and have the result displayed on 7SED. The two input operands are valid if it is in between 0 and 9.

<b> Circuit Performance </b>
The operation of the calculator is demonstrated on December 11th 2020 to Bob with the basic arithmetic operation: addition, subtraction, multiplication, and division. The circuit takes two one-digit inputs from the keypad and displays the inputs to 7SED, then performs basic arithmetic if one of the operation of “+ - * /” is hit in between the two inputs. Then the result is shown on 7SED.

Keypad circuit interface is shown in Figure 1. The circuit successfully performed the predetermined basic arithmetic as shown to Bob. To have a two-digits result displayed on 7SED, the keypad has to be pushed and held for 1ms. 
