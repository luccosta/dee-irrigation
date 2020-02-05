# dee-irrigation
> Codes and instructions of use of the automatic irrigation system implemented in DEE-UFCG

The project proposes to implement an intelligent irrigation system for the DEE environment, with the participation and collaboration of the department and other entities of the academic community. The system would comprise from the collection of information (sensing) and corresponding performance, to the processing and storage of data. A partnership with the PELS-IAS chapter was also devised to develop a modular and complementary system for pumping water powered by solar energy.

![](c432_partitions.gif)

## Table of Contents

- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
- [Running the tests](#running-the-tests)
- [Meta](#meta)
- [Contributing](#contributing)

## Introduction (Techinical description)

A Node MCU ESP8266 will be used in order to read the data obtained by the sensor and, using parameters obtained from bibliographic research, to achieve ideal humidity levels for the detected conditions, the microcontroller will regulate the valve opening, when necessary soil irrigation.

The sensor used will be a Soil Moisture Sensor Hygrometer, which is able to detect the level of soil moisture through its stems according to conductivity, the lower the soil moisture, the lower the conductivity and the higher the soil moisture , the greater the conductivity, being that when the soil is dry the sensor output is in high state, and when humid in low state.

The system will be powered by the electric network that will first pass through a rectifier, which will convert the voltage from alternating to continuous and reduce the magnitude from 220 to 12 V. This 12V voltage will pass through a regulator, causing its reduction from 12 to 5V, which will serve as power for the ESP8266.

![](software_flow_diagram.png)

The system features cloud processing by an AWS IoT Core processor, a managed cloud service that allows devices to be easily and securely connected to cloud applications and other devices. This service allows message processing and routing to AWS endpoints and other devices, which provides secure two-way communication between the ESP8266 and the server.

If you want more information, you can get the [full-report] of this work in portuguese

## Prerequisites

- Arduino Core for ESP8266, PlatformIO.

## Running the tests

Change the directive DEBUG to true in the source code main.cpp and run it on PlatformIO.

```sh
19 #define DEBUG false //if true will ignore if already watered
```

_For more examples and usage, explore the files in the tests folder. (Future)_

## Meta

Lucas Fernando – lucas.costa@ee.ufcg.edu.br

Associated:

Italo Diniz - italo.vilarim@ee.ufcg.edu.br

Distributed under the MIT license. See ``LICENSE`` for more information.

[https://github.com/luccosta/dee-irrigation](https://github.com/luccosta/dee-irrigation)

## Contributing

1. Fork it (<https://github.com/luccosta/dee-irrigation/fork>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request

<!-- Markdown link & img dfn's -->
[article]: https://www.sciencedirect.com/science/article/pii/S0026271408001947
[vrl-repo1]: http://web.eecs.umich.edu/~jhayes/iscas.restore/
[vrl-repo2]: http://www.pld.ttu.ee/~maksim/benchmarks/iscas85/verilog/
[full-report]: https://drive.google.com/file/d/1JrvtunvYSekGLQ2FS-RSYM7KmdqPZ3iJ/view?usp=sharing
[npm-image]: https://img.shields.io/npm/v/datadog-metrics.svg?style=flat-square
[npm-url]: https://npmjs.org/package/datadog-metrics
[npm-downloads]: https://img.shields.io/npm/dm/datadog-metrics.svg?style=flat-square
[travis-image]: https://img.shields.io/travis/dbader/node-datadog-metrics/master.svg?style=flat-square
[travis-url]: https://travis-ci.org/dbader/node-datadog-metrics
[wiki]: https://github.com/yourname/yourproject/wiki
