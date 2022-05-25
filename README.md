# Major Project -G5
## Member Contributions:
- Nathan Ford
- Samson Mackeller
- Aaron Sandelowsky
- Alec Cook
- Leon Ortega
## Instructions For Use
Install CodeWarrior IDE using https://canvas.sydney.edu.au/courses/40267/files/22295254/download?download_frd=1

Download the repository by clicking Code > Download zip.

Navigate to the downloaded repository, open the module you wish to use. If the module contains separate projects for simulation or hardware, select the project you wish to use. Open major_project_drivers.mcp and run the program in the CodeWarrior IDE.
## Background
This Program aims to check stock levels on shelves and detect if a shoppper is geesturing towards a product.
## Modules
### Calibration
This module aims to teach a user how far to place the sensor from the stock and can rottate the sensor to look directly at the centre column of stock. The magnetometer is used to sense the direction the sensor is pointing to in relation the the earths magnetic field. This will then rotate the servo until the direction the sensor is facing is the correct direction of the stores shelves. This allows for an automated direction calibration for the system.

The Lidar sensor is then used pointing at an empty shelf to register if the sensor is the correct calibrated distance from the store shelves. If the depth detected is less than expected to terminal will tell the user to move backwards or conversely if the oppoiste is detected to move forwards. This continues until the sensor is within a sensible depth of the shelves when the terminal will notify the user that the sensor is in the right location.

### Sensing
The Sensing module will cycle around all 9 shelves detetcing the depth data present in each direction. If the Depth data is less than the front of the shelf the program will assume that a hand is present and give the user data that can influence a purchase. If the Depth data is between the front and back of the shelf values, the program will assume that the shelves are stocked. If the depth data demonstrates that it is similar to the distance from the back of the shelf from the sensor, the program will announce that the shelves are empty.

## Future Uses
Currently the world is facing extreme supply chain distruptions due to Covid, fuel, climate change shipping and staffing related shortages leading to empty shelves in supermarkets. For example, covid lead to empty toilet paper shelves and the floods lead to mass food shortages nationawide. If we had our robot moving around supermanrket aisles notifyng staff and online shoppers about stock levels in realtime. This would allow the distribution centres to target shortages quicker and shoppers to choose where to shop based on where theuy can find their products. By creating an automated digital twin for supermarkets and department stores supply levels we can streamline supply chains and allow for quicker reactions by companies to ensure sales.
