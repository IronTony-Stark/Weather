# Weather

## Console App to get Weather Information using Boost and OpenWeatherMap API

### Demo
`Weather -h`
```
Main options:
-h [ --help ]         Show help
-c [ --city ] arg     Enter city (If the city name 
is divided into several words - use _ instead of space)
-t [ --token ] arg    Enter token
```

`Weather -c New_York`
```
City: New York
Temperature: 2.6°C
Wind's speed: 1.88 meter/sec
Wind's direction: 348°
```

### How to get a token
1. Open the following site: https://home.openweathermap.org/users/sign_up
2. Sing up there
3. In purpose field select "Education/science"
4. Check your email, open letter and press "Verify"
5. Your API will be here: https://home.openweathermap.org/api_keys
6. Read the API doc: https://openweathermap.org/current

### How to use
- First of all, you need to get an api key (token) from **OpenWeatherMap**.
  Please, use the instructions written above
- To show help write `Weather`, `Weather -h` or `Weather --help`
- To view city's weather write `Weather -c <Your_City> -t <token>`
- If you get tired of specifying token on every request,
  you can also create `config.txt` _near your
  executable_ and put the token there in the following format: `token = <token>`.
  Now you can simply use `Weather -c <Your_City>`

### How to build
1. Install Boost 
   
I'm using 1.73, but I think any version >= 1.66 should be ok
- [Download](https://www.boost.org/users/history/version_1_73_0.html)
- Install
  - [Unix](https://www.boost.org/doc/libs/1_73_0/more/getting_started/unix-variants.html#prepare-to-use-a-boost-library-binary)
  - [Windows](https://www.boost.org/doc/libs/1_73_0/more/getting_started/windows.html#prepare-to-use-a-boost-library-binary)


2. In `CmakeList.txt` specify how to find Boost. If you're having trouble, 
  please see [FindBoost](https://cmake.org/cmake/help/latest/module/FindBoost.html)
   

3. Build 
```
- cd <project root>
- mkdir build
- cd build
- cmake -G <Generator> ..
- make
- Weather
```
`cmake -G` - To list all generators

If `make` doesn't work for you, try `cmake --build build --target Weather` from project root
