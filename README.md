# Major Project -G5
## Member Contributions:
- Nathan Ford
  - Depth calibration for sensor
  - Conversion of lidar readings to dimensions
- Samson Mackeller
  - Hand location program
  - Box construction
  - Midpoint detection
  - Depth detections
  - Greater accuracy calibrations
- Aaron Sandelowsky
  - Documentation
  - Timing Delays for hand sensing
  - Original Program flowcharts
- Alec Cook
  - Magnetometer Calibration
  -  Overall integration
  -  Polling of right column
  -  Delays
  -  Halt sensor whilst hand detected
  -  Video
- Leon Ortega
  - Work with LED's when hand detected 
## Instructions For Use
Install CodeWarrior IDE using https://canvas.sydney.edu.au/courses/40267/files/22295254/download?download_frd=1

Download the repository by clicking Code > Download zip.

Navigate to the downloaded repository, open the module you wish to use. If the module contains separate projects for simulation or hardware, select the Information Displa-tion Creation folder. Open major_project_drivers.mcp and run the program in the CodeWarrior IDE.

Magnetometer values desired (as calibration is required) can be accessed at the top of the magnetometer.c file, otherwise program will run and calculate as code continues.

## Background
This Program aims to check stock levels on shelves and detect if a shoppper is geesturing towards a product. Due to the high competition and overcrowding in the market of groceries and other goods, consumers may be deceived by similar looking packaging of various qualities. Also individuals may have visual impairements leading to an inability to distinguish between similar looking packaging. Another concern is covid-19 and flu season where people don't want to touch items that have been infested by other peoples germs. 

Therefore our design aims to be an automated shopping assistant that can allow people to distinguish between products without the need to pick them up. This will increase shoppers safety and scam awareness with our technology aiding shoppers while they are in the supermarket. 
## Modules
### Calibration
This module aims to teach a user how far to place the sensor from the stock and can rottate the sensor to look directly at the centre column of stock. The magnetometer is used to sense the direction the sensor is pointing to in relation the the earths magnetic field. This will then rotate the servo until the direction the sensor is facing is the correct direction of the stores shelves. This allows for an automated direction calibration for the system.

The Lidar sensor is then used pointing at an empty shelf to register if the sensor is the correct calibrated distance from the store shelves. If the depth detected is less than expected to terminal will tell the user to move backwards or conversely if the oppoiste is detected to move forwards. This continues until the sensor is within a sensible depth of the shelves when the terminal will notify the user that the sensor is in the right location.

Function 1: align_with_magnetic_field()
- Read magnetic fields in order to determine central x-position
- Will rotate the sensor around 180 degrees, until a suitable magnetic reading is achieved
- Sets value for x_pos, used throuhgout the rest of the program

Function 2: set_midpoints_box_array(x_pos, y_pos, box_array)
- Will utilise known conversions of servo readings to real measurements to complete calculations
- Assumes a 15cm difference in both x and y directions of gridpoints
- Results in slightly smaller alterations moving further from the centre (as greater angle change results in greater distance change)
- Will assign azimuth and elevation values to box_array[i] struct objects

Function 3: misaligned_midpoint(box_array, home_x, home_y, azimuth789)
- Will take values from some given cetnre point in positive and negative azimuth plane
- Results in observing a 'flat' surface, with equal distances left and right, thus an ideal 'centre point' is identified
- Allows for more robust system

-------------------------------------------------------------------------------------------------------------------------------

## Testing Procedures

Throughout the calibration phase, utilising the serial output is crucial to understand what's working and what isn't.
- Setting outputs for magnetometer readings
- Setting outputs for lidar distance readings
- Setting outputs for midpoints of points and azimuth values

Further, modules can be tested in isolation by setting triggers (eg establish_boxes, check_midpoints, checking_for_arm) to 1, resulting in while() loops not triggering. Simply set the aspects of the code you don't want to run to 1, and leave others at 0.

## Discussion Questions

- What happens when a slanted surface is encountered for the more precise checks?
- What happens if the magnetometer orientates at a point where the sensor can't properly rotate?
- What if the sensed boxes are at different depths?

------------------------------------------------------------------------------------------------------------------------------

### Sensing
The Sensing module will cycle around all 9 shelves detetcing the depth data present in each direction. If the Depth data is less than the front of the shelf the program will assume that a hand is present and give the user data that can influence a purchase. If the Depth data is between the front and back of the shelf values, the program will assume that the shelves are stocked. If the depth data demonstrates that it is similar to the distance from the back of the shelf from the sensor, the program will announce that the shelves are empty.

Function 1: hand_covered_boxes(struct box* box_array)
- Identifies which boxes are covered, by either a hand or other object
- Assigns struct value .hand_covering to 1 if covered, 0 if not

Function 2: highest_box_probability(struct box* box_array)
- Will use the given covered/uncovered boxes to determine most likely hand position
- Since a hand can't extend without an arm, all boxes between the user (on the right) to the covered point must be filled
- Assigns a 'priority', assuming the furthest left and highest up covered points to be more likely hands
- Outputs the box selected, then able to display information

------------------------------------------------------------------------------------------------------------------------------

## Testing Procedures

Throughout the testing of the hand and box detection modules, the use of the serial port was crucial in identifying the different values being assigned at different points in time.
- Setting outputs for when a 'hand' is detected whilst polling the right column
- Displaying a matrix of 1s and 0s of which boxes were covered, like so:

      0 0 0         1 2 3
      0 1 1   -->   4 5 6
      0 0 0         7 8 9
      
      - Would result in box 5 being selected
 
- Displaying the final information was useful in ensuring the correct procedures were occuring

## Discussion Questions

- What happens if all the boxes are covered?
- What happens if none of the boxes are covered?
- What happens if a hand is detected, but moves before it is sensed again?
- What happens if an arm is stuck in diagonally?

------------------------------------------------------------------------------------------------------------------------------

## Future Uses
Currently the world is facing extreme supply chain distruptions due to Covid, fuel, climate change shipping and staffing related shortages leading to empty shelves in supermarkets. For example, covid lead to empty toilet paper shelves and the floods lead to mass food shortages nationawide. If we had our robot moving around supermanrket aisles notifyng staff and online shoppers about stock levels in realtime. This would allow the distribution centres to target shortages quicker and shoppers to choose where to shop based on where theuy can find their products. By creating an automated digital twin for supermarkets and department stores supply levels we can streamline supply chains and allow for quicker reactions by companies to ensure sales.
