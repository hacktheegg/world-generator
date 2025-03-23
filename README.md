
# world-generator

A small duo project to create a randomly generated world that is visually viewable through OpenGL




## Run Locally

Clone world-generator

```bash
  git clone https://github.com/hacktheegg/world-generator.git
```

Go to the project directory

```bash
  cd world-generator
```

Install dependencies

```bash
  git submodule init
  git submodule update
```

Compile with Cmake

`windows`
```cmd
  cmake . -G "MinGW Makefiles"
  cmake --build .
```

`linux`
```bash
  cmake .
  make
```

Running Program

`windows`
```cmd
  ( cd .\build\windows\ && .\world-generator.exe )
```

`linux`
```bash
  ( cd ./build/linux/ && .\world-generator )
```


## Authors

- [@hacktheegg](https://github.com/hacktheegg)
- [@Meijuta](https://github.com/Meijuta)


## Acknowledgements

 - [Awesome Readme Templates](https://awesomeopensource.com/project/elangosundar/awesome-README-templates)
 - [Awesome README](https://github.com/matiassingers/awesome-readme)
 - [How to write a Good readme](https://bulldogjob.com/news/449-how-to-write-a-good-readme-for-your-github-project)


