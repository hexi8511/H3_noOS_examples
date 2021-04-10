# PC project for LittleV Graphics Library

The [Littlev Graphic Library](https://github.com/littlevgl/lvgl) is written mainly for microcontrollers and embedded systems however you can run the library **on your PC** as well without any embedded hardware. The code written on PC can be simply copied when your are using an embedded system.

Using a PC simulator instead of an embedded hardware has got several advantages:
* **Costs $0** because you you don't have to buy design PCB
* **Fast** because you don't have to design an manufacture PCB
* **Collaborative** because any number of developers can work without hardware requirements
* **Developer friendly** because much easier and faster to debug on PC

## Requirements
* The PC simulator is cross platform so **Windows, Linux and OSX** is supported
* **SDL** a low level driver library to use graphics, handle mouse, keyboard etc.
* This project (configured for **Eclipse CDT IDE**)

## Usage

### Get the PC project

Clone the PC project and the related sub modules:
```
git clone https://github.com/littlevgl/proj_pc.git
cd proj_pc
git submodule update --init
```

or download it as zip [here](htp://www.gl.littlev.hu/download/pc_simulator_v5_0_0.zip).

### Install SDL
You can download SDL from https://www.libsdl.org/

On Linux you can install it via terminal:
1. Find the current version of SDL2: `apt-cache search libsdl2` (e.g. libsdl2-2.0-0)
2. Install SDL2: `sudo apt-get install libsdl2-2.0-0` (replace with the found version)
3. Install SDL2 development package: `sudo apt-get install libsdl2-dev`
4. If build essentials are not installed yet: `sudo apt-get install build-essential`

### Install Eclipse CDT
Download and install Eclipse CDT from  http://www.eclipse.org/cdt/

### Import the PC simulator project
1. Open Eclipse CDT
2. Click **File->Import** and choose **General->Existing project into Workspace**
3. Browse the root directory of the project and click Finish
4. Build your project and run it

## Contributing
1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request!

If you find an issue, please report it via GitHub!

## Donate
If you are pleased with the graphics library and found it useful pleas support its further development:

[![Donate](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=LWHHAQYZMRQJS)
