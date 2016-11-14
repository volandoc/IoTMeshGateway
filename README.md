GLEmbGateway
============

### Table of Contents
<pre>
<a href="#introduction">Introduction</a>
    <a href="#features">Features</a>
    <a href="#environment">Environment</a>
    <a href="#dependencies">Dependencies</a>
<a href="#getting-started">Getting Started</a>
    <a href="#download">Download</a>
    <a href="#compiling">Compiling</a>
    <a href="#installing">Installing</a>
    <a href="#runing">Runing</a>
<a href="#configuration">Configuration</a>
</pre>

# Introduction
Now a wide variety of IoT devices running on different technologies and protocols. They can use wireless technology (wi-fi, Bluetooth, ZWaiv, NFC, etc.), or wired technologies (Ethernet, OneWair, I2C, SPI). These devices can control other physical devices, or be terminal primitives.

But the lack of common standards limits the ability to use all IoT devices that exactly needed in a particular situation.

Gateway - project, which was created for combining different devices running on different standards and protocols, and to help them communicate with each other.

 [Goto Top](#table-of-contents)

### Features
 * Flexible - GLEmbGateway designed as a plugin system, all parts of software (except core part) is a separated plugin which coresponds on specific set of functionality
 * Lightweight - Core part of GLEmbGateway is simple and can run on different platforms from laptops and pc's to embeded with limited resources, like home routers

 [Goto Top](#table-of-contents)

### Environment
GLEmbGateway running on linux systems, for more information please read ["Environment setup"](docs/environment.md)

 [Goto Top](#table-of-contents)

### Dependencies
 * [Mosquitto](https://mosquitto.org/) library used for communictaion ower mqtt protocol
 * [POCO](https://pocoproject.org/)  C++ framework used for working with SQLite, json, XML

for more information please read ["Dependencies instalation"](docs/dependencies.md)

 [Goto Top](#table-of-contents)

# Getting started
### Donwload
Stable version using git 
```bash
git clone https://portal-ua.globallogic.com/gitlab/andrii.maslianyi/EmbGateway.git
cd EmbGateway
```

Latest version using git 
```bash
git clone https://portal-ua.globallogic.com/gitlab/andrii.maslianyi/EmbGateway.git
cd EmbGateway
git checkout dev-POCO
```

 [Goto Top](#table-of-contents)

### Compiling
The build system is based on GNU Make 3.80 (or newer). If you do not have GNU Make 3.80 (or newer) installed on your machine, you will need to download it from <http://directory.fsf.org/devel/build/make.html> and build and install it prior to building the EmbGateway sources.

To compile go to source folder and run
```bash
$ make all
```

If you prefer to use IDE for compiling project contains files for [QT-Creator](https://www.qt.io/ide/). Please [download](https://www.qt.io/download/) QT-Creator version 5.5.0 or later and install folowing manual by QT team.

After instalation run QT-Creator and press "Open Project" button in main screen. Provide path to EmbGateway project files. When project successfuly open go to "projects" tab in left menu and in "build&run" tab add build-step "Make" with argument "install"

[Goto Top](#table-of-contents)

### Installing
Not implemented yet

[Goto Top](#table-of-contents)

### Runing
After compiling run
```bash
$ cd bin && ./embgateway
```

[Goto Top](#table-of-contents)

# Configuration
There is basic configiration files in [conf](conf/) folder.

[embgateway.properties](conf/embgateway.properties) - contains configuration for core component and ro POCO Logger instances.

Any othre configuration can be placed in ane place by desire .For example InnerBus core component has its own configuration file in component folder.
In same way all components can utilize POCO::Configuration library and store its own configuration

[Goto Top](#table-of-contents)