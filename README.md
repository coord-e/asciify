# asciify

A small tool to convert any image into ascii art

[![asciicast](https://asciinema.org/a/9COkbNaCL8mBK1dOLwaMcYqcu.png)](https://asciinema.org/a/9COkbNaCL8mBK1dOLwaMcYqcu)

## Example

`gopher.png`:
![gopher](https://blog.golang.org/gopher/gopher.png)

go gopher by [Renee French](http://reneefrench.blogspot.jp/) | [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/)

This can be converted as follows:

```
$ asciify gopher.png -r 8 -c 4
                      , ,,,;: ;  : . ..,;:;\.
                   < , , ,  _, ,;<:.  <,  .  \
                ,        '      . :.,\ ~~'!   \
              .        :'         \  \    >\
           :!_. .. .   ''       ..., (      <\
        /,<;       :~;   ;         [         \\
       \«\       , ,.;'    {,,,, ;;           \\
    _,// \\      \ ,,    ,=>:::,,              \\
   ,,<\\ \\        , . ' .,  , ;.                \
 ,\, ,\\  \         ,;  ,   ,_ »'               ,»«
    \\\\     =(::,<; \    ':  :                  ,\~
  \~. \               ~,,.    :.                  ,\,
    ~ ;                    ..;,.                   ,\ .,,.
       ..                                           ,  ,:>,
       \:.                                           \ =, \
        \;                                           \\ ,
         \\                             ::>          \,
          \}.                         \.,, >
           .°~                       ,;    ;/         \\
              \\                    .     ,           \\
              ,\                                      \\
               :\                                     \\
               ,\.                                    \\
                 \                                    ,\
                 '                                    ,
                 \                                    ,
                 \                                   ,\
                 \                                   / ;
                 ,                                  //  ~\
                \                                 ./(,~ ::\
                  \                             .,,   < '
                 [ \                         .,
                    >,                   ._
                    ~ :.....       ,__._
                       ,   :;:::,===
                      , . .».
                      :;,, ,
 ```
By the way, asciify isn't written in Go (I love gopher)

## Getting started

### Prerequirements
 - opencv
 - opencv_contribs
 - tesseract
 - [Taywee/args](https://github.com/Taywee/args)

### Clone, build, and install

```bash
git clone https://github.com/coord-e/asciify
cd asciify
mkdir build && cd $_
cmake ..
make
sudo make install
```

## Thanks to

Taywee/args - [https://github.com/Taywee/args](https://github.com/Taywee/args)
