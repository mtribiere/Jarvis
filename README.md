# Jarvis
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![CMake](https://github.com/mtribiere/Jarvis/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/mtribiere/Jarvis/actions/workflows/cmake.yml)<br>

## Why Jarvis ?
There is already out there a lot of Home Assistants, and very complete solutions for creating a so-called Smart House. So why Jarvis ?
All those Assistants and solutions are offering hundred of features that most user will in fact never use, not always maintenable code, and complex architectures.
Jarvis is my minimalistic approch to Smart House, with only a few but actually usefull features.

"Better do a little well, than a great deal badly."
  -Socrates
  
## Description
A set of tools and protocols to automate your house<br>
For now, there is 3 main componants in this chain, the App, the Executor, and the Firmware (Tasmota).
<br><br>
<img align="middle" src="pics/arch.png" width="600">
<br><br>

## Demo
[![Watch the video](https://img.youtube.com/vi/Sw54WxUngtA/default.jpg)](https://youtu.be/Sw54WxUngtA)
  
## Compiling the Executor
The executor can be compiled with 2 modes, controled by the ```CONSOLE_BUILD``` variable at the top of <b><i>main.cpp</i></b>

Before compiling, you need to install Boost, libhttpserver and Paho-MQTT.

```console
cd Jarvis/Executor
mkdir build && cd build
cmake ..
make
./bin/jarvis_executor
```

## Todo List
- [x] Custom firmware for the SOnOff switch
- [ ] Get ride of Arduino Core for the official ESP8266 SDK
- [ ] Basic API for direct commands
- [ ] Use a Intent Parser/NLP for the chat API (Mycroft adapt, Wit.ai,...)
- [x] Basic if-based Chat bot for Action
- [x] Switch to CMake
- [ ] Make a good UI for the App

Crafted with :heart: by mtribiere
