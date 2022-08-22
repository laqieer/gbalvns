# gbalvns
Leaf Visual Novel System for GBA

VN/AVG/ADV/Gal game engine based on [sizuku_gba2](http://akkera102.sakura.ne.jp/gbadev/index.php?NO.100%20sizuku_gba2).

## Demo

[Download](https://github.com/laqieer/sizuku/releases)

### Screenshot

![](https://user-images.githubusercontent.com/8841957/185962493-bed92953-1d01-4a38-9be2-50333a5123ab.png)
![](https://user-images.githubusercontent.com/8841957/185962498-8d2f0b4f-6571-4fc3-9dcc-15ec573b4934.png)
![](https://user-images.githubusercontent.com/8841957/185962500-e2cd89bd-8dd3-4eef-82dc-c3b00a7e701f.png)

![](https://user-images.githubusercontent.com/8841957/185962501-b465de1c-11c8-418e-91a5-37847e6193dd.png)
![](https://user-images.githubusercontent.com/8841957/185962503-53b15280-12da-46c6-9dd2-4f0cfef7b1b3.png)
![](https://user-images.githubusercontent.com/8841957/185962509-4350b9be-4f07-4d39-ae42-454ceff77712.png)

### Credit

フリー素材:

- Background: きまぐれアフター
- Character: キャラクターなんとか機
- Music: 魔王魂

## Feature

Create your own games on GBA without coding! No programming skills are required!

- Story branching
- Save & load at any point
- Skip to the next branch point & return to the previous branch point freely
- Environment configuration
- Logging

## Prerequisite

- Install [devkitPro](https://devkitpro.org/)
- Build [wav28ad](https://github.com/laqieer/wav28ad) and `cp /path/to/your/built/wav28ad tool/`

## Build

`make`

## Doc

Messages, events and animations are written in Script Markup Language.

### Message & Event

`asset/script/`

[sizuku_gba2's doc](http://akkera102.sakura.ne.jp/gbadev/index.php?Doc.15%20PC%A5%B2%A1%BC%A5%E0%A1%D6%BC%B6%A1%D7%A4%CE%B0%DC%BF%A2%CA%FD%CB%A1%281%29)

### Animation

`core/res/`

`core/anime.c`

