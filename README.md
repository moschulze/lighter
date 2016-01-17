# Lighter

**This software is currently being developed and some features might be missing or broken!**

Lighter is a tool to illuminate your home, your shop, your garden or anything else.

By defining scenes that can be independently started and stopped you create the desired atmosphere by the press of just one button. Also the scenes can consist of multiple steps so it is possible to realise color changes or moving lights with different speeds.

The software uses the [Art-Net protocol](https://en.wikipedia.org/wiki/Art-Net) to send DMX data to the devices. It is designed to run on Unix systems and currently has successfully being tested on the following systems:

- Ubuntu 14.04

It can be controlled via an easy to use browser interface (**Not implemented yet**). If you want to write your own Interface there is a HTTP API that can be used to control nearly all features of this software.

## Installation
### Prerequisite
To download and compile the software you need the following tools:

- Git
- CMake (version >=  3.2)
- Make
- The g++ compiler

On a Debian system (like Ubuntu) you can install these tools by executing the following command on the command line:

```sh
sudo apt-get install build-essential cmake git
```

### Downloading the sources
Clone the sources and initialise the external libraries:

```sh
git clone https://github.com/moschulze/lighter
cd lighter
git submodule init
git submodule update
```

### Compile it
To compile the sources it is recommended to create a separate directory. On the command line go to the directory where you cloned the source files and create the build directory:

```sh
mkdir build
```

Then you switch to the build directory and execute CMake and make to build the application:
```sh
cd build
cmake ..
make
```

If everything goes correct you should see an executable with the name "lighter" in the build directory. You can copy it to the desired location or run it directly from here.

## Configuration
### Introduction
The whole configuration is stored in JSON format and is divided into five sections:

1. Main configuration
2. Device type configuration
3. Interface configuration
4. Device configuration
5. Scene configuration

The main configuration and the ones for the device types need to be edited by hand. All the others are managed by the software.

### Main configuration
On startup the application looks in the directory from which it has been started for a file called "config.json" to load the main configuration parameters. The defaults are as follows:

```json
{
  "server.port": 8080,
  "deviceTypes.path": "./types/",
  "devices.path": "./devices.json",
  "interfaces.path": "./interfaces.json",
  "scenes.path": "./scenes.json"
}
```

#### Details
|Key             |Type   |Explanation                                                 |
|----------------|-------|------------------------------------------------------------|
|server.port     |Integer|Port for the HTTP API server to run on                      |
|deviceTypes.path|String |Path to the directory of the device type configuration files|
|devices.path    |String |Path to the configuration file for the devices              |
|interfaces.path |String |Path to the configuration file for the interfaces           |
|scenes.path     |String |Path to the configuration file for the scenes               |

### Device types
This configurations contain information about the different types of devices that are usable. This includes the name of the type and the number and names of the slots it uses. The name of the file must be unique as it is used to identify the type. When you need a new device type for which there isn't already a configuration you can simply create your own.

To explain the structure of this configuration file we have a look at the configuration for a generic RGB LED or RGB LED strip. The file name is "rgb_led.json" and the content is the following:

```json
{The different steps of the scene (explained below)
  "name": "RGB LED",
  "slots": {
    "Red": 1,
    "Green": 2,
    "Blue": 3
  }
}
```
#### Details
|Key             |Type       |Explanation                                                      |
|----------------|-----------|-----------------------------------------------------------------|
|name            |String     |The name of the type                                             |
|slots           |JSON-Object|Key-value pairs for the name and number of slots this device uses|

### Interfaces
This configuration is important to know where to send the data so it reaches the right devices. The structure is as follows:

```json
{
  "id1": {
    "name": "First interface",
    "universe": 0,
    "address": "host"
  },
  "id2": {
    "name": "Second interface",
        "universe": 1,
        "address": "host"
  }
  …
}
```

#### Details
The file consists of multiple entries for the different interfaces. Every entry is a pair of a unique id, which is used to identify the interface, and a json object with the configuration parameters for this interface. The possible parameters are in the table below:

|Key             |Type   |Explanation                                                      |
|----------------|-------|-----------------------------------------------------------------|
|name            |String |The name of the interface                                        |
|universe        |Integer|The DMX universe this interface belongs to                       |
|address         |String |The network address of this interface                            |

### Devices
This configuration describes the different devices thar are present. It has the following structure:

```json
{
  "id1": {
    "type": "rgb_led",
    "name": "First device",
    "universe": 0,
    "startAddress": 0
  },
  "id2": {
      "type": "rgb_led",
      "name": "Second device",
      "universe": 0,
      "startAddress": 3
  }
  …
}
```

#### Details
The file consists of multiple entries for the different devices. Every entry has a unique id, which is internally used to identify the device, and the configuration parameters for this device. The parameters are listed in the table below:

|Key             |Type   |Explanation                                                      |
|----------------|-------|-----------------------------------------------------------------|
|type            |String |The unique id of the typ of this device                          |
|name            |String |The name of the device                                           |
|universe        |Integer|The DMX universe in which the device is present                  |
|startAddress    |Integer|The first address of the device                                  |

### Scenes
This is the heart of the application. In this configuration the actual values for the different scenes get stored. The basic structure is as follows:

```json
{
  "scene1": {
    "name": "First Scene",
    "steps": {
      "1": {
        "fadeInTime": 1000,
        "duration": 1000,
        "fadeInAnimation": "linear",
        "next": "2",
        "data": {
          "test1": [
            255,
            0,
            0
          ]
        }
      },
      "2": {
        "fadeInTime": 1000,
        "duration": 1000,
        "fadeInAnimation": "sinus",
        "next": "3",
        "data": {
          "test1": [
            0,
            255,
            0
          ]
        }
      }
      …
    }
  }
}
```

#### Details
This configuration consists of multiple scenes with a unique id. Every scene has two configuration values:

|Key             |Type       |Explanation                                                      |
|----------------|-----------|-----------------------------------------------------------------|
|name            |String     |The name of the scene                                            |
|steps           |JSON-Object|The different steps of the scene (explained below)               |

The steps also have a unique id for identification and have the following configuration values:

|Key             |Type       |Explanation                                                                                                                                    |
|----------------|-----------|-----------------------------------------------------------------------------------------------------------------------------------------------|
|fadeInTime      |Integer    |The time in milliseconds it should take to fade to this step                                                                                   |
|duration        |Integer    |The time in milliseconds the step should be active (after the fade in) before switching to the next step. A value of -1 makes the step infinite|
|fadeInAnimation |String     |The animation type for the fade in. Possible values are "linear" and "sinus"                                                                   |
|next            |String     |The id of the step that should be active after this one. If the value is empty ("") the scene will automatically end after this step           |
|data            |JSON-Object|The data that should be send to the devices for this step (explained below)                                                                    |

The data consists of the unique id of the device they should be send to and the data as a json array in the order of the slots.

## HTTP API
The HTTP API simply works by sending GET, PUSH, POST and DELETE requests with the required data to the server.

You can find all currently implemented API requests listed below. The URI parts surrounded by braces are placeholders and will be explained by a table.

### Start scene
Method: GET
URI: /api/scene/start/{id}

|Placeholder|Explanation                                 |
|-----------|--------------------------------------------|
|{id}       |The unique id of the scene you want to start|

### Stop scene
Method: GET
URI: /api/scene/stop/{id}

|Placeholder|Explanation                                |
|-----------|-------------------------------------------|
|{id}       |The unique id of the scene you want to stop|