Dependencies
============

### Table of Contents
<pre>
<a href="#mosquitto">Mosquitto</a>
<a href="#poco">POCO</a>
<a href="#easylogging">Easylogging++</a>
</pre>


# Mosquitto
### Description
[Mosquitto](https://mosquitto.org/) library is a verry usefull implementation of mqtt protocol. This library provides synchronous and asynchronous thread-safe clients for communictaion with mqtt brocker. It's suports SSL/TLS connection, internal user verification by username and password
See the following links for more information on MQTT:

- Community page: <http://mqtt.org/>
- MQTT v3.1.1 standard: <http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/mqtt-v3.1.1.html>

### Compilation
In most linux repositories mosquitto provided without dinamyc libraries, so you need to download and install it from sources.

The following packages are required for mosquitto:

* tcp-wrappers (optional, package name libwrap0-dev)
```bash
$ sudo apt-get install libwrap0-dev
```

* openssl (version 1.0.0 or greater if TLS-PSK support is needed, can be disabled)
```bash
$ sudo apt-get install openssl libssl-dev
```

* c-ares (for DNS-SRV support, can be disabled)
```bash
$ sudo apt-get install libc-ares-dev
```

* libuuid (from e2fsprogs, can be disabled)
```bash
$ sudo apt-get install uuid-dev
```

* On Windows, the Redhat pthreads library is required if threading support is
  to be included.

[Download](https://mosquitto.org/download/) mosquitto sources, and extrarct downloaded archive

To compile run 
```bash
$ make all
```
but also see the file config.mk for more details on the various options that can be compiled in.

Where possible use the Makefiles to compile. This is particularly relevant for the client libraries as symbol information will be included.  Use cmake to compile on Windows or Mac.

### Instalation

To install run 
```bash
$ make install
```
[Goto Top](#table-of-contents)

# POCO C++
### Description
[POCO](https://pocoproject.org/) open source C++ class libraries for building network- and internet-based applications that run on desktop, server, mobile and embedded systems

### Compilation
[Download](https://pocoproject.org/download/index.html) POCO C++ Libraries Complete Edition sources, and extrarct downloaded archive

The following libraries require third-party software (header files and libraries) being installed to build properly:

 * NetSSL_OpenSSL and Crypt require OpenSSL.
```bash
$ sudo apt-get install openssl libssl-dev
```

 * Data/ODBC requires ODBC (unixODBC or iODBC on Unix/Linux)
```bash
$ sudo apt-get install libiodbc2 libiodbc2-dev
```

 * Data/MySQL requires the MySQL client.
On Unix/Linux platforms, use the package management system of your choice to install the necessary files. Alternatively, you can of course build MySQL yourself from source.

To compile run
```bash
$ ./configure
$ make -s
```

### Instalation
To install compl=iled libraries run
```bash
$ make install
```


For detailed instruction please withit [POCO - Getting Started](https://pocoproject.org/docs/00200-GettingStarted.html)

[Goto Top](#table-of-contents)

# Easylogging ++
[Easylogging++](https://github.com/easylogging/easyloggingpp) - very simple but powerfull logging library (added to sources of GLEmbGateway, look for easylogging.h in src foulder)
